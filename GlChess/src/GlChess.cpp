#include "GlChess.h"
#include <imgui.h>
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
}

void GlChess::_renderDockSpace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse 
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
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

	Lighthouse::Renderer::onWindowResize(windowContentSize.x, windowContentSize.y);
	_chessRenderer->updateWindowSize(windowContentSize.x, windowContentSize.y);
	_chessRenderer->updateWindowPos(windowOffset.x, windowOffset.y - 1);

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
	for (int i = 0; i < moves.size(); i++)
	{
		if (ImGui::Button(std::string(moves[i]).c_str()))
		{
			_chessRenderer->showMove(i + 1);
		}
	}
	ImGui::End();
}

void GlChess::_renderMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Game", "")) { LH_INFO("clicked on New Game!"); }
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

bool GlChess::_onWindowResized(Lighthouse::WindowResizeEvent& e)
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