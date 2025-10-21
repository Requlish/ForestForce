/******************************************************************************/
/*!
\file		Level_Editor.cpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for the level editor. This file contains the
			necessary function implementations required for the level editor
			- Level_Editor_Init
				Initializes all necessary data to its initial state
			- Level_Editor_Update
				Checks for user input and updates the necessary data on the map
			- Level_Editor_Draw
				Calls functions in Graphics.cpp to handle the map drawing as well
				as handles the drawing of the editor UI
			- Level_Editor_Unload
				Frees map data and all textures
			- Add_Asset
				Add any game object the player specifies onto the map
			- Set_Objectives
				Set objectives, as specified by the player, and determines which
				objective to display

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Level_Editor.hpp"
#include "Map.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Fileio.hpp"
#include "Graphics_Utils.hpp"
#include "Objectives.hpp"

static f32 font_width{}, font_height{};
static const f32 pop_up[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
static const f32 pop_up_color[4] = { 200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f };
static std::string POP_UP_TEXT1;
static const char* POP_UP_TEXT2 = "Press any key to continue!";
static const f32 pop_up_text_color[4] = { 50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f };

static const f32 button_true[4] = { -0.3f, -0.4f, 0.4f, 0.2f };
static const f32 button_false[4] = { 0.3f, -0.4f, 0.4f, 0.2f };
static bool confirmation_pop_up_triggered = false;
static bool any_key_pop_up_triggered = false;
static std::string warning_text;

static std::string panel_button_text;
static f32 close_open_panel_button[4] = { 0.85f, -0.25f, 0.2f, 0.08f };
static const f32 close_open_panel_button_colour[4] = { 241.0f / 255.0f, 196.0f / 255.0f, 15.0f / 255.0f, 1.0f };

static const int tooltip_width = 1600;
static const int tooltip_height = 900;

Panel UI_Panel{ 0.f, -0.7f, 2.2f, 0.75f };

f32 mouse_x{}, mouse_y{};

Tiles** Create_Grid{ nullptr };

s8 MAX_SIZE_TILE_X = 100;
s8 MAX_SIZE_TILE_Y = 100;
s8 MIN_SIZE_TILE_X = 15;
s8 MIN_SIZE_TILE_Y = 15;

s8 weather_set;
u8 set_tiles;
u8 set_draw_mode;
u8 terrain_to_set;
u8 type_to_set;
s8 fire_strength;

bool set_width;
bool set_height;

bool popup_triggered;

static AEMtx33 scale{};
static AEMtx33 translate{};
static AEMtx33 transform{};

Game_object_Stats game_obj_stats;
Editor_Asset editor_assets;
Editor_Objectives editor_objectives;
u8 change_which_slot;
f64 accumulate_time;

// panel ui and related variables
std::vector<text> list_of_text{
	{"width", UI_Panel.panel_pos_x - 0.76f, UI_Panel.panel_pos_y + 0.22f},
	{"height", UI_Panel.panel_pos_x - 0.26f, UI_Panel.panel_pos_y + 0.22f},
	{"objectives", UI_Panel.panel_pos_x + 0.11f, UI_Panel.panel_pos_y + 0.22f},
	{"weather", UI_Panel.panel_pos_x + 0.61f, UI_Panel.panel_pos_y + 0.22f},
	{"tiles", UI_Panel.panel_pos_x - 0.89f, UI_Panel.panel_pos_y - 0.08f},
	{"terrain", UI_Panel.panel_pos_x - 0.49f, UI_Panel.panel_pos_y - 0.08f},
	{"fire", UI_Panel.panel_pos_x + 0.01f, UI_Panel.panel_pos_y - 0.08f},
	{"player", UI_Panel.panel_pos_x + 0.40f, UI_Panel.panel_pos_y - 0.08f},
	{"eraser", UI_Panel.panel_pos_x + 0.60f, UI_Panel.panel_pos_y - 0.08f},
	{"save", UI_Panel.panel_pos_x + 0.81f, UI_Panel.panel_pos_y - 0.08f}
};

std::vector<buttons> ui_buttons{
	{BUTTON_ICONS::RIVER_UI ,UI_Panel.panel_pos_x - 0.85f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_TYPE, Map::RIVER, Map::NOTHING, DRAW_MODE::MULTI_DRAW, 0},
	{BUTTON_ICONS::GRASS_UI ,UI_Panel.panel_pos_x - 0.75f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_TYPE, Map::GRASS, Map::NOTHING, DRAW_MODE::MULTI_DRAW, 0},
	{BUTTON_ICONS::DIRT_UI ,UI_Panel.panel_pos_x - 0.65f, UI_Panel.panel_pos_y - 0.18f, 0.06f, 0.1f, TO_SET::SET_TYPE, Map::DIRT, Map::NOTHING, DRAW_MODE::MULTI_DRAW, 0},

	{BUTTON_ICONS::BUSH_UI ,UI_Panel.panel_pos_x - 0.45f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_TERRAIN, 0, Map::BUSH, DRAW_MODE::MULTI_DRAW, 0},
	{BUTTON_ICONS::TREE_UI ,UI_Panel.panel_pos_x - 0.35f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_TERRAIN, 0, Map::TREE, DRAW_MODE::MULTI_DRAW, 0},
	{BUTTON_ICONS::HILL_UI ,UI_Panel.panel_pos_x - 0.25f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_TERRAIN, 0, Map::HILL, DRAW_MODE::MULTI_DRAW, 0},
	{BUTTON_ICONS::MOUNTAIN_UI ,UI_Panel.panel_pos_x - 0.15f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_TERRAIN, 0, Map::MOUNTAIN, DRAW_MODE::MULTI_DRAW, 0},

	{BUTTON_ICONS::FIRE1_UI ,UI_Panel.panel_pos_x + 0.05f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_FIRE, 0, Map::NOTHING, DRAW_MODE::MULTI_DRAW, 1},
	{BUTTON_ICONS::FIRE2_UI ,UI_Panel.panel_pos_x + 0.15f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_FIRE, 0, Map::NOTHING, DRAW_MODE::MULTI_DRAW, 2},
	{BUTTON_ICONS::FIRE3_UI ,UI_Panel.panel_pos_x + 0.25f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::SET_FIRE, 0, Map::NOTHING, DRAW_MODE::MULTI_DRAW, 3},

	{BUTTON_ICONS::PLAYER_UI ,UI_Panel.panel_pos_x + 0.45f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::PLAYER, 0, Map::NOTHING, DRAW_MODE::SINGLE_DRAW, 0},

	{BUTTON_ICONS::ERASER_UI ,UI_Panel.panel_pos_x + 0.65f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::ERASE, 0},

	{BUTTON_ICONS::SAVE_UI ,UI_Panel.panel_pos_x + 0.85f, UI_Panel.panel_pos_y - 0.18f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},

	{BUTTON_ICONS::ARSONIST_UI ,UI_Panel.panel_pos_x + 0.15f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::SET_TERRAIN, 0, Map::ENEMY, DRAW_MODE::SINGLE_DRAW, 0},
	{BUTTON_ICONS::HOUSE_UI ,UI_Panel.panel_pos_x + 0.25f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::SET_TERRAIN, 0, Map::HOUSE, DRAW_MODE::SINGLE_DRAW, 0},
	{BUTTON_ICONS::TIME_LIMIT_UI ,UI_Panel.panel_pos_x + 0.35f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::TREE_COUNT_UI ,UI_Panel.panel_pos_x + 0.45f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},

	{BUTTON_ICONS::CLOUDY_UI ,UI_Panel.panel_pos_x + 0.65f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::CLEAR_UI ,UI_Panel.panel_pos_x + 0.75f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::SUN_UI ,UI_Panel.panel_pos_x + 0.85f, UI_Panel.panel_pos_y + 0.12f, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},

	{BUTTON_ICONS::SLIDER_LEFT_WIDTH ,UI_Panel.panel_pos_x - 0.85f, UI_Panel.panel_pos_y + 0.12f, 0.075f, 0.075f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::SLIDER_RIGHT_WIDTH ,UI_Panel.panel_pos_x - 0.55f, UI_Panel.panel_pos_y + 0.12f, 0.075f, 0.075f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::SLIDER_LEFT_HEIGHT ,UI_Panel.panel_pos_x - 0.35f, UI_Panel.panel_pos_y + 0.12f, 0.075f, 0.075f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::SLIDER_RIGHT_HEIGHT ,UI_Panel.panel_pos_x - 0.05f, UI_Panel.panel_pos_y + 0.12f, 0.075f, 0.075f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0}
};

std::vector<buttons> Popup_Sliders{
	{BUTTON_ICONS::SLIDER_RIGHT_WIDTH ,0.2f, 0, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::SLIDER_LEFT_WIDTH ,-0.2f, 0, 0.065f, 0.1f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::EXIT_BUTTON ,0.35f, 0.32f, 0.04f, 0.07f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0},
	{BUTTON_ICONS::CONFIRM_BUTTON ,0, -0.2f, 0.25f, 0.2f, TO_SET::NOTHING_TO_SET, 0, Map::NOTHING, DRAW_MODE::NOTHING_TO_DRAW, 0}
};

bool ui_panel_up{true};
bool exit_or_save;
bool Open_Tooltip;
bool any_pop_up_triggered;

std::vector<u8> display_objectives_list;

u8 popup_objective_to_set;

s16 TL_TC_tracker[2]{10,0};

// LOAD TEXTURES
void Editor_Asset::Editor_Asset_Load()
{
	grass_tile = AEGfxTextureLoad("Assets/Tile/grass_tile.png");
	river_tile = AEGfxTextureLoad("Assets/Tile/river_tile.png");
	dirt_tile = AEGfxTextureLoad("Assets/Tile/dirt_tile.png");
	tree_obj = AEGfxTextureLoad("Assets/Terrain/tree.png");
	bush_obj = AEGfxTextureLoad("Assets/Terrain/bush.png");
	mountain_obj = AEGfxTextureLoad("Assets/Terrain/mountain.png");
	house_obj = AEGfxTextureLoad("Assets/Terrain/house.png");
	hill_obj = AEGfxTextureLoad("Assets/Terrain/hill.png");
	fire_obj = AEGfxTextureLoad("Assets/Terrain/fire.png");
	enemy_obj = AEGfxTextureLoad("Assets/Unit/enemy.png");
	player_obj = AEGfxTextureLoad("Assets/Unit/player.png");
	background_obj = AEGfxTextureLoad("Assets/Menu/forest_background.jpg");
	// editor ui button assets
	grass_button = AEGfxTextureLoad("Assets/UI/Editor_UI/grass_button.png");
	river_button = AEGfxTextureLoad("Assets/UI/Editor_UI/river_button.png");
	dirt_button = AEGfxTextureLoad("Assets/UI/Editor_UI/dirt_button.png");
	tree_button = AEGfxTextureLoad("Assets/UI/Editor_UI/tree_button.png");
	bush_button = AEGfxTextureLoad("Assets/UI/Editor_UI/bush_button.png");
	mountain_button = AEGfxTextureLoad("Assets/UI/Editor_UI/mountain_button.png");
	house_button = AEGfxTextureLoad("Assets/UI/Editor_UI/house_button.png");
	hill_button = AEGfxTextureLoad("Assets/UI/Editor_UI/hill_button.png");
	fire1_button = AEGfxTextureLoad("Assets/UI/Editor_UI/fire_1_button.png");
	fire2_button = AEGfxTextureLoad("Assets/UI/Editor_UI/fire_2_button.png");
	fire3_button = AEGfxTextureLoad("Assets/UI/Editor_UI/fire_3_button.png");
	enemy_button = AEGfxTextureLoad("Assets/UI/Editor_UI/enemy_button.png");
	player_button = AEGfxTextureLoad("Assets/UI/Editor_UI/player_button.png");
	cloudy_button = AEGfxTextureLoad("Assets/UI/Editor_UI/cloudy_button.png");
	clear_button = AEGfxTextureLoad("Assets/UI/Editor_UI/clear_sky_button.png");
	sunny_button = AEGfxTextureLoad("Assets/UI/Editor_UI/sun_button.png");
	erase_button = AEGfxTextureLoad("Assets/UI/Editor_UI/eraser_button.png");
	time_limit_button = AEGfxTextureLoad("Assets/UI/Editor_UI/time_limit_button.png");
	tree_count_button = AEGfxTextureLoad("Assets/UI/Editor_UI/tree_count_button.png");
	save_button = AEGfxTextureLoad("Assets/UI/Editor_UI/save_button.png");
	// Buttons for popup
	right_slider_button = AEGfxTextureLoad("Assets/UI/Buttons/blue_slider_right.png");
	left_slider_button = AEGfxTextureLoad("Assets/UI/Buttons/blue_slider_left.png");
	exit_button = AEGfxTextureLoad("Assets/UI/Buttons/blue_box_cross.png");
	tooltip = AEGfxTextureLoad("Assets/UI/Editor_UI/editor_tooltips.png");
}

// UNLOAD TEXTURES
void Editor_Asset::Editor_Asset_Unload()
{
	AEGfxTextureUnload(grass_tile);
	AEGfxTextureUnload(river_tile);
	AEGfxTextureUnload(dirt_tile);
	AEGfxTextureUnload(tree_obj);
	AEGfxTextureUnload(bush_obj);
	AEGfxTextureUnload(mountain_obj);
	AEGfxTextureUnload(house_obj);
	AEGfxTextureUnload(hill_obj);
	AEGfxTextureUnload(fire_obj);
	AEGfxTextureUnload(enemy_obj);
	AEGfxTextureUnload(player_obj);
	AEGfxTextureUnload(background_obj);
	AEGfxTextureUnload(grass_button);
	AEGfxTextureUnload(river_button);
	AEGfxTextureUnload(dirt_button);
	AEGfxTextureUnload(tree_button);
	AEGfxTextureUnload(bush_button);
	AEGfxTextureUnload(mountain_button);
	AEGfxTextureUnload(house_button);
	AEGfxTextureUnload(hill_button);
	AEGfxTextureUnload(fire1_button);
	AEGfxTextureUnload(fire2_button);
	AEGfxTextureUnload(fire3_button);
	AEGfxTextureUnload(enemy_button);
	AEGfxTextureUnload(player_button);
	AEGfxTextureUnload(cloudy_button);
	AEGfxTextureUnload(clear_button);
	AEGfxTextureUnload(sunny_button);
	AEGfxTextureUnload(erase_button);
	AEGfxTextureUnload(time_limit_button);
	AEGfxTextureUnload(tree_count_button);
	AEGfxTextureUnload(save_button);
	AEGfxTextureUnload(right_slider_button);
	AEGfxTextureUnload(left_slider_button);
	AEGfxTextureUnload(exit_button);
	AEGfxTextureUnload(tooltip);
}

void Level_Editor_Load()
{
	map_asset.Tile_Asset_Load();
	button_default = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_1.png");
	button_hovered = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_2.png");
	button_pressed = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_3.png");
	panel = AEGfxTextureLoad("Assets/UI/Buttons/grey_panel.png");

	TILE_X = 15;
	TILE_Y = 15;
	game_obj_stats.TILE_WIDTH = 15;
	game_obj_stats.TILE_HEIGHT = 15;

	editor_assets.Editor_Asset_Load();

	Create_Grid = new Tiles * [MAX_SIZE_TILE_Y];
	for (int i{}; i < MAX_SIZE_TILE_Y; ++i)
	{
		Create_Grid[i] = new Tiles[MAX_SIZE_TILE_X];
	}
}

void Level_Editor_Init()
{

	// INITALIZE VARIABLES
	set_tiles = TO_SET::SET_TERRAIN;
	set_draw_mode = MULTI_DRAW;

	terrain_to_set = Map::TREE;
	type_to_set = Map::GRASS;

	fire_strength = 0;

	game_obj_stats.player_x = 0;
	game_obj_stats.player_y = 0;

	set_width = true;
	set_height = false;

	for (int i{}; i < MAX_SIZE_TILE_Y; ++i)
	{
		for (int j{}; j < MAX_SIZE_TILE_X; ++j)
		{
			Create_Grid[i][j].terrain_type = Map::NOTHING;
			Create_Grid[i][j].tile_type = Map::GRASS;
		}
	}

	display_objectives_list = {Objectives::CATCH_ARSONIST, Objectives::EMPTY, Objectives::EMPTY};

	change_which_slot = 2;

	editor_objectives = {
		{static_cast<s8>(0),static_cast<s8>(0)},
		{static_cast<s8>(TILE_X - 1),static_cast<s8>(TILE_Y - 1)},
		10,
		0
	};

	Create_Grid[editor_objectives.arsonist_coord.second][editor_objectives.arsonist_coord.first].terrain_type = Map::ENEMY;

	popup_triggered = false;

	TL_TC_tracker[0] = 10;
	TL_TC_tracker[1] = 0;

	accumulate_time = 0;

	exit_or_save = false;
	confirmation_pop_up_triggered = false;
	any_key_pop_up_triggered = false;
	if (!ui_panel_up)
	{
		Open_Close_Panel();
	}

	panel_button_text = "Close";

	Open_Tooltip = false;

	any_pop_up_triggered = false;

	Draw_Map_Editor_Init();
}

void Level_Editor_Update()
{
	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		warning_text = "You are about to exit the editor";
		exit_or_save = false;
		confirmation_pop_up_triggered = true;
		any_pop_up_triggered = true;
		if (popup_triggered)
		{
			popup_triggered = false;
		}
	}

	// This checks for the user mouse inputs and sets
	// assets directly onto the map where the player is clicking
	if (!ui_panel_up)
	{
		// SET THE ASSETS ONTO TILES
		if (!confirmation_pop_up_triggered)
		{
			if (set_draw_mode == DRAW_MODE::MULTI_DRAW)
			{
				if (AEInputCheckCurr(AEVK_LBUTTON)) {
					s8 grid_x{}, grid_y{};
					Get_Mouse_Tile_Coordinates(grid_x, grid_y);

					Add_Asset(grid_x, grid_y);
				}
			}
			else if (set_draw_mode == DRAW_MODE::SINGLE_DRAW)
			{
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					s8 grid_x{}, grid_y{};
					Get_Mouse_Tile_Coordinates(grid_x, grid_y);

					Add_Asset(grid_x, grid_y);
					if (set_tiles == TO_SET::PLAYER)
					{
						Place_Player(grid_x, grid_y);
					}
				}
			}
			else if (set_draw_mode == DRAW_MODE::ERASE)
			{
				if (AEInputCheckCurr(AEVK_LBUTTON)) {
					s8 grid_x{}, grid_y{};
					Get_Mouse_Tile_Coordinates(grid_x, grid_y);

					Erase_Assets(grid_x, grid_y);
				}
			}
		}
	}

	// INPUT TO TRIGGER THE PANEL
	if (!popup_triggered)
	{
		if (AEInputCheckTriggered(AEVK_W))
		{
			Open_Close_Panel();
		}
	}

	// SAVE FUNCTION
	if (AEInputCheckTriggered(AEVK_S))
	{
		warning_text = "Save Changes";
		confirmation_pop_up_triggered = true;
		exit_or_save = true;
	}

	// ERASER FUNCTION
	if (AEInputCheckTriggered(AEVK_E))
	{
		set_draw_mode = DRAW_MODE::ERASE;
	}

	// CLEAR ALL OBJECTS ON MAP (EXCEPT PLAYER)
	if (AEInputCheckTriggered(AEVK_D))
	{
		Clear_Visible_Map();
	}

	// RESET MAP SIZE TO ORIGINAL SIZE (EXCEPT PLAYER)
	if (AEInputCheckTriggered(AEVK_R))
	{
		Reset_Map_Size();
	}

	// OPEN TOOLTIP
	if (AEInputCheckTriggered(AEVK_Q))
	{
		Open_Tooltip = !Open_Tooltip;
	}
	// Input check for tree count / time limit popup
	if (popup_triggered)
	{
		if (confirmation_pop_up_triggered)
		{
			confirmation_pop_up_triggered = false;
		}

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			for (size_t i{}; i < Popup_Sliders.size(); ++i)
			{
				Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);
				if (Is_Point_In_Rect(
					(Popup_Sliders[i].button_pos_x - Popup_Sliders[i].button_scale_x / 2.0f),
					(Popup_Sliders[i].button_pos_x + Popup_Sliders[i].button_scale_x / 2.0f),
					(Popup_Sliders[i].button_pos_y + Popup_Sliders[i].button_scale_y / 2.0f),
					(Popup_Sliders[i].button_pos_y - Popup_Sliders[i].button_scale_y / 2.0f),
					mouse_x, mouse_y))
				{
					if (Popup_Sliders[i].type == BUTTON_ICONS::SLIDER_LEFT_WIDTH)
					{
						if (popup_objective_to_set == Objectives::TIME_LIMIT)
						{
							if ((TL_TC_tracker[0] - 10) <= 10)
							{
								return;
							}

							TL_TC_tracker[0] -= 10;
						}
						else if (popup_objective_to_set == Objectives::TREE_COUNT)
						{
							if ((TL_TC_tracker[1] - 10) <= 0)
							{
								TL_TC_tracker[1] = 0;
								return;
							}

							TL_TC_tracker[1] -= 10;
						}
					}

					else if (Popup_Sliders[i].type == BUTTON_ICONS::SLIDER_RIGHT_WIDTH)
					{
						if (popup_objective_to_set == Objectives::TIME_LIMIT)
						{
							TL_TC_tracker[0] += 10;
						}
						else if (popup_objective_to_set == Objectives::TREE_COUNT)
						{
							if ((TL_TC_tracker[1] + 10) >= Total_Tree_Count())
							{
								TL_TC_tracker[1] = Total_Tree_Count();
								return;
							}

							TL_TC_tracker[1] += 10;
						}
					}

					if (Popup_Sliders[i].type == BUTTON_ICONS::CONFIRM_BUTTON)
					{
						if (popup_objective_to_set == Objectives::TIME_LIMIT)
						{
							Set_Objectives(BUTTON_ICONS::TIME_LIMIT_UI, TL_TC_tracker[0], 0);
						}
						else if (popup_objective_to_set == Objectives::TREE_COUNT)
						{
							if (Total_Tree_Count() <= 0)
							{
								return;
							}
							else if (TL_TC_tracker[1] == 0)
							{
								return;
							}
							Set_Objectives(BUTTON_ICONS::TREE_COUNT_UI, 0, TL_TC_tracker[1]);
						}

						popup_triggered = false;
						any_pop_up_triggered = false;
					}

					if (Popup_Sliders[i].type == BUTTON_ICONS::EXIT_BUTTON)
					{
						popup_triggered = false;
						any_pop_up_triggered = false;
					}
				}
			}
		}
	}

	// This section handles the inputs for the buttons on the panel
	if (!any_pop_up_triggered)
	{
		// This is to check for the size changing arrow buttons, this will allow players to
		// on to the button to increase the size
		if (AEInputCheckCurr(AEVK_LBUTTON))
		{
			for (size_t i{}; i < ui_buttons.size(); ++i)
			{
				Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);
				if (Is_Point_In_Rect(
					(ui_buttons[i].button_pos_x - ui_buttons[i].button_scale_x / 2.0f),
					(ui_buttons[i].button_pos_x + ui_buttons[i].button_scale_x / 2.0f),
					(ui_buttons[i].button_pos_y + ui_buttons[i].button_scale_y / 2.0f),
					(ui_buttons[i].button_pos_y - ui_buttons[i].button_scale_y / 2.0f),
					mouse_x, mouse_y))
				{
					accumulate_time += delta_time;
					if (accumulate_time >= 0.08f)
					{
						if (ui_buttons[i].type == BUTTON_ICONS::SLIDER_LEFT_WIDTH)
						{
							if (TILE_X <= MIN_SIZE_TILE_X)
							{
								return;
							}

							--TILE_X;

							for (s8 j {}; j < TILE_Y; ++j)
							{
								if (Create_Grid[j][TILE_X].terrain_type == Map::ENEMY)
								{
									if (Create_Grid[j][TILE_X - 1].terrain_type != Map::NOTHING)
									{
										Create_Grid[j][TILE_X - 1].terrain_type = Map::NOTHING;
									}
									else if (Create_Grid[j][TILE_X - 1].tile_type == Map::RIVER)
									{
										Create_Grid[j][TILE_X - 1].tile_type = Map::GRASS;
									}

									Create_Grid[editor_objectives.arsonist_coord.second][editor_objectives.arsonist_coord.first].terrain_type = Map::NOTHING;
									Create_Grid[j][TILE_X - 1].terrain_type = Map::ENEMY;
									editor_objectives.arsonist_coord.first = TILE_X-1;
									editor_objectives.arsonist_coord.second = j;
								}
								if (j == game_obj_stats.player_y && TILE_X == game_obj_stats.player_x)
								{
									if (Create_Grid[j][TILE_X - 1].terrain_type != Map::NOTHING)
									{
										Create_Grid[j][TILE_X - 1].terrain_type = Map::NOTHING;
									}
									else if (Create_Grid[j][TILE_X - 1].tile_type == Map::RIVER)
									{
										Create_Grid[j][TILE_X - 1].tile_type = Map::GRASS;
									}

									game_obj_stats.player_x = TILE_X - 1;
								}

							}
						}
						else if (ui_buttons[i].type == BUTTON_ICONS::SLIDER_RIGHT_WIDTH)
						{
							if (TILE_X >= MAX_SIZE_TILE_X)
							{
								return;
							}

							s8 y_check_ahead = TILE_Y + 1;
							y_check_ahead = y_check_ahead > MAX_SIZE_TILE_Y ? MAX_SIZE_TILE_Y : y_check_ahead;
							for (int k{}; k < y_check_ahead; ++k)
							{
								Create_Grid[k][TILE_X].terrain_type = Map::NOTHING;
								Create_Grid[k][TILE_X].tile_type = Map::GRASS;
								Create_Grid[k][TILE_X].fire = 0;
							}
							++TILE_X;


						}
						else if (ui_buttons[i].type == BUTTON_ICONS::SLIDER_LEFT_HEIGHT)
						{
							if (TILE_Y <= MIN_SIZE_TILE_Y)
							{
								return;
							}

							--TILE_Y;
							for (s8 l{}; l < TILE_X; ++l)
							{
								if (Create_Grid[TILE_Y][l].terrain_type == Map::ENEMY)
								{
									if (Create_Grid[TILE_Y - 1][l].terrain_type != Map::NOTHING)
									{
										Create_Grid[TILE_Y - 1][l].terrain_type = Map::NOTHING;
									}
									else if (Create_Grid[TILE_Y - 1][l].tile_type == Map::RIVER)
									{
										Create_Grid[TILE_Y - 1][l].tile_type = Map::GRASS;
									}

									Create_Grid[editor_objectives.arsonist_coord.second][editor_objectives.arsonist_coord.first].terrain_type = Map::NOTHING;
									Create_Grid[TILE_Y - 1][l].terrain_type = Map::ENEMY;
									editor_objectives.arsonist_coord.first = l;
									editor_objectives.arsonist_coord.second = TILE_Y - 1;
								}

								if (l == game_obj_stats.player_x && TILE_Y == game_obj_stats.player_y)
								{
									if (Create_Grid[TILE_Y - 1][l].terrain_type != Map::NOTHING)
									{
										Create_Grid[TILE_Y - 1][l].terrain_type = Map::NOTHING;
									}
									else if (Create_Grid[TILE_Y - 1][l].tile_type == Map::RIVER)
									{
										Create_Grid[TILE_Y - 1][l].tile_type = Map::GRASS;
									}

									game_obj_stats.player_y = TILE_Y - 1;
								}
			
							}
						}
						else if (ui_buttons[i].type == BUTTON_ICONS::SLIDER_RIGHT_HEIGHT)
						{
							if (TILE_Y >= MAX_SIZE_TILE_Y)
							{
								return;
							}

							s8 x_check_ahead = TILE_X + 1;
							x_check_ahead = x_check_ahead > MAX_SIZE_TILE_X ? MAX_SIZE_TILE_X : x_check_ahead;
							for (int m{}; m < x_check_ahead; ++m)
							{
								Create_Grid[TILE_Y][m].terrain_type = Map::NOTHING;
								Create_Grid[TILE_Y][m].tile_type = Map::GRASS;
								Create_Grid[TILE_Y][m].fire = 0;
							}
							++TILE_Y;
						
						}

						accumulate_time = 0.f;
					}
				}
			}
		}

		// This is for checking input specifically for the other buttons (E.g button to set trees)
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			for (size_t i{}; i < ui_buttons.size() - 4; ++i)
			{
				Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);
				if (Is_Point_In_Rect(
					(ui_buttons[i].button_pos_x - ui_buttons[i].button_scale_x / 2.0f),
					(ui_buttons[i].button_pos_x + ui_buttons[i].button_scale_x / 2.0f),
					(ui_buttons[i].button_pos_y + ui_buttons[i].button_scale_y / 2.0f),
					(ui_buttons[i].button_pos_y - ui_buttons[i].button_scale_y / 2.0f),
					mouse_x, mouse_y))
				{
					set_tiles = ui_buttons[i].to_set;
					set_draw_mode = ui_buttons[i].set_draw_mode;
					type_to_set = ui_buttons[i].set_type;
					terrain_to_set = ui_buttons[i].set_terrain;
					fire_strength = ui_buttons[i].fire_strength;

					if (ui_buttons[i].type == BUTTON_ICONS::CLEAR_UI)
					{
						weather_set = 1;
					}
					else if (ui_buttons[i].type == BUTTON_ICONS::CLOUDY_UI)
					{
						weather_set = 0;
					}
					else if (ui_buttons[i].type == BUTTON_ICONS::SUN_UI)
					{
						weather_set = 2;
					}

					if (ui_buttons[i].type == BUTTON_ICONS::TIME_LIMIT_UI)
					{
						popup_triggered = true;
						any_pop_up_triggered = true;
						popup_objective_to_set = Objectives::TIME_LIMIT;
					}
					else if (ui_buttons[i].type == BUTTON_ICONS::TREE_COUNT_UI)
					{
						popup_triggered = true;
						any_pop_up_triggered = true;
						popup_objective_to_set = Objectives::TREE_COUNT;
					}

					if (ui_buttons[i].type == BUTTON_ICONS::SAVE_UI)
					{
						warning_text = "Save Changes";
						confirmation_pop_up_triggered = true;
						exit_or_save = true;
					}
				}
			}

		}
	}

	// This section handles the inputs for confirmation popup
	if (any_key_pop_up_triggered) {
		if (Any_Key_Was_Pressed()) {
			any_key_pop_up_triggered = false;
		}
	}
	if (confirmation_pop_up_triggered) 
	{
		if (popup_triggered)
		{
			popup_triggered = false;
		}
		else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (Is_Point_In_Rect(
				(button_false[0] - button_false[2] / 2.0f),
				(button_false[0] + button_false[2] / 2.0f),
				(button_false[1] + button_false[3] / 2.0f),
				(button_false[1] - button_false[3] / 2.0f),
				mouse_x, mouse_y))
			{
				confirmation_pop_up_triggered = false;
				any_pop_up_triggered = false;
			}
			else if (Is_Point_In_Rect(
				(button_true[0] - button_true[2] / 2.0f),
				(button_true[0] + button_true[2] / 2.0f),
				(button_true[1] + button_true[3] / 2.0f),
				(button_true[1] - button_true[3] / 2.0f),
				mouse_x, mouse_y))
			{
				confirmation_pop_up_triggered = false;
				any_pop_up_triggered = false;
				if (exit_or_save)
				{
					if (Write_File(Create_Grid, game_obj_stats, display_objectives_list, editor_objectives, weather_set))
					{
						POP_UP_TEXT1 = "Your level has been saved!";
					}
					else {
						POP_UP_TEXT1 = "Oh no! Something went wrong!";
					}
					any_key_pop_up_triggered = true;
				}
				else {
					next = GS_MAINMENU;
				}
			}
		}
	}

	// This section handles input for the button that opens and closes the panel
	Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);
	if ((Is_Point_In_Rect(
		(close_open_panel_button[0] - close_open_panel_button[2] / 2.0f),
		(close_open_panel_button[0] + close_open_panel_button[2] / 2.0f),
		(close_open_panel_button[1] + close_open_panel_button[3] / 2.0f),
		(close_open_panel_button[1] - close_open_panel_button[3] / 2.0f),
		mouse_x, mouse_y)))
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			Open_Close_Panel();
		}
	}
	else if (ui_panel_up && !popup_triggered && !confirmation_pop_up_triggered && !any_key_pop_up_triggered)
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			if (!Is_Point_In_Rect(
				(UI_Panel.panel_pos_x - UI_Panel.panel_scale_x / 2.0f),
				(UI_Panel.panel_pos_x + UI_Panel.panel_scale_x / 2.0f),
				(UI_Panel.panel_pos_y + UI_Panel.panel_scale_y / 2.0f),
				(UI_Panel.panel_pos_y - UI_Panel.panel_scale_y / 2.0f),
				mouse_x, mouse_y))
			{
				Open_Close_Panel();
			}

		}
	}

	// This section checks for whether the house is on the map, if not remove objective 
	// Also to check if there are any trees on map after setting tree count objective
	// if tree count that has been set is greater than the total number of trees, change tree count to total number of trees
	// at the same time, check if the total tree count is 0, if so, remove the objective
	for (size_t i{ 1 }; i < display_objectives_list.size(); ++i)
	{
		if (display_objectives_list[i] == Objectives::SAVE_HOUSE)
		{
			if (editor_objectives.house_coord.first > TILE_X-1 || editor_objectives.house_coord.second > TILE_Y-1 
				|| Create_Grid[editor_objectives.house_coord.second][editor_objectives.house_coord.first].terrain_type != Map::HOUSE)
			{
				display_objectives_list[i] = Objectives::EMPTY;
			}
		}
		else if (display_objectives_list[i] == Objectives::TREE_COUNT)
		{
			if (editor_objectives.tree_count > Total_Tree_Count())
			{
				editor_objectives.tree_count = Total_Tree_Count();
				TL_TC_tracker[1] = Total_Tree_Count();
			}
			else if (Total_Tree_Count() == 0)
			{
				display_objectives_list[i] = Objectives::EMPTY;
				TL_TC_tracker[1] = 0;
			}
		}
	}

	if (editor_objectives.tree_count > Total_Tree_Count())
	{
		TL_TC_tracker[1] = Total_Tree_Count();
	}
	else if (Total_Tree_Count() == 0)
	{
		TL_TC_tracker[1] = 0;
	}
}

void Level_Editor_Draw()
{
	// Draw the background
	Draw_Background( editor_assets.background_obj);
	f32 world_x_pos{}, world_y_pos{}, world_x_scale{}, world_y_scale{};
	Draw_Map_Editor_Init();
	// Handles the drawing of the map
	Draw_Map_Editor( Create_Grid, editor_assets, game_obj_stats);
	// Handles the drawing of the editor ui panel
	Draw_Panel(UI_Panel.panel_pos_x, UI_Panel.panel_pos_y, UI_Panel.panel_scale_x, UI_Panel.panel_scale_y, 0.6f, 0.2f, 0.3f, 1.0f);
	
	// Handles the displaying of the text
	for (size_t i{}; i < list_of_text.size(); ++i)
	{
		AEGfxPrint(p_smaller_font, list_of_text[i].text_to_display.c_str(), list_of_text[i].text_pos_x, list_of_text[i].text_pos_y, 0.8f, 1.0f, 1.f, 1.f, 1.f);
	}

	// This handles the displaying of the map's current width and height
	std::string map_width_string = std::to_string(TILE_X);
	std::string map_height_string = std::to_string(TILE_Y);
	AEGfxPrint(p_font, map_width_string.c_str(), UI_Panel.panel_pos_x - 0.73f, UI_Panel.panel_pos_y + 0.1f, 0.5f, 1.0f, 1.f, 1.f, 1.f);
	AEGfxPrint(p_font, map_height_string.c_str(), UI_Panel.panel_pos_x - 0.23f, UI_Panel.panel_pos_y + 0.1f, 0.5f, 1.0f, 1.f, 1.f, 1.f);

	//This section handles the drawing of the buttons in the panel
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	
	//Iterate through the vector of buttons
	for (size_t j{}; j < ui_buttons.size(); ++j)
	{
		Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, ui_buttons[j].button_pos_x, ui_buttons[j].button_pos_y, false);
		Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, ui_buttons[j].button_scale_x, ui_buttons[j].button_scale_y, true);

		AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
		AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
		AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
		AEGfxSetTransform(transform.m);
		// Now check what kind of button its is
		// for e.g if its is suppose to be a button for setting the trees
		// Use AEGfxTextureSet to set to the tree button
		switch (ui_buttons[j].type)
		{
			case(BUTTON_ICONS::BUSH_UI):
			{
				AEGfxTextureSet(editor_assets.bush_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::TREE_UI):
			{
				AEGfxTextureSet(editor_assets.tree_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::HILL_UI):
			{
				AEGfxTextureSet(editor_assets.hill_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::MOUNTAIN_UI):
			{
				AEGfxTextureSet(editor_assets.mountain_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::RIVER_UI):
			{
				AEGfxTextureSet(editor_assets.river_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::GRASS_UI):
			{
				AEGfxTextureSet(editor_assets.grass_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::DIRT_UI):
			{
				AEGfxTextureSet(editor_assets.dirt_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::FIRE1_UI):
			{
				AEGfxTextureSet(editor_assets.fire1_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::FIRE2_UI):
			{
				AEGfxTextureSet(editor_assets.fire2_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::FIRE3_UI):
			{
				AEGfxTextureSet(editor_assets.fire3_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::PLAYER_UI):
			{
				AEGfxTextureSet(editor_assets.player_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::ERASER_UI):
			{
				AEGfxTextureSet(editor_assets.erase_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::SAVE_UI):
			{
				AEGfxTextureSet(editor_assets.save_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::ARSONIST_UI):
			{
				AEGfxTextureSet(editor_assets.enemy_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::HOUSE_UI):
			{
				AEGfxTextureSet(editor_assets.house_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::TIME_LIMIT_UI):
			{
				AEGfxTextureSet(editor_assets.time_limit_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::TREE_COUNT_UI):
			{
				AEGfxTextureSet(editor_assets.tree_count_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::CLOUDY_UI):
			{
				AEGfxTextureSet(editor_assets.cloudy_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::CLEAR_UI):
			{
				AEGfxTextureSet(editor_assets.clear_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::SUN_UI):
			{
				AEGfxTextureSet(editor_assets.sunny_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::SLIDER_LEFT_WIDTH):
			{
				AEGfxTextureSet(editor_assets.left_slider_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::SLIDER_LEFT_HEIGHT):
			{
				AEGfxTextureSet(editor_assets.left_slider_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::SLIDER_RIGHT_HEIGHT):
			{
				AEGfxTextureSet(editor_assets.right_slider_button, 0, 0);
				break;
			}
			case(BUTTON_ICONS::SLIDER_RIGHT_WIDTH):
			{
				AEGfxTextureSet(editor_assets.right_slider_button, 0, 0);
				break;
			}
		}
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}

	// This section handles the displaying of the current weather
	std::string weather{};
	switch (weather_set) 
	{
		case(0):
		{
			weather = "Cloudy";
			break;
		}
		case(1):
		{
			weather = "Clear";
			break;
		}
		case(2):
		weather = "Sunny";
		break;
	}
	AEGfxPrint(p_font, "Weather Set :", 0.60f, 0.75f, 0.5f, 1.0f, 1.f, 1.f, 1.f);
	AEGfxPrint(p_font, weather.c_str(), 0.60f, 0.65f, 0.5f, 1.0f, 1.f, 1.f, 1.f);

	// This section handles the displaying of the objectives onto the screen
	std::string objtive_string{};
	f32 objtive_string_y_pos{ 0.65f };
	AEGfxPrint(p_font, "Objectives Set :", -0.95f, 0.75f, 0.5f, 1.0f, 1.f, 1.f, 1.f);
	for (size_t i{}; i < display_objectives_list.size(); ++i)
	{
		switch (display_objectives_list[i])
		{
			case (Objectives::CATCH_ARSONIST):
			{
				objtive_string = "Catch Arsonist";
				AEGfxPrint(p_smaller_font, objtive_string.c_str(), -0.95f, objtive_string_y_pos, 0.7f, 1.0f, 1.f, 1.f, 1.f);
				break;
			}
			case (Objectives::SAVE_HOUSE):
			{
				objtive_string = "Save House";
				AEGfxPrint(p_smaller_font, objtive_string.c_str(), -0.95f, objtive_string_y_pos, 0.7f, 1.0f, 1.f, 1.f, 1.f);
				break;
			}
			case (Objectives::TIME_LIMIT):
			{
				objtive_string = "Time Limit : ";
				std::string minutes = std::to_string(editor_objectives.time_limit / 60);
				minutes = (minutes.size() < 2) ? "0" + minutes : minutes;
				std::string seconds = std::to_string(editor_objectives.time_limit % 60);
				seconds = (seconds.size() < 2) ? "0" + seconds : seconds;
				objtive_string += minutes + ":" + seconds;
				AEGfxPrint(p_smaller_font, objtive_string.c_str(), -0.95f, objtive_string_y_pos, 0.7f, 1.0f, 1.f, 1.f, 1.f);
				break;
			}
			case (Objectives::TREE_COUNT) :
			{
				objtive_string = "Tree Count : " + std::to_string(editor_objectives.tree_count);
				AEGfxPrint(p_smaller_font, objtive_string.c_str(), -0.95f, objtive_string_y_pos, 0.7f, 1.0f, 1.f, 1.f, 1.f);
				break;
			}
			case (Objectives::EMPTY):
			{
				objtive_string = "No Objectives Here";
				AEGfxPrint(p_smaller_font, objtive_string.c_str(), -0.95f, objtive_string_y_pos, 0.7f, 1.0f, 1.f, 1.f, 1.f);
				break;
			}
		}

		objtive_string_y_pos -= 0.1f;
	}

	// This section handles the drawing of the button that opens and close the Panel
	Draw_Button(close_open_panel_button[0], close_open_panel_button[1], close_open_panel_button[2], close_open_panel_button[3],
		 close_open_panel_button_colour[0], close_open_panel_button_colour[1], close_open_panel_button_colour[2], close_open_panel_button_colour[3]);
	AEGfxGetPrintSize(p_button_font, panel_button_text.c_str(), 1.0f, &font_width, &font_height);
	AEGfxPrint(p_button_font, panel_button_text.c_str(), close_open_panel_button[0] - font_width / 2.0f + 0.04f, close_open_panel_button[1] - font_height / 2.0f + 0.02f, 0.5f,
		pop_up_text_color[0], pop_up_text_color[1], pop_up_text_color[2], pop_up_text_color[3]);

	// This section handles the drawing of the popups
	if (confirmation_pop_up_triggered) {
		Draw_Popup(warning_text);
	}
	else if (popup_triggered)
	{
		Draw_Popup_Box( popup_objective_to_set, Popup_Sliders, editor_assets, TL_TC_tracker);
	}
	else if (any_key_pop_up_triggered)
	{
		f32 text_offset = 0.05f; // Distance between the 2 lines of text (halfed)
		Draw_Panel(pop_up[0], pop_up[1], pop_up[2], pop_up[3], 
			pop_up_color[0], pop_up_color[1], pop_up_color[2], pop_up_color[3]);

		AEGfxGetPrintSize(p_button_font, POP_UP_TEXT1.c_str(), 1.0f, &font_width, &font_height);
		AEGfxPrint(p_button_font, POP_UP_TEXT1.c_str(), pop_up[0] - font_width / 2.0f, pop_up[1] - font_height / 2.0f + text_offset, 1.0f,
			pop_up_text_color[0], pop_up_text_color[1], pop_up_text_color[2], pop_up_text_color[3]);

		AEGfxGetPrintSize(p_button_font, POP_UP_TEXT2, 1.0f, &font_width, &font_height);
		AEGfxPrint(p_button_font, POP_UP_TEXT2, pop_up[0] - font_width / 2.0f, pop_up[1] - font_height / 2.0f - text_offset, 1.0f,
			pop_up_text_color[0], pop_up_text_color[1], pop_up_text_color[2], pop_up_text_color[3]);
	}

	// Print onto the screen the two instructions at the top left corner
	AEGfxPrint(p_tool_tips_font, "Press ESC to exit", -0.95f, 0.9f, 1, 1, 1, 1, 1);
	AEGfxPrint(p_tool_tips_font, "Press Q for tooltip", -0.73f, 0.9f, 1, 1, 1, 1, 1);

	// This section is for displaying the tooltips
	{
		f32 cam_x, cam_y;
		AEGfxGetCamPosition(&cam_x, &cam_y);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency

		f32 opacity{ 0.f };
		opacity = Open_Tooltip ? 1.f : 0.f;
		AEGfxSetTransparency(opacity);

		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxTextureSet(editor_assets.tooltip, 0, 0);

		AEMtx33Scale(&scale, tooltip_width, tooltip_height); // Scaling Factors (Size of Squares)
		AEMtx33Trans(&translate, cam_x, cam_y); // Sets Position
		AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

void Level_Editor_Free()
{
	Camera_Free();
}

void Level_Editor_Unload()
{
	map_asset.Tile_Asset_Unload();
	editor_assets.Editor_Asset_Unload();
	AEGfxTextureUnload(panel);
	AEGfxTextureUnload(button_default);
	AEGfxTextureUnload(button_hovered);
	AEGfxTextureUnload(button_pressed);
	Free_Map_Data();
}

// Function to add objects on to the map
void Add_Asset(s8 grid_x, s8 grid_y)
{
	// To check if clicking out of bounds
	if (grid_x < 0 || grid_y < 0 || grid_x > TILE_X || grid_y > TILE_Y)
	{
		return;
	}
	// Otherwise if setting terrain
	if (set_tiles == SET_TERRAIN)
	{
		if (Create_Grid[grid_y][grid_x].terrain_type == Map::ENEMY || (grid_x == game_obj_stats.player_x) && (grid_y == game_obj_stats.player_y)) // Check if terrain is being set on player
		{																																		  // Return if true
			return;
		}

		if (terrain_to_set == Map::ENEMY) // Otherwise, if setting arsonist
		{

			if ((Create_Grid[grid_y][grid_x].tile_type != Map::GRASS) && (Create_Grid[grid_y][grid_x].tile_type != Map::DIRT) ||
				(Create_Grid[grid_y][grid_x].terrain_type != Map::NOTHING) || (grid_x == game_obj_stats.player_x) && (grid_y == game_obj_stats.player_y)) // Check for this conditions,
			{																																			  // If true, return
				return;
			}

			Create_Grid[editor_objectives.arsonist_coord.second][editor_objectives.arsonist_coord.first].terrain_type = Map::NOTHING; // Otherwise, set the enemy on to that specified tile
			Create_Grid[grid_y][grid_x].terrain_type = Map::ENEMY;
			editor_objectives.arsonist_coord.first = grid_x;
			editor_objectives.arsonist_coord.second = grid_y;
			Set_Objectives(BUTTON_ICONS::ARSONIST_UI); // And set the objectives 
		}

		Create_Grid[grid_y][grid_x].terrain_type = terrain_to_set; // set terrain for ordinary terrain (e.g trees and bushes)

		if (terrain_to_set == Map::HOUSE) // If its a house, do the following
		{
			Create_Grid[editor_objectives.house_coord.second][editor_objectives.house_coord.first].terrain_type = Map::NOTHING;
			Create_Grid[grid_y][grid_x].terrain_type = Map::HOUSE;
			editor_objectives.house_coord.first = grid_x;
			editor_objectives.house_coord.second = grid_y;
			Set_Objectives(BUTTON_ICONS::HOUSE_UI); // and set the objectives 
		}


	}
	
	else if (set_tiles == SET_TYPE) // Else if setting tile typw
	{
		if ((grid_x == game_obj_stats.player_x) && (grid_y == game_obj_stats.player_y) && (type_to_set == Map::RIVER) || // Check for this conditions
			(Create_Grid[grid_y][grid_x].terrain_type == Map::ENEMY) && (type_to_set == Map::RIVER))					// If true, return
		{
			return;
		}

		Create_Grid[grid_y][grid_x].tile_type = type_to_set; // Otherwise, set the tile type
	}

	else if (set_tiles == SET_FIRE) // Lastly, if setting fire
	{
		if ( (Create_Grid[grid_y][grid_x].terrain_type == Map::MOUNTAIN) || (Create_Grid[grid_y][grid_x].terrain_type == Map::HILL)  // Check for this conditions
			|| (Create_Grid[grid_y][grid_x].tile_type == Map::RIVER) || (Create_Grid[grid_y][grid_x].terrain_type == Map::ENEMY) || // If true, return
			(grid_x == game_obj_stats.player_x) && (grid_y == game_obj_stats.player_y))
		{
			return;
		}

		Create_Grid[grid_y][grid_x].fire = fire_strength; // Otherwise, set the fire
	}
}

// Function to place player on the map
void Place_Player(s8 grid_x, s8 grid_y)
{
	if (grid_x < 0 || grid_y < 0 || grid_x > TILE_X || grid_y > TILE_Y) // If out of range, return
	{
		return;
	}
	// Otherwise check for the following conditions
	if ( (Create_Grid[grid_y][grid_x].terrain_type == Map::TREE) || (Create_Grid[grid_y][grid_x].terrain_type == Map::MOUNTAIN) || 
		(Create_Grid[grid_y][grid_x].terrain_type == Map::HOUSE) || (Create_Grid[grid_y][grid_x].terrain_type == Map::ENEMY) || 
		(Create_Grid[grid_y][grid_x].tile_type == Map::RIVER) || (Create_Grid[grid_y][grid_x].fire > 0) )
	{
		return;
	}
	// Set player on to map if the test above goes through	
	game_obj_stats.player_x = grid_x;
	game_obj_stats.player_y = grid_y;
}

// Sets objectives that was specified by the player, as well as dictates what objective to display
void Set_Objectives(u8 objective_type, s16 time_limit, s16 tree_count)
{
	u8 What_To_Display{};

	switch (objective_type) // Check what objective to set
	{
	case(BUTTON_ICONS::HOUSE_UI):
		What_To_Display = Objectives::SAVE_HOUSE;
		break;
	case(BUTTON_ICONS::ARSONIST_UI):
		What_To_Display = Objectives::CATCH_ARSONIST;
		break;
	case(BUTTON_ICONS::TIME_LIMIT_UI):
		editor_objectives.time_limit = time_limit;
		What_To_Display = Objectives::TIME_LIMIT;
		break;
	case(BUTTON_ICONS::TREE_COUNT_UI):
		editor_objectives.tree_count = tree_count;
		What_To_Display = Objectives::TREE_COUNT;
		break;
	default:
		break;
	}

	if (What_To_Display == Objectives::CATCH_ARSONIST) // Now check if the objective to sets is arsonist, if yes skip
	{
		return;
	}
	else 
	{
		for (size_t i{ 1 }; i < display_objectives_list.size(); ++i) // Iterate through the list of objectives
		{
			if (display_objectives_list[i] == Objectives::EMPTY) // If any empty slots, insert the objectives there
			{
				display_objectives_list[i] = What_To_Display;
				return;
			}
			
			if (display_objectives_list[1] == What_To_Display || display_objectives_list[2] == What_To_Display) //Otherwise check if the objective to set is already in there
			{
				return;
			}
			else // Otherwise, set the objectives
			{
				if (display_objectives_list[change_which_slot] == Objectives::SAVE_HOUSE) // If the objective that is to be replaced is SAVE HOUSE, remove house on map
				{
					Create_Grid[editor_objectives.house_coord.second][editor_objectives.house_coord.first].terrain_type = Map::NOTHING;
				}

				display_objectives_list[change_which_slot] = What_To_Display; //Set the objective

				if (change_which_slot == 1) // And swap the slot to insert to next
				{
					change_which_slot = 2;
				}
				else {
					change_which_slot = 1;
				}
			}
		}

	}
}

// This functions handles the opening and closing of the panel
void Open_Close_Panel()
{
	ui_panel_up = !ui_panel_up;
	if (ui_panel_up)
	{
		panel_button_text = "Close";
		UI_Panel.panel_pos_y = -0.7f;
		close_open_panel_button[1] += 0.65f;
		for (size_t j{}; j < list_of_text.size(); ++j)
		{
			list_of_text[j].text_pos_y += 0.65f;
		}
		for (size_t i{}; i < ui_buttons.size(); ++i)
		{
			ui_buttons[i].button_pos_y += 0.65f;
		}
	}
	else {
		panel_button_text = "Open";
		UI_Panel.panel_pos_y = -1.35f;
		close_open_panel_button[1] -= 0.65f;
		for (size_t j{}; j < list_of_text.size(); ++j)
		{
			list_of_text[j].text_pos_y -= 0.65f;
		}
		for (size_t i{}; i < ui_buttons.size(); ++i)
		{
			ui_buttons[i].button_pos_y -= 0.65f;
		}
	}
}

// This sets draw mode to erase anything, but the player and the arsonist
void Erase_Assets(s8 grid_x, s8 grid_y)
{
	if (grid_x < 0 || grid_y < 0 || grid_x > TILE_X || grid_y > TILE_Y)
	{
		return;
	}
	else if (Create_Grid[grid_y][grid_x].terrain_type == Map::ENEMY)
	{
		return;
	}

	Create_Grid[grid_y][grid_x].tile_type = Map::GRASS;
	Create_Grid[grid_y][grid_x].terrain_type = Map::NOTHING;
	Create_Grid[grid_y][grid_x].fire = 0;
}

// Clear all objects on the visible map
void Clear_Visible_Map()
{
	for (int i{}; i < TILE_Y; ++i)
	{
		for (int j{}; j < TILE_X; ++j)
		{
			if (Create_Grid[i][j].terrain_type == Map::ENEMY)
				continue;

			Create_Grid[i][j].terrain_type = Map::NOTHING;
			Create_Grid[i][j].tile_type = Map::GRASS;
			Create_Grid[i][j].fire = 0;
		}
	}
}

// Helper function to count the total number of trees on the map
s16 Total_Tree_Count()
{
	s16 tree_on_map{};
	for (int i{}; i < TILE_Y; ++i)
	{
		for (int j{}; j < TILE_X; ++j)
		{
			if (Create_Grid[i][j].terrain_type == Map::TREE)
			{
				++tree_on_map;
			}
		}
	}
	return tree_on_map;
}

// Helper function to reset the map size to the original size
void Reset_Map_Size()
{	
	if (TILE_X == MIN_SIZE_TILE_X && TILE_Y == MIN_SIZE_TILE_Y) // Check if map size is already default size
	{
		return;
	}
	else // If not, set the map size to default size
	{
		TILE_X = MIN_SIZE_TILE_X;
		TILE_Y = MIN_SIZE_TILE_Y;

		if (editor_objectives.arsonist_coord.first > MIN_SIZE_TILE_X || editor_objectives.arsonist_coord.second > MIN_SIZE_TILE_Y) // Set arsonist to original position, and check
		{																															// if the area is passable, if not, set it to passable
			if (Create_Grid[MIN_SIZE_TILE_Y-1][MIN_SIZE_TILE_X-1].tile_type == Map::RIVER)
			{
				Create_Grid[MIN_SIZE_TILE_Y-1][MIN_SIZE_TILE_X-1].tile_type = Map::GRASS;
			}
			Create_Grid[editor_objectives.arsonist_coord.second][editor_objectives.arsonist_coord.first].terrain_type = Map::NOTHING;
			Create_Grid[MIN_SIZE_TILE_Y-1][MIN_SIZE_TILE_X-1].terrain_type = Map::ENEMY;
			editor_objectives.arsonist_coord.first = MIN_SIZE_TILE_X-1;
			editor_objectives.arsonist_coord.second = MIN_SIZE_TILE_Y-1;
		}

		if (game_obj_stats.player_x > MIN_SIZE_TILE_X || game_obj_stats.player_y > MIN_SIZE_TILE_Y) // Now handle the player 
		{																							// If reset the player back to its original position
			if (Create_Grid[0][0].tile_type == Map::RIVER)											// And clear whatever tile that has been placed there 
			{
				Create_Grid[0][0].tile_type = Map::GRASS;
			}
			Create_Grid[0][0].terrain_type = Map::NOTHING;
			game_obj_stats.player_x = 0;
			game_obj_stats.player_y = 0;
		}
	}
}
// This function performs the freeing of the entire map's memory that was allocated prior
void Free_Map_Data()
{
	for (int i{}; i < MAX_SIZE_TILE_Y; ++i)
	{
		delete [] Create_Grid[i];
	}

	delete[] Create_Grid;
}
