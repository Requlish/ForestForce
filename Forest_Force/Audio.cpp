/******************************************************************************/
/*!
\file		Audio.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for functions that deal with audio.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "Audio.hpp"

static const float pitch{ 1.0f }; // We won't be playing with pitch in the game

s8 music_volume{}; // Number from 0 to 100
s8 sound_volume{}; // Number from 0 to 100

AEAudio menu_music{};
AEAudio level_select_music{};
AEAudio game_music{};
AEAudio victory_music{};
AEAudio defeat_music{};
AEAudio credits_music{};

AEAudio cast_water{};
AEAudio cast_earth{};
AEAudio cast_air{};
AEAudio swap_water{};
AEAudio swap_earth{};
AEAudio swap_air{};

AEAudioGroup music_group; // Generic Music Group
AEAudioGroup sound_group; // Generic Sound Group

void Load_Music() {
	menu_music = AEAudioLoadMusic("Assets/Music/Menu.mp3");
	level_select_music = AEAudioLoadMusic("Assets/Music/Level_Select.mp3");
	game_music = AEAudioLoadMusic("Assets/Music/Game.mp3");
	victory_music = AEAudioLoadMusic("Assets/Music/Victory.mp3");
	defeat_music = AEAudioLoadMusic("Assets/Music/Defeat.mp3");
	credits_music = AEAudioLoadMusic("Assets/Music/Credits.mp3");
	music_volume = BASE_MUSIC_VOLUME;
}

void Load_Sound() {
	cast_water = AEAudioLoadSound("Assets/Sound/cast_water.mp3");
	cast_earth = AEAudioLoadSound("Assets/Sound/cast_earth.mp3");
	cast_air = AEAudioLoadSound("Assets/Sound/cast_wind.mp3");
	swap_water = AEAudioLoadSound("Assets/Sound/swap_water.mp3");
	swap_earth = AEAudioLoadSound("Assets/Sound/swap_earth.mp3");
	swap_air = AEAudioLoadSound("Assets/Sound/swap_wind.mp3");
	sound_volume = BASE_SOUND_VOLUME;
}

/**************************************************************************/
/*!
	Call this function to play Music
*/
/**************************************************************************/
void Start_Music(AEAudio music) {
	AEAudioStopGroup(music_group);
	AEAudioPlay(music, music_group, static_cast<float>(music_volume) / 100.0f, pitch, -1);	// -1 to loop
}

/**************************************************************************/
/*!
	Call this function to play Sound Effects (SFX)
*/
/**************************************************************************/
void Play_Sound(AEAudio sound) {
	AEAudioPlay(sound, sound_group, static_cast<float>(sound_volume) / 100.0f, pitch, 0);	// 0 to play once
}

/**************************************************************************/
/*!
	Called to adjust music volume
*/
/**************************************************************************/
void Adjust_Music(s8 volume) {
	if (music_volume + volume > 100) music_volume = 100;
	else if (music_volume + volume < 0) music_volume = 0;
	else music_volume += volume;
	AEAudioSetGroupVolume(music_group, static_cast<float>(music_volume) / 100.0f);
}

/**************************************************************************/
/*!
	Called to adjust sound volume
*/
/**************************************************************************/
void Adjust_Sound(s8 volume) {
	if (sound_volume + volume > 100) sound_volume = 100;
	else if (sound_volume + volume < 0) sound_volume = 0;
	else sound_volume += volume;
	AEAudioSetGroupVolume(sound_group, static_cast<float>(sound_volume) / 100.0f);
}

void Unload_Music() {
	AEAudioUnloadAudio(menu_music);
	AEAudioUnloadAudio(level_select_music);
	AEAudioUnloadAudio(game_music);
	AEAudioUnloadAudio(victory_music);
	AEAudioUnloadAudio(defeat_music);
	AEAudioUnloadAudio(credits_music);
}

void Unload_Sound() {
	AEAudioUnloadAudio(cast_water);
	AEAudioUnloadAudio(cast_earth);
	AEAudioUnloadAudio(cast_air);
	AEAudioUnloadAudio(swap_water);
	AEAudioUnloadAudio(swap_earth);
	AEAudioUnloadAudio(swap_air);
}
