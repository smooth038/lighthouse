#include "Fen.h"
#include <sstream>

Fen::Fen()
	: _code(STARTING_POSITION)
{
}

Fen::Fen(ChessBoard& board)
	: Fen(board.toFen())
{
}

Fen::Fen(std::string code)
	: _code(code)
{
}

std::string Fen::getString() const
{
	return _code;
}

std::string Fen::getPosition() const
{
	std::istringstream iss(_code);
	std::string position;
	std::getline(iss, position, ' ');
	return position;
}
