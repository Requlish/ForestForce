/******************************************************************************/
/*!
\file		GameStateManager.hpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains header for the Game State Manager.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

typedef void(*FP)(void);

extern int current, previous, next;

extern FP fp_load, fp_initialize, fp_update, fp_draw, fp_free, fp_unload;

void GSM_Initialize(int starting_state);
void GSM_Update();

