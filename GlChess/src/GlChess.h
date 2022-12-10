#pragma once

#include "Lighthouse.h"
#include "ChessRenderer3D.h"
#include "ChessLogic/Chessboard.h"
#include <chrono>

class ChessRenderer;

class GlChess : public Lighthouse::Layer
{
public:
	GlChess(std::unique_ptr<Lighthouse::Window>& window);
	virtual ~GlChess() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;
	virtual void onUpdate() override;
	virtual void onEvent(Lighthouse::Event& e) override;
	virtual void onImGuiRender() override;

private:
	std::unique_ptr<ChessRenderer> _chessRenderer;

	void _newGame();
	void _renderDockSpace();
	void _renderBoardWindow();
	void _renderNotationWindow();
	void _renderMenuBar();
	void _renderPromotionDialog();
	void _renderFps();

	// Event handling
	bool _onWindowResized(Lighthouse::WindowResizeEvent& e);
	bool _onKeyPressed(Lighthouse::KeyPressedEvent& e);
	bool _onKeyReleased(Lighthouse::KeyReleasedEvent& e);
	bool _onMouseMoved(Lighthouse::MouseMovedEvent& e);
	bool _onMouseButtonPressed(Lighthouse::MouseButtonPressedEvent& e);
	bool _onMouseButtonReleased(Lighthouse::MouseButtonReleasedEvent& e);

	std::chrono::high_resolution_clock::time_point _timeSinceLast20Frames;
	int _frameCount = 0;
	float _lastFps = 0.0f;
};
