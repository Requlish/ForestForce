/******************************************************************************/
/*!
\file		pch.hpp
\author 	Chua Jim Hans, Lim Zhen Eu Damon, Titus Kwong Wen Shuen, Ho Zhen Hao
\par    	Email: c.jimhans@digipen.edu, l.zheneudamon@digipen.edu
			tituswhenshuen.kwong@digipen.edu, zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file is the Precompiled Header for Forest Force.
			Contribution Percentages:
			Lim Zhen Eu Damon (30%)
			Chua Jim Hans (30%)
			Ho Zhen Hao (20%)
			Titus Kwong Wen Shuen (20%)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "AEEngine.h"
#include <crtdbg.h>					// To check for memory leaks
#include <iostream>					// C++ Library For debugging
#include <fstream>					// C++ Library For File IO
#include <vector>					// C++ Library For Dynamic Array
#include <string>					// C++ Library For String to Text
#include <random>					// C++ Library For random number generator
#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "Utils.hpp"
#include "Particle.hpp"

#endif //PCH_H
