/******************************************************************************/
/*!
\file		Main_Menu.hpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements main menu.
			The file contains
			Main_Menu_Load			- loads textures for main menu UI, loads
									  particle system
			Main_Menu_Initialize	- initializes music
			Main_Menu_Update		- handles user inputs to navigate to different
									  game states, updates particle system
			Main_Menu_Draw			- renders background, buttons, texts,
									  draws particle system effects
			Main_Menu_Free			- NA
			Main_Menu_Unload		- unloads textures

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern s8 p_button_font, p_title_font;

extern AEGfxVertexList* p_mesh;

void Main_Menu_Load();

void Main_Menu_Initialize();

void Main_Menu_Update();

void Main_Menu_Draw();

void Main_Menu_Free();

void Main_Menu_Unload();