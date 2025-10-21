/******************************************************************************/
/*!
\file		Fileio.hpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the header for the File Input / Output code.
			This file contains the declaration / prototype of the functions
			Read_File() and Write_File()

			This file also contains the struct the represents the data to be
			included in each tile

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "pch.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Objectives.hpp"
#include "Level_Editor.hpp"
#include <map>
#include <sstream>

struct Tile_Stats {
	s8 fire_water_strength_value;
	s8 type_value;
	s8 terrain_value;
};

struct Fire_List {
	s8 fire_x;
	s8 fire_y;
	s8 fire_strength;
};

int Read_File(std::string const& filename);

int Write_File(Tiles** Created_Grid, Game_object_Stats game_obj_stats, std::vector<u8> const& objective_list, Editor_Objectives const& editor_obj, s8 weather);