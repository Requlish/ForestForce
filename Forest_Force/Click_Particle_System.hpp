/******************************************************************************/
/*!
\file		Click_Particle_System.hpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements particle system on-click.
			The file contains
			Click_Particles_Load 		- resizes vector to fit maximum
										  number of particles, sets alive
										  counter to 0
			Click_Particles_Spawn		- spawns particles at mouse position,
										  creates particles with predefined
										  range of values
			Click_Particles_Update		- updates position of particles
										  based on velocity and delta time
			Click_Particles_Swap		- swaps 'dead' particles with new 
										  particles, decrements 'alive' counter
			Click_Particles_Emitter		- renders and draws particles

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern size_t CLICK_PARTICLE_MAX_AMOUNT;
extern size_t CLICK_PARTICLE_SPAWN_ONCLICK;
extern f32 CLICK_PARTICLE_SCALE;
extern f32 CLICK_PARTICLE_VELOCITY;
extern f64 CLICK_PARTICLE_LIFETIME_MIN;
extern f64 CLICK_PARTICLE_LIFETIME_MAX;

extern AEGfxVertexList* p_mesh;

void Click_Particles_Load();

void Click_Particles_Spawn(s32 mouseX, s32 mouseY, size_t numParticles);

void Click_Particles_Update();
	 
void Click_Particles_Swap();
	 
void Click_Particles_Emitter();


