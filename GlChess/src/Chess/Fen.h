#pragma once

#include "ChessBoard.h"
#include <string>

class ChessBoard;

class Fen
{
public:
	Fen();
	Fen(ChessBoard& board);
	Fen(std::string code);

	std::string getString() const;
	std::string getPosition() const;

	inline static std::string const STARTING_POSITION = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
	//inline static std::string const STARTING_POSITION = "P6P/1P4P1/2P2P2/3PP3/3PP3/2P2P2/1P4P1/P6P w - - 0 1";

private:
	std::string _code;
};
