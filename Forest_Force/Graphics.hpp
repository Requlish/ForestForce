/******************************************************************************/
/*!
\file		Graphics.hpp
\author 	Chua Jim Hans, Lim Zhen Eu Damon, Ho Zhen Hao
\par    	Email: c.jimhans@digipen.edu, l.zheneudamon@digipen.edu,
			zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the header for Draw related functions.
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
#pragma once
#include "Player.hpp"
#include "pch.hpp"
#include "Map.hpp"
#include "Objectives.hpp"
#include "Level_Editor.hpp"

extern AEGfxVertexList* p_mesh;
extern s8 p_font, p_smaller_font;
extern s8 p_spell_ui_font;
extern s8 p_objectives_font;
extern s8 p_tool_tips_font;
extern AEGfxTexture* how_to_play_image;
extern bool next_rotate;
extern bool previous_rotate;
extern bool player_moved;

extern f32 VICTORY_TITLE_TEXT_OFFSET;
extern f32 VICTORY_NEXT_BUTTON[4];
extern f32 VICTORY_RESTART_BUTTON[4];
extern f32 VICTORY_RETURN_BUTTON[4];
extern f32 STAR[4];
extern f32 STAR_X_OFFSET;
extern std::string VICTORY_TITLE_TEXT;
extern std::string VICTORY_NEXT_TEXT;
extern std::string VICTORY_RESTART_TEXT;
extern std::string VICTORY_LEVEL_SELECT_TEXT;

extern f32 PAUSED_TITLE_TEXT_OFFSET;
extern f32 PAUSE_RESUME_BUTTON[4];
extern f32 PAUSE_RESTART_BUTTON[4];
extern f32 PAUSE_HOW_TO_PLAY_BUTTON[4];
extern f32 PAUSE_LEVEL_SELECT_BUTTON[4];
extern f32 PAUSE_MAIN_MENU_BUTTON[4];
extern f32 PAUSE_QUIT_BUTTON[4];
extern std::string PAUSED_TITLE_TEXT;
extern std::string PAUSED_RESUME_TEXT;
extern std::string PAUSED_RESTART_TEXT;
extern std::string PAUSED_HOW_TO_PLAY_TEXT;
extern std::string PAUSED_LEVEL_SELECT_TEXT;
extern std::string PAUSED_MAIN_MENU_TEXT;
extern std::string PAUSED_QUIT_TEXT;

extern f32 DEFEAT_TITLE_TEXT_OFFSET;
extern f32 DEFEAT_RESTART_BUTTON[4];
extern f32 DEFEAT_RETURN_BUTTON[4];
extern std::string DEFEAT_TITLE_TEXT;
extern std::string DEFEAT_RESTART_TEXT;
extern std::string DEFEAT_LEVEL_SELECT_TEXT;

extern f32 SPELL_UI_BIG_SPELL_ICON_SIZE;
extern f32 SPELL_UI_SMALL_SPELL_ICON_SIZE;
extern f32 SPELL_UI_OFFSET_X;
extern f64 SPELL_UI_ANIMATION_TIME;

void Draw_Player(Player& player);
void Draw_Spell_UI(Player& player); 

void Draw_Background(AEGfxTexture* asset);

void Draw_Map(Tile_Asset& asset);
void Draw_Player_Spell_Range(Player& player);
void Draw_Paused_Objectives_UI(Objectives& objectives);
void Draw_Objectives_UI(Objectives& objectives, Tile_Asset& asset);

void Draw_Victory_Screen(Objectives& objectives);
void Draw_Pause_Screen();
void Draw_Defeat_Screen();

void Draw_Map_Editor_Init();
void Draw_Map_Editor(Tiles** editor_grid, Editor_Asset& editor_assets, Game_object_Stats game_obj_stats); // Draw map for editor
void Draw_Player_Editor(s8 const& Player_x, s8 const& Player_y, Editor_Asset& editor_assets); // Draw player for editor
void Draw_Popup_Box(u8& Objective_To_Display, std::vector<buttons>& Popup_Buttons, Editor_Asset& editor_assets, s16 const* TL_TC_Array);

void Draw_How_To_Play();