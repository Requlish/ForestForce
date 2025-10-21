/******************************************************************************/
/*!
\file		Level.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the header for the Forest Force game.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern s8 level_state; // Game State in Level
extern AEGfxVertexList* p_mesh;

void Level_Load();

void Level_Initialize();

void Level_Update();

void Level_Draw();

void Level_Free();

void Level_Unload();