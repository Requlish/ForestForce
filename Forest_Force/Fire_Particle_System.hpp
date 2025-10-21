/******************************************************************************/
/*!
\file		Fire_Particle_System.hpp
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

#pragma once

extern size_t FIRE_PARTICLE_MAX_AMOUNT;
extern f32 FIRE_PARTICLE_SCALE;
extern f32 FIRE_PARTICLE_OFFSET;
extern f32 FIRE_PARTICLE_VELOCITY_MIN;
extern f32 FIRE_PARTICLE_VELOCITY_MAX;
extern f64 FIRE_PARTICLE_LIFETIME_MIN;
extern f64 FIRE_PARTICLE_LIFETIME_MAX;
extern f64 FIRE_PARTICLE_SPAWN_TIME;

extern AEGfxVertexList* p_mesh;

void Fire_Particles_Load();

void Fire_Particles_Update();

void Fire_Particles_Swap();

void Fire_Particles_Emitter();

void Fire_Particles_Unload();

