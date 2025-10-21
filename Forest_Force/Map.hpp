/******************************************************************************/
/*!
\file		Map.hpp
\author 	Chua Jim Hans
\par    	Email: c.jimhans@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the Map class. Map class contains the main game
			logic of the environmental simulation of Forest Force.
			The main functions of Map.cpp are:
			-Tile_Asset_Load
				Loads Assets to draw the Map of Forest Force
			-Tile_Asset_Unload
				Unloads Assets to draw the Map of Forest Force
			-Map_Load
				Dynamically allocates Tiles array for Map of Forest Force
			-Map_Init
				Initialises the double buffer Tiles arrays and other Map related objects
			-Map_Update
				Updates an alternating double buffers with game logic
			-Map_Unload
				Frees Tiles array for Map of Forest Force
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "pch.hpp"

extern s8 WATER_STRENGTH_MULTIPLER;
extern f64 BEHAVIOUR_TIME;
extern s8 MAX_FIRE_VALUE;
extern s8 MAX_WATER_VALUE;
extern f64 WIND_TIMER;
extern s8 MAX_EARTH_DURABILTY;
extern s8 MAX_WOOD_DURABILTY;
extern s16 MAX_HOUSE_DURABILTY;

struct Tile_Asset
{
	AEGfxTexture* grass_tile{ nullptr };
	AEGfxTexture* river_tile{ nullptr };
	AEGfxTexture* river_tile_2{ nullptr };
	AEGfxTexture* dirt_tile{ nullptr };
	AEGfxTexture* tree_obj{ nullptr };
	AEGfxTexture* bush_obj{ nullptr };
	AEGfxTexture* mountain_obj{ nullptr };
	AEGfxTexture* house_obj{ nullptr };
	AEGfxTexture* hill_obj{ nullptr };
	AEGfxTexture* fire_obj{ nullptr };
	AEGfxTexture* enemy_obj{ nullptr };
	AEGfxTexture* background_obj{ nullptr };
	void Tile_Asset_Load();
	void Tile_Asset_Unload();

};
struct Tiles
{
	s8  fire{}, wood_durability{}, earth_durability{};
	u8 terrain_type{}, tile_type{};
	bool is_passable{}, is_near_fire{}, is_earth_permanant{};
};

class Map
{
	s8 x_size{};
	s8 y_size{};
	Tiles** refer_grid{ nullptr };
	Tiles** display_grid{ nullptr };
	Tiles** init_grid{ nullptr };
	Tiles** initial_refer_grid{ nullptr };
	Tiles** initial_display_grid{ nullptr };
	s16 tree_count{};
	u8 wind_direction{ WIND::NO_WIND };
	u8 map_weather{};
	f64 wind_timer{};
	s16 house_durability{ 0 };
	f64 fire_spread_time{};


public:
	
	 f64 max_wind_time{ };
	 s8 max_earth_durability{ };
	 s8 max_wood_durability{ };
	s16 max_house_durability{ };
	enum TYPE:u8 {
		RIVER = 0, GRASS, DIRT
	};
	enum TERRAIN :u8 {
		NOTHING = 0, BUSH, TREE, HOUSE, HILL, MOUNTAIN ,ENEMY
	};
	enum WIND :u8 {
		NO_WIND = 0, UP, DOWN, LEFT, RIGHT
	};
	enum WEATHER :u8 {
		CLOUDY = 0, CLEAR, SUNNY
	};

	Map(s8 x_value = 0, s8 y_value = 0);

	void Map_Unload();

	void Map_Load(s8 x_value, s8 y_value);

	//This function is currently not being called anywhere,
	//but will be done in the future when addition checks 
	//for player_lose are implemented
	s8 Get_Fire( s8 x_value,  s8 y_value);

	bool Get_Passable( s8 x_value,  s8 y_value);

	Tiles* Get_Tile( s8 x_value,  s8 y_value);

	void Set_Fire( s8 x_value,  s8 y_value, s8 fire_value);

	f64 Get_Fire_Time();

	void Set_Init_Fire( s8 x_value,  s8 y_value, s8 fire_value);

	void Set_Earth( s8 x_value,  s8 y_value);

	void Set_Wind(u8 direction);

	u8 Get_Wind();

	void Map_Init();

	void Tile_Init(s8 type,  s8 x_value,  s8 y_value, u8 terrain);

	void Weather_Init(u8 weather);

	void Objective_Init();

	s16 Get_Max_House_Durability();

	s16 Get_Curr_House_Durability();

	s16 Wood_Count();

	void Fire_Behaviour( s8 x_value,  s8 y_value);

	void Earth_Behaviour( s8 x_value,  s8 y_value);

	void Wind_Behaviour();

	void State_Behaviour( s8 x_value,  s8 y_value);

	void Swap_Grid();

	Tiles** Get_Refer_Grid();

	Tiles** Get_Display_Grid();

	void Player_Lose();
};
void Map_Update();
bool Behaviour_Update();
bool Fire_Check();
void Fire_Update();


extern Map level_map;
extern Tile_Asset map_asset;