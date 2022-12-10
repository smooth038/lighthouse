#include "GlChess.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

GlChess::GlChess(std::unique_ptr<Lighthouse::Window>& window) 
		: Lighthouse::Layer("GlChess 3D", window), _chessRenderer(std::make_unique<ChessRenderer3D>(window)) 
{
}

void GlChess::onAttach()
{
	LH_INFO("GlChess attached!");
	_chessRenderer->buildScene(true);
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
	dispatcher.dispatch<Lighthouse::KeyPressedEvent>([this](Lighthouse::KeyPressedEvent& e) { return e.handled || _onKeyPressed(e); });
	dispatcher.dispatch<Lighthouse::KeyReleasedEvent>([this](Lighthouse::KeyReleasedEvent& e) { return e.handled || _onKeyReleased(e); });
	dispatcher.dispatch<Lighthouse::MouseMovedEvent>([this](Lighthouse::MouseMovedEvent& e) { return e.handled || _onMouseMoved(e); });
	dispatcher.dispatch<Lighthouse::MouseButtonPressedEvent>([this](Lighthouse::MouseButtonPressedEvent& e) { return e.handled || _onMouseButtonPressed(e); });
	dispatcher.dispatch<Lighthouse::MouseButtonReleasedEvent>([this](Lighthouse::MouseButtonReleasedEvent& e) { return e.handled || _onMouseButtonReleased(e); });
	dispatcher.dispatch<Lighthouse::WindowResizeEvent>([this](Lighthouse::WindowResizeEvent& e) { return e.handled || _onWindowResized(e); });
}


void GlChess::onImGuiRender()
{
    Lighthouse::RenderCommand::clearCanvas(0.0f, 0.0f, 0.88f, 1.0f);
	//ImGui::ShowDemoWindow();
	_renderDockSpace();
	_renderBoardWindow();
	_renderNotationWindow();
	_renderPromotionDialog();
	_renderFps();
}

void GlChess::_newGame()
{
	_chessRenderer.reset();
	Lighthouse::Renderer::getScene().clearScene();
	Lighthouse::Renderer::getCamera() = Lighthouse::Camera(_window->getWidth(), _window->getHeight());
	_chessRenderer = std::make_unique<ChessRenderer3D>(_window);
	_chessRenderer->buildScene();
}

void GlChess::_renderDockSpace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse 
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiConfigFlags_NoMouseCursorChange;
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("DockSpace", NULL, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceId = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

	_renderMenuBar();

	ImGui::End();
}

void GlChess::_renderBoardWindow()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("Board Window", NULL );
	
	ImGui::PopStyleVar(3);

	ImVec2 viewportMin = ImGui::GetMainViewport()->WorkPos;
	ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
	ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
	ImVec2 windowContentSize = ImVec2(contentMax.x - contentMin.x, contentMax.y - contentMin.y);

	// We offset contentMin (in window coordinates) with absolute window coordinates 
	ImVec2 windowPos = ImVec2(contentMin.x + ImGui::GetWindowPos().x, contentMin.y + ImGui::GetWindowPos().y);
	ImVec2 windowOffset = ImVec2(windowPos.x - viewportMin.x, windowPos.y - viewportMin.y);

	Lighthouse::Renderer::onWindowResize(static_cast<unsigned int>(windowContentSize.x), static_cast<unsigned int>(windowContentSize.y));
	_chessRenderer->updateWindowSize(static_cast<unsigned int>(windowContentSize.x), static_cast<unsigned int>(windowContentSize.y));
	_chessRenderer->updateWindowPos(static_cast<unsigned int>(windowOffset.x), static_cast<unsigned int>(windowOffset.y - 1));

	ImGui::Image((void*)(intptr_t)Lighthouse::Renderer::getRenderTexture(), windowContentSize, ImVec2(0, 1), ImVec2(1, 0));

	bool isHovered = ImGui::IsWindowHovered();
	bool isFocused = ImGui::IsWindowFocused();
	Lighthouse::Application::get()->getImGuiLayer()->blockEvents(!isHovered && !isFocused);

	ImGui::End();
}


void GlChess::_renderNotationWindow()
{
	ChessBoard* board = _chessRenderer->getBoard();
	std::vector<HalfMove> moves = board->getMoves();
	std::vector<Fen> history = board->getHistory();

	ImGui::Begin("Game Notation");

	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) 
	{ 
		_chessRenderer->showMove(_chessRenderer->getViewedMove() - 1);
	}
	ImGui::SameLine();
	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) 
	{ 
		_chessRenderer->showMove(_chessRenderer->getViewedMove() + 1);
	}
	ImGui::Separator();

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
	ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.84f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(117.0f / 360.0f, 0.49f, 0.86f));
	for (unsigned int i = 0; i < moves.size(); i++)
	{
		if (i % 2 == 1)
		{
			ImGui::SameLine();
		}
		if (i % 2 == 0)
		{
			std::string moveNumber = std::to_string((i / 2) + 1) + ". ";
			ImGui::Text(moveNumber.c_str());
			ImGui::SameLine();
		}
		if (i + 1 == _chessRenderer->getViewedMove())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(117.0f / 360.f, 1.0f, 0.68f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(117.0f / 360.f, 1.0f, 0.76f));
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.36f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(72.0f / 360.0f, 0.34f, 0.67f));
		}
		std::string buttonId = std::string(moves[i]) + (moves[i].getIsMate() ? " " : "") + "##" + std::to_string(i);
		if (ImGui::Button(buttonId.c_str()))
		{
			_chessRenderer->showMove(i + 1);
		}
		ImGui::PopStyleColor(2);
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(2);
	ImGui::SetScrollHereY(0.999f);
	ImGui::End();
}


void GlChess::_renderPromotionDialog()
{
	_chessRenderer->renderPromotionDialog();
}

void GlChess::_renderFps()
{
	if (_frameCount % 20 == 0)
	{
		_frameCount = 0;
		auto now = std::chrono::high_resolution_clock::now();
		auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - _timeSinceLast20Frames);
		_lastFps = 1000.0f * 20.0f / delta.count();
		_timeSinceLast20Frames = now;
	}

	ImGui::Begin("FPS");
	std::string fpsMessage = "Refresh rate : " + std::to_string(_lastFps) + " fps";
	ImGui::Text(fpsMessage.c_str());
	ImGui::End();

	_frameCount++;
}

void GlChess::_renderMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Game", "")) { _newGame(); }
			ImGui::Separator();
			if (ImGui::MenuItem("Quit", "")) { exit(0); }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", "")) { LH_INFO("clicked on About!"); }
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

bool GlChess::_onWindowResized(Lighthouse::WindowResizeEvent&)
{
	return false;
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