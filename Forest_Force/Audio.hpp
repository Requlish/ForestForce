/******************************************************************************/
/*!
\file		Audio.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for functions that deal with audio.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

extern s8 BASE_MUSIC_VOLUME;
extern s8 BASE_SOUND_VOLUME;

extern s8 music_volume;
extern s8 sound_volume;

extern AEAudio menu_music;
extern AEAudio level_select_music;
extern AEAudio game_music;
extern AEAudio victory_music;
extern AEAudio defeat_music;
extern AEAudio credits_music;

extern AEAudio cast_water;
extern AEAudio cast_earth;
extern AEAudio cast_air;
extern AEAudio swap_water;
extern AEAudio swap_earth;
extern AEAudio swap_air;

extern AEAudioGroup music_group; // Generic Music Group
extern AEAudioGroup sound_group; // Generic Sound Group

void Load_Music();
void Load_Sound();

void Start_Music(AEAudio music);
void Play_Sound(AEAudio sound);

void Adjust_Music(s8 volume);
void Adjust_Sound(s8 volume);

void Unload_Music();
void Unload_Sound();