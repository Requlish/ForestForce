/******************************************************************************/
/*!
\file		Water_Particle_System.hpp
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

#pragma once

extern size_t WATER_PARTICLE_MAX_AMOUNT;
extern f32 WATER_PARTICLE_SCALE;
extern f32 WATER_PARTICLE_VELOCITY;
extern f64 WATER_PARTICLE_LIFETIME_MIN;
extern f64 WATER_PARTICLE_LIFETIME_MAX;

extern AEGfxVertexList* p_mesh;

void Water_Particles_Load();

void Water_Particles_Update();

void Water_Particles_Swap();

void Water_Particles_Spawn(s32 tile_x, s32 tile_y);

void Water_Particles_Emitter();

void Water_Particles_Unload();

