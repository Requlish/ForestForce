/******************************************************************************/
/*!
\file		Player.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for the Player Class.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "pch.hpp"

extern f64 PLAYER_BASE_MOVE_SPEED;
extern f64 PLAYER_CAST_WATER_FREQUENCY;
extern f64 PLAYER_CAST_EARTH_FREQUENCY;
extern f64 PLAYER_CAST_AIR_FREQUENCY;
extern s8 PLAYER_WATER_RANGE;
extern s8 PLAYER_EARTH_RANGE;
extern s8 PLAYER_AIR_RANGE;

class Player
{
private:
	s8 intital_x{}, initial_y{};
	s8 x{}, y{};
	// Variable facing is currently not used. This will be used when adding
	// more images/textures to show where the player is moving towards
	u8 facing{}, selected_spell{};

	// Player Move/Cast Speed is stored in Frequency. (5 means, move 5 times a second).
	f64 player_move_speed{}, player_move_cooldown{};
	f64 water_cast_speed{}, earth_cast_speed{}, air_cast_speed{};
	f64 water_spell_cooldown{}, earth_spell_cooldown{}, air_spell_cooldown{};
	s8 water_spell_range{}, earth_spell_range{}, air_spell_range{};

	AEGfxTexture* sprite_texture{ nullptr };
	AEGfxTexture* water_spell_texture{ nullptr };
	AEGfxTexture* earth_spell_texture{ nullptr };
	AEGfxTexture* air_spell_texture{ nullptr };

public:

	enum SPELL : u8 {
		WATER = 0, EARTH, AIR, TOTAL_SPELLS
	};

	enum DIRECTION : u8 {
		UP, DOWN, LEFT, RIGHT
	};

	void Load(s8 player_initial_x, s8 player_initial_y);
	void Initialize();
	void Change_Spell(bool forward);
	bool Castable();
	bool Cast(s8 grid_x, s8 grid_y);
	bool Move(u8 direction);
	bool Moveable();
	void Unload();
	void Get_Player_World_Coordinates(f32& x, f32& y);
	void Get_Player_Tiles_Coordinates(s8& x, s8& y); 

	// Graphics related Functions
	friend void Draw_Player(Player& player);
	friend void Draw_Spell_UI(Player& player);
	friend void Draw_Player_Spell_Range(Player& player);
};


extern Player player;

void Player_Input();