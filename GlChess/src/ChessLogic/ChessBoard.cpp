#pragma once 

#include <sstream>
#include <iostream>
#include <exception>
#include <memory>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include "ChessBoard.h"
    
const Fen ChessBoard::STARTING_POSITION = Fen(Fen::STARTING_POSITION);

ChessBoard::ChessBoard() : ChessBoard(STARTING_POSITION)
{
}

ChessBoard::ChessBoard(const Fen& fen)
{
    std::istringstream iss(fen.getString());

    std::string piecePlacement;
    std::getline(iss, piecePlacement, ' ');

    std::string activeColor;
    std::getline(iss, activeColor, ' ');
    if (activeColor != "w" && activeColor != "b")
    {
        throw std::invalid_argument("Active color field should be either 'w' or 'b', but was " + activeColor + ".");
    }
    _activePlayerColor = activeColor == "w" ? Color::WHITE : Color::BLACK;

    std::string castlingRights;
    std::getline(iss, castlingRights, ' ');
    std::string validFields[16] = { "-", "q", "k", "kq", "Q", "Qq", "Qk", "Qkq", "K", "Kq", "Kk", "Kkq", "KQ", "KQq", "KQk", "KQkq" };
    bool found = false;
    for (int i = 0; i < 16; i++)
    {
        if (validFields[i] == castlingRights)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        throw std::invalid_argument("Invalid castling right field: " + castlingRights);
    }
    _isWhiteCastleShortAllowed = castlingRights.find("K") != std::string::npos;
    _isWhiteCastleLongAllowed  = castlingRights.find("Q") != std::string::npos;
    _isBlackCastleShortAllowed = castlingRights.find("k") != std::string::npos;
    _isBlackCastleLongAllowed  = castlingRights.find("q") != std::string::npos;
    
    std::string enPassantSquare;
    std::getline(iss, enPassantSquare, ' ');
    Square tmp = Square(enPassantSquare);
    if (tmp.getFile() == -1)
        _enPassantSquare = nullptr;
    else
		_enPassantSquare = &_squares[tmp.getFile()][tmp.getRank()];

    std::string halfmoveClock;
    std::getline(iss, halfmoveClock, ' ');
    _halfmoveClock = std::stoi(halfmoveClock);

    std::string fullmoveNumber;
    std::getline(iss, fullmoveNumber, ' ');
    _fullmoveNumber = std::stoi(fullmoveNumber);

    iss = std::istringstream(piecePlacement);
    std::string rankData;
    char rank = 7;
    while (std::getline(iss, rankData, '/'))
    {
        char file = 0;

        for (char c : rankData) 
        {
            if (c > '0' && c <= '9')
            {
                int nextFile = file + (c - '0');
                for (; file < nextFile; file++)
                {
                    if (file > 7) break;
                    _squares[file][rank] = Square(file, rank);
                }
                continue;
            }

            _squares[file][rank] = Square(file, rank, std::make_shared<Piece>(c), &_squares[file][rank]);
            std::shared_ptr<Piece> p = _squares[file][rank].getPiece();
            p->setName(generatePieceName(p->getColor(), p->getType()));
            if (++file > 7) break;
        }

        if (--rank < 0) break;
    }

    _history.push_back(fen);
}

std::string ChessBoard::getPieceStringType(PieceType type)
{
	switch (type)
	{
	case PieceType::PAWN:
		return "pawn";
	case PieceType::KNIGHT:
		return "knight";
	case PieceType::BISHOP:
		return "bishop";
	case PieceType::ROOK:
		return "rook";
	case PieceType::QUEEN:
		return "queen";
	case PieceType::KING:
		return "king";
	}
}

std::string ChessBoard::generatePieceName(Color color, PieceType type)
{
	std::string stringColor = color == Color::WHITE ? "white" : "black";
    std::string stringType = getPieceStringType(type);

	std::string name = stringColor + "_" + stringType;

	if (type != PieceType::KING)
	{
		if (!_pieceCount.count(name))
		{
			_pieceCount.insert(std::make_pair(name, 1));
		}
		else
		{
			_pieceCount[name] += 1;
		}
		name += "_" + std::to_string(_pieceCount[name]);
	}

    return name;
}

ChessBoard::~ChessBoard()
{
}

Fen ChessBoard::toFen()
{
    std::ostringstream oss(std::ostringstream::ate);

    int rank = 7;
    int file = 0;
    while (rank >= 0)
    {
        file = 0;
        int spaceCount = 0;
        while (file < 8)
        {
            if (!_squares[file][rank].getPiece())
            {
                spaceCount++;
            }
            else
            {
                if (spaceCount > 0)
                {
                    oss << spaceCount;
                    spaceCount = 0;
                }
                std::shared_ptr<Piece> p = _squares[file][rank].getPiece();
                oss << std::string(*p);
            }
            file++;
        }
        if (spaceCount > 0)
        {
            oss << spaceCount;
spaceCount = 0;
        }
        if (rank > 0)
        {
            oss << "/";
        }
        rank--;
    }

    char activeColor = _activePlayerColor == Color::WHITE ? 'w' : 'b';
    oss << " " << activeColor << " ";

    if (!_isWhiteCastleShortAllowed && !_isWhiteCastleLongAllowed && !_isBlackCastleShortAllowed && !_isBlackCastleLongAllowed)
    {
        oss << "-";
    }
    else
    {
        if (_isWhiteCastleShortAllowed) oss << "K";
        if (_isWhiteCastleLongAllowed)  oss << "Q";
        if (_isBlackCastleShortAllowed) oss << "k";
        if (_isBlackCastleLongAllowed)  oss << "q";
    }

    std::string enPassantSquare = _enPassantSquare ? std::string(*_enPassantSquare) : "-";
    oss << " " << enPassantSquare;
    oss << " " << _halfmoveClock;
    oss << " " << _fullmoveNumber;

    return Fen(oss.str());
}

AllSquaresIterator ChessBoard::begin()
{
    return AllSquaresIterator(&_squares[0][0]);
}

AllSquaresIterator ChessBoard::end()
{
    return ++AllSquaresIterator(&_squares[7][7]);
}

void ChessBoard::printBoard()
{
    std::string hl = "+---+---+---+---+---+---+---+---+";
    std::cout << hl << std::endl;
    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << "|";
        for (int file = 0; file < 8; file++)
        {
            char c = _squares[file][rank].getPiece() != nullptr ? _squares[file][rank].getPiece()->toChar() : ' ';
            std::cout << " " << c << " |";
        }
        std::cout << std::endl << hl << std::endl;
    }
}

void ChessBoard::printGame()
{
    for (HalfMove move : _moves)
    {
        move.printMove();
    }
    std::cout << std::endl;
}

Square* ChessBoard::getSquare(std::string squareName)
{
    if (squareName.length() != 2) throw std::invalid_argument(std::string("Invalid square: ") + squareName);
    char f = squareName[0] - 'a';
    char r = squareName[1] - '1';
    if (f < 0 || f > 7) throw std::invalid_argument(std::string("Invalid square: ") + squareName);
    if (r < 0 || r > 7) throw std::invalid_argument(std::string("Invalid square: ") + squareName);
    return &_squares[f][r];
}

std::vector<std::shared_ptr<Piece>> ChessBoard::getPlayerPieces(Color color)
{
    std::vector<std::shared_ptr<Piece>> pieces;
    for (auto it = begin(); it != end(); it++)
    {
        if (it->getPiece() && it->getPiece()->getColor() == color) pieces.push_back(it->getPiece());
    }
    return pieces;
}

std::vector<std::shared_ptr<Piece>> ChessBoard::getAllPieces()
{
    std::vector<std::shared_ptr<Piece>> pieces;
    for (auto it = begin(); it != end(); it++)
    {
        if (it->getPiece()) pieces.push_back(std::move(it->getPiece()));
    }
    return pieces;
}

std::vector<HalfMove> ChessBoard::getLegalMoves()
{
    if (_shouldRecalculateLegalMoves)
    {
        _recalculateLegalMoves();
        _shouldRecalculateLegalMoves = false;
    }
    return _legalMoves;
}

void ChessBoard::_recalculateLegalMoves()
{
    std::vector<HalfMove> allLegalMoves;
    _getRawMoves(allLegalMoves);
	_getCastleMoves(allLegalMoves);
    _removeEnemyChecks(allLegalMoves);
    _applyDisambiguation(allLegalMoves);
    _applyChecks(allLegalMoves);

    _legalMoves = allLegalMoves;
}

void ChessBoard::makeMove(HalfMove& move)
{
    move.setFullMove(_fullmoveNumber);
    _moves.push_back(move);

    Square* orig = &_squares[move.getOrigin()->getFile()][move.getOrigin()->getRank()];
    Square* dest = &_squares[move.getDestination()->getFile()][move.getDestination()->getRank()];

    std::shared_ptr<Piece> piece = orig->getPiece();

    if (piece->getType() == PieceType::KING)
    {
        if (piece->getColor() == Color::WHITE)
        {
            _isWhiteCastleShortAllowed = false;
            _isWhiteCastleLongAllowed = false;
        }
        else
        {
            _isBlackCastleShortAllowed = false;
            _isBlackCastleLongAllowed = false;
        }
    }
    if (orig == &_squares[0][0] || dest == &_squares[0][0])
    {
        _isWhiteCastleLongAllowed = false;
    }
    if (orig == &_squares[7][0] || dest == &_squares[7][0])
    {
        _isWhiteCastleShortAllowed = false;
    }
    if (orig == &_squares[0][7] || dest == &_squares[0][7])
    {
        _isBlackCastleLongAllowed = false;
    }
    if (orig == &_squares[7][7] || dest == &_squares[7][7])
    {
        _isBlackCastleShortAllowed = false;
    }

    if (move.isCapture() || piece->getType() == PieceType::PAWN)
    {
        _halfmoveClock = 0;
    }
    else
    {
        _halfmoveClock++;
    }

    if (piece->getColor() == Color::BLACK)
    {
        _fullmoveNumber++;
    }

    if (piece->getType() == PieceType::PAWN && abs(dest->getRank() - orig->getRank()) == 2)
    {
        char r = piece->getColor() == Color::WHITE ? 2 : 5;
        _enPassantSquare = &_squares[orig->getFile()][r];
    }
    else
    {
        _enPassantSquare = nullptr;
    }

    _squares[dest->getFile()][dest->getRank()].setPiece(piece);
    _squares[orig->getFile()][orig->getRank()].setPiece(nullptr);
    piece->setSquare(&_squares[dest->getFile()][dest->getRank()]);
    _cycleColor();

	switch (move.getFlag())
	{
    case 'e':
        // en passant
		{
			char capturedPawnRank = piece->getColor() == Color::WHITE ? 4 : 3;
			_squares[dest->getFile()][capturedPawnRank].setPiece(nullptr);
		}
        break;
    case 'o':
		{
        // short castle
			std::shared_ptr<Piece> king = piece;
            char rank = piece->getColor() == Color::WHITE ? 0: 7;
			std::shared_ptr<Piece> rook = _squares[7][rank].getPiece();
            king->setSquare(&_squares[6][rank]);
            rook->setSquare(&_squares[5][rank]);
            _squares[6][rank].setPiece(king);
            _squares[5][rank].setPiece(rook);
            _squares[4][rank].setPiece(nullptr);
            _squares[7][rank].setPiece(nullptr);
		}
        break;
    case 'O':
		{
        // long castle
			std::shared_ptr<Piece> king = piece;
            char rank = piece->getColor() == Color::WHITE ? 0 : 7;
            std::shared_ptr<Piece> rook = _squares[0][rank].getPiece();
            king->setSquare(&_squares[2][rank]);
            rook->setSquare(&_squares[3][rank]);
            _squares[2][rank].setPiece(king);
            _squares[3][rank].setPiece(rook);
            _squares[4][rank].setPiece(nullptr);
            _squares[0][rank].setPiece(nullptr);
		}
        break;
    case 'N':
        piece->setType(PieceType::KNIGHT);
        break;
    case 'B':
        piece->setType(PieceType::BISHOP);
        break;
    case 'R':
        piece->setType(PieceType::ROOK);
        break;
    case 'Q':
        piece->setType(PieceType::QUEEN);
        break;
    default:
        break;
	}

    Fen fen = toFen();
    _history.push_back(toFen());
    _shouldRecalculateLegalMoves = true;
}

bool ChessBoard::makeMove(Square* origin, Square* destination, char promotion)
{
    std::vector<HalfMove> allLegalMoves = getLegalMoves();
    std::vector<HalfMove> candidates;
    std::copy_if(allLegalMoves.begin(), allLegalMoves.end(),
       std::back_inserter(candidates), [origin, destination](HalfMove& move) {
            return move.getOrigin() == origin && move.getDestination() == destination; 
        }
    );

	if (candidates.size() == 1)
	{
		makeMove(candidates[0]);
		return true;

	}

	if (candidates.size() > 1)
	{
        auto move = std::find_if(candidates.begin(), candidates.end(), [promotion](HalfMove& move) {
            return move.getFlag() == promotion;
        });
        if (move != std::end(candidates))
        {
            makeMove(*move);
            return true;
        }
	}

    return false;
}

void ChessBoard::_cycleColor()
{
    _activePlayerColor = _activePlayerColor == Color::WHITE ? Color::BLACK : Color::WHITE;
    _shouldRecalculateLegalMoves = true;
}

void ChessBoard::_getRawMoves(std::vector<HalfMove>& moves)
{
    for (std::shared_ptr<Piece> piece : getPlayerPieces(_activePlayerColor))
    {
        switch (piece->getType())
        {
        case PieceType::PAWN:            
            _getPawnMoves(piece, moves);
            break;
        case PieceType::KNIGHT:
            _getKnightMoves(piece, moves);
            break;
        case PieceType::BISHOP:
            _getBishopMoves(piece, moves);
            break;
        case PieceType::ROOK:
            _getRookMoves(piece, moves);
            break;
        case PieceType::QUEEN:
            _getQueenMoves(piece, moves);
            break;
        case PieceType::KING:
            _getKingMoves(piece, moves);
            break;
        }
    }
}

void ChessBoard::_getPawnMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves)
{
	Square* orig = piece->getSquare();
	unsigned char r = orig->getRank();
	unsigned char f = orig->getFile();
	Color activePieceColor = piece->getColor();
	bool isWhite = activePieceColor == Color::WHITE;
	bool isBlack = !isWhite;

	bool canAdvanceOneSquare = (isWhite && r < 7 && !_squares[f][r + 1].getPiece())
		|| (isBlack && r > 0 && !_squares[f][r - 1].getPiece());
	if (canAdvanceOneSquare)
	{
		Square* dest = &_squares[f][isWhite ? r + 1 : r - 1];
		moves.push_back(HalfMove(orig, dest));
	}
	bool canAdvanceTwoSquares = canAdvanceOneSquare && 
		((isWhite && r == 1 && !_squares[f][3].getPiece())
		|| (isBlack && r == 6 && !_squares[f][4].getPiece()));
	if (canAdvanceTwoSquares) {
		Square* dest = &_squares[f][isWhite ? 3 : 4];
		moves.push_back(HalfMove(orig, dest));
	}
	bool canAdvanceToPromotion = canAdvanceOneSquare && (isWhite && r == 6) || (isBlack && r == 1);
	if (canAdvanceToPromotion) {
		Square* dest = &_squares[f][isWhite ? 7 : 0];
		moves.push_back(HalfMove(orig, dest, 'N'));
		moves.push_back(HalfMove(orig, dest, 'B'));
		moves.push_back(HalfMove(orig, dest, 'R'));
		moves.push_back(HalfMove(orig, dest, 'Q'));
	}
	Square* leftCaptureSquare = f > 0 && r > 0 && r < 7 ? &_squares[f - 1][isWhite ? r + 1 : r - 1] : nullptr;
	Square* rightCaptureSquare = f < 7 && r > 0 && r < 7 ? &_squares[f + 1][isWhite ? r + 1 : r - 1] : nullptr;
	bool canCaptureEnPassantLeft = _enPassantSquare && _enPassantSquare == leftCaptureSquare;
	bool canCaptureEnPassantRight = _enPassantSquare && _enPassantSquare == rightCaptureSquare;
	bool canCaptureLeft = leftCaptureSquare && f > 0 
        && (_isEnemyPiece(activePieceColor, leftCaptureSquare) || canCaptureEnPassantLeft);
	bool canCaptureRight = rightCaptureSquare && f < 7 
        && (_isEnemyPiece(activePieceColor, rightCaptureSquare) || canCaptureEnPassantRight);
	bool wouldPromote = isWhite ? r == 6 : r == 1;

	if (canCaptureLeft)
	{
		Square* dest = leftCaptureSquare;
		if (wouldPromote)
		{
			moves.push_back(HalfMove(orig, dest, 'N'));
			moves.push_back(HalfMove(orig, dest, 'B'));
			moves.push_back(HalfMove(orig, dest, 'R'));
			moves.push_back(HalfMove(orig, dest, 'Q'));
		}
        else
        {
            if (canCaptureEnPassantLeft)
            {
                HalfMove enPassant = HalfMove(orig, dest, 'e');
                enPassant.setCapturedPiece(dest->getPiece());
                moves.push_back(enPassant);
            }
            else
            {
                moves.push_back(HalfMove(orig, dest));
            }
        }
	}
	if (canCaptureRight)
	{
		Square* dest = rightCaptureSquare;
		if (wouldPromote)
		{
			moves.push_back(HalfMove(orig, dest, 'N'));
			moves.push_back(HalfMove(orig, dest, 'B'));
			moves.push_back(HalfMove(orig, dest, 'R'));
			moves.push_back(HalfMove(orig, dest, 'Q'));
		}
        else
        {
            if (canCaptureEnPassantRight)
            {
                HalfMove enPassant = HalfMove(orig, dest, 'e');
                enPassant.setCapturedPiece(dest->getPiece());
                moves.push_back(enPassant);
            }
            else
            {
                moves.push_back(HalfMove(orig, dest));
            }
        }
	}
}
void ChessBoard::_getKnightMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves)
{
	Square* orig = piece->getSquare();
	unsigned char r = orig->getRank();
	unsigned char f = orig->getFile();
	Color activePieceColor = piece->getColor();

	std::vector<Square*> validSquares;
	if (f < 7 && r < 6) validSquares.push_back(&_squares[f + 1][r + 2]);
	if (f < 6 && r < 7) validSquares.push_back(&_squares[f + 2][r + 1]);
	if (f < 6 && r > 0) validSquares.push_back(&_squares[f + 2][r - 1]);
	if (f < 7 && r > 1) validSquares.push_back(&_squares[f + 1][r - 2]);
	if (f > 0 && r > 1) validSquares.push_back(&_squares[f - 1][r - 2]);
	if (f > 1 && r > 0) validSquares.push_back(&_squares[f - 2][r - 1]);
	if (f > 1 && r < 7) validSquares.push_back(&_squares[f - 2][r + 1]);
	if (f > 0 && r < 6) validSquares.push_back(&_squares[f - 1][r + 2]);
	for (auto dest : validSquares)
	{
		std::shared_ptr<Piece> p = dest->getPiece();
		if (!p || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
	}
}

void ChessBoard::_getBishopMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves)
{
	Square* orig = piece->getSquare();
	unsigned char r = orig->getRank();
	unsigned char f = orig->getFile();
	Color activePieceColor = piece->getColor();

    for (char file = f + 1, rank = r + 1; file <= 7 && rank <= 7; file++, rank++)
    {
        Square* dest = &_squares[file][rank];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
    for (char file = f + 1, rank = r - 1; file <= 7 && rank >= 0; file++, rank--)
    {
        Square* dest = &_squares[file][rank];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
    for (char file = f - 1, rank = r - 1; file >= 0 && rank >= 0; file--, rank--)
    {
        Square* dest = &_squares[file][rank];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
    for (char file = f - 1, rank = r + 1; file >= 0 && rank <= 7; file--, rank++)
    {
        Square* dest = &_squares[file][rank];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
}

void ChessBoard::_getRookMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves)
{
	Square* orig = piece->getSquare();
	unsigned char r = orig->getRank();
	unsigned char f = orig->getFile();
	Color activePieceColor = piece->getColor();

    for (char file = f + 1; file <= 7; file++)
    {
        Square* dest = &_squares[file][r];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
    for (char rank = r - 1; rank >= 0; rank--)
    {
        Square* dest = &_squares[f][rank];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
    for (char file = f - 1; file >= 0; file--)
    {
        Square* dest = &_squares[file][r];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;
    }
    for (char rank = r + 1; rank <= 7; rank++)
    {
        Square* dest = &_squares[f][rank];
		if (!dest->getPiece() || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
        if (dest->getPiece()) break;

    }
}

void ChessBoard::_getQueenMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves)
{
    _getBishopMoves(piece, moves);
    _getRookMoves(piece, moves);
}

void ChessBoard::_getKingMoves(std::shared_ptr<Piece> piece, std::vector<HalfMove>& moves)
{
	Square* orig = piece->getSquare();
	unsigned char r = orig->getRank();
	unsigned char f = orig->getFile();
	Color activePieceColor = piece->getColor();

	std::vector<Square*> validSquares;
	if (f < 7 && r < 7) validSquares.push_back(&_squares[f + 1][r + 1]);
	if (f < 7) validSquares.push_back(&_squares[f + 1][r]);
	if (f < 7 && r > 0) validSquares.push_back(&_squares[f + 1][r - 1]);
	if (r > 0) validSquares.push_back(&_squares[f][r - 1]);
	if (f > 0 && r > 0) validSquares.push_back(&_squares[f - 1][r - 1]);
	if (f > 0) validSquares.push_back(&_squares[f - 1][r]);
	if (f > 0 && r < 7) validSquares.push_back(&_squares[f - 1][r + 1]);
	if (r < 7) validSquares.push_back(&_squares[f][r + 1]);
	for (auto dest : validSquares)
	{
		std::shared_ptr<Piece> p = dest->getPiece();
		if (!p || _isEnemyPiece(activePieceColor, dest)) 
            moves.push_back(HalfMove(orig, dest));
	}
}

void ChessBoard::_getCastleMoves(std::vector<HalfMove>& moves)
{
    if (_isCheck()) return;

    bool isShortCastleAllowed = (_activePlayerColor == Color::WHITE && _isWhiteCastleShortAllowed)
        || (_activePlayerColor == Color::BLACK && _isBlackCastleShortAllowed);

    bool isLongCastleAllowed = (_activePlayerColor == Color::WHITE && _isWhiteCastleLongAllowed)
        || (_activePlayerColor == Color::BLACK && _isBlackCastleLongAllowed);

	char r = _activePlayerColor == Color::WHITE ? 0 : 7;

    // TODO: Check for mistakes in castling logic. There seems to be some doubtful stuff here.
	if (isShortCastleAllowed 
        && !_squares[5][r].getPiece() 
        && !_squares[6][r].getPiece())
	{
		Fen fen = toFen();
		ChessBoard copy = ChessBoard(fen);
		HalfMove oneSquare = HalfMove(&_squares[4][r], &_squares[5][r]);
		copy.makeMove(oneSquare);
		copy._cycleColor();
		bool oneSquareSafe = !copy._isCheck();
        if (oneSquareSafe)
        {
            HalfMove castleShort = HalfMove(&_squares[4][r], &_squares[6][r], 'o');
            moves.push_back(castleShort);
        }
	}
	if (isLongCastleAllowed 
        && !_squares[3][r].getPiece()
        && !_squares[2][r].getPiece()
        && !_squares[1][r].getPiece())
	{
		Fen fen = toFen();
		ChessBoard copy = ChessBoard(fen);
		HalfMove oneSquare = HalfMove(&copy._squares[4][r], &copy._squares[3][r]);
		copy.makeMove(oneSquare);
		copy._cycleColor();
		bool oneSquareSafe = !copy._isCheck();
		HalfMove twoSquares = HalfMove(&copy._squares[3][r], &copy._squares[2][r]);
		copy.makeMove(twoSquares);
		copy._cycleColor();
		bool twoSquaresSafe = !copy._isCheck();
		if (oneSquareSafe && twoSquaresSafe)
		{
			HalfMove castleLong = HalfMove(&_squares[4][r], &_squares[2][r], 'O');
			moves.push_back(castleLong);
		}
	}
}

bool ChessBoard::_isEnemyPiece(Color activeColor, Square* square) const
{
    if (!square->getPiece()) return false;
	bool isWhite = activeColor == Color::WHITE;
	bool isBlack = !isWhite;
    Color pieceColor = square->getPiece()->getColor();
    return (isWhite && pieceColor == Color::BLACK || (isBlack && pieceColor == Color::WHITE));
}

bool ChessBoard::_isFriendPiece(Color activeColor, Square* square) const
{
    return square->getPiece() && !_isEnemyPiece(activeColor, square);
}

void ChessBoard::_removeEnemyChecks(std::vector<HalfMove>& moves)
{
	Fen fen = toFen();
	moves.erase(std::remove_if(moves.begin(), moves.end(), [fen, this](HalfMove& move) {
        ChessBoard copy = ChessBoard(fen);
        HalfMove m(move, this);
        copy.makeMove(m);
        copy._cycleColor();
        return copy._isCheck();
	}), moves.end());
}

void ChessBoard::_applyDisambiguation(std::vector<HalfMove>& moves)
{
    if (moves.size() == 0) return;

	std::map<std::string,std::set<HalfMove*>> duplicates;
    for (auto it1 = moves.begin(); it1 < moves.end() - 1; it1++) 
    {
        if (duplicates.find(std::string(*it1)) != duplicates.end()) continue;
        for (auto it2 = it1 + 1; it2 < moves.end(); it2++)
        {
            if (std::string(*it1) == std::string(*it2)) {
                if (duplicates.find(std::string(*it1)) == duplicates.end())
                {
					std::set<HalfMove*> homonyms;
                    homonyms.insert(&*it1);
                    homonyms.insert(&*it2);
                    duplicates.insert(std::make_pair(std::string(*it1), homonyms));
                }
                else
                {
                    duplicates[std::string(*it1)].insert(&*it2);
                }
            }
        }
    }

    for (auto it = duplicates.begin(); it != duplicates.end(); it++)
    {
        std::set<HalfMove*> temp = it->second;
        std::vector<HalfMove*> homonyms(temp.begin(), temp.end());
        for (auto moveA = homonyms.begin(); moveA != homonyms.end(); moveA++)
        {
			HalfMove* a = *moveA;
            bool isFileUnique = true;
            bool isRankUnique = true;
            for (auto moveB = homonyms.begin(); moveB != homonyms.end(); moveB++)
            {
                if (*moveA == *moveB) continue;
                HalfMove* b = *moveB;
                if (a->getOrigin()->getFile() == b->getOrigin()->getFile()) isFileUnique = false;
                if (a->getOrigin()->getRank() == b->getOrigin()->getRank()) isRankUnique = false;
            }

			std::string disambiguation;
            if (isFileUnique)
            {
                char file = a->getOrigin()->getFile() + 'a';
                disambiguation += file;
            }
            else if (isRankUnique)
            {
                char rank = a->getOrigin()->getRank() + '1';
                disambiguation += rank;
            }
            else
            {
                char file = a->getOrigin()->getFile() + 'a';
                char rank = a->getOrigin()->getRank() + '1';
                (disambiguation += file) += rank;
            }
            a->setDisambiguation(disambiguation);
        }
    }
}

void ChessBoard::_applyChecks(std::vector<HalfMove>& moves)
{
    if (moves.size() == 0) return;

	Fen fen = toFen();
    for (HalfMove& move : moves)
    {
        ChessBoard copy = ChessBoard(fen);
        HalfMove m(move);
        copy.makeMove(m);
        if (copy._isCheck())
        {
			move.setCheck(true);
			move.setMate(copy._isMate(false));
        }
    }
}

bool ChessBoard::_isCheck()
{
    std::vector<HalfMove> checkingColorNextPossibleMoves;
    Fen fen = toFen();
    ChessBoard copy = ChessBoard(fen);
    copy._cycleColor();
    copy._getRawMoves( checkingColorNextPossibleMoves);

    for (HalfMove& move : checkingColorNextPossibleMoves)
    {
        if (move.getDestination()->getPiece() && move.getDestination()->getPiece()->getType() == PieceType::KING)
        {
            return true;
        }
    }
    return false;
}

bool ChessBoard::_isMate(bool skipVerifyCheck)
{
    if (!skipVerifyCheck && !_isCheck()) return false;

    return getLegalMoves().size() == 0;
}

bool ChessBoard::_isDraw()
{
    if (_halfmoveClock >= 100) {
        std::cout << "Draw by 50 move rule." << std::endl;
        return true;
    }
    if (getLegalMoves().size() == 0 && !_isCheck()) {
        std::cout << "Draw by stalemate." << std::endl;
        return true;
    }
    
    std::vector<std::string> positions;
    std::transform(std::begin(_history), std::end(_history), std::back_inserter(positions), [](Fen& fen) {
        return fen.getPosition();
   	});
   
    std::string currentPosition = toFen().getPosition();
    char repeatCount = 0;
    for (std::string position : positions)
    {
        if (position == currentPosition)
        {
            repeatCount++;
		}
    }
    if (repeatCount >= 3)
    {
        std::cout << "Draw by threefold repetition." << std::endl;
        return true;
    }

    std::vector<std::shared_ptr<Piece>> whitePieces = getPlayerPieces(Color::WHITE);
    std::vector<std::shared_ptr<Piece>> blackPieces = getPlayerPieces(Color::BLACK);
    
	std::vector<std::shared_ptr<Piece>> allPieces;
	allPieces.insert(allPieces.end(), whitePieces.begin(), whitePieces.end());
	allPieces.insert(allPieces.end(), blackPieces.begin(), blackPieces.end());

    bool insufficientMaterial = false;
    if (allPieces.size() == 2)
    {
        insufficientMaterial = true;
    }
    else if (allPieces.size() == 3)
    {
        for (auto& piece : allPieces)
        {
            if (piece->getType() == PieceType::BISHOP || piece->getType() == PieceType::KNIGHT)
            {
                insufficientMaterial = true;
            }
        }
    }
    else if (allPieces.size() == 4)
    {
        std::shared_ptr<Piece> memo;
        for (auto& piece : allPieces)
        {
            if (piece->getType() == PieceType::BISHOP)
            {
                if (!memo)
                {
                    memo = piece;
                }
                else if (piece->getColor() != memo->getColor() 
                    && piece->getSquare()->getSquareType() == memo->getSquare()->getSquareType())
                {
					insufficientMaterial = true;
                }
            }
        }
    }
    if (insufficientMaterial)
    {
        std::cout << "Draw by insufficient material." << std::endl;
        return true;
    }

    return false;
}
