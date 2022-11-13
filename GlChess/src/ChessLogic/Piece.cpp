#include "Piece.h"

Piece::Piece(char type, Square* square)
	: _square(square)
{
	std::string validPieces = "PpNnBbRrQqKk";
	bool found = false;
	for (char c : validPieces) 
	{
		if (type == c) 
		{
			found = true;
			break;
		}
	}
	if (!found) throw std::invalid_argument("Cannot instantiate piece with type " + type);

	if (toupper(type) == type) {
		_color = Color::WHITE;
	}
	else
	{
		_color = Color::BLACK;
	}

	switch (toupper(type))
	{
	case 'P':
		_type = PieceType::PAWN;
		break;
	case 'N':
		_type = PieceType::KNIGHT;
		break;
	case 'B':
		_type = PieceType::BISHOP;
		break;
	case 'R':
		_type = PieceType::ROOK;
		break;
	case 'Q':
		_type = PieceType::QUEEN;
		break;
	case 'K':
		_type = PieceType::KING;
		break;
	default:
		throw std::invalid_argument("Piece type is wrong: " + type);
	}
}

std::string Piece::getName()
{
	return _name;
}

char Piece::toChar() const
{
	switch (_type)
	{
	case PieceType::PAWN:
		return _color == Color::WHITE ? 'P' : 'p';
	case PieceType::KNIGHT:
		return _color == Color::WHITE ? 'N' : 'n';
	case PieceType::BISHOP:
		return _color == Color::WHITE ? 'B' : 'b';
	case PieceType::ROOK:
		return _color == Color::WHITE ? 'R' : 'r';
	case PieceType::QUEEN:
		return _color == Color::WHITE ? 'Q' : 'q';
	case PieceType::KING:
		return _color == Color::WHITE ? 'K' : 'k';
	default:
		char type = static_cast<char>(_type);
        throw std::invalid_argument("Invalid piece type: " + type);
	}
}

Square* Piece::getSquare()
{
	return _square;
}

std::ostream& operator<<(std::ostream& os, const Piece& piece)
{
	os << piece.toChar();
	return os;
}

Piece::operator std::string() const
{
	std::string a = std::string(1, toChar());
	return a;
}
