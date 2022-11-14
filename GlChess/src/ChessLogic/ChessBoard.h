#pragma once

#include <string>
#include <iterator>
#include <vector>
#include <unordered_map>
#include "Fen.h"
#include "Square.h"
#include "HalfMove.h"

class Fen;
class HalfMove;
class Square;

enum Color;
enum PieceType;

struct AllSquaresIterator;

class ChessBoard
{
public:
	ChessBoard();
	ChessBoard(const Fen& fen);
	virtual ~ChessBoard();

	Fen toFen();
	static const Fen STARTING_POSITION;

	AllSquaresIterator begin();
	AllSquaresIterator end();

	void printBoard();
	void printGame();

	Square* getSquare(std::string squareName);
	std::string generatePieceName(Color color, PieceType type);

	Color getActivePlayerColor() { return _activePlayerColor; }
	std::vector<std::shared_ptr<Piece>> getPlayerPieces(Color color);
	std::vector<std::shared_ptr<Piece>> getAllPieces();
	std::vector<HalfMove> getLegalMoves();
	std::vector<Fen> getHistory() { return _history; }
	std::vector<HalfMove> getMoves() { return _moves; }

	void makeMove(HalfMove& move);
	std::vector<HalfMove> getCandidateMoves(Square* origin, Square* destination);
	bool makeMove(Square* origin, Square* destination, char promotion = 0);

	bool getIsMate() { return _isMate(); }
	bool getIsDraw() { return _isDraw(); }
	
	static std::string getPieceStringType(PieceType type);

private:
	Square _squares[8][8];
	std::vector<Fen> _history;
	std::vector<HalfMove> _moves;

	std::unordered_map<std::string, unsigned int> _pieceCount;

	std::vector<HalfMove> _legalMoves;
	bool _shouldRecalculateLegalMoves = true;
	void _recalculateLegalMoves();

	Color _activePlayerColor;
	void _cycleColor();

	bool _isWhiteCastleShortAllowed;
	bool _isWhiteCastleLongAllowed;
	bool _isBlackCastleShortAllowed;
	bool _isBlackCastleLongAllowed;

	Square* _enPassantSquare;
	int _halfmoveClock;
	int _fullmoveNumber;

	bool _isEnemyPiece(Color activeColor, Square* square) const;
	bool _isFriendPiece(Color activeColor, Square* square) const;

	void _getRawMoves(std::vector<HalfMove>& moves);

	void _getPawnMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves);
	void _getKnightMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves);
	void _getBishopMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves);
	void _getRookMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves);
	void _getQueenMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves);
	void _getKingMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves);

	void _getCastleMoves(std::vector<HalfMove>& moves);
	void _removeEnemyChecks(std::vector<HalfMove>& moves);
	void _applyDisambiguation(std::vector<HalfMove>& moves);
	void _applyChecks(std::vector<HalfMove>& moves);

	bool _isCheck();
	bool _isMate(bool skipVerifyCheck = false);
	bool _isDraw();
};

struct AllSquaresIterator
{
	using iterator_category = std::contiguous_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Square;
	using pointer = Square*;
	using reference = Square&;

	AllSquaresIterator(Square* ptr) : _ptr(ptr) {}

	Square& operator*() const { return *_ptr; }
	Square* operator->() { return _ptr; }
	Square* get() { return _ptr; }

	AllSquaresIterator& operator++() 
	{ 
		_ptr += 1;
		return *this;
	}
	
	// postfix
	AllSquaresIterator operator++(int) 
	{ 
		AllSquaresIterator tmp = _ptr;
		_ptr += 1;
		return tmp;
	}

	AllSquaresIterator operator--() 
	{ 
		_ptr -= 1;
		return *this;
	}
	
	// postfix
	AllSquaresIterator operator--(int) 
	{ 
		AllSquaresIterator tmp = _ptr;
		_ptr -= 1;
		return tmp;
	}

	AllSquaresIterator& operator+=(int increment)
	{
		_ptr += increment;
		return *this;
	}

	AllSquaresIterator& operator-=(int decrement)
	{
		_ptr -= decrement;
		return *this;
	}

	AllSquaresIterator operator+(int increment)
	{
		return AllSquaresIterator(_ptr + increment);
	}

	AllSquaresIterator operator-(int decrement)
	{
		return AllSquaresIterator(_ptr - decrement);
	}

	std::size_t operator-(AllSquaresIterator other)
	{
		return (_ptr - other._ptr);
	}

	friend bool operator==(const AllSquaresIterator& i, const AllSquaresIterator& j) { return i._ptr == j._ptr; };
	friend bool operator!=(const AllSquaresIterator& i, const AllSquaresIterator& j) { return i._ptr != j._ptr; };

private:
	Square* _ptr;
};
