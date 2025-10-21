/******************************************************************************/
/*!
\file		Utils.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for various helper functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

extern f64 delta_time;

bool Any_Key_Was_Pressed();

bool Is_Point_In_Rect(f32 rect_left, f32 rect_right, f32 rect_top, f32 rect_bottom, f32 mouse_x, f32 mouse_y);

void Update_Delta_Time(f64& dt);

void Get_Mouse_World_Coordinates(s32& mouse_x, s32& mouse_y);

void Get_Mouse_Tile_Coordinates(s8& x, s8& y);

void Get_Mouse_Normalized_Coordinates(f32& mouse_x, f32& mouse_y);

float Random_Float(float min, float max);