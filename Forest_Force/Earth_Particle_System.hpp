/******************************************************************************/
/*!
\file		Earth_Particle_System.hpp
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

#pragma once

extern size_t EARTH_PARTICLE_MAX_AMOUNT;
extern f32 EARTH_PARTICLE_OFFSET_X;
extern f32 EARTH_PARTICLE_OFFSET_Y;
extern f32 EARTH_PARTICLE_SCALE;
extern f32 EARTH_PARTICLE_VELOCITY_X;
extern f32 EARTH_PARTICLE_VELOCITY_Y_MIN;
extern f32 EARTH_PARTICLE_VELOCITY_Y_MAX;
extern f32 EARTH_PARTICLE_GRAVITY;		// Balancking CHeck
extern f64 EARTH_PARTICLE_LIFETIME_MIN;
extern f64 EARTH_PARTICLE_LIFETIME_MAX;

extern AEGfxVertexList* p_mesh;

void Earth_Particles_Load();

void Earth_Particles_Spawn(s32 tile_x, s32 tile_y);

void Earth_Particles_Update();

void Earth_Particles_Swap();

void Earth_Particles_Emitter();

void Earth_Particles_Unload();

