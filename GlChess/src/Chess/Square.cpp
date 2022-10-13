#include "Square.h"
#include <sstream>

Square::Square(std::string squareName, std::shared_ptr<Piece> piece, Square* ref)
	: _piece(piece), _file(-1), _rank(-1)
{
	if (squareName != "-") 
	{
		if (squareName.length() != 2) throw std::invalid_argument(std::string("Invalid square: ") + squareName);
		char f = squareName[0];
		char r = squareName[1];
		if (f < 'a' || f > 'h' || r < '1' || r > '8') throw std::invalid_argument(std::string("Invalid square: ") + squareName);
		_file = f - 'a';
		_rank = r - '1';
	}

	if (_piece != nullptr)
	{
		_piece->setSquare(ref);
	}
}

Square::Square(char file, char rank, std::shared_ptr<Piece> piece, Square* ref)
	: _piece(piece)
{
	std::ostringstream oss(std::ostringstream::ate);
	if (file < 0 || file > 7) throw std::invalid_argument(std::string("Invalid file: ") + file);
	if (rank < 0 || rank > 7) throw std::invalid_argument(std::string("Invalid rank: ") + rank);
	_file = file;
	_rank = rank;

	if (_piece != nullptr)
	{
		_piece->setSquare(ref);
	}
}

char Square::getFile() const
{
	return _file;
}

char Square::getRank() const
{
	return _rank;
}

SquareType Square::getSquareType()
{
	return (_file + _rank) % 2 == 0 ? SquareType::DARK : SquareType::LIGHT;
}

std::shared_ptr<Piece> Square::getPiece()
{
	return _piece;
}

void Square::setPiece(std::shared_ptr<Piece> piece)
{
	_piece = piece;
}

Square::operator std::string() const
{	
	char f = 'a' + _file;
	char r = '1' + _rank;
	std::ostringstream oss(std::ostringstream::ate);
	oss << f;
	oss << r;
	return oss.str();
}

std::ostream& operator<<(std::ostream& os, const Square& square)
{
	os << std::string(square);
	return os;
}
