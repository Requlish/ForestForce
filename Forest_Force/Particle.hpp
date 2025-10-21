/******************************************************************************/
/*!
\file		Particle.hpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains a struct that identifies particles for various particle
			systems.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

struct Particle {
	f32 x, y;							// Position 
	f32 vel_x, vel_y;					// Velocity 
	f32 scale_x, scale_y;				// Size
	f32 rotation;						// Rotation
	f32 color_r, color_g, color_b;		// Color 
	f64 lifetime;						// Lifetime 
};