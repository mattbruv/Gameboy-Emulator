#include"debugger.h"

debugger::debugger()
{
	window.create(sf::VideoMode(1000, 600), "Debugger");
	ImGui::SFML::Init(window);
}

debugger::~debugger()
{
	ImGui::SFML::Shutdown();
	window.close();
}

void debugger::update(CPU& cpu)
{
	//handle input
	sf::Event event;
	while (window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
			window.close();
	}

	ImGui::SFML::Update(window,deltaClock.restart());

	window.clear(sf::Color(15, 34, 50));

	renderCpuState(cpu);
	renderWRAMState(cpu);
	renderVRAMState(cpu);

	ImGui::SFML::Render(window);
	window.display();
}

void debugger::renderCpuState(CPU& cpu) const
{
	//set window pos and size
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 200));
	ImGui::StyleColorsLight();

	ImGui::Begin("CPU state", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);	//no window resize and mov;

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Program Counter:  ");
	ImGui::SameLine();
	ImGui::Text("%04X", cpu.reg_PC);

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Stack Pointer:    ");
	ImGui::SameLine();
	ImGui::Text("%04X", cpu.reg_SP);

	ImGui::NewLine();

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Registers");
	//all registers
	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Reg_A: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_A);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "        Reg_B: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_B);

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Reg_C: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_C);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "        Reg_D: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_D);

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Reg_E: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_E);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "        Reg_H: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_H);

	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "Reg_L: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_L);
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0.3f, 0.2f, 1.0f, 1.0f), "        Reg_F: ");
	ImGui::SameLine();
	ImGui::Text("%02X", cpu.reg_F);


	ImGui::End();
}

void debugger::renderWRAMState(CPU& cpu)
{
	ImGui::SetNextWindowPos(ImVec2(0, 200));
	ImGui::SetNextWindowSize(ImVec2(500, 400));
	ImGui::StyleColorsLight();

	ImGui::Begin("WRAM State", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	memEditor.ReadOnly = true;
	memEditor.DrawContents(cpu.memory->WRAM.data(), 8192);
	ImGui::End();
}

void debugger::renderVRAMState(CPU& cpu)
{
	ImGui::SetNextWindowPos(ImVec2(500, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 600));
	ImGui::StyleColorsLight();

	ImGui::Begin("VRAM State", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	memEditor.ReadOnly = true;
	memEditor.DrawContents(cpu.memory->VRAM.data(), 8192);
	ImGui::End();
}