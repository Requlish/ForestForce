/******************************************************************************/
/*!
\file		Utils.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for various helper functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"

f64 old_time{};
f64 delta_time{};

/**************************************************************************/
/*!
	This function returns true if any key was pressed
*/
/**************************************************************************/
bool Any_Key_Was_Pressed() {
	u8 key_value = 0;
	for (int i = 0; i < 256; i++, key_value++) {
		if (AEInputCheckTriggered(key_value)) {
			return true;
		}
	}
	return false;
}

/**************************************************************************/
/*!
	This function checks if a point is in a rectangle (static collision)
*/
/**************************************************************************/
bool Is_Point_In_Rect(f32 rect_left, f32 rect_right, f32 rect_top, f32 rect_bottom, f32 mouse_x, f32 mouse_y) {
	if (mouse_x < rect_left || mouse_x > rect_right) {
		return false;
	}
	else if (mouse_y > rect_top || mouse_y < rect_bottom) {
		return false;
	}
	return true;
}

/**************************************************************************/
/*!
	This function is used to update delta time.
*/
/**************************************************************************/
void Update_Delta_Time(f64 &dt) {
	if (old_time) {
		f64 new_time = AEGetTime(nullptr);
		dt = new_time - old_time;
		old_time = new_time;
	}
	else { // will be runned during first frame
		old_time = AEGetTime(nullptr);
		dt = 0;
	}
}

/**************************************************************************/
/*!
	This function updates the mouse_x and mouse_y with mouse world 
	coordinates.
*/
/**************************************************************************/
void Get_Mouse_World_Coordinates(s32 &mouse_x, s32 &mouse_y) {
	// Convert From Screen Coords to Worlds Coords
	AEInputGetCursorPosition(&mouse_x, &mouse_y);
	mouse_x -= AEGfxGetWindowWidth() / 2;		
	mouse_y = AEGfxGetWindowHeight() / 2 - mouse_y;

	// Add Camera to Coords
	f32 cam_x{}, cam_y{};

	AEGfxGetCamPosition(&cam_x, &cam_y);
	mouse_x += static_cast<s32>(cam_x);
	mouse_y += static_cast<s32>(cam_y);
}

/**************************************************************************/
/*!
	This function updates the x and y with mouse tile coordinates.
*/
/**************************************************************************/
void Get_Mouse_Tile_Coordinates(s8 &x, s8 &y) {
	s32 mouse_x{}, mouse_y{};
	Get_Mouse_World_Coordinates(mouse_x, mouse_y);

	if (mouse_x < 0 || mouse_y < 0) {
		x = INVALID_TILE;
		y = INVALID_TILE;
		return;
	}

	f32 f_mouse_x = static_cast<f32>(mouse_x);
	f32 f_mouse_y = static_cast<f32>(mouse_y);
	
	f_mouse_x /= static_cast<f32>(GRID_SIZE);
	f_mouse_y /= static_cast<f32>(GRID_SIZE);

	mouse_x = static_cast<s32>(f_mouse_x);
	mouse_y = static_cast<s32>(f_mouse_y);

	if (mouse_x > (TILE_X - 1) || mouse_y > (TILE_Y - 1))
	{
		x = INVALID_TILE;
		y = INVALID_TILE;
	}
	else
	{
		x = static_cast<s8>(mouse_x);
		y = static_cast<s8>(mouse_y);
	}

}

/**************************************************************************/
/*!
	This function updates the mouse_x and mouse_y with mouse normalized
	coordinates.
*/
/**************************************************************************/
void Get_Mouse_Normalized_Coordinates(f32& mouse_x, f32& mouse_y) {
	s32 m_x{}, m_y{};
	AEInputGetCursorPosition(&m_x, &m_y);
	mouse_x = (m_x * 2.0f / static_cast<f32>(AEGfxGetWindowWidth())) - 1.0f; // Set mouse X from screen coor to normalized coor
	mouse_y = 1.0f - (m_y * 2.0f / static_cast<f32>(AEGfxGetWindowHeight())); // Set mouse Y from screen coor to normalized coor
}

/**************************************************************************/
/*!
	This function returns a random float given a min and a max.
*/
/**************************************************************************/
float Random_Float(float min, float max) {
	std::random_device rd;  // Random number
	std::default_random_engine eng(rd()); // Seed generator
	std::uniform_real_distribution<float> distr(min, max); // Define range

	return distr(eng);
}