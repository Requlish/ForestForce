/******************************************************************************/
/*!
\file		Water_Particle_System.cpp
\author 	Chua Jim Hans
\par    	Email: c.jimhans@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements water particle system.
			The file contains

			Water_Particles_Load		- initializes alive object
			Water_Particles_Spawn		- spawns particles at tile position
			Water_Particles_Update		- updates position of particles based on
										velocity and delta time 
			Water_Particles_Swap		- swaps 'dead' particles with new particles
			Water_Particles_Emitter		- applies transformations to each particle
										and draws the particles with respect to
										Map
			Water_Particles_Unload		- frees the particles

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Water_Particle_System.hpp"
#include "Map.hpp"
#include "Player.hpp"

static std::vector<Particle> water_particles;
static size_t water_alive = 0;

/**************************************************************************/
/*!
	 This function Loads Water particle System
*/
/**************************************************************************/
void Water_Particles_Load() {
	water_particles.resize(WATER_PARTICLE_MAX_AMOUNT);
	water_alive = 0;
}

/**************************************************************************/
/*!
	 This function initializes new water particles with respect to tile
	 location
*/
/**************************************************************************/
void Water_Particles_Spawn(s32 tile_x, s32 tile_y) {
	for (size_t i = 0; i < WATER_PARTICLE_MAX_AMOUNT; ++i) {
		if (water_alive < WATER_PARTICLE_MAX_AMOUNT) {
			Particle data = {};
			data.x = static_cast<f32>(tile_x);
			data.y = static_cast<f32>(tile_y);
			f32 vel_x = Random_Float(-WATER_PARTICLE_VELOCITY, WATER_PARTICLE_VELOCITY);
			f32 vel_y = Random_Float(-WATER_PARTICLE_VELOCITY, WATER_PARTICLE_VELOCITY);
			data.vel_x = (vel_x - vel_y) * sinf(PI / 4.0f);	// 45 deg rotation (factorized, sin45 = cos45)
			data.vel_y = (vel_x + vel_y) * sinf(PI / 4.0f);	// 45 deg rotation (factorized, sin45 = cos45)
			data.lifetime = Random_Float(static_cast<f32>(WATER_PARTICLE_LIFETIME_MIN), static_cast<f32>(WATER_PARTICLE_LIFETIME_MAX));
			data.scale_x = WATER_PARTICLE_SCALE;
			data.scale_y = WATER_PARTICLE_SCALE;
			data.rotation = Random_Float(0.0f, 2.0f * PI);

			data.color_r = 0.f;
			data.color_g = 0.f;
			data.color_b = 1.0f;
	

			++water_alive;
			water_particles[water_alive - 1] = data;
		}
	}
}

/**************************************************************************/
/*!
	 This function updates behaviour of water particles
*/
/**************************************************************************/
void Water_Particles_Update() {

	for (size_t i = 0; i < water_alive; ++i) {
		water_particles[i].lifetime -= delta_time;
			water_particles[i].x += water_particles[i].vel_x * static_cast<f32>(delta_time);
			water_particles[i].y += water_particles[i].vel_y * static_cast<f32>(delta_time);
	}
}

/**************************************************************************/
/*!
	 This function swap particles to ensure that alive particles are to the 
	 front of the array
*/
/**************************************************************************/
void Water_Particles_Swap() {
	for (size_t i = 0; i < water_alive; ++i) {
		if (water_particles[i].lifetime < 0.0f) {
			Particle swap = water_particles[i];
			water_particles[i] = water_particles[water_alive - 1];
			water_particles[water_alive - 1] = swap;
			--water_alive;
		}
	}
}

/**************************************************************************/
/*!
	 This function draws water particle with respect to the Map tiles
*/
/**************************************************************************/
void Water_Particles_Emitter() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEMtx33 map_scale{};
	AEMtx33 map_translate{};
	AEMtx33 map_transform{};
	AEMtx33Trans(&map_translate, static_cast<f32>((GRID_SIZE) / 2.0f), static_cast<f32>((GRID_SIZE) / 2.0f));
	AEMtx33Scale(&map_scale, GRID_SIZE, GRID_SIZE);
	AEMtx33Concat(&map_transform, &map_translate, &map_scale);


	for (size_t i = 0; i < water_alive; ++i) {
		// Scale, Rotate, Translate, Transform
		AEMtx33 scale, rot, trans, transform;

		AEMtx33Scale(&scale, water_particles[i].scale_x, water_particles[i].scale_y);
		AEMtx33Rot(&rot, water_particles[i].rotation);
		AEMtx33Trans(&trans, water_particles[i].x * static_cast<f32>(GRID_SIZE), water_particles[i].y * static_cast<f32>(GRID_SIZE));
		AEMtx33Concat(&transform, &rot, &scale);
		AEMtx33Concat(&transform, &map_transform, &transform);
		AEMtx33Concat(&transform, &trans, &transform);
	
		AEGfxSetTransform(transform.m);
		AEGfxSetColorToMultiply(static_cast<float>(water_particles[i].color_r),
								static_cast<float>(water_particles[i].color_g), 
								static_cast<float>(water_particles[i].color_b), 
								static_cast<float>(water_particles[i].lifetime / WATER_PARTICLE_LIFETIME_MAX));
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
