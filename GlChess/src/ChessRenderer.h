#pragma once
#include "GlChess.h"
#include "ChessLogic/ChessBoard.h"

class ChessRenderer {
public:
	ChessRenderer(std::unique_ptr<Lighthouse::Window>& window) : _window(window) {}
	virtual ~ChessRenderer() = default;

	virtual void buildScene() = 0;
	virtual void onUpdate()   = 0;

	// Event handling
	virtual bool onWindowResized      (Lighthouse::WindowResizeEvent& e)        = 0;
	virtual bool onKeyPressed         (Lighthouse::KeyPressedEvent& e)          = 0;
	virtual bool onKeyReleased        (Lighthouse::KeyReleasedEvent& e)         = 0;
	virtual bool onMouseMoved         (Lighthouse::MouseMovedEvent& e)          = 0;
	virtual bool onMouseButtonPressed (Lighthouse::MouseButtonPressedEvent& e)  = 0;
	virtual bool onMouseButtonReleased(Lighthouse::MouseButtonReleasedEvent& e) = 0;

protected:
	std::unique_ptr<Lighthouse::Window>& _window;
};
