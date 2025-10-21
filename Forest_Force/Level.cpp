/******************************************************************************/
/*!
\file		Level.cpp
\author 	Chua Jim Hans, Lim Zhen Eu Damon, Titus Kwong Wen Shuen, Ho Zhen Hao
\par    	Email: c.jimhans@digipen.edu, l.zheneudamon@digipen.edu
			tituswenshuen.kwong@digipen.edu, zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the 6 main functions for the Forest Force game.
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
#include "pch.hpp"
#include "Level.hpp"
#include "Map.hpp"
#include "Graphics.hpp"
#include "Camera.hpp"
#include "Graphics_Utils.hpp"
#include "Player.hpp"
#include "Fileio.hpp"
#include "Objectives.hpp"
#include "Tutorial.hpp"
#include "User_Data.hpp"
#include "Audio.hpp"
#include "Fire_Particle_System.hpp"
#include "Water_Particle_System.hpp"
#include "Earth_Particle_System.hpp"

static const std::string CONFIRMATION_EXIT_GAME_TEXT = "You are about to exit the game";

enum GS_STATES : u8;								// Forward Declaration of GS_States
const u8 LAST_LEVEL{ GS_LEVEL_EDITOR };				// Max Level we have in Forest Have

s8 level_state{};									// Game State in Level
s8 TILE_X;											// Map X (Map Width)
s8 TILE_Y;											// Map Y (Map Height)
float GRID_SIZE;									// Scale of each Grid (In world coordinates)

static std::string warning_text;
static bool confirmation_pop_up_triggered = false;
static bool how_to_play_triggered = false;

void Level_Load()
{
	// Add Image Border
	how_to_play_image = AEGfxTextureLoad("Assets/Menu/how_to_play.png");
	button_default = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_1.png");
	button_hovered = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_2.png");
	button_pressed = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_3.png");
	panel = AEGfxTextureLoad("Assets/UI/Buttons/grey_panel.png");

	// Add Tiles
	map_asset.Tile_Asset_Load();

	//Load Particle
	Fire_Particles_Load();
	Water_Particles_Load();
	Earth_Particles_Load();

	// Reads file and Loads Map, Player and Objectives
	const std::string LEVEL_FILE_PATH{ "Data/Levels/level" };
	for (u8 i{}; i <= LAST_LEVEL - GS_LEVEL1; ++i) { 
		if (current == GS_LEVEL1 + i) {
			std::string level =  std::to_string(i+1) + ".txt";
			Read_File(LEVEL_FILE_PATH + level);
			break;
		}
	}
}

void Level_Initialize()
{
	// Start Game in playing mode
	level_state = LS_PLAYING;

	// Initialize Player, Map, Objectives
	player.Initialize();
	level_map.Map_Init();
	objectives.Initialize();
	Camera_Init();

	// Play Music
	Start_Music(game_music);
}

void Level_Update()
{
	f32 mouse_x{}, mouse_y{};

	Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);

	if (!tutorial_done) {
		Tutorials_Update();
	}
	else if (confirmation_pop_up_triggered) {
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			confirmation_pop_up_triggered = false;
		}
		else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (Is_Point_In_Rect(
				(POPUP_BUTTON_FALSE[0] - POPUP_BUTTON_FALSE[2] / 2.0f),
				(POPUP_BUTTON_FALSE[0] + POPUP_BUTTON_FALSE[2] / 2.0f),
				(POPUP_BUTTON_FALSE[1] + POPUP_BUTTON_FALSE[3] / 2.0f),
				(POPUP_BUTTON_FALSE[1] - POPUP_BUTTON_FALSE[3] / 2.0f),
				mouse_x, mouse_y))
			{
				confirmation_pop_up_triggered = false;
			}
			else if (Is_Point_In_Rect(
				(POPUP_BUTTON_TRUE[0] - POPUP_BUTTON_TRUE[2] / 2.0f),
				(POPUP_BUTTON_TRUE[0] + POPUP_BUTTON_TRUE[2] / 2.0f),
				(POPUP_BUTTON_TRUE[1] + POPUP_BUTTON_TRUE[3] / 2.0f),
				(POPUP_BUTTON_TRUE[1] - POPUP_BUTTON_TRUE[3] / 2.0f),
				mouse_x, mouse_y))
			{
				confirmation_pop_up_triggered = false;
				next = GS_QUIT;
			}
		}
	}
	else if (how_to_play_triggered) {
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			how_to_play_triggered = false;
		}
	}
	else if (level_state == LS_PAUSED) {
		// Input For Forest Force while Paused
		if (AEInputCheckTriggered(AEVK_ESCAPE)) {
			level_state = LS_PLAYING;
		}

		if (Is_Point_In_Rect( // Check if Resume Button is clicked
			PAUSE_RESUME_BUTTON[0] - PAUSE_RESUME_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			PAUSE_RESUME_BUTTON[0] + PAUSE_RESUME_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			PAUSE_RESUME_BUTTON[1] + PAUSE_RESUME_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			PAUSE_RESUME_BUTTON[1] - PAUSE_RESUME_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			level_state = LS_PLAYING;
		}
		else if (Is_Point_In_Rect( // Check if Restart Button is clicked
			PAUSE_RESTART_BUTTON[0] - PAUSE_RESTART_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			PAUSE_RESTART_BUTTON[0] + PAUSE_RESTART_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			PAUSE_RESTART_BUTTON[1] + PAUSE_RESTART_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			PAUSE_RESTART_BUTTON[1] - PAUSE_RESTART_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = GS_RESTART;
		}
		else if (Is_Point_In_Rect( // Check if How_To_Play Button is clicked
			PAUSE_HOW_TO_PLAY_BUTTON[0] - PAUSE_HOW_TO_PLAY_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			PAUSE_HOW_TO_PLAY_BUTTON[0] + PAUSE_HOW_TO_PLAY_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			PAUSE_HOW_TO_PLAY_BUTTON[1] + PAUSE_HOW_TO_PLAY_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			PAUSE_HOW_TO_PLAY_BUTTON[1] - PAUSE_HOW_TO_PLAY_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			how_to_play_triggered = true;
		}
		else if (Is_Point_In_Rect( // Check if Level_Select Button is clicked
			PAUSE_LEVEL_SELECT_BUTTON[0] - PAUSE_LEVEL_SELECT_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			PAUSE_LEVEL_SELECT_BUTTON[0] + PAUSE_LEVEL_SELECT_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			PAUSE_LEVEL_SELECT_BUTTON[1] + PAUSE_LEVEL_SELECT_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			PAUSE_LEVEL_SELECT_BUTTON[1] - PAUSE_LEVEL_SELECT_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = MS_LEVELSELECT;
		}
		else if (Is_Point_In_Rect( // Check if Main_Menu Button is clicked
			PAUSE_MAIN_MENU_BUTTON[0] - PAUSE_MAIN_MENU_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			PAUSE_MAIN_MENU_BUTTON[0] + PAUSE_MAIN_MENU_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			PAUSE_MAIN_MENU_BUTTON[1] + PAUSE_MAIN_MENU_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			PAUSE_MAIN_MENU_BUTTON[1] - PAUSE_MAIN_MENU_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = GS_MAINMENU;
		}
		else if (Is_Point_In_Rect( // Check if Quit Button is clicked
			PAUSE_QUIT_BUTTON[0] - PAUSE_QUIT_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			PAUSE_QUIT_BUTTON[0] + PAUSE_QUIT_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			PAUSE_QUIT_BUTTON[1] + PAUSE_QUIT_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			PAUSE_QUIT_BUTTON[1] - PAUSE_QUIT_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			confirmation_pop_up_triggered = true;
			warning_text = CONFIRMATION_EXIT_GAME_TEXT;
		}
	}
	else if (level_state == LS_PLAYING) {
		// Input For Forest Force while Playing
		{
			if (AEInputCheckTriggered(AEVK_ESCAPE)) {
				level_state = LS_PAUSED;
			}

			if (AEInputCheckTriggered(AEVK_SPACE))
			{
				Toggle_Zoom();
			}

			Player_Input();
		}

		// Update the Game while playing
		Map_Update();
		objectives.Update();
		Fire_Particles_Update();
		Fire_Particles_Swap();
		Water_Particles_Update();
		Water_Particles_Swap();
		Earth_Particles_Update();
		Earth_Particles_Swap();
	}
	else if (level_state == LS_DEFEAT) {
		// Input for Forest Force while on defeat screen
		Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);

		if (Is_Point_In_Rect( // Check if Restart Button is clicked
			DEFEAT_RESTART_BUTTON[0] - DEFEAT_RESTART_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			DEFEAT_RESTART_BUTTON[0] + DEFEAT_RESTART_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			DEFEAT_RESTART_BUTTON[1] + DEFEAT_RESTART_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			DEFEAT_RESTART_BUTTON[1] - DEFEAT_RESTART_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = GS_RESTART;
		}
		else if (Is_Point_In_Rect( // Check if Level_Select Button is clicked
			DEFEAT_RETURN_BUTTON[0] - DEFEAT_RETURN_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			DEFEAT_RETURN_BUTTON[0] + DEFEAT_RETURN_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			DEFEAT_RETURN_BUTTON[1] + DEFEAT_RETURN_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			DEFEAT_RETURN_BUTTON[1] - DEFEAT_RETURN_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = MS_LEVELSELECT;
		}
	}
	else if (level_state == LS_VICTORY) {
		// Input for Forest Force while on victory screen
		Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);

		if (Is_Point_In_Rect( // Check if Resume Button is clicked
			VICTORY_NEXT_BUTTON[0] - VICTORY_NEXT_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			VICTORY_NEXT_BUTTON[0] + VICTORY_NEXT_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			VICTORY_NEXT_BUTTON[1] + VICTORY_NEXT_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			VICTORY_NEXT_BUTTON[1] - VICTORY_NEXT_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (current != LAST_LEVEL) next = current + 1;			
		}
		else if (Is_Point_In_Rect( // Check if Restart Button is clicked
			VICTORY_RESTART_BUTTON[0] - VICTORY_RESTART_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			VICTORY_RESTART_BUTTON[0] + VICTORY_RESTART_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			VICTORY_RESTART_BUTTON[1] + VICTORY_RESTART_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			VICTORY_RESTART_BUTTON[1] - VICTORY_RESTART_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = GS_RESTART;
		}
		else if (Is_Point_In_Rect( // Check if Level_Select Button is clicked
			VICTORY_RETURN_BUTTON[0] - VICTORY_RETURN_BUTTON[2] / 2.0f, // pos.x - scale.x / 2
			VICTORY_RETURN_BUTTON[0] + VICTORY_RETURN_BUTTON[2] / 2.0f, // pos.x + scale.x / 2
			VICTORY_RETURN_BUTTON[1] + VICTORY_RETURN_BUTTON[3] / 2.0f, // pos.y + scale.y / 2
			VICTORY_RETURN_BUTTON[1] - VICTORY_RETURN_BUTTON[3] / 2.0f, // pos.y - scale.y / 2
			mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON)) {
			next = MS_LEVELSELECT;	
		}
	}
	
}

void Level_Draw()
{
	Draw_Background(map_asset.background_obj);
	Draw_Map(map_asset);	// Also Draws the player
	
	if (level_state == LS_PLAYING)
	{
		Update_Zoom();
		Draw_Player_Spell_Range(player);
		Fire_Particles_Emitter();
		Water_Particles_Emitter();
		Earth_Particles_Emitter();
		Draw_Spell_UI(player);
		Draw_Objectives_UI(objectives, map_asset);
	}
	else if (level_state == LS_PAUSED) {
		Draw_Pause_Screen();
		Draw_Paused_Objectives_UI(objectives);
	}
	else if (level_state == LS_VICTORY) {
		player.Moveable(); // This line will finish the animation of the player moving
		Draw_Victory_Screen(objectives);
	}
	else if (level_state == LS_DEFEAT) {
		Draw_Defeat_Screen();
	}
	if (!tutorial_done) {
		Tutorials_Draw();
	}

	// Popup
	if (confirmation_pop_up_triggered) {
		Draw_Popup(warning_text);
	}

	if (how_to_play_triggered) {
		Draw_How_To_Play();
	}
}

void Level_Free()
{
	Camera_Free();
}

void Level_Unload()
{
	AEGfxTextureUnload(button_default);
	AEGfxTextureUnload(button_hovered);
	AEGfxTextureUnload(button_pressed);
	AEGfxTextureUnload(panel);
	AEGfxTextureUnload(how_to_play_image);

	// Unload Tiles Art (Textures) Assets
	map_asset.Tile_Asset_Unload();

	// Unload Player, Map and Objectives
	player.Unload();
	level_map.Map_Unload();
	objectives.Unload();
}