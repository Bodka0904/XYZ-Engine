#include "Menu.h"


#include "imgui/imgui.h"
#include <imgui/examples/imgui_impl_glfw.h>
#include <imgui/examples/imgui_impl_opengl3.h>



bool Menu::GameRunning()
{
	return m_GameRunning;
}

bool Menu::GameStart()
{
	if (m_GameStarted)
	{
		m_GameStarted = false;
		return true;
	}
	return false;
}

void Menu::SetEndGame()
{
	m_GameRunning = false;
	m_GameOver = true;
}

void Menu::Update()
{
	if (!m_GameRunning)
	{
		ImGuiBegin();
		ImGui::Begin("Menu",(bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
		if (ImGui::Button("Start Game",ImVec2(500,100)) && !m_GameStarted)
		{
			m_GameStarted = true;
			m_GameRunning = true;
			m_GameOver = false;
		}
		else if (ImGui::Button("Exit Game", ImVec2(500, 100)))
		{
			XYZ::Application::Get().Stop();
		}

		if (m_GameOver)
		{
			ImGui::Text("GAME OVER","");
		}
		ImGuiEnd();
	}
}

void Menu::ImGuiBegin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Menu::ImGuiEnd()
{
	
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
