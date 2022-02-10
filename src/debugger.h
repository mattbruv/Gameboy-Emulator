#pragma once
#include<SFML\Window.hpp>
#include<SFML\Graphics.hpp>
#include<imgui.h>
#include<imgui-SFML.h>
#include<imgui_memory_editor.h>
#include"cpu.h"

class debugger
{
public:

	sf::RenderWindow window;
	sf::Clock deltaClock;
	MemoryEditor memEditor;

	debugger();
	~debugger();

	void update(CPU& cpu);

	void renderCpuState(CPU& cpu) const;

	void renderWRAMState(CPU& cpu);

	void renderVRAMState(CPU& cpu);
};
