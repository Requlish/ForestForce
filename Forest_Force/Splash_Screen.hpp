/******************************************************************************/
/*!
\file		Splash_Screen.hpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements splash screen.
			The file contains
			Splash_Screen_Load			- loads textures, calculates scaling
								          factors based on window dimensions
			Splash_Screen_Initialize	- NA
			Splash_Screen_Update		- updates alpha value of logo,
										  transitions to main menu after
										  certain time or user input
			Splash_Screen_Draw			- renders logo, then displays team
										  name
			Splash_Screen_Free			- NA
			Splash_Screen_Unload		- unloads textures

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern AEGfxVertexList* p_mesh;
extern s8 p_splash_font;

void Splash_Screen_Load();

void Splash_Screen_Initialize();

void Splash_Screen_Update();

void Splash_Screen_Draw();

void Splash_Screen_Free();

void Splash_Screen_Unload();