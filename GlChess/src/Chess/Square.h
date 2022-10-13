#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include "Piece.h"

class Piece;

enum SquareType
{
	LIGHT,
	DARK
};

struct Square {
public:
	Square() : _file(0), _rank(0), _piece(nullptr) {}
	Square(std::string squareName, std::shared_ptr<Piece> piece = nullptr, Square* ref = nullptr);
	Square(char file, char rank, std::shared_ptr<Piece> piece = nullptr, Square* ref = nullptr);

	char getFile() const;
	char getRank() const;
	SquareType getSquareType();

	std::shared_ptr<Piece> getPiece();
	void setPiece(std::shared_ptr<Piece> piece);

	operator std::string() const;
	friend std::ostream& operator<<(std::ostream& os, const Square& square);

private:
	char _file;
	char _rank;
	std::shared_ptr<Piece> _piece;
};
