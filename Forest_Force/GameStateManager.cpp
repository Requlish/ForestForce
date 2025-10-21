/******************************************************************************/
/*!
\file		GameStateManager.cpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswhenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the handling of Game States.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "GameStateManager.hpp"
#include "Level.hpp"
#include "Main_Menu.hpp"
#include "Splash_Screen.hpp"
#include "Credits.hpp"
#include "Level_Select.hpp"
#include "Level_Editor.hpp"
#include "Settings.hpp"

int current = 0, previous = 0, next = 0; // Game States in Gamestate Manager

FP fp_load = nullptr, fp_initialize = nullptr, fp_update = nullptr, fp_draw = nullptr, fp_free = nullptr, fp_unload = nullptr;

void GSM_Initialize(int starting_state)
{
	current = previous = next = starting_state;
}

void GSM_Update()
{
	switch (next)
	{
	case GS_SPLASHSCREEN:		
		fp_load = Splash_Screen_Load;
		fp_initialize = Splash_Screen_Initialize;
		fp_update = Splash_Screen_Update;
		fp_draw = Splash_Screen_Draw;
		fp_free = Splash_Screen_Free;
		fp_unload = Splash_Screen_Unload;
		break;
	case GS_MAINMENU:		
		fp_load = Main_Menu_Load;
		fp_initialize = Main_Menu_Initialize;
		fp_update = Main_Menu_Update;
		fp_draw = Main_Menu_Draw;
		fp_free = Main_Menu_Free;
		fp_unload = Main_Menu_Unload;
		break;
	case GS_LEVEL1:
	case GS_LEVEL2:
	case GS_LEVEL3:
	case GS_LEVEL4:
	case GS_LEVEL5:
	case GS_LEVEL6:
	case GS_LEVEL_EDITOR:
		fp_load = Level_Load;
		fp_initialize = Level_Initialize;
		fp_update = Level_Update;
		fp_draw = Level_Draw;
		fp_free = Level_Free;
		fp_unload = Level_Unload;
		break;
	case GS_EDITOR:
		fp_load = Level_Editor_Load;
		fp_initialize = Level_Editor_Init;
		fp_update = Level_Editor_Update;
		fp_draw = Level_Editor_Draw;
		fp_free = Level_Editor_Free;
		fp_unload = Level_Editor_Unload;
		break;
	case MS_SETTINGS:
		fp_load = Settings_Load;
		fp_initialize = Settings_Initialize;
		fp_update = Settings_Update;
		fp_draw = Settings_Draw;
		fp_free = Settings_Free;
		fp_unload = Settings_Unload;
		break;
	case MS_CREDITS:
		fp_load = Credits_Load;
		fp_initialize = Credits_Initialize;
		fp_update = Credits_Update;
		fp_draw = Credits_Draw;
		fp_free = Credits_Free;
		fp_unload = Credits_Unload;
		break;
	case MS_LEVELSELECT:
		fp_load = Level_Select_Load;
		fp_initialize = Level_Select_Initialize;
		fp_update = Level_Select_Update;
		fp_draw = Level_Select_Draw;
		fp_free = Level_Select_Free;
		fp_unload = Level_Select_Unload;
		break;
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;
	default:
		break;
	}
}