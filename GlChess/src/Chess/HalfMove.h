#pragma once

#include "Piece.h"
#include "ChessBoard.h"

class ChessBoard;

class HalfMove
{
public:
	HalfMove(Square* orig, Square* dest, char flag = 0);
	HalfMove(HalfMove& fromMove, ChessBoard* board);

	void setDisambiguation(std::string disambiguation);
	void setCheck(bool isCheck);
	void setMate(bool isMate);
	void setFullMove(int fullMove);
	void setCapturedPiece(std::shared_ptr<Piece> piece);

	Square* getOrigin();
	Square* getDestination();

	bool getIsMate() { return _isMate; }
	char getFlag() { return _flag; }
	bool isCapture() const;

	void printMove();

	operator std::string() const;
	friend std::ostream& operator<<(std::ostream& os, const HalfMove& move);
	
private:
	std::string _toString() const;

	Square* _orig;
	Square* _dest;
	std::string _disambiguation;

	bool _isEnPassantCapture() const;
	bool _isCheck;
	bool _isMate;
	char _flag;

	int _fullMove;
	std::shared_ptr<Piece> _movedPiece;
	std::shared_ptr<Piece> _capturedPiece;
};
