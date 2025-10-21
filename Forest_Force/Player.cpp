/******************************************************************************/
/*!
\file		Player.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for the player class.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Player.hpp"
#include "Map.hpp"
#include "Level.hpp"
#include "Graphics.hpp"
#include "Graphics_Utils.hpp"
#include "Audio.hpp"
#include "Water_Particle_System.hpp"
#include "Earth_Particle_System.hpp"

Player player;

/**************************************************************************/
/*!
	This function sets the spawn location of the player and the textures
	as well as the various const read from file.
*/
/**************************************************************************/
void Player::Load(s8 player_initial_x, s8 player_initial_y) {
	this->intital_x = player_initial_x;
	this->initial_y = player_initial_y;
	this->sprite_texture = AEGfxTextureLoad("Assets/Unit/player.png");
	this->water_spell_texture = AEGfxTextureLoad("Assets/UI/Spell/water_spell.png");
	this->earth_spell_texture = AEGfxTextureLoad("Assets/UI/Spell/earth_spell.png");
	this->air_spell_texture = AEGfxTextureLoad("Assets/UI/Spell/air_spell.png");

	// Updating CONST
	this->player_move_speed = PLAYER_BASE_MOVE_SPEED;
	this->water_cast_speed = 1.0 / PLAYER_CAST_WATER_FREQUENCY;
	this->earth_cast_speed = 1.0 / PLAYER_CAST_EARTH_FREQUENCY;
	this->air_cast_speed = 1.0 / PLAYER_CAST_AIR_FREQUENCY;
	this->water_spell_range = PLAYER_WATER_RANGE;
	this->earth_spell_range = PLAYER_EARTH_RANGE;
	this->air_spell_range = PLAYER_AIR_RANGE;
}

/**************************************************************************/
/*!
	This function unloads the textures for the player (and its spells)
*/
/**************************************************************************/
void Player::Unload() {
	AEGfxTextureUnload(this->sprite_texture);
	AEGfxTextureUnload(this->water_spell_texture);
	AEGfxTextureUnload(this->earth_spell_texture);
	AEGfxTextureUnload(this->air_spell_texture);
}

/**************************************************************************/
/*!
	This is an accessor for the player's world coords
*/
/**************************************************************************/
void Player::Get_Player_World_Coordinates(f32& x_coord,  f32& y_coord) 
{
	Get_Tiles_To_World_Coordinates(this->x, this->y, x_coord, y_coord);

}

/**************************************************************************/
/*!
	This is an accessor for the player's tile coords
*/
/**************************************************************************/
void Player::Get_Player_Tiles_Coordinates(s8& x_coord,s8& y_coord) {
	x_coord = this->x;
	y_coord = this->y;
}

/**************************************************************************/
/*!
	This function is resets the players cooldown and is called on restart.
*/
/**************************************************************************/
void Player::Initialize() {
	this->x = this->intital_x;
	this->y = this->initial_y;

	this->facing = DOWN;
	this->selected_spell = WATER;
	this->player_move_speed = PLAYER_BASE_MOVE_SPEED;
	this->player_move_cooldown = 0;
	this->water_spell_cooldown = 0;
	this->earth_spell_cooldown = 0;
	this->air_spell_cooldown = 0;
}

/**************************************************************************/
/*!
	This function is called to change the spell. The boolean determines 
	rotation / next spell.
*/
/**************************************************************************/
void Player::Change_Spell(bool forward) {
	if (forward) {
		this->selected_spell = (this->selected_spell + 1) % TOTAL_SPELLS;
		next_rotate = true;
	}
	else {
		this->selected_spell = (TOTAL_SPELLS + this->selected_spell - 1) % TOTAL_SPELLS;
		previous_rotate = true;
	}
	switch (this->selected_spell) {
	case WATER:
		// Water Spell
		Play_Sound(swap_water);
		this->selected_spell = WATER;
		break;
	case EARTH:
		// Earth Spell
		Play_Sound(swap_earth);
		this->selected_spell = EARTH;
		break;
	case AIR:
		// AIR Spell
		Play_Sound(swap_air);
		this->selected_spell = AIR;
		break;
	}
}

/**************************************************************************/
/*!
	This function is called each frame to reduce cooldown and check if
	spell can be casted.
*/
/**************************************************************************/
bool Player::Castable() {
	// 0. Decrement Cool Downs
	this->water_spell_cooldown -= delta_time;
	this->earth_spell_cooldown -= delta_time;
	this->air_spell_cooldown -= delta_time;

	// 1. Check if current spell is castable
	bool can_cast{ false };
	switch (this->selected_spell) {
	case WATER:
		// Water Spell
		can_cast = (this->water_spell_cooldown <= 0);
		break;
	case EARTH:
		// Earth Spell
		can_cast = (this->earth_spell_cooldown <= 0);
		break;
	case AIR:
		// AIR Spell
		can_cast = (this->air_spell_cooldown <= 0);
		break;
	}

	// 2. Return Result
	return can_cast;
}

/**************************************************************************/
/*!
	This function is called on click when player can cast its spell. It
	will check if the spell is in range and cast the appropriate spell.
	It also contains the behaviour for each spell cast.
*/
/**************************************************************************/
bool Player::Cast(s8 grid_x, s8 grid_y) {
	bool out_of_range{ false };
	switch (this->selected_spell) {
	case WATER: // Water Spell
		
		out_of_range = (std::abs(this->x - grid_x) + std::abs(this->y - grid_y)) > this->water_spell_range;
		if (out_of_range) {
			return false;
		}
		// This function is incharge of setting both fire and water. Water is negative.
		level_map.Set_Fire(static_cast<s64>(grid_x), static_cast<s64>(grid_y), -3);
		Water_Particles_Spawn(grid_x, grid_y);
		this->water_spell_cooldown = 1.0 / this->water_cast_speed;
		Play_Sound(cast_water);
		break;
	case EARTH: // Earth Spell
		out_of_range = (std::abs(this->x - grid_x) + std::abs(this->y - grid_y)) > this->earth_spell_range;
		if (out_of_range) {
			return false;
		}
		level_map.Set_Earth(static_cast<s64>(grid_x), static_cast<s64>(grid_y));
		Earth_Particles_Spawn(grid_x, grid_y);
		this->earth_spell_cooldown = 1.0 / this->earth_cast_speed;
		Play_Sound(cast_earth);
		break;
	case AIR: // AIR Spell
		out_of_range = (std::abs(this->x - grid_x) + std::abs(this->y - grid_y)) > this->air_spell_range;
		if (out_of_range) {
			return false;
		}

		s8 i{ grid_x - this->x }; // i is x value. (-1 if left of player)
		s8 j{ grid_y - this->y }; // j is y value. (-1 if down of player)
		if(i)
		{
			if (i < 0)level_map.Set_Wind(Map::WIND::LEFT);
			if (i > 0)level_map.Set_Wind(Map::WIND::RIGHT);
		}
		else if (j)
		{
			if (j < 0)level_map.Set_Wind(Map::WIND::DOWN);
			if (j > 0)level_map.Set_Wind(Map::WIND::UP);
		}
		else
		{
			level_map.Set_Wind(Map::WIND::NO_WIND);
		}

		this->air_spell_cooldown = 1.0 / this->air_cast_speed;
		Play_Sound(cast_air);
		break;
	}
	return true;
}

/**************************************************************************/
/*!
	This function checks if player is moveable. It also handles the 
	decrement of the cool down. It updates a boolean that identifies if the 
	player has moved.
*/
/**************************************************************************/
bool Player::Moveable() {
	player_moved = false;
	if (this->player_move_cooldown <= 0) {
		this->player_move_speed = PLAYER_BASE_MOVE_SPEED;
		return true;
	}
	else {
		this->player_move_cooldown -= delta_time;
		return false;
	}
}

/**************************************************************************/
/*!
	This function moves the player based on the direction. It handles the
	checks if the player is moving out of the map / into impassable terrain.
*/
/**************************************************************************/
bool Player::Move(u8 direction) {
	s8 x_movement{}, y_movement{};
	switch (direction) {
	case LEFT:
		x_movement--;
		this->facing = LEFT;
		break;
	case RIGHT:
		x_movement++;
		this->facing = RIGHT;
		break;
	case UP:
		y_movement++;
		this->facing = UP;
		break;
	case DOWN:
		y_movement--;
		this->facing = DOWN;
		break;
	}

	bool out_of_bounds = (static_cast<s64>(this->x)+ x_movement < 0) || (static_cast<s64>(this->x)+ x_movement >= TILE_X) || (static_cast<s64>(this->y) + y_movement < 0) || (static_cast<s64>(this->y) + y_movement >= TILE_Y);
	bool tile_passable{false};

	// Only run "tile_passable" check if not out_of_bounds
	if (!out_of_bounds) tile_passable = (level_map.Get_Passable(static_cast<s64>(this->x) + x_movement, static_cast<s64>(this->y) + y_movement));
	else return false;
	
	if (tile_passable) {	//BUGFIX: Check Player lose when walking into fire (level_map.tile)
		player_moved = true;
		this->x += x_movement;
		this->y += y_movement;
		// Check if player got die
		if (level_map.Get_Fire(this->x, this->y) > 0) { 
			Start_Music(defeat_music);
			level_state = LS_DEFEAT;
		}

		// Check if player walking on slower train
		u8 walking_terrain = level_map.Get_Tile(this->x, this->y)->terrain_type;
		if (walking_terrain == Map::TERRAIN::BUSH || walking_terrain == Map::TERRAIN::HILL) {
			// Slower Move speed when bashing through jungle and climbing hills
			this->player_move_speed /= 2;
		}
		this->player_move_cooldown = 1.0 / this->player_move_speed;
		return true;
	}
	else {
		return false;
	}
}

/**************************************************************************/
/*!
	This function encapsulates all the input required for the player. This
	is called in the level game state.
*/
/**************************************************************************/
void Player_Input() {
	// Only Check Input if Player can move
	if (player.Moveable()) {
		if (AEInputCheckCurr(AEVK_W)) {
			player.Move(Player::UP);
		}
		else if (AEInputCheckCurr(AEVK_A)) {
			player.Move(Player::LEFT);
		}
		else if (AEInputCheckCurr(AEVK_S)) {
			player.Move(Player::DOWN);
		}
		else if (AEInputCheckCurr(AEVK_D)) {
			player.Move(Player::RIGHT);
		}
	}

	// Only Check Input if Player can cast Spells
	if (player.Castable()) {
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			s8 grid_x{}, grid_y{};
			Get_Mouse_Tile_Coordinates(grid_x, grid_y);

			if (grid_x != INVALID_TILE)
				player.Cast(grid_x, grid_y);
		}
	}

	// Checks for Switching Spells
	if (AEInputCheckTriggered(AEVK_E)) {
		player.Change_Spell(true);
	}
	else if (AEInputCheckTriggered(AEVK_Q)) {
		player.Change_Spell(false);
	}
}


