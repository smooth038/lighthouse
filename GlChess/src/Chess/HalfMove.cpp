#include "HalfMove.h"
#include <sstream>

HalfMove::HalfMove(Square* orig, Square* dest, char flag)
	: _orig(orig), _dest(dest), _isCheck(false), _isMate(false), _disambiguation(""), _flag(flag)
	, _fullMove(0), _movedPiece(orig->getPiece()), _capturedPiece(dest->getPiece()) 
{
	if (_movedPiece->getType() == PieceType::PAWN && isCapture())
	{
		_disambiguation = 'a' + _orig->getFile();
	}
}

HalfMove::HalfMove(HalfMove& fromMove, ChessBoard* board)
{
	_orig = board->getSquare(std::string(*fromMove.getOrigin()));
	_dest = board->getSquare(std::string(*fromMove.getDestination()));
	_isCheck = fromMove._isCheck;
	_isMate = fromMove._isMate;
	_disambiguation = fromMove._disambiguation;
	_flag = fromMove._flag;
	_fullMove = fromMove._fullMove;
	_movedPiece = _orig->getPiece();
	_capturedPiece = _dest->getPiece();
}

void HalfMove::setDisambiguation(std::string disambiguation)
{
	_disambiguation = disambiguation;
}

void HalfMove::setCheck(bool isCheck)
{
	_isCheck = isCheck;
}

void HalfMove::setMate(bool isMate)
{
	_isMate = isMate;
}

void HalfMove::setFullMove(int fullMove)
{
	_fullMove = fullMove;
}

void HalfMove::setCapturedPiece(std::shared_ptr<Piece> piece)
{
	_capturedPiece = piece;
}

Square* HalfMove::getOrigin()
{
	return _orig;
}

Square* HalfMove::getDestination()
{
	return _dest;
}

std::string HalfMove::_toString() const
{
	if (_flag == 'o') {
		return "O-O";
	}
	if (_flag == 'O') {
		return "O-O-O";
	}

    std::ostringstream oss(std::ostringstream::ate);
	PieceType type = _movedPiece->getType();

	if (type != PieceType::PAWN)
	{
		oss << (char)type;
	}

	oss << _disambiguation;
	
	if (isCapture()) 
	{
		oss << 'x';
	}

	oss << *_dest;

	if (type == PieceType::PAWN && _flag)
	{
		if (_flag && _flag != 'e')
			oss << '=' << _flag;
		else
			oss << "(e.p.)";
	}

	if (_isCheck)
	{
		if (_isMate)
		{
			oss << '#';
		}
		else {
			oss << '+';
		}
	}

	return oss.str();
}

bool HalfMove::isCapture() const
{
	return (_capturedPiece && _movedPiece->getColor() != _capturedPiece->getColor()) || _isEnPassantCapture();
}

void HalfMove::printMove()
{
	std::ostringstream oss(std::ostringstream::ate);
	if (_fullMove != 0 && _movedPiece->getColor() == Color::WHITE)
	{
		oss << _fullMove;
		oss << ". ";
	}
	oss << _toString();
	oss << " ";
	std::cout << oss.str();
}

bool HalfMove::_isEnPassantCapture() const
{
	return _flag == 'e';
}

std::ostream& operator<<(std::ostream& os, const HalfMove& halfMove)
{
	os << halfMove._toString();
	return os;
}

HalfMove::operator std::string() const
{	
	return _toString();
}

