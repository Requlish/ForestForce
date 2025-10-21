/******************************************************************************/
/*!
\file		Fire_Particle_System.cpp
\author 	Chua Jim Hans
\par    	Email: c.jimhans@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements particle system.
			The file contains

			Particles_Load		- initializes alive object
			Particles_Spawn		- spawns particles at tile position
			Particles_Update	- updates position of particles based on
								  velocity and delta time as well as
								  wind_behaviour
			Particles_Swap		- swaps 'dead' particles with new particles
			Particles_Emitter	- applies transformations to each particle
								  and draws the particles with respect to
								  Map
			Particles_Unload	- frees the particles

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Fire_Particle_System.hpp"
#include "Map.hpp"
#include "Player.hpp"

static std::vector<Particle> fire_particles;
static size_t fire_alive;

/**************************************************************************/
/*!
	 This function Loads Fire particle System
*/
/**************************************************************************/
void Fire_Particles_Load() {
	fire_particles.resize(FIRE_PARTICLE_MAX_AMOUNT);
	fire_alive = 0;
}

/**************************************************************************/
/*!
	 This function initializes new fire particles with respect to tile
	 location
*/
/**************************************************************************/
void Fire_Particles_Spawn(s32 tile_x, s32 tile_y, size_t num_particles) {
	for (size_t i = 0; i < num_particles; ++i) {
		if (fire_alive < FIRE_PARTICLE_MAX_AMOUNT) {
			Particle data = {};
			data.x = static_cast<f32>(tile_x) + Random_Float(-FIRE_PARTICLE_OFFSET, FIRE_PARTICLE_OFFSET);
			data.y = static_cast<f32>(tile_y) + Random_Float(-FIRE_PARTICLE_OFFSET, FIRE_PARTICLE_OFFSET);
			data.vel_x = 0;
			data.vel_y = Random_Float(FIRE_PARTICLE_VELOCITY_MIN, FIRE_PARTICLE_VELOCITY_MAX);
			data.lifetime = Random_Float(static_cast<float>(FIRE_PARTICLE_LIFETIME_MIN), static_cast<float>(FIRE_PARTICLE_LIFETIME_MAX));
			data.scale_x = FIRE_PARTICLE_SCALE;
			data.scale_y = FIRE_PARTICLE_SCALE;
			data.rotation = 0;
			data.color_r = 0.1f;
			data.color_g = 0.1f;
			data.color_b = 0.1f;

			++fire_alive;
			fire_particles[fire_alive - 1] = data;
		}
	}
}

/**************************************************************************/
/*!
	 This function updates behaviour of fire particles
*/
/**************************************************************************/
void Fire_Particles_Update() {

	static f64 animation_time_counter = FIRE_PARTICLE_SPAWN_TIME;

	if (animation_time_counter > 0.0)
	{
		animation_time_counter -= delta_time;
	}
	else
	{
		animation_time_counter = FIRE_PARTICLE_SPAWN_TIME;

		std::pair<s8, s8> temp = {};
		player.Get_Player_Tiles_Coordinates(temp.first, temp.second);
		int draw_limit_y = static_cast<int>(AEGfxGetWindowHeight() / GRID_SIZE);
		int draw_limit_x = static_cast<int>(AEGfxGetWindowWidth() / GRID_SIZE);
		//Draw Tile Type first
		for (int j = (TILE_Y - 1); j >= 0; j--)
		{
			if (j < (temp.second - draw_limit_y) || j >(temp.second + draw_limit_y)) continue;
			for (int i = 0; i < TILE_X; i++)
			{
				if (i < (temp.first - draw_limit_x) || i >(temp.first + draw_limit_x)) continue;

				if((level_map.Get_Display_Grid())[j][i].fire > 0)
				Fire_Particles_Spawn(i, j, (level_map.Get_Display_Grid())[j][i].fire);
			}
		}
	}



	for (size_t i = 0; i < fire_alive; ++i) {
		fire_particles[i].lifetime -= delta_time;

		switch (level_map.Get_Wind())
		{
		case(Map::NO_WIND):
			fire_particles[i].x += fire_particles[i].vel_x * static_cast<f32>(delta_time);
			fire_particles[i].y += fire_particles[i].vel_y * static_cast<f32>(delta_time);
			break;
		case(Map::WIND::LEFT):
			fire_particles[i].x += (fire_particles[i].vel_x - FIRE_PARTICLE_VELOCITY_MAX) * static_cast<f32>(delta_time);
			fire_particles[i].y += fire_particles[i].vel_y * static_cast<f32>(delta_time);
			break;
		case(Map::WIND::RIGHT):
			fire_particles[i].x += (fire_particles[i].vel_x + FIRE_PARTICLE_VELOCITY_MAX) * static_cast<f32>(delta_time);
			fire_particles[i].y += fire_particles[i].vel_y * static_cast<f32>(delta_time);
			break;
		case(Map::WIND::UP):
			fire_particles[i].x += fire_particles[i].vel_x * static_cast<f32>(delta_time);
			fire_particles[i].y += (fire_particles[i].vel_y + FIRE_PARTICLE_VELOCITY_MAX) * static_cast<f32>(delta_time);
			break;
		case(Map::WIND::DOWN):
			fire_particles[i].x += fire_particles[i].vel_x * static_cast<f32>(delta_time);
			fire_particles[i].y += (fire_particles[i].vel_y - FIRE_PARTICLE_VELOCITY_MAX) * static_cast<f32>(delta_time);
			break;
		default:
			fire_particles[i].x += fire_particles[i].vel_x * static_cast<f32>(delta_time);
			fire_particles[i].y += fire_particles[i].vel_y * static_cast<f32>(delta_time);
			break;
		}


	}
}

/**************************************************************************/
/*!
	 This function swap particles to ensure that alive particles are to the
	 front of the array
*/
/**************************************************************************/
void Fire_Particles_Swap() {
	for (size_t i = 0; i < fire_alive; ++i) {
		if (fire_particles[i].lifetime < 0.0f) {
			Particle swap = fire_particles[i];
			fire_particles[i] = fire_particles[fire_alive - 1];
			fire_particles[fire_alive - 1] = swap;
			--fire_alive;
		}
	}
}

/**************************************************************************/
/*!
	 This function draws fire particle with respect to the Map tiles
*/
/**************************************************************************/
void Fire_Particles_Emitter() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEMtx33 map_scale{};
	AEMtx33 map_translate{};
	AEMtx33 map_transform{};
	AEMtx33Trans(&map_translate, static_cast<f32>((GRID_SIZE) / 2.0f), static_cast<f32>((GRID_SIZE) / 2.0f));
	AEMtx33Scale(&map_scale, GRID_SIZE, GRID_SIZE);
	AEMtx33Concat(&map_transform, &map_translate, &map_scale);


	for (size_t i = 0; i < fire_alive; ++i) {
		// Scale, Rotate, Translate, Transform
		AEMtx33 scale, rot, trans, transform;

		AEMtx33Scale(&scale, fire_particles[i].scale_x, fire_particles[i].scale_y);
		AEMtx33Rot(&rot, fire_particles[i].rotation);
		AEMtx33Trans(&trans, fire_particles[i].x * static_cast<f32>(GRID_SIZE), fire_particles[i].y * static_cast<f32>(GRID_SIZE));
		AEMtx33Concat(&transform, &rot, &scale);
		AEMtx33Concat(&transform, &map_transform, &transform);
		AEMtx33Concat(&transform, &trans, &transform);
	
		AEGfxSetTransform(transform.m);
		AEGfxSetColorToMultiply(static_cast<float>(fire_particles[i].color_r),
								static_cast<float>(fire_particles[i].color_g), 
								static_cast<float>(fire_particles[i].color_b), 
								static_cast<float>(fire_particles[i].lifetime / FIRE_PARTICLE_LIFETIME_MAX));
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

