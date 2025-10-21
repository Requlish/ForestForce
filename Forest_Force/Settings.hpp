/******************************************************************************/
/*!
\file		Settings.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for Settings related functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern s8 p_font, p_smaller_font;
extern s8 p_button_font, p_title_font;
extern AEGfxVertexList* p_mesh;

void Settings_Load();

void Settings_Initialize();

void Settings_Update();

void Settings_Draw();

void Settings_Free();

void Settings_Unload();