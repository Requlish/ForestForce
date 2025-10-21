/******************************************************************************/
/*!
\file		Earth_Particle_System.cpp
\author 	Chua Jim Hans
\par    	Email: c.jimhans@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements earth particle system.
			The file contains

			Earth_Particles_Load		- initializes alive object
			Earth_Particles_Spawn		- spawns particles at tile position
			Earth_Particles_Update		- updates position of particles based on
										velocity and delta time 
			Earth_Particles_Swap		- swaps 'dead' particles with new particles
			Earth_Particles_Emitter		- applies transformations to each particle
										and draws the particles with respect to
										Map
			Earth_Particles_Unload		- frees the particles

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Earth_Particle_System.hpp"
#include "Map.hpp"
#include "Player.hpp"

static std::vector<Particle> earth_particles;
static size_t earth_alive = 0;

/**************************************************************************/
/*!
	 This function Loads Earth particle System
*/
/**************************************************************************/
void Earth_Particles_Load() {
	earth_particles.resize(EARTH_PARTICLE_MAX_AMOUNT);
	earth_alive = 0;
}

/**************************************************************************/
/*!
	 This function initializes new earth particles with respect to tile
	 location
*/
/**************************************************************************/
void Earth_Particles_Spawn(s32 tile_x, s32 tile_y) {
	for (size_t i = 0; i < EARTH_PARTICLE_MAX_AMOUNT; ++i) {
		if (earth_alive < EARTH_PARTICLE_MAX_AMOUNT) {
			Particle data = {};
			data.x = static_cast<f32>(tile_x) + Random_Float(-EARTH_PARTICLE_OFFSET_X, EARTH_PARTICLE_OFFSET_X);
			data.y = static_cast<f32>(tile_y) + EARTH_PARTICLE_OFFSET_Y;
			data.vel_x = Random_Float(-EARTH_PARTICLE_VELOCITY_X, EARTH_PARTICLE_VELOCITY_X);
			data.vel_y = Random_Float(EARTH_PARTICLE_VELOCITY_Y_MIN, EARTH_PARTICLE_VELOCITY_Y_MAX);
			data.lifetime = Random_Float(static_cast<float>(EARTH_PARTICLE_LIFETIME_MIN), static_cast<float>(EARTH_PARTICLE_LIFETIME_MAX));
			data.scale_x = EARTH_PARTICLE_SCALE;
			data.scale_y = EARTH_PARTICLE_SCALE;
			data.rotation = 0;
			data.color_r = 0.75f;
			data.color_g = 0.6f;
			data.color_b = 0.2f;

			++earth_alive;
			earth_particles[earth_alive - 1] = data;
		}
	}
}

/**************************************************************************/
/*!
	 This function updates behaviour of earth particles
*/
/**************************************************************************/
void Earth_Particles_Update() {
	for (size_t i = 0; i < earth_alive; ++i) {
		earth_particles[i].lifetime -= delta_time;

		earth_particles[i].vel_y -= EARTH_PARTICLE_GRAVITY * static_cast<f32>(delta_time);
		earth_particles[i].x += earth_particles[i].vel_x * static_cast<f32>(delta_time);
		earth_particles[i].y += earth_particles[i].vel_y * static_cast<f32>(delta_time);

	}
}

/**************************************************************************/
/*!
	 This function swap particles to ensure that alive particles are to the
	 front of the array
*/
/**************************************************************************/
void Earth_Particles_Swap() {
	for (size_t i = 0; i < earth_alive; ++i) {
		if (earth_particles[i].lifetime < 0.0f) {
			Particle swap = earth_particles[i];
			earth_particles[i] = earth_particles[earth_alive - 1];
			earth_particles[earth_alive - 1] = swap;
			--earth_alive;
		}
	}
}

/**************************************************************************/
/*!
	 This function draws earth particle with respect to the Map tiles
*/
/**************************************************************************/
void Earth_Particles_Emitter() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEMtx33 map_scale{};
	AEMtx33 map_translate{};
	AEMtx33 map_transform{};
	AEMtx33Trans(&map_translate, static_cast<f32>((GRID_SIZE) / 2.0f), static_cast<f32>((GRID_SIZE) / 2.0f));
	AEMtx33Scale(&map_scale, GRID_SIZE, GRID_SIZE);
	AEMtx33Concat(&map_transform, &map_translate, &map_scale);


	for (size_t i = 0; i < earth_alive; ++i) {
		// Scale, Rotate, Translate, Transform
		AEMtx33 scale, rot, trans, transform;

		AEMtx33Scale(&scale, earth_particles[i].scale_x, earth_particles[i].scale_y);
		AEMtx33Rot(&rot, earth_particles[i].rotation);
		AEMtx33Trans(&trans, earth_particles[i].x * static_cast<f32>(GRID_SIZE), earth_particles[i].y * static_cast<f32>(GRID_SIZE));
		AEMtx33Concat(&transform, &rot, &scale);
		AEMtx33Concat(&transform, &map_transform, &transform);
		AEMtx33Concat(&transform, &trans, &transform);
	
		AEGfxSetTransform(transform.m);
		AEGfxSetColorToMultiply(static_cast<float>(earth_particles[i].color_r),
								static_cast<float>(earth_particles[i].color_g), 
								static_cast<float>(earth_particles[i].color_b), 
								static_cast<float>(earth_particles[i].lifetime / EARTH_PARTICLE_LIFETIME_MAX));
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

