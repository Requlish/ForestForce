/******************************************************************************/
/*!
\file		Fileio.cpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file is source file for File Input / Output code.
			This file contains the implementation / definition of
			HELPER FUNCTIONS :
			- Find_Tile_Type()
				- Find and return the corresponding tile type
			- Find_Terrain_Value()
				- Find and return the tile's terrain value
			- Splice_Cell_Stats()
				- Splice the value extracted from the file
			- Split_Objectives_String()
				- Split the string containing the objectives
			- Concat_Cell_Stats()
				- Concate the independent variables into a single three-digit value

			MAIN FUNCTIONS :
			- Read_File()
				- Take in a string containing the file name and read the file
				- Assign values to
					- TILES_X
					- TILES_Y
					- fire_x
					- fire_y
					- fire_strength
				- Call the load functions for tiles, map, player and objectives
			- Write_File()
				- Write a file that contains the data of
					- The grid's width
					- The grid's height
					- The player's x position
					- The player's y position
					- The grid data itself

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "Fileio.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Objectives.hpp"
#include "Level_Editor.hpp"

// Used for Loading Objectives
static std::vector<std::string> obj;
static std::vector<std::string> obj_param;

// Store the tile stats into the a 2D vector
static std::vector <std::vector < Tile_Stats > > Tile_Vector;

// List of keyphrases the function Read_File() to catch
static std::vector<std::string> Game_Object_Stats{ "Width :" ,"Height :" ,"Player X :" , "Player Y :" , "Objectives :" , "Weather :" };

// The following maps are for the the helpers functions to find the values mapped to certain values
static std::map<s8, Map::WEATHER> Weather_Map{ {static_cast<s8>(0), Map::CLOUDY}, {static_cast<s8>(1), Map::CLEAR}, {static_cast<s8>(2), Map::SUNNY}};

static std::map<s8, Map::TYPE> Type_Map{ {static_cast<s8>(0), Map::RIVER}, {static_cast<s8>(1), Map::GRASS}, {static_cast<s8>(2), Map::DIRT} };

static std::map<s8, Map::TERRAIN> Terrain_Map{ {static_cast<s8>(0), Map::NOTHING}, {static_cast<s8>(1), Map::BUSH}, {static_cast<s8>(2), Map::TREE}, {static_cast<s8>(3), Map::HOUSE}, {static_cast<s8>(4), Map::HILL}, {static_cast<s8>(5), Map::MOUNTAIN}, {static_cast<s8>(6),Map::ENEMY} };

static std::map<std::string, s8> Objectives_list{ {"EM", Objectives::EMPTY}, {"CA", Objectives::CATCH_ARSONIST}, {"TL", Objectives::TIME_LIMIT}, {"TC", Objectives::TREE_COUNT}, {"SH", Objectives::SAVE_HOUSE} };

namespace {
	// HELPER FUNCTIONS
	// Find and return the weather
	Map::WEATHER Find_Weather_Value(s8 const& Weather)
	{
		return Weather_Map.find(Weather)->second;
	}

	// Find and return the corresponding tile type value
	Map::TYPE Find_Tile_Type(s8 const& tiles_type)
	{
		return Type_Map.find(tiles_type)->second;
	}

	// Find and return the corresponding terrain value
	Map::TERRAIN Find_Terrain_Value(s8 const& tile_Terrain_Value)
	{
		return Terrain_Map.find(tile_Terrain_Value)->second;
	}

	// Find and return the number in reference to the string passed
	s8 Find_Objective(std::string const& objective_string)
	{
		return Objectives_list.find(objective_string)->second;
	}

	// Splice the numbers of each cell
	Tile_Stats Splice_Cell_Stats(s64 const& cell_stats)
	{
		Tile_Stats tmp{ 0,0,0 };
		tmp.type_value = static_cast<s8>(cell_stats / 100);
		tmp.terrain_value = (cell_stats % 100) / 10;
		tmp.fire_water_strength_value = (cell_stats % 100) % 10;
		return tmp;
	}

	// Split / Splice the string under Objectives 
	void Split_Objectives_String(std::string const& objtive_str)
	{
		std::string current_str;

		for (char const& w : objtive_str)
		{
			current_str += w;
			if (w == ' ' || w == '\r' || w == '\t' || w == '\n' || w == '\f' || w == '\v')
			{
				size_t is_num = current_str.find_first_of("1234567890");
				current_str.erase(current_str.end() - 1, current_str.end());
				if (is_num != std::string::npos)
				{
					obj_param.push_back(current_str);
				}
				else {
					obj.push_back(current_str);
				}
				current_str = "";
			}
		}

	}

	std::string Create_Objective_String(std::vector<u8> const& objective_list, Editor_Objectives const& obj_list)
	{
		std::stringstream objective_string{};
		for (size_t i{}; i < objective_list.size(); ++i)
		{
			switch (objective_list[i])
			{
			case(Objectives::EMPTY):
				objective_string << "EM " << 0 << " ";
				break;
			case(Objectives::CATCH_ARSONIST):
				objective_string << "CA " << static_cast<s32>(obj_list.arsonist_coord.first) << "," << static_cast<s32>(obj_list.arsonist_coord.second) << " ";
				break;
			case(Objectives::SAVE_HOUSE):
				objective_string << "SH " << static_cast<s32>(obj_list.house_coord.first) << "," << static_cast<s32>(obj_list.house_coord.second) << " ";
				break;
			case(Objectives::TIME_LIMIT):
				objective_string << "TL " << obj_list.time_limit << " ";
				break;
			case(Objectives::TREE_COUNT):
				objective_string << "TC " << obj_list.tree_count << " ";
				break;
			}

		}
		return objective_string.str();
	}

	// concatenate values from the three variables into a single value
	std::string Concat_Cell_Stats(s8 const& cell_type, s8 const& cell_terrain, s8 const& cell_fw_strength)
	{
		std::stringstream cell_stats{}; 
		cell_stats << static_cast<s32>(cell_type) << static_cast<s32>(cell_terrain) << static_cast<s32>(cell_fw_strength);
		return cell_stats.str();
	}
}

// Read .txt file from the Assets folder via the specified file name
int Read_File(std::string const& filename)
{
	s8 player_x{}, player_y{};
	s8 weather_value{};
	size_t num_pos;
	std::string num_string;

	std::fstream open_level_file(filename, std::ios_base::in);
	if (!open_level_file.is_open())
	{
		next = GS_QUIT;
		return 0;
	}

	// Check the first 5 lines of the file
	for (size_t i{}; i < Game_Object_Stats.size(); ++i)
	{
		std::string Current_File_Line;
		getline(open_level_file, Current_File_Line);

		num_pos = Current_File_Line.find_first_of("1234567890");
		num_string = Current_File_Line.substr(num_pos);

		// Now check each line
		// If the line contains "Width : "
		// Assign the value to TILE_X
		if (Game_Object_Stats[i] == "Width :")
		{
			TILE_X = static_cast<s8>(std::stoi(num_string));
		}

		// If the line contains "Height : "
		// Assign the value to TILE_Y
		else if (Game_Object_Stats[i] == "Height :")
		{
			TILE_Y = static_cast<s8>(std::stoi(num_string));
		}

		// If the line contains "Player X : "
		// Assign the value to player_x
		else if (Game_Object_Stats[i] == "Player X :")
		{
			player_x = static_cast<s8>(std::stoi(num_string));
		}

		// If the line contains "Player Y : "
		// Assign the value to player_y
		else if (Game_Object_Stats[i] == "Player Y :")
		{
			player_y = static_cast<s8>(std::stoi(num_string));
		}

		// If the line contains "Objectives : "
		// Split the string into the three objectives in the string
		else if (Game_Object_Stats[i] == "Objectives :")
		{
			size_t colon = Current_File_Line.find_first_of(":");
			std::string objtive_string = Current_File_Line.substr(colon + 2);

			// This call to Split_Objective_String will split the string
			// Into the three objectives
			obj.clear();
			obj_param.clear();
			Split_Objectives_String(objtive_string);
		}

		else if (Game_Object_Stats[i] == "Weather :")
		{
			weather_value = static_cast<s8>(std::stoi(num_string));
			weather_value = Find_Weather_Value(weather_value);
		}
	}

	// Initializing the tile vector first
	Tile_Vector.resize(TILE_X);

	for (s8 i{}; i < TILE_X; ++i)
	{
		Tile_Vector[i] = std::vector <Tile_Stats>(TILE_Y);
		for (s8 j{}; j < TILE_Y; ++j)
		{
			Tile_Vector[i][j].type_value = 0;
			Tile_Vector[i][j].terrain_value = 0;
			Tile_Vector[i][j].fire_water_strength_value = 0;
		}
	}
	std::vector<Fire_List> fire_list;

	// Now set the values from the tile vector
	for (s8 i = (TILE_Y - 1); i >= 0; --i)
	{
		for (s8 j{}; j < TILE_X; ++j)
		{
			s64 cell_values;
			open_level_file >> cell_values;
			Tile_Vector[j][i] = Splice_Cell_Stats(cell_values);

			if (Tile_Vector[j][i].fire_water_strength_value != 0)
			{
				Fire_List tmp{ j,i,Tile_Vector[j][i].fire_water_strength_value };
				fire_list.push_back(tmp);
			}
		}
	}

	// Now call Map_Load with TILE_X and TILE_Y as parameters
	level_map.Map_Load(TILE_X, TILE_Y);

	for (s8 y = (TILE_Y - 1); y >= 0; --y)
	{
		for (s8 x{}; x < TILE_X; ++x)
		{
			level_map.Tile_Init(Find_Tile_Type(Tile_Vector[x][y].type_value), x, y, Find_Terrain_Value(Tile_Vector[x][y].terrain_value));
		}
	}
	
	// Call Set_Init_Fire and set the fire's x and y positions as well as strength
	for (size_t i{}; i < fire_list.size(); ++i)
	{
		level_map.Set_Init_Fire(fire_list[i].fire_x, fire_list[i].fire_y, fire_list[i].fire_strength);
	}
	level_map.Weather_Init(weather_value);
	// Call Load member function from player to set the player's initial position
	player.Load(player_x, player_y);

	// Call Load member function from objectives to set the objectives of the level
	objectives.Load(Find_Objective(obj[0]), obj_param[0], Find_Objective(obj[1]), obj_param[1], Find_Objective(obj[2]), obj_param[2]);

	//Close file stream
	open_level_file.close();

	return 1;
}

// Write data collected from level editor to a .txt file
int Write_File(Tiles **Created_Grid, Game_object_Stats game_obj_stats, std::vector<u8> const& objective_list, Editor_Objectives const& editor_obj ,s8 weather)
{
	// write data into file level7.txt (user generated level)
	std::string level = "Data/Levels/level";
	level += std::to_string(LAST_LEVEL - GS_LEVEL1 + 1) + ".txt";
	std::fstream write_level_file(level.c_str(), std::ios_base::out);
	
	// Check if can write into a .txt file
	if (!write_level_file.is_open())
	{
		return 0;
	}

	// Write into ofstream the first 5 necessary stats
	write_level_file << "Width : " << static_cast<s32>(TILE_X) << '\n';
	write_level_file << "Height : " << static_cast<s32>(TILE_Y) << '\n';
	write_level_file << "Player X : " << static_cast<s32>(game_obj_stats.player_x) << '\n';
	write_level_file << "Player Y : " << static_cast<s32>(game_obj_stats.player_y) << '\n';
	write_level_file << "Objectives : " << Create_Objective_String(objective_list, editor_obj) << "\n";
	write_level_file << "Weather : " << static_cast<s32>(weather) << "\n";

	for (s8 y = (TILE_Y - 1); y >= 0; --y)
	{
		for (s8 x{}; x < TILE_X; ++x)
		{

			// Call to Concat_Cell_Stats to concatenate the three values 
			std::string cell_stats = Concat_Cell_Stats(Created_Grid[y][x].tile_type, Created_Grid[y][x].terrain_type, Created_Grid[y][x].fire);

			// Now insert the value in cell_stats into the file stream
			write_level_file << cell_stats << " ";
		}
		write_level_file << '\n';
	}

	// Close the file stream
	write_level_file.close();

	return 1;
}
