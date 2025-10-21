/******************************************************************************/
/*!
\file		GameStateList.hpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the list of Game States and Dimensions related
			constants used in Forest Force.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern s8 TILE_X; 
extern s8 TILE_Y;	

extern float GRID_SIZE;
extern const u8 LAST_LEVEL;

enum GS_STATES : u8 {
	GS_SPLASHSCREEN = 0,
	GS_MAINMENU,
	GS_EDITOR,

	GS_LEVEL1, GS_LEVEL2, GS_LEVEL3, GS_LEVEL4,
	GS_LEVEL5, GS_LEVEL6, GS_LEVEL_EDITOR,

	MS_CREDITS,
	MS_SETTINGS,
	MS_LEVELSELECT,
	MS_MAINMENU,

	GS_QUIT,
	GS_RESTART
};

enum GAME_SETTINGS {
	WINDOW_LENGTH = 1600,
	WINDOW_WIDTH = 900
};

enum LEVEL_STATES : u8 {
	LS_PAUSED,
	LS_PLAYING,
	LS_DEFEAT,
	LS_VICTORY
};

enum STATUS : u8 {
	INVALID_TILE = 200
};