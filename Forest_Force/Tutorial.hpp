/******************************************************************************/
/*!
\file		Tutorial.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for functions that are used for tutorial.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

extern s8 p_tutorial_font;
extern AEGfxVertexList* p_mesh;

void Tutorials_Update();

void Tutorials_Draw();

void Tutorials_Reset();