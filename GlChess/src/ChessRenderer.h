#pragma once
#include "GlChess.h"
#include "ChessLogic/ChessBoard.h"

class ChessRenderer {
public:
	ChessRenderer(std::unique_ptr<Lighthouse::Window>& window) 
		: _window(window), _windowWidth(window->getWidth()), _windowHeight(window->getHeight()), _windowOffsetX(0), _windowOffsetY(0) {}
	virtual ~ChessRenderer() = default;

	virtual void buildScene() = 0;
	virtual void onUpdate()   = 0;

	void updateWindowSize(unsigned int width, unsigned int height) { _windowWidth = width; _windowHeight = height; }
	void updateWindowPos(unsigned int x, unsigned int y) { _windowOffsetX = x; _windowOffsetY = y; }

	ChessBoard* getBoard() { return &_board; }
	unsigned int getViewedMove() { return _viewedMove; }
	virtual void showMove(int number) = 0;
	virtual void showCurrentMove() = 0;

	// Event handling
	virtual bool onWindowResized      (Lighthouse::WindowResizeEvent& e)        = 0;
	virtual bool onKeyPressed         (Lighthouse::KeyPressedEvent& e)          = 0;
	virtual bool onKeyReleased        (Lighthouse::KeyReleasedEvent& e)         = 0;
	virtual bool onMouseMoved         (Lighthouse::MouseMovedEvent& e)          = 0;
	virtual bool onMouseButtonPressed (Lighthouse::MouseButtonPressedEvent& e)  = 0;
	virtual bool onMouseButtonReleased(Lighthouse::MouseButtonReleasedEvent& e) = 0;

protected:
	ChessBoard _board;
	ChessBoard _viewedBoard;
	unsigned int _viewedMove = 0;

	std::unique_ptr<Lighthouse::Window>& _window;
	unsigned int _windowOffsetX;
	unsigned int _windowOffsetY;
	unsigned int _windowWidth;
	unsigned int _windowHeight;
};
