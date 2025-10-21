/******************************************************************************/
/*!
\file		File_Load.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for functions that deal with loading game 
			parameters from file. Each function here loads a specific data
			from a specific file. Load_Game_Parameters is used to encapsulates
			all other functions defined.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "File_Load.hpp"

static const std::string GAME_DATA_FILE_PATH = "Data/Game/";

// For Audio.cpp
s8 BASE_MUSIC_VOLUME{};
s8 BASE_SOUND_VOLUME{};

// For Player.cpp
f64 PLAYER_BASE_MOVE_SPEED{};
f64 PLAYER_CAST_WATER_FREQUENCY{};
f64 PLAYER_CAST_EARTH_FREQUENCY{};
f64 PLAYER_CAST_AIR_FREQUENCY{};
s8 PLAYER_WATER_RANGE{};
s8 PLAYER_EARTH_RANGE{};
s8 PLAYER_AIR_RANGE{};

// For Map.cpp
f64 WIND_TIMER{};
f64 BEHAVIOUR_TIME{};
s8 MAX_FIRE_VALUE{};
s8 MAX_WATER_VALUE{};
s8 WATER_STRENGTH_MULTIPLER{};
s8 MAX_EARTH_DURABILTY{};
s8 MAX_WOOD_DURABILTY{};
s16 MAX_HOUSE_DURABILTY{};

// For Click_Particle_System.cpp
size_t CLICK_PARTICLE_MAX_AMOUNT{};
size_t CLICK_PARTICLE_SPAWN_ONCLICK{};
f32 CLICK_PARTICLE_SCALE{};
f32 CLICK_PARTICLE_VELOCITY{};
f64 CLICK_PARTICLE_LIFETIME_MIN{};
f64 CLICK_PARTICLE_LIFETIME_MAX{};

// For Victory Screen in Graphics.cpp
f32 VICTORY_TITLE_TEXT_OFFSET{};
f32 VICTORY_NEXT_BUTTON[4]{};
f32 VICTORY_RESTART_BUTTON[4]{};
f32 VICTORY_RETURN_BUTTON[4]{};
f32 STAR[4]{};
f32 STAR_X_OFFSET{};
std::string VICTORY_TITLE_TEXT{};
std::string VICTORY_NEXT_TEXT{};
std::string VICTORY_RESTART_TEXT{};
std::string VICTORY_LEVEL_SELECT_TEXT{};

// For Pause Screen in Graphics.cpp
f32 PAUSED_TITLE_TEXT_OFFSET{};
f32 PAUSE_RESUME_BUTTON[4]{};
f32 PAUSE_RESTART_BUTTON[4]{};
f32 PAUSE_HOW_TO_PLAY_BUTTON[4]{};
f32 PAUSE_LEVEL_SELECT_BUTTON[4]{};
f32 PAUSE_MAIN_MENU_BUTTON[4]{};
f32 PAUSE_QUIT_BUTTON[4]{};
std::string PAUSED_TITLE_TEXT{};
std::string PAUSED_RESUME_TEXT{};
std::string PAUSED_RESTART_TEXT{};
std::string PAUSED_HOW_TO_PLAY_TEXT{};
std::string PAUSED_LEVEL_SELECT_TEXT{};
std::string PAUSED_MAIN_MENU_TEXT{};
std::string PAUSED_QUIT_TEXT{};

// For Defeat Screen in Graphics.cpp
f32 DEFEAT_TITLE_TEXT_OFFSET{};
f32 DEFEAT_RESTART_BUTTON[4]{};
f32 DEFEAT_RETURN_BUTTON[4]{};
std::string DEFEAT_TITLE_TEXT{};
std::string DEFEAT_RESTART_TEXT{};
std::string DEFEAT_LEVEL_SELECT_TEXT{};

// For Spell UI in Graphics.cpp
f32 SPELL_UI_BIG_SPELL_ICON_SIZE{};
f32 SPELL_UI_SMALL_SPELL_ICON_SIZE{};
f32 SPELL_UI_OFFSET_X{};
f64 SPELL_UI_ANIMATION_TIME{};

// For Fire_Particle_System.cpp
size_t FIRE_PARTICLE_MAX_AMOUNT{};
f32 FIRE_PARTICLE_OFFSET{};
f32 FIRE_PARTICLE_SCALE{};
f32 FIRE_PARTICLE_VELOCITY_MIN{};
f32 FIRE_PARTICLE_VELOCITY_MAX{};
f64 FIRE_PARTICLE_LIFETIME_MIN{};
f64 FIRE_PARTICLE_LIFETIME_MAX{};
f64 FIRE_PARTICLE_SPAWN_TIME{};

// For Water_Particle_System.cpp
size_t WATER_PARTICLE_MAX_AMOUNT{};
f32 WATER_PARTICLE_SCALE{};
f32 WATER_PARTICLE_VELOCITY{};
f64 WATER_PARTICLE_LIFETIME_MIN{};
f64 WATER_PARTICLE_LIFETIME_MAX{};

// For Earth_Particle_System.cpp
size_t EARTH_PARTICLE_MAX_AMOUNT{};
f32 EARTH_PARTICLE_OFFSET_X{};
f32 EARTH_PARTICLE_OFFSET_Y{};
f32 EARTH_PARTICLE_SCALE{};
f32 EARTH_PARTICLE_VELOCITY_X{};
f32 EARTH_PARTICLE_VELOCITY_Y_MIN{};
f32 EARTH_PARTICLE_VELOCITY_Y_MAX{};
f32 EARTH_PARTICLE_GRAVITY{};			// Balancking CHeck
f64 EARTH_PARTICLE_LIFETIME_MIN{};
f64 EARTH_PARTICLE_LIFETIME_MAX{};

// For Camera.cpp
f64 ZOOM_TIME{};
s8  ZOOMED_IN_TILE_COUNT{};


void Load_Audio_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Audio.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		BASE_MUSIC_VOLUME = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		BASE_SOUND_VOLUME = static_cast<s8>(std::stoi(param_value));
	}
	input_file.close();
}

void Load_Player_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Player.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		PLAYER_BASE_MOVE_SPEED = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		PLAYER_CAST_WATER_FREQUENCY = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		PLAYER_CAST_EARTH_FREQUENCY = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		PLAYER_CAST_AIR_FREQUENCY = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		PLAYER_WATER_RANGE = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		PLAYER_EARTH_RANGE = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		PLAYER_AIR_RANGE = static_cast<s8>(std::stoi(param_value));
	}
	input_file.close();
}

void Load_Map_Parameters()
{
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Map.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		WIND_TIMER = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		BEHAVIOUR_TIME = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		MAX_FIRE_VALUE = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		MAX_WATER_VALUE = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		WATER_STRENGTH_MULTIPLER = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		MAX_EARTH_DURABILTY = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		MAX_WOOD_DURABILTY = static_cast<s8>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		MAX_HOUSE_DURABILTY = static_cast<s16>(std::stoi(param_value));
	}
	input_file.close();
}

void Load_Click_Particles_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Click_Particles.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		CLICK_PARTICLE_MAX_AMOUNT = static_cast<size_t>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		CLICK_PARTICLE_SPAWN_ONCLICK = static_cast<size_t>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		CLICK_PARTICLE_SCALE = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		CLICK_PARTICLE_VELOCITY = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		CLICK_PARTICLE_LIFETIME_MIN = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		CLICK_PARTICLE_LIFETIME_MAX = static_cast<f64>(std::stod(param_value));
	}
	input_file.close();
}

void Load_Victory_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Victory.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		VICTORY_TITLE_TEXT_OFFSET = static_cast<f32>(std::stod(param_value));
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			VICTORY_NEXT_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			VICTORY_RESTART_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			VICTORY_RETURN_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			STAR[i] = static_cast<f32>(std::stod(param_value));
		}
		input_file >> param_name >> param_value;
		STAR_X_OFFSET = static_cast<f32>(std::stod(param_value));
		
		std::getline(input_file, param_value); // Skips the \n after the star offset
		std::getline(input_file, param_value);
		VICTORY_TITLE_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		VICTORY_NEXT_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		VICTORY_RESTART_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		VICTORY_LEVEL_SELECT_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
	}
	input_file.close();
}

void Load_Pause_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Pause.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		PAUSED_TITLE_TEXT_OFFSET = static_cast<f32>(std::stod(param_value));
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			PAUSE_RESUME_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			PAUSE_RESTART_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			PAUSE_HOW_TO_PLAY_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			PAUSE_LEVEL_SELECT_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			PAUSE_MAIN_MENU_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			PAUSE_QUIT_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}

		std::getline(input_file, param_value); // Skips the \n after the quit button
		std::getline(input_file, param_value);
		PAUSED_TITLE_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		PAUSED_RESUME_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		PAUSED_RESTART_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		PAUSED_HOW_TO_PLAY_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		PAUSED_LEVEL_SELECT_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		PAUSED_MAIN_MENU_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		PAUSED_QUIT_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
	}
	input_file.close();
}

void Load_Defeat_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Defeat.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		DEFEAT_TITLE_TEXT_OFFSET = static_cast<f32>(std::stod(param_value));
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			DEFEAT_RESTART_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}
		for (int i{}; i < 4; ++i) { // 4 for pos_x, pos_y, scale_x, scale_y
			input_file >> param_name >> param_value;
			DEFEAT_RETURN_BUTTON[i] = static_cast<f32>(std::stod(param_value));
		}

		std::getline(input_file, param_value); // Skips the \n after the return button
		std::getline(input_file, param_value);
		DEFEAT_TITLE_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		DEFEAT_RESTART_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
		std::getline(input_file, param_value);
		DEFEAT_LEVEL_SELECT_TEXT = param_value.substr(param_value.find_first_of(' ') + 1);
	}
	input_file.close();
}

void Load_Spell_UI_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Spell_UI.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		SPELL_UI_BIG_SPELL_ICON_SIZE = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		SPELL_UI_SMALL_SPELL_ICON_SIZE = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		SPELL_UI_OFFSET_X = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		SPELL_UI_ANIMATION_TIME = static_cast<f64>(std::stod(param_value));
	}
	input_file.close();
}

void Load_Fire_Particles_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Fire_Particles.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_MAX_AMOUNT = static_cast<size_t>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_OFFSET = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_SCALE = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_VELOCITY_MIN = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_VELOCITY_MAX = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_LIFETIME_MIN = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_LIFETIME_MAX = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		FIRE_PARTICLE_SPAWN_TIME = static_cast<f64>(std::stod(param_value));
	}
	input_file.close();
}

void Load_Water_Particles_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Water_Particles.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		WATER_PARTICLE_MAX_AMOUNT = static_cast<size_t>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		WATER_PARTICLE_SCALE = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		WATER_PARTICLE_VELOCITY = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		WATER_PARTICLE_LIFETIME_MIN = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		WATER_PARTICLE_LIFETIME_MAX = static_cast<f64>(std::stod(param_value));
	}
	input_file.close();
}

void Load_Earth_Particles_Parameters() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Earth_Particles.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_MAX_AMOUNT = static_cast<size_t>(std::stoi(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_OFFSET_X = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_OFFSET_Y = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_SCALE = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_VELOCITY_X = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_VELOCITY_Y_MIN = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_VELOCITY_Y_MAX = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_GRAVITY = static_cast<f32>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_LIFETIME_MIN = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		EARTH_PARTICLE_LIFETIME_MAX = static_cast<f64>(std::stod(param_value));
	}
	input_file.close();
}

void Load_Camera() {
	std::ifstream input_file{ GAME_DATA_FILE_PATH + "Camera.txt" };
	if (input_file) {
		std::string param_name{}, param_value{};
		input_file >> param_name >> param_value;
		ZOOM_TIME = static_cast<f64>(std::stod(param_value));
		input_file >> param_name >> param_value;
		ZOOMED_IN_TILE_COUNT = static_cast<s8>(std::stoi(param_value));
	}
	input_file.close();
}

void Load_Game_Parameters() {
	Load_Audio_Parameters();
	Load_Player_Parameters();
	Load_Map_Parameters();
	Load_Click_Particles_Parameters();
	Load_Victory_Parameters();
	Load_Pause_Parameters();
	Load_Defeat_Parameters();
	Load_Spell_UI_Parameters();
	Load_Fire_Particles_Parameters();
	Load_Water_Particles_Parameters();
	Load_Earth_Particles_Parameters();
	Load_Camera();
}