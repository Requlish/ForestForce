/******************************************************************************/
/*!
\file		Objectives.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for the Objective Systems.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "pch.hpp"
#include "Map.hpp"

// Various different objective types.
class Catch_Arsonist
{
private:
	s8 x{}, y{};
	bool completed{ false };
public:
	Catch_Arsonist(std::string const& params) {
		// String -> "23,15" means arsonist_x is 23, arsonist_y is 15
		this->x = static_cast<s8>(std::stoi(params.substr(0, params.find_first_of(","))));
		this->y = static_cast<s8>(std::stoi(params.substr(params.find_first_of(",") + 1, std::string::npos)));
	}
	void Init() {
		this->completed = false;
	}
	std::pair<s8, s8> Get_Tiles_Coordinates() {
		std::pair<s8, s8> pos{ this->x,this->y };
		return pos;
	}
	void Complete() {
		this->completed = true;
	}
	bool Completed() const {
		return this->completed;
	}
};

class Time_Limit
{
private:
	f64 time_limit{};
	f64 time_counter{};
	bool failed{ false };
public:
	Time_Limit(std::string const& params) : time_limit{ static_cast<f64>(std::stod(params)) }, time_counter{} {}
	// String -> "300" means 300 seconds time limit
	void Init() {
		this->time_counter = 0;
		this->failed = false;
	}
	bool Update() {
		time_counter += delta_time;
		if (time_counter > time_limit) return true;
		return false;
	}
	void Fail() {
		this->failed = true;
	}
	bool Failed() const {
		return this->failed;
	}
	f64 GetTimeLimit() const {
		return this->time_limit;
	}
	f64 GetTimeCounter() const {
		return this->time_counter;
	}
};

class Tree_Count
{
private:
	s32 trees_limit{};
	bool failed{ false };
public:
	Tree_Count(std::string const& params) : trees_limit{ static_cast<s32>(std::stoi(params)) } {}
	// String -> "10" means need to save 10 trees
	void Init() {
		this->failed = false;
	}
	bool Update() {
		if (static_cast<s32>(level_map.Wood_Count()) < trees_limit) return true;
		return false;
	}
	void Fail() {
		this->failed = true;
	}
	bool Failed() const {
		return this->failed;
	}
	s32 GetTreeLimit() const {
		return this->trees_limit;
	}
};

class Save_House
{
private:
	s8 x{}, y{};
	bool failed{ false };
public:
	Save_House(std::string const& params) {
		// String -> "23,15" means house_x is 23, house_y is 15
		this->x = static_cast<s8>(std::stoi(params.substr(0, params.find_first_of(","))));
		this->y = static_cast<s8>(std::stoi(params.substr(params.find_first_of(",") + 1, std::string::npos)));
	}
	void Init() {
		this->failed = false;
	}
	bool Update() {
		Tiles house = level_map.Get_Display_Grid()[y][x];
		if (!level_map.Get_Curr_House_Durability()) return true;
		return false;
	}
	void Fail() {
		this->failed = true;
	}
	bool Failed() const {
		return this->failed;
	}
};

// Main Objective Class
class Objectives
{
private:
	u8 stars{};
	AEGfxTexture* empty_star_image{ nullptr };
	AEGfxTexture* yellow_star_image{ nullptr };
	u8* objective{ nullptr };
	Catch_Arsonist* catch_arsonist{ nullptr };
	Time_Limit* time_limit{ nullptr };
	Tree_Count* tree_count{ nullptr };
	Save_House* save_house{ nullptr };

public:
	enum : u8{
		EMPTY = 0, CATCH_ARSONIST, TIME_LIMIT, TREE_COUNT, SAVE_HOUSE
	};

	void Load(s8 obj1, std::string obj1_params, s8 obj2, std::string obj2_params, s8 obj3, std::string obj3_params);
	void Initialize();
	void Update();
	void Unload();

	friend void Draw_Paused_Objectives_UI(Objectives& objectives);					// For drawing Objectives UI (while paused)
	friend void Draw_Objectives_UI(Objectives& objectives, Tile_Asset& asset);		// For drawing Objectives UI (while playing)
	friend void Draw_Victory_Screen(Objectives& objectives);						// For displaying stars
};

extern Objectives objectives;
