/******************************************************************************/
/*!
\file		Credits.hpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements credits screen.
			The file contains
			Credits_Load		- loads textures, sets up transformations for
								  rendering
			Credits_Initialize	- resets starting position of credits, plays
								  credits music
			Credits_Update		- handles user input to increase scroll speed
								  or change game state
			Credits_Draw		- renders background and credits
			Credits_Free		- NA
			Credits_Unload		- unloads textures

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern AEGfxVertexList* p_mesh;
extern s8 p_credits_font;

void Credits_Load();

void Credits_Initialize();

void Credits_Update();

void Credits_Draw();

void Credits_Free();

void Credits_Unload();