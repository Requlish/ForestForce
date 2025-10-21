/******************************************************************************/
/*!
\file		Objectives.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for the objective system.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Objectives.hpp"
#include "Map.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Graphics.hpp"
#include "User_Data.hpp"
#include "Audio.hpp"

Objectives objectives;

/**************************************************************************/
/*!
	This function is called during the loading of a level file, where
	memory will be allocated for each individual objective created. Also
	loads the star texture used in the victory screen.
*/
/**************************************************************************/
void Objectives::Load(s8 obj1, std::string obj1_params,
	s8 obj2, std::string obj2_params,
	s8 obj3, std::string obj3_params){
	this->objective = new u8[3];
	empty_star_image = AEGfxTextureLoad("Assets/UI/Objectives/empty_star.png");
	yellow_star_image = AEGfxTextureLoad("Assets/UI/Objectives/yellow_star.png");

	// Objective 1 memory allocation / creation
	{
		switch (obj1) {
		case Objectives::EMPTY:
			this->objective[0] = Objectives::EMPTY;
			break;
		case Objectives::CATCH_ARSONIST:
			this->catch_arsonist = new Catch_Arsonist(obj1_params);
			this->objective[0] = Objectives::CATCH_ARSONIST;
			break;
		case Objectives::TIME_LIMIT:
			this->time_limit = new Time_Limit(obj1_params);
			this->objective[0] = Objectives::TIME_LIMIT;
			break;
		case Objectives::TREE_COUNT:
			this->tree_count = new Tree_Count(obj1_params);
			this->objective[0] = Objectives::TREE_COUNT;
			break;
		case Objectives::SAVE_HOUSE:
			this->save_house = new Save_House(obj1_params);
			this->objective[0] = Objectives::SAVE_HOUSE;
			break;
		}
	}

	// Objective 2 memory allocation / creation
	{
		switch (obj2) {
		case Objectives::EMPTY:
			this->objective[1] = Objectives::EMPTY;
			break;
		case Objectives::CATCH_ARSONIST:
			this->catch_arsonist = new Catch_Arsonist(obj2_params);
			this->objective[1] = Objectives::CATCH_ARSONIST;
			break;
		case Objectives::TIME_LIMIT:
			this->time_limit = new Time_Limit(obj2_params);
			this->objective[1] = Objectives::TIME_LIMIT;
			break;
		case Objectives::TREE_COUNT:
			this->tree_count = new Tree_Count(obj2_params);
			this->objective[1] = Objectives::TREE_COUNT;
			break;
		case Objectives::SAVE_HOUSE:
			this->save_house = new Save_House(obj2_params);
			this->objective[1] = Objectives::SAVE_HOUSE;
			break;
		}
	}

	// Objective 3 memory allocation / creation
	{
		switch (obj3) {
		case Objectives::EMPTY:
			this->objective[2] = Objectives::EMPTY;
			break;
		case Objectives::CATCH_ARSONIST:
			this->catch_arsonist = new Catch_Arsonist(obj3_params);
			this->objective[2] = Objectives::CATCH_ARSONIST;
			break;
		case Objectives::TIME_LIMIT:
			this->time_limit = new Time_Limit(obj3_params);
			this->objective[2] = Objectives::TIME_LIMIT;
			break;
		case Objectives::TREE_COUNT:
			this->tree_count = new Tree_Count(obj3_params);
			this->objective[2] = Objectives::TREE_COUNT;
			break;
		case Objectives::SAVE_HOUSE:
			this->save_house = new Save_House(obj3_params);
			this->objective[2] = Objectives::SAVE_HOUSE;
			break;
		}
	}
}

/**************************************************************************/
/*!
	This function is called at the start of each level (even on restart)
	to reset the objectives.
*/
/**************************************************************************/
void Objectives::Initialize() {
	this->stars = 0;	// This line is the only line needed from "Free", so its placed here instead
	for (int i{}; i < 3; ++i) {
		switch (this->objective[i]) {
		case Objectives::EMPTY:
			this->stars += 1;
			break;
		case Objectives::CATCH_ARSONIST:
			this->catch_arsonist->Init();
			break;
		case Objectives::TIME_LIMIT:
			this->stars += 1;
			this->time_limit->Init();
			break;
		case Objectives::TREE_COUNT:
			this->stars += 1;
			this->tree_count->Init();
			break;
		case Objectives::SAVE_HOUSE:
			this->stars += 1;
			this->save_house->Init();
			break;
		}
	}
}

/**************************************************************************/
/*!
	This function handles the updates of each objective each frame. This
	is also the function responsible for transitioning into the Victory
	game state.
*/
/**************************************************************************/
void Objectives::Update() {
	if (level_state == LS_PLAYING) {
		if (this->catch_arsonist != nullptr) {
			if (!this->catch_arsonist->Completed()) {
				std::pair<s8, s8> player_pos{};
				player.Get_Player_Tiles_Coordinates(player_pos.first, player_pos.second);
				std::pair<s8, s8> arsonist_pos = this->catch_arsonist->Get_Tiles_Coordinates();
				if (player_pos.first == arsonist_pos.first && player_pos.second == arsonist_pos.second) {
					this->catch_arsonist->Complete();
					this->stars += 1;
					Start_Music(victory_music);
					level_state = LS_VICTORY;
					Complete_Level(static_cast<int>(current - GS_LEVEL1 + 1), static_cast<int>(this->stars));
				}
			}
		}

		if (this->time_limit != nullptr) {
			if (!this->time_limit->Failed()) { 
				if (this->time_limit->Update()) {
					this->time_limit->Fail();
					this->stars -= 1;
				}
			}
		}

		if (this->tree_count != nullptr) {
			if (!this->tree_count->Failed()) {
				if (this->tree_count->Update()) {
					this->tree_count->Fail();
					this->stars -= 1;
				}
			}
		}

		if (this->save_house != nullptr) {
			if (!this->save_house->Failed()) {
				if (this->save_house->Update()) {
					this->save_house->Fail();
					this->stars -= 1;
				}
			}
		}
	}
}

/**************************************************************************/
/*!
	This function unloads objective when exiting level game state. This is
	responsible for deleting the allocated memory and unloading the star
	image asset.
*/
/**************************************************************************/
void Objectives::Unload() {
	delete this->catch_arsonist;
	delete this->time_limit;
	delete this->tree_count;
	delete this->save_house;
	delete this->objective;
	this->catch_arsonist = nullptr;
	this->time_limit = nullptr;
	this->tree_count = nullptr;
	this->save_house = nullptr;
	this->objective = nullptr;
	AEGfxTextureUnload(empty_star_image);
	AEGfxTextureUnload(yellow_star_image);
}