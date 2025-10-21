/******************************************************************************/
/*!
\file		Camera.cpp
\author 	Chua Jim Hans
\par    	Email: c.jimhans@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the Zoom and Camera clamp functions
			definitions for GS_LEVEL:

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
#include "pch.hpp"
#include "Camera.hpp"
#include "Player.hpp"


static f32 lerp_offset_x{};
static f32 lerp_offset_y{};
static f32 camera_x{};
static f32 camera_y{};
static bool fullscreen{ 0 };
static bool transitioning{ 0 };
static f64 time_counter{ 0.0 };


/**************************************************************************/
/*!
	Call this function initialize Camera variables
*/
/**************************************************************************/
void Camera_Init()
{
	//sets minimum grid size
	GRID_SIZE = static_cast<f32>(AEGfxGetWindowHeight() / ZOOMED_IN_TILE_COUNT);
	fullscreen = false;
	time_counter = ZOOM_TIME;
	transitioning = false;

}


/**************************************************************************/
/*!
	 This function Frees Camera variables
*/
/**************************************************************************/
void Camera_Free()
{
	//reset cam position
	AEGfxSetCamPosition(0.f, 0.f);
}

/**************************************************************************/
/*!
	 This function alternates between fullscreen mode and non-fullscreen
	 mode
*/
/**************************************************************************/
void Toggle_Zoom()
{

	fullscreen = !fullscreen;
	transitioning = true;


}

/**************************************************************************/
/*!
	 This function updates zoom when transitioning from Zoom in and
	 Zoom out view
*/
/**************************************************************************/
void Update_Zoom()
{

	f32 min_grid_size = static_cast<f32>(AEGfxGetWindowWidth() / TILE_X < AEGfxGetWindowHeight() / TILE_Y ? AEGfxGetWindowWidth() / (TILE_X + 2) : AEGfxGetWindowHeight() / (TILE_Y + 2));
	f32 max_grid_size = static_cast<f32>(AEGfxGetWindowHeight() / ZOOMED_IN_TILE_COUNT);
	f32 grid_size_diff = max_grid_size - min_grid_size;

	
	if (transitioning)
	{


		if (fullscreen)
		{
			//lerps from min_grid_size to max_grid_size
			//also lerps camera position to center of map
			time_counter = (time_counter > 0.f) ? time_counter - delta_time : 0.0;
			GRID_SIZE = min_grid_size + grid_size_diff * static_cast<f32>(time_counter / ZOOM_TIME);
			f32 cam_x_diff = camera_x - (TILE_X * GRID_SIZE/2.f);
			f32 cam_y_diff = camera_y - (TILE_Y * GRID_SIZE / 2.f);
			f32 fixed_camera_x = (TILE_X * GRID_SIZE / 2.f) + cam_x_diff * static_cast<f32>(time_counter / ZOOM_TIME);
			f32 fixed_camera_y = (TILE_Y * GRID_SIZE / 2.f) + cam_y_diff * static_cast<f32>(time_counter / ZOOM_TIME);
			AEGfxSetCamPosition(fixed_camera_x, fixed_camera_y);


			if (time_counter <= 0.f)
			{
				transitioning = false;
			}
		}
		else
		{
			//lerps from max_grid_size to min_grid_size
			//also lerps camera position to player
			time_counter = (time_counter < ZOOM_TIME) ? time_counter + delta_time : ZOOM_TIME;
			GRID_SIZE = min_grid_size + grid_size_diff * static_cast<f32>(time_counter / ZOOM_TIME);
			f32 cam_x_diff = camera_x - (TILE_X * GRID_SIZE / 2.f);
			f32 cam_y_diff = camera_y - (TILE_Y * GRID_SIZE / 2.f);
			f32 fixed_camera_x = camera_x - cam_x_diff * (1.f - static_cast<f32>(time_counter / ZOOM_TIME));
			f32 fixed_camera_y = camera_y - cam_y_diff * (1.f - static_cast<f32>(time_counter / ZOOM_TIME));
			AEGfxSetCamPosition(fixed_camera_x, fixed_camera_y);
			if (time_counter >= ZOOM_TIME)
			{
				transitioning = false;
			}
		}


	}

}

/**************************************************************************/
/*!
	 This function of Draw_Player and clamps camera to player position 
*/
/**************************************************************************/
void Camera_On_Player(const f32 &offset_x,const f32& offset_y)
{
	lerp_offset_x = offset_x;
	lerp_offset_y = offset_y;
	
	if(!fullscreen)
	{
		std::pair<s8, s8> temp{};
		player.Get_Player_Tiles_Coordinates(temp.first, temp.second);

		f32 border_offset = static_cast<f32>((GRID_SIZE) / 2.0f);
		camera_x = static_cast<f32>(temp.first * GRID_SIZE) + lerp_offset_x + border_offset;
		camera_y = static_cast<f32>(temp.second * GRID_SIZE) + lerp_offset_y + border_offset;

		AEGfxSetCamPosition(camera_x, camera_y);

	}
}
