/******************************************************************************/
/*!
\file		Click_Particle_System.cpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements particle system on-click.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Click_Particle_System.hpp"

static std::vector<Particle> click_particles;
static size_t alive;

/**************************************************************************/
/*!
	Resizes vector to fit maximum number of particles
	Sets alive counter to 0
*/
/**************************************************************************/
void Click_Particles_Load() {
	click_particles.resize(CLICK_PARTICLE_MAX_AMOUNT);
	alive = 0;
}

/**************************************************************************/
/*!
	Spawns particles at mouse position
	Creates particles with predefined range of values
*/
/**************************************************************************/
void Click_Particles_Spawn(s32 mouseX, s32 mouseY, size_t numParticles) {
	for (size_t i = 0; i < numParticles; ++i) {
		if (alive < CLICK_PARTICLE_MAX_AMOUNT) {
			Particle data = {};
			data.x = static_cast<f32>(mouseX);
			data.y = static_cast<f32>(mouseY);
			data.vel_x = Random_Float(-CLICK_PARTICLE_VELOCITY, CLICK_PARTICLE_VELOCITY);
			data.vel_y = Random_Float(-CLICK_PARTICLE_VELOCITY, CLICK_PARTICLE_VELOCITY);
			data.lifetime = Random_Float(static_cast<float>(CLICK_PARTICLE_LIFETIME_MIN), static_cast<float>(CLICK_PARTICLE_LIFETIME_MAX));
			data.scale_x = CLICK_PARTICLE_SCALE;
			data.scale_y = CLICK_PARTICLE_SCALE;
			data.rotation = Random_Float(0.0f, 2.0f * PI);
			data.color_r = Random_Float(0.0f, 1.0f);
			data.color_g = Random_Float(0.0f, 1.0f);
			data.color_b = Random_Float(0.0f, 1.0f);

			++alive;
			click_particles[alive - 1] = data;
		}
	}
}

/**************************************************************************/
/*!
	Updates position of particles based on velocity and delta time
*/
/**************************************************************************/
void Click_Particles_Update() {
	s32 mouse_x, mouse_y;
	Get_Mouse_World_Coordinates(mouse_x, mouse_y);
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		Click_Particles_Spawn(mouse_x, mouse_y, CLICK_PARTICLE_SPAWN_ONCLICK);
	}

	for (size_t i = 0; i < alive; ++i) {
		click_particles[i].lifetime -= delta_time;
		click_particles[i].x += click_particles[i].vel_x * static_cast<f32>(delta_time);
		click_particles[i].y += click_particles[i].vel_y * static_cast<f32>(delta_time);
	}
}

/**************************************************************************/
/*!
	Swaps 'dead' particles with new particles
	Decrements 'alive' counter
*/
/**************************************************************************/
void Click_Particles_Swap() {
	for (size_t i = 0; i < alive; ++i) {
		if (click_particles[i].lifetime < 0.0f) {
			Particle swap = click_particles[i];
			click_particles[i] = click_particles[alive - 1];
			click_particles[alive - 1] = swap;
			--alive;
		}
	}
}

/**************************************************************************/
/*!
	Renders and draws particles
*/
/**************************************************************************/
void Click_Particles_Emitter() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	for (size_t i = 0; i < alive; ++i) {
		// Scale, Rotate, Translate, Transform
		AEMtx33 scale, rot, trans, transform;

		AEMtx33Scale(&scale, click_particles[i].scale_x, click_particles[i].scale_y);
		AEMtx33Rot(&rot, click_particles[i].rotation);
		AEMtx33Trans(&trans, click_particles[i].x, click_particles[i].y);
		AEMtx33Concat(&transform, &rot, &scale);
		AEMtx33Concat(&transform, &trans, &transform);
		AEGfxSetTransform(transform.m);
		AEGfxSetColorToMultiply(static_cast<float>(click_particles[i].color_r),
								static_cast<float>(click_particles[i].color_g), 
								static_cast<float>(click_particles[i].color_b), 
								static_cast<float>(click_particles[i].lifetime / CLICK_PARTICLE_LIFETIME_MAX));
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}
}
