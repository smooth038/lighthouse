#include "GlChess.h"
#include <glm/gtc/type_ptr.hpp>

GlChess::GlChess(std::unique_ptr<Lighthouse::Window>& window) 
		: Lighthouse::Layer("GlChess 3D", window), _chessRenderer(std::make_unique<ChessRenderer3D>(window)) 
{
}

void GlChess::onAttach()
{
	LH_INFO("GlChess attached!");
	_chessRenderer->buildScene();
}

void GlChess::onDetach()
{
	LH_INFO("GlChess detached!");
}

void GlChess::onUpdate()
{
	_chessRenderer->onUpdate();
}

void GlChess::onEvent(Lighthouse::Event& event)
{
	Lighthouse::EventDispatcher dispatcher(event);
	dispatcher.dispatch<Lighthouse::KeyPressedEvent>([this](Lighthouse::KeyPressedEvent& e) { return _onKeyPressed(e); });
	dispatcher.dispatch<Lighthouse::KeyReleasedEvent>([this](Lighthouse::KeyReleasedEvent& e) { return _onKeyReleased(e); });
	dispatcher.dispatch<Lighthouse::MouseMovedEvent>([this](Lighthouse::MouseMovedEvent& e) { return _onMouseMoved(e); });
	dispatcher.dispatch<Lighthouse::MouseButtonPressedEvent>([this](Lighthouse::MouseButtonPressedEvent& e) { return _onMouseButtonPressed(e); });
	dispatcher.dispatch<Lighthouse::MouseButtonReleasedEvent>([this](Lighthouse::MouseButtonReleasedEvent& e) { return _onMouseButtonReleased(e); });
	dispatcher.dispatch<Lighthouse::WindowResizeEvent>([this](Lighthouse::WindowResizeEvent& e) { return _onWindowResized(e); });
}

bool GlChess::_onWindowResized(Lighthouse::WindowResizeEvent& e)
{
	return _chessRenderer->onWindowResized(e);
}

bool GlChess::_onKeyPressed(Lighthouse::KeyPressedEvent& e)
{
	return _chessRenderer->onKeyPressed(e);
}

bool GlChess::_onKeyReleased(Lighthouse::KeyReleasedEvent& e)
{
	return _chessRenderer->onKeyReleased(e);
}

bool GlChess::_onMouseMoved(Lighthouse::MouseMovedEvent& e)
{
	return _chessRenderer->onMouseMoved(e);
}

bool GlChess::_onMouseButtonPressed(Lighthouse::MouseButtonPressedEvent& e)
{
	return _chessRenderer->onMouseButtonPressed(e);
}

bool GlChess::_onMouseButtonReleased(Lighthouse::MouseButtonReleasedEvent& e)
{
	return _chessRenderer->onMouseButtonReleased(e);
}