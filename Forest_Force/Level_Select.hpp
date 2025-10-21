/******************************************************************************/
/*!
\file		Level_Select.hpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements level selector.
			The file contains
			Calculate_Button_Positions	- calculates button positions
										  dynamically
			Level_Select_Load			- loads textures, sets up transformations 
										  for rendering, loads particle system,
										  gets user data
			Level_Select_Initialize		- initializes music
			Level_Select_Update			- handles user inputs to navigate to 
										  level, updates particle system
			Level_Select_Draw			- renders background, buttons, texts, 
										  draws particle system effects,
										  handles colour of level based on level
										  completion
			Level_Select_Free			- NA
			Level_Select_Unload			- unloads textures

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern s8 p_font, p_title_font;
extern AEGfxVertexList* p_mesh;

void Calculate_Button_Positions();

void Level_Select_Load();

void Level_Select_Initialize();

void Level_Select_Update();

void Level_Select_Draw();

void Level_Select_Free();

void Level_Select_Unload();