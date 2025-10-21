/******************************************************************************/
/*!
\file		Graphics.cpp
\author 	Chua Jim Hans, Lim Zhen Eu Damon, Ho Zhen Hao
\par    	Email: c.jimhans@digipen.edu, l.zheneudamon@digipen.edu,
			zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the draw functions for GS_LEVEL
			The main functions include:
			-Draw_Player(Chua Jim Hans)
				Draws player sprite with lerp animations
			-Draw_Background(Chua Jim Hans)
				Draws the background for the main game
			-Draw_Spell_UI(Lim Zhen Eu Damon)
				Draws spellcast UI with lerp animations
			-Draw_Map(Chua Jim Hans)
				Draws 2D map of the current update frame
			-Draw_Player_Spell_Range(Lim Zhen Eu Damon)
				Draws spell range fixed to player position
			-Draw_Paused_Objectives_UI(Lim Zhen Eu Damon)
				Draws objective description while paused
			-Draw_Objectives_UI(Lim Zhen Eu Damon)
				Draws objective UI with respect to current level objectives
			-Draw_Victory_Screen(Lim Zhen Eu Damon)
				Draws victory pop-up and buttons
			-Draw_Paused_Screen(Lim Zhen Eu Damon)
				Draws paused pop-up and buttons
			-Draw_Defeat_Screen(Lim Zhen Eu Damon)
				Draws defeat pop-up and buttons
			-Draw_Map_Editor(Ho Zhen Hao)
				Draws the map specifically for the editor
			-Draw_Player_Editor(Ho Zhen Hao)
				Draws the player specifically for the player
			-Draw_How_To_Play(Lim Zhen Eu Damon)
				Draws the how to play image
			Contribution Percentages:
			Lim Zhen Eu Damon (40%)
			Chua Jim Hans (30%)
			Ho Zhen Hao (30%)
		
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Graphics_Utils.hpp"
#include "Map.hpp"
#include "Objectives.hpp"
#include "Level.hpp"
#include "Level_Editor.hpp"

static const f64 MAP_ANIMATION_TIME{ 1.0f };
static const f32 MAP_COLOR_FIRE[4]{ 1.0f,0.f,0.f,0.2f }; // For the R, G, B, 
static const f32 MAP_COLOR_WATER[4]{ 0.0f, 0.0f, 0.7f, 0.5f }; // For the R, G, B, 
static const f32 COLOR_TEXTURE[4]{ 1.0f, 1.0f, 1.0f, 1.0f }; // For the R, G, B, A

static const f32 OBJ_UI_X_OFFSET = -0.95f;
static const f32 OBJ_UI_Y_OFFSET = 0.7f;
static const f32 OBJ_UI_GAP_MODIFIER = 1.5f;
static const f32 OBJ_UI_TEXTURE_SCALE = 100.0f; // In terms of world coordinates

// Stored as pos_x, pos_y, scale_x, scale_y. All normalized
static const f32 PANEL[4] = { 0.0f, 0.0f, 0.8f, 1.8f };
// Stored as r, g, b, a. Stored as a value from 0.0f to 1.0f (0 to 255).
static const f32 PANEL_COLOR[4] = { 2.0f / 255.0f, 152.0f / 255.0f, 219.0f / 255.0f, 1.0f };
static const f32 BUTTON_COLOR[4] = { 241.0f / 255.0f, 196.0f / 255.0f, 15.0f / 255.0f, 1.0f };

static const std::string press_esc_to_return_text = "Press ESC to Return";

static AEMtx33 scale{};
static AEMtx33 translate{};
static AEMtx33 transform{};

bool next_rotate{ false };
bool previous_rotate{ false };
bool player_moved{ false };

static AEMtx33 map_translate{};
AEGfxTexture* how_to_play_image{ nullptr };


/**************************************************************************/
/*!
	 This function is called in Draw_Map to allow Player to be layered
	 correcting with respect to the rest of the Map.
*/
/**************************************************************************/
void Draw_Player(Player& player_param) {
	const f64 ANIMATION_TIME{ 1.0f / player_param.player_move_speed };
	f64 time_counter{ player_param.player_move_cooldown };
	static f32 offset_x{};
	static f32 offset_y{};
	if (player_moved) {
		switch (player.facing) {
		case Player::LEFT:
			offset_x = GRID_SIZE;
			break;
		case Player::RIGHT:
			offset_x = -GRID_SIZE;
			break;
		case Player::UP:
			offset_y = -GRID_SIZE;
			break;
		case Player::DOWN:
			offset_y = GRID_SIZE;
			break;
		}
	}
	time_counter = (time_counter <= 0) ? 0 : time_counter;	// if less than equal to 0, set to 0
	offset_x = (time_counter <= 0) ? 0 : offset_x;			// if less than equal to 0, set to 0
	offset_y = (time_counter <= 0) ? 0 : offset_y;			// if less than equal to 0, set to 0
	f64 lerp_ratio{ (time_counter / ANIMATION_TIME) };		// this number will go from 1 to 0.
	Camera_On_Player(offset_x * static_cast<f32>(lerp_ratio), offset_y * static_cast<f32>(lerp_ratio));
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxTextureSet(player_param.sprite_texture, 0, 0);
	AEMtx33Scale(&scale, GRID_SIZE, GRID_SIZE); // Scaling Factors (Size of Squares)
	std::pair<f32, f32> player_world{};
	player.Get_Player_World_Coordinates(player_world.first, player_world.second);

	AEMtx33Trans(&translate, player_world.first + static_cast<f32>(offset_x * lerp_ratio), player_world.second + static_cast<f32>(offset_y * lerp_ratio)); // Sets Position
	AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
}

/**************************************************************************/
/*!
	 This function draws the spell UI at the bottom right of the game.
	 It shows the current spell that the player is using and handles the
	 animation for the rotation of the spell.
*/
/**************************************************************************/
void Draw_Spell_UI(Player& player_param) {

	static f64 time_counter{ SPELL_UI_ANIMATION_TIME };
	static f32 previous_animation_offset{};
	static f32 current_animation_offset{};
	static f32 next_animation_offset{};

	// 0a. Check if "change spell" was pressed this frame
	if (next_rotate) {
		next_rotate = false;
		time_counter = 0;
		previous_animation_offset = SPELL_UI_OFFSET_X;
		current_animation_offset = SPELL_UI_OFFSET_X;
		next_animation_offset = -SPELL_UI_OFFSET_X;
	}
	else if (previous_rotate) {
		previous_rotate = false;
		time_counter = 0;
		previous_animation_offset = SPELL_UI_OFFSET_X;
		current_animation_offset = -SPELL_UI_OFFSET_X;
		next_animation_offset = -SPELL_UI_OFFSET_X;
	}

	// 0b. Update Variables for during and after animations
	time_counter = (time_counter < SPELL_UI_ANIMATION_TIME) ? time_counter + delta_time : SPELL_UI_ANIMATION_TIME;
	previous_animation_offset = (time_counter < SPELL_UI_ANIMATION_TIME) ? previous_animation_offset : 0;
	current_animation_offset = (time_counter < SPELL_UI_ANIMATION_TIME) ? current_animation_offset : 0;
	next_animation_offset = (time_counter < SPELL_UI_ANIMATION_TIME) ? next_animation_offset : 0;
	f64 lerp_ratio{ 1.0f - (time_counter / SPELL_UI_ANIMATION_TIME) }; // this number will go from 1 to 0.

	// 0c. Get Camera Coordinates and update where to draw the UI
	f32 cam_x{}, cam_y{};
	AEGfxGetCamPosition(&cam_x, &cam_y);

	f32 previous_spell_x{ cam_x + (AEGfxGetWindowWidth() / 3.0f) - SPELL_UI_OFFSET_X + static_cast<f32>(previous_animation_offset * lerp_ratio) };
	f32 current_spell_x{ cam_x + AEGfxGetWindowWidth() / 3.0f + static_cast<f32>(current_animation_offset * lerp_ratio) };
	f32 next_spell_x{ cam_x + (AEGfxGetWindowWidth() / 3.0f) + SPELL_UI_OFFSET_X + static_cast<f32>(next_animation_offset * lerp_ratio) };
	f32 spell_y{ cam_y - (AEGfxGetWindowHeight() / 3.0f) };

	// 0d. Get correct order of spells and prepare to Draw (Also get player_cast_cooldown)
	AEGfxTexture* previous_spell{ nullptr }, * current_spell{ nullptr }, * next_spell{ nullptr };
	f64 player_cast_cooldown{};
	switch (player_param.selected_spell) {
	case Player::WATER:
		player_cast_cooldown = player_param.water_spell_cooldown;
		current_spell = player_param.water_spell_texture;
		next_spell = player_param.earth_spell_texture;
		previous_spell = player_param.air_spell_texture;
		break;
	case Player::EARTH:
		player_cast_cooldown = player_param.earth_spell_cooldown;
		current_spell = player_param.earth_spell_texture;
		next_spell = player_param.air_spell_texture;
		previous_spell = player_param.water_spell_texture;
		break;
	case Player::AIR:
		player_cast_cooldown = player_param.air_spell_cooldown;
		current_spell = player_param.air_spell_texture;
		next_spell = player_param.water_spell_texture;
		previous_spell = player_param.earth_spell_texture;
		break;
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	f32 opacity = (player_cast_cooldown <= 0) ? 1.0f : 0.25f;
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, opacity);

	// 1. Draw the previous, current and next spell
	{ // Draw Previous Spell
		f64 animation_scale_offset{};
		if (current_animation_offset < 0) { // Previous Spell Animation
			animation_scale_offset = -SPELL_UI_SMALL_SPELL_ICON_SIZE * lerp_ratio;
		}
		else { // Next Spell Animation
			animation_scale_offset = (static_cast<f64>(SPELL_UI_BIG_SPELL_ICON_SIZE) - SPELL_UI_SMALL_SPELL_ICON_SIZE) * lerp_ratio;
		}
		AEGfxTextureSet(previous_spell, 0, 0);
		AEMtx33Scale(&scale, static_cast<f32>(SPELL_UI_SMALL_SPELL_ICON_SIZE + animation_scale_offset), static_cast<f32>(SPELL_UI_SMALL_SPELL_ICON_SIZE + animation_scale_offset)); // Scaling Factors (Size of Squares)
		AEMtx33Trans(&translate, previous_spell_x, spell_y); // Sets Position
		AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}

	{ // Draw Current Spell
		f64 animation_scale_offset{};
		animation_scale_offset = -(SPELL_UI_BIG_SPELL_ICON_SIZE - SPELL_UI_SMALL_SPELL_ICON_SIZE) * lerp_ratio;
		AEGfxTextureSet(current_spell, 0, 0);
		AEMtx33Scale(&scale, static_cast<f32>(SPELL_UI_BIG_SPELL_ICON_SIZE + animation_scale_offset), static_cast<f32>(SPELL_UI_BIG_SPELL_ICON_SIZE + animation_scale_offset)); // Scaling Factors (Size of Squares)
		AEMtx33Trans(&translate, current_spell_x, spell_y); // Sets Position
		AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}

	{ // Draw Next Spell
		f64 animation_scale_offset{};
		if (current_animation_offset < 0) { // Previous Spell Animation
			animation_scale_offset = (static_cast<f64>(SPELL_UI_BIG_SPELL_ICON_SIZE) - SPELL_UI_SMALL_SPELL_ICON_SIZE) * lerp_ratio;
		}
		else { // Next Spell Animation
			animation_scale_offset = -SPELL_UI_SMALL_SPELL_ICON_SIZE * lerp_ratio;
		}
		AEGfxTextureSet(next_spell, 0, 0);
		AEMtx33Scale(&scale, static_cast<f32>(SPELL_UI_SMALL_SPELL_ICON_SIZE + animation_scale_offset), static_cast<f32>(SPELL_UI_SMALL_SPELL_ICON_SIZE + animation_scale_offset)); // Scaling Factors (Size of Squares)
		AEMtx33Trans(&translate, next_spell_x, spell_y); // Sets Position
		AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// 3. Draw the Spell Cooldown if on cooldown
	if (player_cast_cooldown > 0) {
		int whole_numbers = static_cast<int>(player_cast_cooldown);
		int decimals = static_cast<int>(player_cast_cooldown * 10) % 10; // 1.dp
		std::string cool_down = std::to_string(whole_numbers) + "." + std::to_string(decimals);
		const char* pStr = cool_down.c_str();
		f32 w{}, h{};
		AEGfxGetPrintSize(p_spell_ui_font, pStr, 1.f, &w, &h);
		AEGfxPrint(p_spell_ui_font, pStr, (-w / 2) + 2.0f / 3.0f, (-h / 2) - 2.0f / 3.0f, 1, 1, 1, 1, 1);
	}
}

/**************************************************************************/
/*!
	 This function Draws the background of GS_LEVELS
*/
/**************************************************************************/
void Draw_Background(AEGfxTexture* asset)
{
	f32 cam_x{}, cam_y{};
	AEGfxGetCamPosition(&cam_x, &cam_y);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);
	AEGfxTextureSet(asset, 0, 0);

	AEMtx33Scale(&scale, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));
	AEMtx33Trans(&translate, cam_x, cam_y);
	AEMtx33Concat(&transform, &translate, &scale);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
}

/**************************************************************************/
/*!
	 This function Draws the 2D environment of GS_LEVEL such as Tiles,Terrain
	 type, Fire/Water and Player
	 is_near_fire Indicator is also drawn here
*/
/**************************************************************************/
void Draw_Map(Tile_Asset& asset)
{
	static bool alternate_asset{};
	bool is_fire{};
	s32 fire_value{};
	AEMtx33Scale(&scale, GRID_SIZE, GRID_SIZE); // Scaling Factors (Size of Squares)
	AEMtx33 map_scale{};
	AEMtx33 map_transform{};
	static f64 animation_time_counter = MAP_ANIMATION_TIME;

	if (animation_time_counter > 0.f)
	{
		animation_time_counter -= delta_time;
	}
	else
	{
		alternate_asset = !alternate_asset;
		animation_time_counter = MAP_ANIMATION_TIME;
	}



	AEMtx33Trans(&map_translate, static_cast<f32>((GRID_SIZE) / 2.0f), static_cast<f32>((GRID_SIZE) / 2.0f));
	AEMtx33Scale(&map_scale, GRID_SIZE, GRID_SIZE);
	AEMtx33Concat(&map_transform, &map_translate, &map_scale);


	int draw_limit_y = static_cast<int>(AEGfxGetWindowHeight() / GRID_SIZE);
	int draw_limit_x = static_cast<int>(AEGfxGetWindowWidth() / GRID_SIZE);
	std::pair<s8, s8> temp = {};
	player.Get_Player_Tiles_Coordinates(temp.first, temp.second);
	//Draw Tile Type first
	for (int j = (TILE_Y - 1); j >= 0; j--)
	{
		//Draw Terrain Type and Player together {

		if (j < (temp.second - draw_limit_y) || j >(temp.second + draw_limit_y))continue;

		for (int i = 0; i < TILE_X; i++) {
			if (i < (temp.first - draw_limit_x) || i >(temp.first + draw_limit_x))continue;
			Tiles const& grid = (level_map.Get_Display_Grid())[j][i];
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
			AEGfxSetTransparency(1.0f);
			AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);

			switch (grid.tile_type)
			{
			case(Map::GRASS):
				AEGfxTextureSet(asset.grass_tile, 0, 0);
				break;
			case(Map::DIRT):
				AEGfxTextureSet(asset.dirt_tile, 0, 0);
				break;
			case(Map::RIVER):
				if (alternate_asset)
				{
					AEGfxTextureSet(asset.river_tile, 0, 0);
				}
				else
				{
					AEGfxTextureSet(asset.river_tile_2, 0, 0);
				}

				break;

			}


			AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE))); // Sets Position

			AEMtx33Concat(&transform, &translate, &map_transform);

			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

			//Draw Fire Indicator 
			if (grid.is_near_fire)
			{
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				AEGfxTextureSet(NULL, 0, 0);

				AEGfxSetColorToMultiply(MAP_COLOR_FIRE[0], MAP_COLOR_FIRE[1], MAP_COLOR_FIRE[2], MAP_COLOR_FIRE[3]);
				AEMtx33Scale(&scale, 1.f, 1.f);
				AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE))); // Sets Position

				AEMtx33Concat(&transform, &map_transform, &scale);
				AEMtx33Concat(&transform, &translate, &transform);
				AEGfxSetTransform(transform.m);
				AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

			}
		}
	}
	for (int j = (TILE_Y - 1); j >= 0; j--)
	{
		if (j < (temp.second - draw_limit_y) || j >(temp.second + draw_limit_y))continue;
		for (int i = 0; i < TILE_X; i++)
		{
			if (i < (temp.first - draw_limit_x) || i >(temp.first + draw_limit_x))continue;
			Tiles const& grid = (level_map.Get_Display_Grid())[j][i];
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
			AEGfxSetTransparency(1.0f);
			AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);

			float height_ratio{};
			switch (grid.terrain_type)
			{
			case(Map::BUSH):


				AEMtx33Scale(&scale, 3.f, 3.f);
				AEGfxTextureSet(asset.bush_obj, 0, 0);
				break;
			case(Map::TREE):


				AEMtx33Scale(&scale, 2.f, 2.f);
				AEGfxTextureSet(asset.tree_obj, 0, 0);
				break;
			case(Map::MOUNTAIN):
				height_ratio = (static_cast<float>(level_map.max_earth_durability - grid.earth_durability) / 10.f);

				AEMtx33Scale(&scale, 2.f, 3.f - height_ratio);
				AEGfxTextureSet(asset.mountain_obj, 0, 0);
				break;
			case(Map::HILL):

				AEMtx33Scale(&scale, 2.f, 2.f);
				AEGfxTextureSet(asset.hill_obj, 0, 0);
				break;
			case(Map::HOUSE):


				AEMtx33Scale(&scale, 1.5f, 1.5f);
				AEGfxTextureSet(asset.house_obj, 0, 0);
				break;
			case(Map::ENEMY):

				AEMtx33Scale(&scale, 3.f, 2.5f);
				AEGfxTextureSet(asset.enemy_obj, 0, 0);
				break;
			case(Map::NOTHING):
				AEGfxSetTransparency(0.0f);
				break;

			}



			AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE)) + GRID_SIZE / 4.0f); // Sets Position

			AEMtx33Concat(&transform, &map_transform, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

			fire_value = grid.fire;
			is_fire = fire_value > 0 ? 1 : 0;
			fire_value = fire_value >= 0 ? fire_value : -fire_value;
			//Is fire determines to draw Water or Fire
			//Fire value is to check that the tile is not zero
			if (is_fire && fire_value)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
				AEGfxSetTransparency(1.0f);
				AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);
				AEGfxTextureSet(asset.fire_obj, 0, 0);

				AEMtx33Scale(&scale, fire_value / 2.f, fire_value / 2.f);
				AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE)) + GRID_SIZE / 3.0f); // Sets Position

				AEMtx33Concat(&transform, &map_transform, &scale);
				AEMtx33Concat(&transform, &translate, &transform);
				AEGfxSetTransform(transform.m);
				AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
			}
			else if (!is_fire && fire_value)
			{
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				AEGfxTextureSet(NULL, 0, 0);
				AEGfxSetColorToMultiply(MAP_COLOR_WATER[0], MAP_COLOR_WATER[1], MAP_COLOR_WATER[2], MAP_COLOR_WATER[3]);

				AEMtx33Scale(&scale, fire_value / 4.f, fire_value / 4.f);
				AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE))); // Sets Position

				AEMtx33Concat(&transform, &map_transform, &scale);
				AEMtx33Concat(&transform, &translate, &transform);
				AEGfxSetTransform(transform.m);
				AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
			}


		}
		if (temp.second == j)
			Draw_Player(player);
	}

}

/**************************************************************************/
/*!
	 This function draws the range for the spell as highlighted squares.
	 It changes color and range according to the selected spell. It also
	 contains code to mitigate the edge case where player walks into / away
	 from walls.
*/
/**************************************************************************/
void Draw_Player_Spell_Range(Player& player_param) {

	u8 spell_range{};
	f64 player_cast_cooldown{};
	f32 color[4]{ 0,0,0,0.3f }; // For the R, G, B, A
	switch (player_param.selected_spell) {
	case Player::WATER:
		spell_range = player_param.water_spell_range;
		player_cast_cooldown = player_param.water_spell_cooldown;
		color[0] = 11.0f / 255.0f;
		color[1] = 57.0f / 255.0f;
		color[2] = 127.0f / 255.0f;
		break;
	case Player::EARTH:
		spell_range = player_param.earth_spell_range;
		player_cast_cooldown = player_param.earth_spell_cooldown;
		color[0] = 124.0f / 255.0f;
		color[1] = 73.0f / 255.0f;
		color[2] = 38.0f / 255.0f;
		break;
	case Player::AIR:
		spell_range = player_param.air_spell_range;
		player_cast_cooldown = player_param.air_spell_cooldown;
		color[0] = 192.0f / 255.0f;
		color[1] = 192.0f / 255.0f;
		color[2] = 192.0f / 255.0f;
		break;
	}

	const f64 ANIMATION_TIME{ 1.0f / player_param.player_move_speed };
	f64 time_counter{ player_param.player_move_cooldown };
	static f32 offset_x{};
	static f32 offset_y{};
	if (player_moved) {
		switch (player_param.facing) {
		case Player::LEFT:
			offset_x = GRID_SIZE;
			break;
		case Player::RIGHT:
			offset_x = -GRID_SIZE;
			break;
		case Player::UP:
			offset_y = -GRID_SIZE;
			break;
		case Player::DOWN:
			offset_y = GRID_SIZE;
			break;
		}
	}
	time_counter = (time_counter <= 0) ? 0 : time_counter;	// if less than equal to 0, set to 0
	offset_x = (time_counter <= 0) ? 0 : offset_x;			// if less than equal to 0, set to 0
	offset_y = (time_counter <= 0) ? 0 : offset_y;			// if less than equal to 0, set to 0
	f64 lerp_ratio{ (time_counter / ANIMATION_TIME) };		// this number will go from 1 to 0.

	// Draw without Texture
	AEGfxSetRenderMode(AE_GFX_RM_COLOR); // Draw without Texture
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(NULL, 0, 0);
	std::pair<f32, f32> player_world{};
	player_param.Get_Player_World_Coordinates(player_world.first, player_world.second);

	// Draw spell range
	for (s8 rel_x{ -spell_range }; rel_x <= spell_range; rel_x++) {
		bool out_of_bounds_x = (player_param.x + rel_x < 0) || (player_param.x + rel_x >= TILE_X);
		if (out_of_bounds_x) continue;
		for (s8 rel_y{ -spell_range }; rel_y <= spell_range; rel_y++) {
			bool out_of_bounds_y = (player.y + rel_y < 0) || (player.y + rel_y >= TILE_Y);
			bool not_spell_range = (std::abs(rel_x) + std::abs(rel_y) > spell_range);
			if (out_of_bounds_y || not_spell_range) continue;

			// Lerp Animation will adjust scale and translate to move tiles accordingly
			f32 lerp_animation_scale_x = 0;
			f32 lerp_animation_scale_y = 0;
			f32 lerp_animation_translate_x = static_cast<f32>(offset_x * lerp_ratio);
			f32 lerp_animation_translate_y = static_cast<f32>(offset_y * lerp_ratio);

			// This block of code handles the edge case where player moves away from wall
			{
				bool on_left_moving_right = player_param.x + rel_x == 0 && offset_x < 0;
				bool on_right_moving_left = player_param.x + rel_x == TILE_X - 1 && offset_x > 0;
				bool on_bottom_moving_up = player_param.y + rel_y == 0 && offset_y < 0;
				bool on_top_moving_down = player_param.y + rel_y == TILE_Y - 1 && offset_y > 0;
				if (on_left_moving_right || on_right_moving_left) { // Drawing grids left-most or right-most of map
					// This scale makes the grid drawn from big to small.
					lerp_animation_scale_x = -static_cast<f32>(std::abs(offset_x) * lerp_ratio);
					// This translate adjust the position according to the scale to not break the shape of the spell range
					lerp_animation_translate_x += -static_cast<f32>(offset_x / 2.0f * lerp_ratio);
				}

				if (on_bottom_moving_up || on_top_moving_down) { // Drawing grids bottom-most or top-most of map
					// This is the "y" version of the above code
					lerp_animation_scale_y = -static_cast<f32>(std::abs(offset_y) * lerp_ratio);
					lerp_animation_translate_y += -static_cast<f32>(offset_y / 2.0f * lerp_ratio);
				}
			}

			// This block of code handles opacity of grid
			{
				f32 opacity_modifier = 1.0f;

				s8 mouse_x{}, mouse_y{};
				Get_Mouse_Tile_Coordinates(mouse_x, mouse_y);
				std::pair<s8, s8> player_tile{};
				player_param.Get_Player_Tiles_Coordinates(player_tile.first, player_tile.second);
				bool spell_on_cooldown = (player_cast_cooldown > 0);
				bool mouse_on_grid = (mouse_x == player_tile.first + rel_x) && (mouse_y == player_tile.second + rel_y);

				if (spell_on_cooldown) opacity_modifier /= 2.0f; // On CD, less visible
				else if (mouse_on_grid) opacity_modifier *= 2.0f; // Out of CD && mouse in range, more visible

				AEGfxSetColorToMultiply(color[0], color[1], color[2], color[3] * opacity_modifier);
			}

			AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
			AEMtx33Trans(&translate, player_world.first + (rel_x * GRID_SIZE) + lerp_animation_translate_x,
				player_world.second + (rel_y * GRID_SIZE) + lerp_animation_translate_y); // Sets Position
			AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	// This block of code draws the edge case where player moves into wall
	{
		f32 lerp_animation_scale_x = 0;
		f32 lerp_animation_scale_y = 0;
		f32 lerp_animation_translate_x = static_cast<f32>(offset_x * lerp_ratio);
		f32 lerp_animation_translate_y = static_cast<f32>(offset_y * lerp_ratio);

		// Player is near left-most wall AND moving left
		if (player_param.x - spell_range < 0 && offset_x > 0) {
			// This scale makes the grid drawn from big to small.
			lerp_animation_scale_x = -static_cast<f32>(std::abs(offset_x) * (1 - lerp_ratio));
			// This translate shifts the grid to "outside the wall", then applies the adjustment for scale
			lerp_animation_translate_x += -offset_x + static_cast<f32>(offset_x / 2.0f * (1 - lerp_ratio));

			int grids_in_wall{ std::abs(player_param.x - spell_range) }; // identifies number of grids in wall
			for (int i{}; i < grids_in_wall; i++) {
				std::pair<f32, f32> grid_coordinates{}; // Container for coordinates

				// i used for removing the specific case where only 1 square needs to be drawn
				if (i && player_param.y + i < TILE_Y) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(0, player_param.y + static_cast<s8>(i), grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}

				if (player_param.y - i >= 0) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(0, player_param.y - static_cast<s8>(i), grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		// Player is near right-most wall AND moving right
		else if (player_param.x + spell_range >= TILE_X && offset_x < 0) {
			// This scale makes the grid drawn from big to small.
			lerp_animation_scale_x = -static_cast<f32>(std::abs(offset_x) * (1 - lerp_ratio));
			// This translate shifts the grid to "outside the wall", then applies the adjustment for scale
			lerp_animation_translate_x += -offset_x + static_cast<f32>(offset_x / 2.0f * (1 - lerp_ratio));

			int grids_in_wall{ std::abs(player_param.x + spell_range - (TILE_X - 1)) }; // identifies number of grids in wall
			for (int i{}; i < grids_in_wall; i++) {
				std::pair<f32, f32> grid_coordinates{}; // Container for coordinates

				// i used for removing the specific case where only 1 square needs to be drawn
				if (i && player_param.y + i < TILE_Y) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(TILE_X - 1, player_param.y + static_cast<s8>(i), grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}

				if (player_param.y - i >= 0) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(TILE_X - 1, player_param.y - static_cast<s8>(i), grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		// Player is near bottom-most wall AND moving down
		else if (player_param.y - spell_range < 0 && offset_y > 0) {
			// This scale makes the grid drawn from big to small.
			lerp_animation_scale_y = -static_cast<f32>(std::abs(offset_y) * (1 - lerp_ratio));
			// This translate shifts the grid to "outside the wall", then applies the adjustment for scale
			lerp_animation_translate_y += -offset_y + static_cast<f32>(offset_y / 2.0f * (1 - lerp_ratio));

			int grids_in_wall{ std::abs(player_param.y - spell_range) }; // identifies number of grids in wall
			for (int i{}; i < grids_in_wall; i++) {
				std::pair<f32, f32> grid_coordinates{}; // Container for coordinates

				// i used for removing the specific case where only 1 square needs to be drawn
				if (i && player_param.x + i < TILE_X) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(player_param.x + static_cast<s8>(i), 0, grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}

				if (player_param.x - i >= 0) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(player_param.x - static_cast<s8>(i), 0, grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		// Player is near top-most wall AND moving up
		else if (player_param.y + spell_range >= TILE_Y && offset_y < 0) {
			// This scale makes the grid drawn from big to small.
			lerp_animation_scale_y = -static_cast<f32>(std::abs(offset_y) * (1 - lerp_ratio));
			// This translate shifts the grid to "outside the wall", then applies the adjustment for scale
			lerp_animation_translate_y += -offset_y + static_cast<f32>(offset_y / 2.0f * (1 - lerp_ratio));

			int grids_in_wall{ std::abs(player.y + spell_range - (TILE_Y - 1)) }; // identifies number of grids in wall
			for (int i{}; i < grids_in_wall; i++) {
				std::pair<f32, f32> grid_coordinates{}; // Container for coordinates

				// i used for removing the specific case where only 1 square needs to be drawn
				if (i && player_param.x + i < TILE_X) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(player_param.x + static_cast<s8>(i), TILE_Y - 1, grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}

				if (player_param.x - i >= 0) { // Don't draw squares out of range
					Get_Tiles_To_World_Coordinates(player_param.x - static_cast<s8>(i), TILE_Y - 1, grid_coordinates.first, grid_coordinates.second);
					AEMtx33Scale(&scale, GRID_SIZE + lerp_animation_scale_x, GRID_SIZE + lerp_animation_scale_y); // Scaling Factors (Size of Squares)
					AEMtx33Trans(&translate, grid_coordinates.first + lerp_animation_translate_x,
						grid_coordinates.second + lerp_animation_translate_y); // Sets Position
					AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
					AEGfxSetTransform(transform.m);
					AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}
	}
}

/**************************************************************************/
/*!
	 This function draws the objectives when the game is paused.
*/
/**************************************************************************/
void Draw_Paused_Objectives_UI(Objectives& objective) {

	f32 w{}, h{};
	std::string str = "Objectives";
	AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_objectives_font, str.c_str(), OBJ_UI_X_OFFSET, OBJ_UI_Y_OFFSET + (h * OBJ_UI_GAP_MODIFIER), 1, 1, 1, 1, 1);
	for (int i{}; i < 3; ++i) {
		switch (objective.objective[i]) {
		case Objectives::EMPTY:
			break;
		case Objectives::CATCH_ARSONIST:
			str = "Catch the Arsonist";
			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), OBJ_UI_X_OFFSET, OBJ_UI_Y_OFFSET - (h * i * OBJ_UI_GAP_MODIFIER), 1, 1, 1, 1, 1);
			break;
		case Objectives::TIME_LIMIT:
			if (objective.time_limit->Failed()) str = "Time Limit: FAILED";
			else {
				// Update minutes in string
				std::string minutes_counter = std::to_string(static_cast<int>(objective.time_limit->GetTimeCounter()) / 60);
				// Add leading 0 if needed
				minutes_counter = (minutes_counter.length() < 2) ? "0" + minutes_counter : minutes_counter;

				// Similiar code as above
				std::string seconds_counter = std::to_string(static_cast<int>(objective.time_limit->GetTimeCounter()) % 60);
				seconds_counter = (seconds_counter.length() < 2) ? "0" + seconds_counter : seconds_counter;
				std::string minutes_limit = std::to_string(static_cast<int>(objective.time_limit->GetTimeLimit()) / 60);
				minutes_limit = (minutes_limit.length() < 2) ? "0" + minutes_limit : minutes_limit;
				std::string seconds_limit = std::to_string(static_cast<int>(objective.time_limit->GetTimeLimit()) % 60);
				seconds_limit = (seconds_limit.length() < 2) ? "0" + seconds_limit : seconds_limit;

				str = "Time Limit: " + minutes_counter + ":" + seconds_counter + "/" + minutes_limit + ":" + seconds_limit;
			}

			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), OBJ_UI_X_OFFSET, OBJ_UI_Y_OFFSET - (h * i * OBJ_UI_GAP_MODIFIER), 1, 1, 1, 1, 1);
			break;
		case Objectives::TREE_COUNT:
			if (objective.tree_count->Failed()) str = "Tree Count: FAILED";
			else str = "Tree Count: " + std::to_string(level_map.Wood_Count()) + "/" + std::to_string(objective.tree_count->GetTreeLimit());
			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), OBJ_UI_X_OFFSET, OBJ_UI_Y_OFFSET - (h * i * OBJ_UI_GAP_MODIFIER), 1, 1, 1, 1, 1);
			break;
		case Objectives::SAVE_HOUSE:
			if (objective.save_house->Failed()) str = "House: Destroyed";
			else {
				f64 percentage = (static_cast<double>(level_map.Get_Curr_House_Durability()) / static_cast<double>(level_map.Get_Max_House_Durability())) * 100.0;

				// Integer values in percent (digits)
				std::string percent_digits = std::to_string(static_cast<int>(percentage));

				// Decimal values in percent (1 decimal place)
				std::string percent_decimals = std::to_string(static_cast<int>(percentage * 10) % 10);

				str = "House Durability: " + percent_digits + "." + percent_decimals + "%";
			}
			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), OBJ_UI_X_OFFSET, OBJ_UI_Y_OFFSET - (h * i * OBJ_UI_GAP_MODIFIER), 1, 1, 1, 1, 1);
			break;
		}
	}
}

/**************************************************************************/
/*!
	 This function draws the objectives when the game is running.
*/
/**************************************************************************/
void Draw_Objectives_UI(Objectives& objective, Tile_Asset& asset) {

	f32 w{}, h{};
	std::string str = "Objectives";
	AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_objectives_font, str.c_str(), OBJ_UI_X_OFFSET, OBJ_UI_Y_OFFSET + (h * OBJ_UI_GAP_MODIFIER), 1, 1, 1, 1, 1);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	for (int i{}; i < 3; ++i) {

		f32 x_calculated_offset = OBJ_UI_X_OFFSET;
		f32 y_calculated_offset = OBJ_UI_Y_OFFSET - (h * i * OBJ_UI_GAP_MODIFIER);
		f32 OBJ_UI_X_OFFSET_after_text{}, OBJ_UI_Y_OFFSET_after_text{};;
		f32 world_x{}, world_y{};

		switch (objective.objective[i]) {
		case Objectives::EMPTY:
			break;
		case Objectives::CATCH_ARSONIST:
			str = "Catch ";
			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), x_calculated_offset, y_calculated_offset, 1, 1, 1, 1, 1);
			OBJ_UI_X_OFFSET_after_text = w + 0.04f;	// This is calculated using the length of the text then adding the spacing
			OBJ_UI_Y_OFFSET_after_text = h / 2;		// This is calculated using the height of the text
			x_calculated_offset += OBJ_UI_X_OFFSET_after_text;
			y_calculated_offset += OBJ_UI_Y_OFFSET_after_text;

			// Draw Arsonist Texture
			AEGfxTextureSet(asset.enemy_obj, 0, 0);
			AEMtx33Scale(&scale, OBJ_UI_TEXTURE_SCALE, OBJ_UI_TEXTURE_SCALE); // Scaling Factors (Size of Squares)
			Get_Normalized_To_World_Coordinates(world_x, world_y, x_calculated_offset, y_calculated_offset, false);
			AEMtx33Trans(&translate, world_x, world_y); // Sets Position
			AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform

			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

			break;
		case Objectives::TIME_LIMIT:
			if (objective.time_limit->Failed()) str = "Time Limit: FAILED";
			else {
				// Update minutes in string
				std::string minutes_counter = std::to_string(static_cast<int>(objective.time_limit->GetTimeCounter()) / 60);
				// Add leading 0 if needed
				minutes_counter = (minutes_counter.length() < 2) ? "0" + minutes_counter : minutes_counter;

				// Similiar code as above
				std::string seconds_counter = std::to_string(static_cast<int>(objective.time_limit->GetTimeCounter()) % 60);
				seconds_counter = (seconds_counter.length() < 2) ? "0" + seconds_counter : seconds_counter;
				std::string minutes_limit = std::to_string(static_cast<int>(objective.time_limit->GetTimeLimit()) / 60);
				minutes_limit = (minutes_limit.length() < 2) ? "0" + minutes_limit : minutes_limit;
				std::string seconds_limit = std::to_string(static_cast<int>(objective.time_limit->GetTimeLimit()) % 60);
				seconds_limit = (seconds_limit.length() < 2) ? "0" + seconds_limit : seconds_limit;

				str = "Time Limit: " + minutes_counter + ":" + seconds_counter + "/" + minutes_limit + ":" + seconds_limit;
			}

			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), x_calculated_offset, y_calculated_offset, 1, 1, 1, 1, 1);
			break;
		case Objectives::TREE_COUNT:
			if (objective.tree_count->Failed()) str = "Tree Count: FAILED";
			else str = "Tree Count: " + std::to_string(level_map.Wood_Count()) + "/" + std::to_string(objective.tree_count->GetTreeLimit());
			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), x_calculated_offset, y_calculated_offset, 1, 1, 1, 1, 1);
			break;
		case Objectives::SAVE_HOUSE:
			if (objective.save_house->Failed()) str = "House: Destroyed";
			else {
				f64 percentage = (static_cast<double>(level_map.Get_Curr_House_Durability()) / static_cast<double>(level_map.Get_Max_House_Durability())) * 100.0;

				// Integer values in percent (digits)
				std::string percent_digits = std::to_string(static_cast<int>(percentage));

				// Decimal values in percent (1 decimal place)
				std::string percent_decimals = std::to_string(static_cast<int>(percentage * 10) % 10);

				str = "House Durability: " + percent_digits + "." + percent_decimals + "%";
			}
			AEGfxGetPrintSize(p_objectives_font, str.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_objectives_font, str.c_str(), x_calculated_offset, y_calculated_offset, 1, 1, 1, 1, 1);
			break;
		}
	}
}

/**************************************************************************/
/*!
	 This function draws the victory screen. It draws stars based on
	 how many objectives are cleared
*/
/**************************************************************************/
void Draw_Victory_Screen(Objectives& objective) {

	Draw_Panel(PANEL[0], PANEL[1], PANEL[2], PANEL[3],
		PANEL_COLOR[0], PANEL_COLOR[1], PANEL_COLOR[2], PANEL_COLOR[3]);

	f32 w{}, h{};

	AEGfxGetPrintSize(p_font, VICTORY_TITLE_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_font, VICTORY_TITLE_TEXT.c_str(), -w / 2, -h / 2 + VICTORY_TITLE_TEXT_OFFSET, 1, 1, 1, 1, 1); // 0.6f is y_offset

	// This section handles the drawing of stars
	{
		 // Spacing between stars in normalized form

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE); // Draw with Textures
		AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
		AEGfxSetTransparency(1.0f);
		AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);

		for (int i{ -1 }, star_count{0}; i < 2; ++i, ++star_count) {
			f32 star_world_pos_x{}, star_world_pos_y{};
			Get_Normalized_To_World_Coordinates(star_world_pos_x, star_world_pos_y, STAR[0] + (STAR_X_OFFSET * i), STAR[1], false);
			f32 star_world_scale_x{}, star_world_scale_y{};
			Get_Normalized_To_World_Coordinates(star_world_scale_x, star_world_scale_y, STAR[2], STAR[3], true);

			AEMtx33Scale(&scale, star_world_scale_x, star_world_scale_y);	// Scaling Factors (Size of Squares)
			AEMtx33Trans(&translate, star_world_pos_x, star_world_pos_y);	// Sets Position
			AEMtx33Concat(&transform, &translate, &scale);					// Puts them into a transform
			AEGfxSetTransform(transform.m);

			if (star_count < objective.stars) {
				AEGfxTextureSet(objective.yellow_star_image, 0, 0);
			}
			else {
				AEGfxTextureSet(objective.empty_star_image, 0, 0);
			}
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
		}

	}

	// This section handles the drawing of buttons
	{ 
		if (current != LAST_LEVEL) { // Don't Draw Next if no more next
			Draw_Button(VICTORY_NEXT_BUTTON[0], VICTORY_NEXT_BUTTON[1], VICTORY_NEXT_BUTTON[2], VICTORY_NEXT_BUTTON[3],
				BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
			AEGfxGetPrintSize(p_smaller_font, VICTORY_NEXT_TEXT.c_str(), 1.f, &w, &h);
			AEGfxPrint(p_smaller_font, VICTORY_NEXT_TEXT.c_str(), -w / 2, -h / 2 + VICTORY_NEXT_BUTTON[1], 1, 1, 1, 1, 1);
		}

		Draw_Button(VICTORY_RESTART_BUTTON[0], VICTORY_RESTART_BUTTON[1], VICTORY_RESTART_BUTTON[2], VICTORY_RESTART_BUTTON[3],
			BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
		AEGfxGetPrintSize(p_smaller_font, VICTORY_RESTART_TEXT.c_str(), 1.f, &w, &h);
		AEGfxPrint(p_smaller_font, VICTORY_RESTART_TEXT.c_str(), -w / 2, -h / 2 + VICTORY_RESTART_BUTTON[1], 1, 1, 1, 1, 1);

		Draw_Button(VICTORY_RETURN_BUTTON[0], VICTORY_RETURN_BUTTON[1], VICTORY_RETURN_BUTTON[2], VICTORY_RETURN_BUTTON[3],
			BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
		AEGfxGetPrintSize(p_smaller_font, VICTORY_LEVEL_SELECT_TEXT.c_str(), 1.f, &w, &h);
		AEGfxPrint(p_smaller_font, VICTORY_LEVEL_SELECT_TEXT.c_str(), -w / 2, -h / 2 + VICTORY_RETURN_BUTTON[1], 1, 1, 1, 1, 1);
	}
}

/**************************************************************************/
/*!
	 This function draws the pause screen / pop up.
*/
/**************************************************************************/
void Draw_Pause_Screen() {

	Draw_Panel(PANEL[0], PANEL[1], PANEL[2], PANEL[3],
		PANEL_COLOR[0], PANEL_COLOR[1], PANEL_COLOR[2], PANEL_COLOR[3]);

	f32 w{}, h{};

	AEGfxGetPrintSize(p_font, PAUSED_TITLE_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_font, PAUSED_TITLE_TEXT.c_str(), -w / 2, -h / 2 + PAUSED_TITLE_TEXT_OFFSET, 1, 1, 1, 1, 1);

	Draw_Button(PAUSE_RESUME_BUTTON[0], PAUSE_RESUME_BUTTON[1], PAUSE_RESUME_BUTTON[2], PAUSE_RESUME_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, PAUSED_RESUME_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, PAUSED_RESUME_TEXT.c_str(), -w / 2, -h / 2 + PAUSE_RESUME_BUTTON[1], 1, 1, 1, 1, 1);

	Draw_Button(PAUSE_RESTART_BUTTON[0], PAUSE_RESTART_BUTTON[1], PAUSE_RESTART_BUTTON[2], PAUSE_RESTART_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, PAUSED_RESTART_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, PAUSED_RESTART_TEXT.c_str(), -w / 2, -h / 2 + PAUSE_RESTART_BUTTON[1], 1, 1, 1, 1, 1);

	Draw_Button(PAUSE_HOW_TO_PLAY_BUTTON[0], PAUSE_HOW_TO_PLAY_BUTTON[1], PAUSE_HOW_TO_PLAY_BUTTON[2], PAUSE_HOW_TO_PLAY_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, PAUSED_HOW_TO_PLAY_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, PAUSED_HOW_TO_PLAY_TEXT.c_str(), -w / 2, -h / 2 + PAUSE_HOW_TO_PLAY_BUTTON[1], 1, 1, 1, 1, 1);

	Draw_Button(PAUSE_LEVEL_SELECT_BUTTON[0], PAUSE_LEVEL_SELECT_BUTTON[1], PAUSE_LEVEL_SELECT_BUTTON[2], PAUSE_LEVEL_SELECT_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, PAUSED_LEVEL_SELECT_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, PAUSED_LEVEL_SELECT_TEXT.c_str(), -w / 2, -h / 2 + PAUSE_LEVEL_SELECT_BUTTON[1], 1, 1, 1, 1, 1);

	Draw_Button(PAUSE_MAIN_MENU_BUTTON[0], PAUSE_MAIN_MENU_BUTTON[1], PAUSE_MAIN_MENU_BUTTON[2], PAUSE_MAIN_MENU_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, PAUSED_MAIN_MENU_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, PAUSED_MAIN_MENU_TEXT.c_str(), -w / 2, -h / 2 + PAUSE_MAIN_MENU_BUTTON[1], 1, 1, 1, 1, 1);

	Draw_Button(PAUSE_QUIT_BUTTON[0], PAUSE_QUIT_BUTTON[1], PAUSE_QUIT_BUTTON[2], PAUSE_QUIT_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, PAUSED_QUIT_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, PAUSED_QUIT_TEXT.c_str(), -w / 2, -h / 2 + PAUSE_QUIT_BUTTON[1], 1, 1, 1, 1, 1);
}

/**************************************************************************/
/*!
	 This function draws the defeat screen.
*/
/**************************************************************************/
void Draw_Defeat_Screen() {

	Draw_Panel(PANEL[0], PANEL[1], PANEL[2], PANEL[3],
		PANEL_COLOR[0], PANEL_COLOR[1], PANEL_COLOR[2], PANEL_COLOR[3]);

	f32 w{}, h{};

	AEGfxGetPrintSize(p_font, DEFEAT_TITLE_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_font, DEFEAT_TITLE_TEXT.c_str(), -w / 2, -h / 2 + DEFEAT_TITLE_TEXT_OFFSET, 1, 1, 1, 1, 1);

	Draw_Button(DEFEAT_RESTART_BUTTON[0], DEFEAT_RESTART_BUTTON[1], DEFEAT_RESTART_BUTTON[2], DEFEAT_RESTART_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, DEFEAT_RESTART_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, DEFEAT_RESTART_TEXT.c_str(), -w / 2, -h / 2 + DEFEAT_RESTART_BUTTON[1], 1, 1, 1, 1, 1);

	Draw_Button(DEFEAT_RETURN_BUTTON[0], DEFEAT_RETURN_BUTTON[1], DEFEAT_RETURN_BUTTON[2], DEFEAT_RETURN_BUTTON[3],
		BUTTON_COLOR[0], BUTTON_COLOR[1], BUTTON_COLOR[2], BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_smaller_font, DEFEAT_LEVEL_SELECT_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, DEFEAT_LEVEL_SELECT_TEXT.c_str(), -w / 2, -h / 2 + DEFEAT_RETURN_BUTTON[1], 1, 1, 1, 1, 1);
}

/**************************************************************************/
/*!
	 This function initializes the camera for the map.
*/
/**************************************************************************/
void Draw_Map_Editor_Init()
{
	GRID_SIZE = static_cast<f32>(AEGfxGetWindowWidth() / TILE_X < AEGfxGetWindowHeight() / TILE_Y ? AEGfxGetWindowWidth() / (TILE_X + 2) : AEGfxGetWindowHeight() / (TILE_Y + 2));
	f32 editor_cam_x = (TILE_X * GRID_SIZE / 2.f);
	f32 editor_cam_y = (TILE_Y* GRID_SIZE / 2.f);
	AEGfxSetCamPosition(editor_cam_x, editor_cam_y);

}

/**************************************************************************/
/*!
	 This function draws the map in the level editor.
*/
/**************************************************************************/
void Draw_Map_Editor(Tiles** editor_grid, Editor_Asset & editor_assets, Game_object_Stats game_obj_stats)
{

	bool is_fire{};
	s32 fire_value{};
	AEMtx33Scale(&scale, GRID_SIZE, GRID_SIZE); // Scaling Factors (Size of Squares)
	AEMtx33 map_scale = { 0 };
	AEMtx33 map_transform = { 0 };

	AEMtx33Trans(&map_translate, static_cast<f32>((GRID_SIZE) / 2.0f), static_cast<f32>((GRID_SIZE) / 2.0f));
	AEMtx33Scale(&map_scale, GRID_SIZE, GRID_SIZE);
	AEMtx33Concat(&map_transform, &map_translate, &map_scale);

	//Draw Tile Type first
	for (int j = (TILE_Y - 1); j >= 0; j--) {

		for (int i = 0; i < TILE_X; i++) {
			Tiles const* grid = &(editor_grid[j][i]);
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
			AEGfxSetTransparency(1.0f);
			AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);
			AEMtx33Scale(&scale, GRID_SIZE, GRID_SIZE);

			switch (grid->tile_type)
			{
			case(Map::GRASS):
				AEGfxTextureSet(editor_assets.grass_tile, 0, 0);
				break;
			case(Map::DIRT):
				AEGfxTextureSet(editor_assets.dirt_tile, 0, 0);
				break;
			case(Map::RIVER):
				AEGfxTextureSet(editor_assets.river_tile, 0, 0);
				break;
			}


			AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE))); // Sets Position

			AEMtx33Concat(&transform, &translate, &map_transform);

			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
		}
	}
	//Draw Terrain Type and Player together
	for (int j = (TILE_Y - 1); j >= 0; j--) {

		for (int i = 0; i < TILE_X; i++) {
			Tiles const* grid = &(editor_grid[j][i]);
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
			AEGfxSetTransparency(1.0f);
			AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);

			switch (grid->terrain_type)
			{
			case(Map::BUSH):


				AEMtx33Scale(&scale, 3.f, 3.f);
				AEGfxTextureSet(editor_assets.bush_obj, 0, 0);
				break;
			case(Map::TREE):


				AEMtx33Scale(&scale, 2.f, 2.f);
				AEGfxTextureSet(editor_assets.tree_obj, 0, 0);
				break;
			case(Map::MOUNTAIN):


				AEMtx33Scale(&scale, 2.f, 3.f);
				AEGfxTextureSet(editor_assets.mountain_obj, 0, 0);
				break;
			case(Map::HILL):


				AEMtx33Scale(&scale, 2.f, 2.5f);
				AEGfxTextureSet(editor_assets.hill_obj, 0, 0);
				break;
			case(Map::HOUSE):


				AEMtx33Scale(&scale, 1.5f, 1.5f);
				AEGfxTextureSet(editor_assets.house_obj, 0, 0);
				break;
			case(Map::ENEMY):
				AEMtx33Scale(&scale, 3.f, 2.5f);
				AEGfxTextureSet(editor_assets.enemy_obj, 0, 0);

				break;
			case(Map::NOTHING):
				AEGfxSetTransparency(0.0f);
				break;

			}
			AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE)) + GRID_SIZE / 4.0f); // Sets Position

			AEMtx33Concat(&transform, &map_transform, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

			fire_value = grid->fire;
			is_fire = fire_value > 0 ? 1 : 0;
			fire_value = fire_value >= 0 ? fire_value : -fire_value;
			if (is_fire && fire_value)
			{
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
				AEGfxSetTransparency(1.0f);
				AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);
				AEGfxTextureSet(map_asset.fire_obj, 0, 0);

				AEMtx33Scale(&scale, fire_value / 2.f, fire_value / 2.f);
				AEMtx33Trans(&translate, static_cast<f32>(i * (GRID_SIZE)), static_cast<f32>(j * (GRID_SIZE)) + GRID_SIZE / 3.0f); // Sets Position

				AEMtx33Concat(&transform, &map_transform, &scale);
				AEMtx33Concat(&transform, &translate, &transform);
				AEGfxSetTransform(transform.m);
				AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
			}

		}
		if (game_obj_stats.player_y == j)
			Draw_Player_Editor( game_obj_stats.player_x, game_obj_stats.player_y, editor_assets);
	}
}


/**************************************************************************/
/*!
	 This function draws the player on to screen in the level editor. This
	 function does not include the animation logic in the other draw player
	 function as it is not required in the level editor.
*/
/**************************************************************************/
void Draw_Player_Editor( s8 const& Player_x, s8 const& Player_y, Editor_Asset & editor_assets)
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);

	AEGfxTextureSet(editor_assets.player_obj, 0, 0);
	AEMtx33Scale(&scale, GRID_SIZE, GRID_SIZE); // Scaling Factors (Size of Squares)
	std::pair<f32, f32> player_world{};
	Get_Tiles_To_World_Coordinates(Player_x, Player_y, player_world.first, player_world.second);
	AEMtx33Trans(&translate, player_world.first, player_world.second); // Sets Position
	AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
}


/**************************************************************************/
/*!
	 This function draws the popup box that allows players to
	 set objectives (tree count and time limit) in the level editor.
*/
/**************************************************************************/
void Draw_Popup_Box( u8 & Objective_To_Display, std::vector<buttons> & Popup_Buttons, Editor_Asset& editor_assets, s16 const* TL_TC_Array) {
	f32 world_x_pos{}, world_y_pos{}, world_x_scale{}, world_y_scale{};
	static const char* top_text;
	static const char* button_text = "OK";

	// Stored as pos_x, pos_y, scale_x, scale_y. All normalized
	static const f32 popup_box[4] = { 0.0f, 0.0f, 0.8f, 0.8f };
	// Stored as r, g, b, a. Stored as a value from 0.0f to 1.0f (0 to 255).
	static const f32 panel_color[4] = { 2.0f / 255.0f, 152.0f / 255.0f, 219.0f / 255.0f, 1.0f };
	static const f32 button_color[4] = { 241.0f / 255.0f, 196.0f / 255.0f, 15.0f / 255.0f, 1.0f };

	Draw_Panel(popup_box[0], popup_box[1], popup_box[2], popup_box[3],
		panel_color[0], panel_color[1], panel_color[2], panel_color[3]);

	f32 w{}, h{};

	if (Objective_To_Display == Objectives::TIME_LIMIT)
	{
		top_text = "Set level's time limit";
	}
	else if (Objective_To_Display == Objectives::TREE_COUNT)
	{
		top_text = "Set level's tree count";
	}
	AEGfxGetPrintSize(p_smaller_font, top_text, 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, top_text, -w / 2, -h / 2 + 0.2f, 1, 1, 1, 1, 1);

	std::string objective_string{};
	if (Objective_To_Display == Objectives::TIME_LIMIT)
	{
		objective_string = std::to_string(TL_TC_Array[0]);
	}
	else if (Objective_To_Display == Objectives::TREE_COUNT)
	{
		objective_string = std::to_string(TL_TC_Array[1]);
	}
	AEGfxGetPrintSize(p_smaller_font, objective_string.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, objective_string.c_str(), -w / 2, -h / 2, 1, 1, 1, 1, 1);

	for (size_t i{}; i < Popup_Buttons.size()-1; ++i)
	{
		Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, popup_box[0] + Popup_Buttons[i].button_pos_x, popup_box[1] + Popup_Buttons[i].button_pos_y, false);
		Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, Popup_Buttons[i].button_scale_x, Popup_Buttons[i].button_scale_y, true);

		AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
		AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
		AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
		AEGfxSetTransform(transform.m);
		switch (Popup_Buttons[i].type)
		{
		case(BUTTON_ICONS::SLIDER_LEFT_WIDTH):
			AEGfxTextureSet(editor_assets.left_slider_button, 0, 0);
			break;
		case(BUTTON_ICONS::SLIDER_RIGHT_WIDTH):
			AEGfxTextureSet(editor_assets.right_slider_button, 0, 0);
			break;
		case(BUTTON_ICONS::EXIT_BUTTON):
			AEGfxTextureSet(editor_assets.exit_button, 0, 0);
			break;
		}
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}


	Draw_Button(Popup_Buttons[3].button_pos_x, Popup_Buttons[3].button_pos_y, Popup_Buttons[3].button_scale_x, Popup_Buttons[3].button_scale_y,
		button_color[0], button_color[1], button_color[2], button_color[3]);
	AEGfxGetPrintSize(p_smaller_font, button_text, 1.f, &w, &h);
	AEGfxPrint(p_smaller_font, button_text, -w / 2, -h / 2 + Popup_Buttons[3].button_pos_y, 1, 1, 1, 1, 1);
}

/**************************************************************************/
/*!
	 This function draws the how to play popup.
*/
/**************************************************************************/
void Draw_How_To_Play() {
	f32 cam_x, cam_y;
	AEGfxGetCamPosition(&cam_x, &cam_y);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(COLOR_TEXTURE[0], COLOR_TEXTURE[1], COLOR_TEXTURE[2], COLOR_TEXTURE[3]);

	AEGfxTextureSet(how_to_play_image, 0, 0);
	AEMtx33Scale(&scale, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));		// Default scale
	AEMtx33Trans(&translate, cam_x, cam_y); // Sets Position
	AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

	AEGfxPrint(p_tool_tips_font, press_esc_to_return_text.c_str(), -0.9f, 0.9f, 1, 1, 1, 1, 1);
}