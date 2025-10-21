/******************************************************************************/
/*!
\file		Level_Editor.hpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for the level editor. This file contains the
			declaration for functions required for the implementataion of the 
			level editor.

			This file also contains the necessary data structures, variables and
			extern variables for the implementation of the level editor.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

extern AEGfxVertexList* p_mesh;
extern s8 p_font, p_smaller_font;
extern s8 p_button_font, p_title_font;

enum BUTTON_ICONS : u8 {
	RIVER_UI = 0, GRASS_UI, DIRT_UI, BUSH_UI, TREE_UI, HILL_UI, MOUNTAIN_UI, FIRE1_UI, FIRE2_UI, FIRE3_UI, PLAYER_UI, ERASER_UI, SAVE_UI,
	ARSONIST_UI, HOUSE_UI, TIME_LIMIT_UI, TREE_COUNT_UI, CAT_UI, CLOUDY_UI, CLEAR_UI, SUN_UI, SLIDER_LEFT_WIDTH, SLIDER_LEFT_HEIGHT,
	SLIDER_RIGHT_WIDTH, SLIDER_RIGHT_HEIGHT, CONFIRM_BUTTON, EXIT_BUTTON
};

enum TO_SET :u8 {
	SET_TERRAIN = 0, SET_TYPE, SET_FIRE, PLAYER, NOTHING_TO_SET
};

enum DRAW_MODE :u8 {
	MULTI_DRAW = 0, SINGLE_DRAW, ERASE, NOTHING_TO_DRAW
};

enum FIRE : u8 {
	FIRE1 = 0, FIRE2, FIRE3
};

struct Panel {
	f32 panel_pos_x;
	f32 panel_pos_y;
	f32 panel_scale_x;
	f32 panel_scale_y;
};

struct text {
	std::string text_to_display;
	f32 text_pos_x;
	f32 text_pos_y;
};

struct buttons {
	u8 type;
	f32 button_pos_x;
	f32 button_pos_y;
	f32 button_scale_x;
	f32 button_scale_y;
	u8 to_set;
	u8 set_type;
	u8 set_terrain;
	u8 set_draw_mode;
	s8 fire_strength;
};

struct Editor_Objectives
{
	std::pair<s8, s8> house_coord;
	std::pair<s8, s8> arsonist_coord;
	s16 time_limit{};
	s16 tree_count{};
};

struct Game_object_Stats
{
	s8 TILE_WIDTH;
	s8 TILE_HEIGHT;
	s8 player_x;
	s8 player_y;
};

struct Editor_Asset
{
	AEGfxTexture* grass_tile{ nullptr };
	AEGfxTexture* river_tile{ nullptr };
	AEGfxTexture* dirt_tile{ nullptr };
	AEGfxTexture* tree_obj{ nullptr };
	AEGfxTexture* bush_obj{ nullptr };
	AEGfxTexture* mountain_obj{ nullptr };
	AEGfxTexture* house_obj{ nullptr };
	AEGfxTexture* hill_obj{ nullptr };
	AEGfxTexture* fire_obj{ nullptr };
	AEGfxTexture* enemy_obj{ nullptr };
	AEGfxTexture* player_obj{ nullptr };
	AEGfxTexture* background_obj{ nullptr };

	AEGfxTexture* grass_button{ nullptr };
	AEGfxTexture* river_button{ nullptr };
	AEGfxTexture* dirt_button{ nullptr };
	AEGfxTexture* tree_button{ nullptr };
	AEGfxTexture* bush_button{ nullptr };
	AEGfxTexture* mountain_button{ nullptr };
	AEGfxTexture* house_button{ nullptr };
	AEGfxTexture* hill_button{ nullptr };
	AEGfxTexture* fire1_button{ nullptr };
	AEGfxTexture* fire2_button{ nullptr };
	AEGfxTexture* fire3_button{ nullptr };
	AEGfxTexture* enemy_button{ nullptr };
	AEGfxTexture* player_button{ nullptr };
	AEGfxTexture* cloudy_button{ nullptr };
	AEGfxTexture* clear_button{ nullptr };
	AEGfxTexture* sunny_button{ nullptr };
	AEGfxTexture* erase_button{ nullptr };
	AEGfxTexture* time_limit_button{ nullptr };
	AEGfxTexture* tree_count_button{ nullptr };
	AEGfxTexture* save_button{ nullptr };

	AEGfxTexture* right_slider_button{ nullptr };
	AEGfxTexture* left_slider_button{ nullptr };
	AEGfxTexture* exit_button{ nullptr };
	AEGfxTexture* tooltip{ nullptr };
	void Editor_Asset_Load();
	void Editor_Asset_Unload();
};

void Level_Editor_Load();
void Level_Editor_Init();
void Level_Editor_Update();
void Level_Editor_Draw();
void Level_Editor_Free();
void Level_Editor_Unload();

void Add_Asset(s8 grid_x, s8 grid_y);
void Erase_Assets(s8 grid_x, s8 grid_y);
void Place_Player(s8 grid_x, s8 grid_y);
void Clear_Visible_Map();
void Reset_Map_Size();
s16 Total_Tree_Count();
void Set_Objectives(u8 objective_type, s16 time_limit = 0, s16 tree_count = 0);
void Open_Close_Panel();
void Free_Map_Data();