#pragma once

#include <vector>
#include <string>
#include <stdexcept>

class Square;

enum PieceType {
	PAWN = 'P',
	KNIGHT = 'N',
	BISHOP = 'B',
	ROOK = 'R',
	QUEEN = 'Q',
	KING = 'K'
};

enum Color {
	BLACK,
	WHITE
};

class Piece {
public:
	Piece(char type, Square* square = nullptr);

	PieceType getType() { return _type; }
	Color getColor() { return _color; }
	Square* getSquare();

	void setSquare(Square* square) { _square = square; }
	void setType(PieceType type) { _type = type; }
	void setName(std::string name) { _name = name; }

	std::string getName();

	friend std::ostream& operator<<(std::ostream& os, const Piece& piece);
	operator std::string() const;
	char toChar() const;

private:
	PieceType _type;
	Color _color;

	std::string _name;

	Square* _square;

};
