#pragma once

#include <string>
#include <iterator>
#include <vector>
#include "Fen.h"
#include "Square.h"
#include "HalfMove.h"

class Fen;
class HalfMove;

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

	Color getActivePlayerColor() { return _activePlayerColor; }
	std::vector<std::shared_ptr<Piece>> getPlayerPieces(Color color);
	std::vector<std::shared_ptr<Piece>> getAllPieces();
	std::vector<HalfMove> getLegalMoves();

	void makeMove(HalfMove& move);

	bool getIsMate() { return _isMate(); }
	bool getIsDraw() { return _isDraw(); }
	
private:
	Square _squares[8][8];
	std::vector<Fen> _history;
	std::vector<HalfMove> _moves;

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

