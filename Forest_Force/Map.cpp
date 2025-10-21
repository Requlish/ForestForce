/******************************************************************************/
/*!
\file		Map.cpp
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
#include "pch.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "Audio.hpp"

Map level_map{};

Tile_Asset map_asset;

//Annonymous Namespace
namespace
{
	f64 behaviour_timer{};
	f64 fire_timer{};
}

/**************************************************************************/
/*!
	 This function Loads all Map Asset 
*/
/**************************************************************************/
void Tile_Asset::Tile_Asset_Load()
{
	grass_tile = AEGfxTextureLoad("Assets/Tile/grass_tile.png");
	river_tile = AEGfxTextureLoad("Assets/Tile/river_tile.png");
	river_tile_2 = AEGfxTextureLoad("Assets/Tile/river_tile_2.png");
	dirt_tile = AEGfxTextureLoad("Assets/Tile/dirt_tile.png");
	tree_obj = AEGfxTextureLoad("Assets/Terrain/tree.png");
	bush_obj = AEGfxTextureLoad("Assets/Terrain/bush.png");
	mountain_obj = AEGfxTextureLoad("Assets/Terrain/mountain.png");
	house_obj = AEGfxTextureLoad("Assets/Terrain/house.png");
	hill_obj = AEGfxTextureLoad("Assets/Terrain/hill.png");
	fire_obj = AEGfxTextureLoad("Assets/Terrain/fire.png");
	enemy_obj = AEGfxTextureLoad("Assets/Unit/enemy.png");
	background_obj = AEGfxTextureLoad("Assets/Menu/forest_background.jpg");

}

/**************************************************************************/
/*!
	 This function Unloads all Map Asset
*/
/**************************************************************************/
void Tile_Asset::Tile_Asset_Unload()
{
	AEGfxTextureUnload(grass_tile);
	AEGfxTextureUnload(river_tile);
	AEGfxTextureUnload(river_tile_2);
	AEGfxTextureUnload(dirt_tile);
	AEGfxTextureUnload(tree_obj);
	AEGfxTextureUnload(bush_obj);
	AEGfxTextureUnload(mountain_obj);
	AEGfxTextureUnload(house_obj);
	AEGfxTextureUnload(hill_obj);
	AEGfxTextureUnload(fire_obj);
	AEGfxTextureUnload(enemy_obj);
	AEGfxTextureUnload(background_obj);
}

/**************************************************************************/
/*!
	 Map Constructor
*/
/**************************************************************************/
Map::Map(s8 x_value, s8 y_value) : x_size{ x_value }, y_size{ y_value }, refer_grid{ nullptr }, display_grid{ nullptr }
{
}

/**************************************************************************/
/*!
	 This function Loads the Map object and dynamically allocates 
	 2D Tile arrays and initializes other map parameters
*/
/**************************************************************************/
void Map::Map_Load(s8 x_value, s8 y_value)
{
	x_size = x_value;
	y_size = y_value;
	if (refer_grid == nullptr && display_grid == nullptr)
	{
		display_grid = new Tiles * [y_size] {};
		refer_grid = new Tiles * [y_size] {};
		init_grid = new Tiles * [y_size] {};
		for (int i{}; i < y_size; ++i)
		{
			display_grid[i] = new Tiles[x_size]{};
			refer_grid[i] = new Tiles[x_size]{};
			init_grid[i] = new Tiles[x_size]{};
		}
		initial_display_grid = display_grid;
		initial_refer_grid = refer_grid;
	}

	max_wind_time = WIND_TIMER;
	max_earth_durability = MAX_EARTH_DURABILTY;
	max_wood_durability = MAX_WOOD_DURABILTY;
	max_house_durability = MAX_HOUSE_DURABILTY;

}

/**************************************************************************/
/*!
	 This function initializes the double buffer of the Map object and
	 also initializes the relavant objective variables
*/
/**************************************************************************/
void Map::Map_Init()
{
	display_grid = initial_display_grid;
	refer_grid = initial_refer_grid;
	for (int i{}; i < TILE_Y; ++i)
	{
		for (int j{}; j < TILE_X; ++j)
		{
			display_grid[i][j] = init_grid[i][j];
			refer_grid[i][j] = init_grid[i][j];
		}

	}

	Objective_Init();


}

/**************************************************************************/
/*!
	 This function unloads the Map object and Frees any dynamically allocated
	 objects
*/
/**************************************************************************/
void Map::Map_Unload()
{
	if (refer_grid != nullptr && display_grid != nullptr)
	{

		for (int i{}; i < y_size; ++i)
		{
			delete[] display_grid[i];
			delete[] refer_grid[i];
			delete[] init_grid[i];
		}
		delete[] display_grid;
		delete[] refer_grid;
		delete[] init_grid;
	}
	refer_grid = nullptr;
	display_grid = nullptr;
	init_grid = nullptr;
}



/**************************************************************************/
/*!
	 Accessor Function to Fire spread Time
*/
/**************************************************************************/
f64 Map::Get_Fire_Time()
{
	return fire_spread_time;
}

/**************************************************************************/
/*!
	 Accessor Function to get Tile fire value
*/
/**************************************************************************/
s8 Map::Get_Fire( s8 x_value,  s8 y_value)
{
	if (y_value > (y_size - static_cast<s8>(1)) || y_value < static_cast<s8>(0) || x_value >(x_size - static_cast<s8>(1)) || x_value < static_cast<s8>(0))
		return 0;
	else return display_grid != nullptr ? display_grid[y_value][x_value].fire : 0;
}

/**************************************************************************/
/*!
	 Accessor Function to get specified Tile* position of Updatable 2D Tile array
*/
/**************************************************************************/
Tiles* Map::Get_Tile( s8 x_value,  s8 y_value)
{
	return display_grid != nullptr ? &display_grid[y_value][x_value] : nullptr;
}

/**************************************************************************/
/*!
	 Accessor Function to get specified Tile is passable
*/
/**************************************************************************/
bool Map::Get_Passable( s8 x_value,  s8 y_value)
{
	if (y_value > (y_size - static_cast<s8>(1)) || y_value < static_cast<s8>(0) || x_value >(x_size - static_cast<s8>(1)) || x_value < static_cast<s8>(0))
		return 0;
	else return display_grid != nullptr ? display_grid[y_value][x_value].is_passable : 0;
}

/**************************************************************************/
/*!
	 Mutator Function to set specified Tile fire value
*/
/**************************************************************************/
void Map::Set_Fire( s8 x_value,  s8 y_value, s8 fire_value)
{

	if (display_grid[y_value][x_value].terrain_type < Map::HILL && display_grid[y_value][x_value].tile_type != RIVER)
	{
		display_grid[y_value][x_value].fire = fire_value;
	}
}


/**************************************************************************/
/*!
	 Mutator fuction that set fire for init_grid
*/
/**************************************************************************/
void Map::Set_Init_Fire( s8 x_value,  s8 y_value, s8 fire_value)
{

	if (init_grid[y_value][x_value].terrain_type < Map::HILL && init_grid[y_value][x_value].tile_type != RIVER)
	{
		init_grid[y_value][x_value].fire = fire_value;
	}
}

/**************************************************************************/
/*!
	 Mutator fuction that set wind direction of map object
*/
/**************************************************************************/
void Map::Set_Wind(u8 direction)
{
	wind_direction = direction;
	wind_timer = max_wind_time;
}

/**************************************************************************/
/*!
	 Assesor fuction that get wind direction of map object
*/
/**************************************************************************/
u8 Map::Get_Wind()
{
	return wind_direction;
}

/**************************************************************************/
/*!
	 Mutator fuction that set specified Tile earth value and earth related
	 parameters
*/
/**************************************************************************/
void Map::Set_Earth( s8 x_value,  s8 y_value)
{
	if (display_grid[y_value][x_value].terrain_type != Map::ENEMY)
	{	display_grid[y_value][x_value].is_earth_permanant = false;
		refer_grid[y_value][x_value].is_earth_permanant = false;
		
		display_grid[y_value][x_value].earth_durability = max_earth_durability;
		display_grid[y_value][x_value].wood_durability = 0;
		display_grid[y_value][x_value].terrain_type = MOUNTAIN;

		if (display_grid[y_value][x_value].terrain_type == HOUSE)
			house_durability = 0;
		if (display_grid[y_value][x_value].terrain_type == TREE)
			--tree_count;
	}
}


/**************************************************************************/
/*!
	 this function updates the fire/water spread and fire/water interactions
	 with other terrain types
*/
/**************************************************************************/
void Map::Fire_Behaviour( s8 x_value,  s8 y_value)
{
	Tiles& refer = refer_grid[y_value][x_value];
	Tiles& update = display_grid[y_value][x_value];
	bool is_river{};
	s8 temp_fire{};
	s8 fire_count{};
	s8 water_count{};
	s8 max_water{};
	s8 max_fire{};
	s8 raw_wood{ refer.wood_durability };
	s8 raw_fire{};

	//1.) Don't Update Fire if current Tile is River/Mountain/Hill/Enemy
	if (refer.tile_type != RIVER && refer.terrain_type < Map::HILL)
	{
		//2.1) Uses 2 for loops to check surrounding 3 x 3 tiles around the center tile
		for (int j{ -1 }; j < 2; j++)
		{
			//2.2)Reading Out of Bounds Check
			if (((j + y_value) > (y_size - 1)) || ((j + y_value) < 0))continue;
			for (int i{ -1 }; i < 2; i++)
			{
				//2.3)Reading Out of Bounds Check
				if (((i + x_value) > (x_size - 1)) || ((i + x_value) < 0))continue;
				//2.4)Gets Fire value of tiles within 3x3 block
				temp_fire = refer_grid[y_value + j][x_value + i].fire;
				//2.5)Checks if Current tile is next to river within 3x3 block
				if (refer_grid[y_value + j][x_value + i].tile_type == RIVER)is_river = true;
				// 3.)Checks if Fire/Water is present on this tile, Collates total Fire/Water values in 3x3
				//  Tiles may be skipped due to Wind behaviour or Out of Bound from above checks
				if (temp_fire)
				{
					if (temp_fire < 0)
					{
						if (!j || !i)//checks in + shape for water 
						{
							max_water += temp_fire;
							++water_count;
						}
					}
					else
					{

						switch (level_map.wind_direction)
						{
						case(WIND::NO_WIND):
							if (!j|| !i)//No wind check + shape for fire
							{
								max_fire += temp_fire;
								++fire_count;
							}
							break;
						case(WIND::UP):
							if (j == -1 || (!i && !j))//Wind Direction Up
							{
								max_fire += temp_fire;
								++fire_count;
							}
							break;
						case(WIND::DOWN):
							if (j == 1 || (!i && !j))//Wind Direction Down
							{
								max_fire += temp_fire;
								++fire_count;
							}
							break;
						case(WIND::LEFT):
							if (i == 1 || (!i && !j))//Wind Direction Left
							{
								max_fire += temp_fire;
								++fire_count;
							}
							break;
						case(WIND::RIGHT):
							if (i == -1 || (!i && !j))//Wind Direction Right
							{
								max_fire += temp_fire;
								++fire_count;
							}
							break;
						default:
							if (!j || !i)//Guard
							{
								max_fire += temp_fire;
								++fire_count;
							}
							break;
						}
					}
				}
			}
		}
		// 4.)Determines the Raw Fire value of the center tile by summating the postive values(Fire) and negative values(Water)
		//	  To allow Water to be dominant, WATER_STRENGTH_MUTIPLIER is used
		raw_fire = max_water * WATER_STRENGTH_MULTIPLER + max_fire;

		// 5.)Calculates the final Fire/Water behaviour of the current tile based on the Raw Fire Value
		if (raw_fire != 0)// if Raw fire is not 0
		{
			raw_wood = raw_fire > 0 ? refer.wood_durability - refer.fire : refer.wood_durability;//reduce wood durability if fire is present
			if (raw_wood <= 0 && refer.wood_durability > 0 && refer.terrain_type == TERRAIN::TREE)//If was tree and fire burn finish
			{
				--tree_count;

			}
			raw_wood = raw_wood < 0 ? 0 : raw_wood;//Prevent wood durability from going negative
			if (refer.terrain_type == HOUSE)
			{

				house_durability = raw_fire > 0 ? house_durability - refer.fire : house_durability;
				house_durability = house_durability < 0 ? 0 : house_durability;
				
			}
			if (raw_fire > 0)// else if fire wins
			{
				if (Fire_Check())//check if fire can spread
				{
					if (raw_wood > 0 || (refer.terrain_type == HOUSE && Get_Curr_House_Durability()))//checks presence of wood or house
					{
						if (!max_water)//if theres wood AND no water , fire grows bigger
						{
							raw_fire = raw_fire / fire_count + 1;
						}
						else//else fire will be 
						{
							raw_fire = raw_fire / fire_count;
						}
					}
					else//no wood fire grows smaller
					{
						raw_fire = raw_fire / fire_count - 1;
						raw_fire = raw_fire < 0 ? 0 : raw_fire;
					}
				}
				else//don't update fire if not time to spread
				{
					raw_fire = refer.fire;
				}
			}
			else if (raw_fire < 0)// else if water wins
			{
				if (is_river && refer.fire < -2)//River is present and water is max water
				{
					raw_fire = refer.fire;
				}
				else if (max_fire)//Water wins but there fire so no need to manually reduce water level
				{
					raw_fire = (raw_fire / WATER_STRENGTH_MULTIPLER) / water_count;
				}
				else//Water wins but theres no fire , need to manually reduce water level
				{
					raw_fire = (raw_fire / WATER_STRENGTH_MULTIPLER) / water_count + 1;
					raw_fire = raw_fire < 0 ? raw_fire : 0;
				}
			}

		}
		// 6.)Fire Indicator update 
		if (max_fire && !max_water)
		{
			update.is_near_fire = true;
		}
		else
		{
			update.is_near_fire = false;
		}

		//Caps fire to [-3,3]
		raw_fire = raw_fire > MAX_FIRE_VALUE  ? MAX_FIRE_VALUE : raw_fire;
		raw_fire = raw_fire < -MAX_WATER_VALUE ? -MAX_WATER_VALUE : raw_fire;
		update.fire = (raw_fire);
		update.wood_durability = (raw_wood);
	}
	// 7.)Prevents Fire from existing on Tiles with Earth(HILLS/MOUNTAIN)  
	if (refer.earth_durability > 0)
	{
		update.fire = 0;
		update.wood_durability = 0;
		update.is_near_fire = false;
	}
}

/**************************************************************************/
/*!
	 this function updates the earth behaviour of double buffer Tile grid
*/
/**************************************************************************/
void Map::Earth_Behaviour( s8 x_value,  s8 y_value)
{
	Tiles& refer = refer_grid[y_value][x_value];
	Tiles& update = display_grid[y_value][x_value];
	s8 raw_earth = refer.earth_durability;
	if (!refer.is_earth_permanant)
	{
		if (raw_earth > 0)
		{
			raw_earth -= 2;

			raw_earth = raw_earth > 0 ? raw_earth : 0;
		}
		update.earth_durability = raw_earth;
	}

}

/**************************************************************************/
/*!
	 this function updates the wind behaviour of double buffer Tile grid
*/
/**************************************************************************/
void Map::Wind_Behaviour()
{
	if (wind_timer > 0)
	{
		wind_timer -= delta_time;
	}
	else
	{
		wind_direction = WIND::NO_WIND;
	}
}

/**************************************************************************/
/*!
	 this function updates the tile state of double buffer Tile grid
*/
/**************************************************************************/
void Map::State_Behaviour(s8 x_value, s8 y_value)
{
	Tiles& update = display_grid[y_value][x_value];
	bool river = update.tile_type == RIVER;//check presence of river


	if(update.terrain_type != ENEMY)
	{
		if (update.wood_durability == 0 && (update.terrain_type == TREE || update.terrain_type == BUSH))//if becomes no wood durability and was TREE
		{
			update.terrain_type = NOTHING;
			if (river)//if River
			{
				update.is_passable = 0;
			}
			else
			{
				update.is_passable = 1;
			}
		}
		else if (house_durability == 0 && update.terrain_type == HOUSE)//if house no durability and was HOUSE
		{
			update.terrain_type = NOTHING;
			update.is_passable = 1;
		}
		if (!update.is_earth_permanant && update.wood_durability == 0 && update.terrain_type != HOUSE)//if no wood ,no house is present and earth can be changed
		{
			if (update.earth_durability > 0)//if theres earth
			{
				if (update.earth_durability > max_earth_durability / 4 * 3)
				{
					update.terrain_type = MOUNTAIN;
					update.is_passable = 0;
				}
				else
				{
					update.terrain_type = HILL;
					update.is_passable = 1;
				}
			}
			else
			{
				update.terrain_type = NOTHING;
				if (river)
				{
					update.is_passable = 0;
				}
				else
				{
					update.is_passable = 1;
				}
			}
		}
		else if (update.is_earth_permanant)
		{
			if (update.earth_durability > max_earth_durability / 2)
			{

				update.terrain_type = MOUNTAIN;
				update.is_passable = 0;
			}
			else
			{
				update.terrain_type = HILL;
				update.is_passable = 1;
			}
		}
	}

}

/**************************************************************************/
/*!
	 this function initializes a specified tile of 
	 Tile Init grid
*/
/**************************************************************************/
void Map::Tile_Init(s8 type,  s8 x_value,  s8 y_value, u8 terrain)
{

	Tiles& init = init_grid[y_value][x_value];
	init.tile_type = type;


	init.terrain_type = terrain;

	if (init.tile_type == GRASS || init.tile_type == DIRT)
	{
		init.is_passable = 1;
	}

	switch (init.terrain_type)
	{
	case(NOTHING):
		init.earth_durability = 0;
		init.wood_durability = 0;
		init.is_passable = 1;
		break;
	case(HILL):
		init.earth_durability = max_earth_durability / 2;
		init.is_passable = 1;
		init.is_earth_permanant = 1;
		break;
	case(MOUNTAIN):
		init.earth_durability = max_earth_durability;
		init.is_passable = 0;
		init.is_earth_permanant = 1;
		break;
	case(BUSH):
		init.earth_durability = 0;
		init.wood_durability = max_wood_durability/3;
		init.is_passable = 1;
		break;
	case(TREE):
		init.earth_durability = 0;
		init.wood_durability = max_wood_durability;
		init.is_passable = 0;
		break;
	case(HOUSE):
		init.earth_durability = 0;
		init.wood_durability = 0;
		init.is_passable = 0;
		break;
	case(ENEMY):
		init.earth_durability = 0;
		init.wood_durability = 0;
		init.is_passable = 1;
		break;
	default:
		break;
	}

	if (init.tile_type == RIVER)
	{
		init.is_passable = 0;
	}

	init.fire = 0;
}

/**************************************************************************/
/*!
	 Accessor Function to tree count
*/
/**************************************************************************/
s16 Map::Wood_Count()
{
	return tree_count;
}

/**************************************************************************/
/*!
	 This function initializes Weather variable
*/
/**************************************************************************/
void Map::Weather_Init(u8 weather)
{
	map_weather = weather;
	switch (map_weather)
	{
	case(CLOUDY):
		fire_spread_time = BEHAVIOUR_TIME * 3.0;
		break;
	case(CLEAR):
		fire_spread_time = BEHAVIOUR_TIME * 2.0;
		break;
	case(SUNNY):
		fire_spread_time = BEHAVIOUR_TIME;
		break;
	default:
		fire_spread_time = BEHAVIOUR_TIME * 2.0;
		break;
	}
}

/**************************************************************************/
/*!
	 This function initializes the objective vairables such as 
	 tree count and house durability if relevant
*/
/**************************************************************************/
void Map::Objective_Init()
{
	tree_count = 0;
	house_durability = 0;
	if (refer_grid != nullptr || !tree_count) 
	{
		for (int i{}; i < y_size; ++i)
		{
			for (int j{}; j < x_size; ++j)
			{
				if (init_grid[i][j].terrain_type == TREE)++tree_count;
				if (init_grid[i][j].terrain_type == HOUSE)house_durability = max_house_durability;
			}
		}
	}
}

/**************************************************************************/
/*!
	 Accessor fuction to max house durability
*/
/**************************************************************************/
s16 Map::Get_Max_House_Durability()
{
	return max_house_durability;
}

/**************************************************************************/
/*!
	 Accessor fuction to current house durability
*/
/**************************************************************************/
s16 Map::Get_Curr_House_Durability()
{
	return house_durability;
}

/**************************************************************************/
/*!
	 Accessor fuction to refer grid
*/
/**************************************************************************/
Tiles** Map::Get_Refer_Grid()
{
	return refer_grid;
}

/**************************************************************************/
/*!
	 Accessor fuction to Display grid
*/
/**************************************************************************/
Tiles** Map::Get_Display_Grid()
{
	return display_grid;
}

/**************************************************************************/
/*!
	 function is called to give a time based behaviour update
*/
/**************************************************************************/
bool Behaviour_Update() {
	if (behaviour_timer <= 0)
	{
		return true;
	}
	else
	{
		behaviour_timer -= delta_time;
		return false;
	}
}

/**************************************************************************/
/*!
	 function is called to give a time based fire spread update
*/
/**************************************************************************/
bool Fire_Check()
{
	if (fire_timer <= 0) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

/**************************************************************************/
/*!
	 function is called to give a time based fire spread update
*/
/**************************************************************************/
void Fire_Update()
{
	if (fire_timer > 0) {
		fire_timer -= delta_time;
	}
}

/**************************************************************************/
/*!
	 Swap the double buffer grid 
*/
/**************************************************************************/
void Map::Swap_Grid()
{
	Tiles** temp = refer_grid;
	refer_grid = display_grid;
	display_grid = temp;
}

/**************************************************************************/
/*!
	 This function updates all behaviour of Map object
*/
/**************************************************************************/
void Map_Update()
{
	Fire_Update();
	level_map.Wind_Behaviour();
	if (Behaviour_Update())
	{
		level_map.Swap_Grid();
		for (int i = 0; i < TILE_Y; i++)
		{
			for (int j = 0; j < TILE_X; j++)
			{
				level_map.Fire_Behaviour(static_cast<s8>(j), static_cast<s8>(i));
				level_map.Earth_Behaviour(static_cast<s8>(j), static_cast<s8>(i));
				level_map.State_Behaviour(static_cast<s8>(j), static_cast<s8>(i));
			}
		}
		level_map.Player_Lose();

		if (Fire_Check())
		{
			fire_timer = level_map.Get_Fire_Time();
		}
		behaviour_timer = BEHAVIOUR_TIME;
	}

}

/**************************************************************************/
/*!
	This function is called in Map to check for Player lose on behavior
	update
*/
/**************************************************************************/
void Map::Player_Lose()
{
	std::pair<s8, s8> temp = {};
	player.Get_Player_Tiles_Coordinates(temp.first, temp.second);
	if(display_grid[temp.second][temp.first].fire > 0 || !display_grid[temp.second][temp.first].is_passable)
	{
		Start_Music(defeat_music);
		level_state = LS_DEFEAT;
	}
}