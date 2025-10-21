/******************************************************************************/
/*!
\file		Camera.hpp
\author 	Chua Jim Hans
\par    	Email: c.jimhans@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the Zoom and Camera clamp functions
			declarations for GS_LEVEL:

			Camera_Init			-Initializes key variables for Camera functions
			Camera_Free			-Resets Camera position
			Toggle_Zoom			-Switches from fullscreen to not fullscreen
								 when called
			Update_Zoom			-Updates Camera Position and Grid Size via lerp with
								 respect to fullscreen or not fullscreen
			Camera_On_Player	-Clamps Camera to player position when not
								 fullscreen
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "pch.hpp"
extern f64 ZOOM_TIME;
extern s8  ZOOMED_IN_TILE_COUNT;

void Camera_Init();
void Camera_Free();
void Toggle_Zoom();
void Update_Zoom();
void Camera_On_Player(const f32& lerp_offset_x, const f32& lerp_offset_y);



