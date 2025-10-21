/******************************************************************************/
/*!
\file		System.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This source file contains System related functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "Graphics.hpp"
#include "Objectives.hpp"
#include "User_Data.hpp"
#include "Tutorial.hpp"
#include "Main_Menu.hpp"
#include "Audio.hpp"
#include "File_Load.hpp"

// All Fonts are initialized here and declared using extern on their respective header
s8 p_font{};			 
s8 p_smaller_font{};
s8 p_spell_ui_font{};
s8 p_tutorial_font{};
s8 p_button_font{};
s8 p_title_font{};
s8 p_objectives_font{};
s8 p_tool_tips_font{};
s8 p_splash_font{};
s8 p_credits_font{};

// Generic Mesh (white rect) to be used for drawing assets
// This is the only mesh used in our project and is initialized
// here to prevent multiple creation and deletion of meshes
AEGfxVertexList* p_mesh{ nullptr };		

/**************************************************************************/
/*!
	This function is called near the start of application launch. It handles
	the loading of fonts, game parameters, game save data, Music and Sound.
	It also creates a mesh here as the whole game uses the same mesh.
*/
/**************************************************************************/
void System_Initialize()
{
	// Changing the window title
	AESysSetWindowTitle("Forest Force");

	// Load Fonts
	p_font = AEGfxCreateFont("Assets/Fonts/Archivo-Regular.ttf", 72);					// Font for Pause
	p_smaller_font = AEGfxCreateFont("Assets/Fonts/Archivo-Regular.ttf", 36);			// Font for How to Pause
	p_spell_ui_font = AEGfxCreateFont("Assets/Fonts/Archivo-Regular.ttf", 40);			// Font for Spell cooldown
	p_tutorial_font = AEGfxCreateFont("Assets/Fonts/PressStart2P-Regular.ttf", 20);		// Font for Tutorial
	p_button_font = AEGfxCreateFont("Assets/Fonts/PixelifySans-Regular.ttf", 50);		// Font for Buttons
	p_title_font = AEGfxCreateFont("Assets/Fonts/PixelifySans-Regular.ttf", 100);		// Font for Title
	p_objectives_font = AEGfxCreateFont("Assets/Fonts/Archivo-Regular.ttf", 30);		// Font for Objectives
	p_tool_tips_font = AEGfxCreateFont("Assets/Fonts/Archivo-Regular.ttf", 20);			// Font for Tool-Tips
	p_splash_font = AEGfxCreateFont("Assets/Fonts/PixelifySans-Regular.ttf", 300);		// Font for Splash Screen
	p_credits_font = AEGfxCreateFont("Assets/Fonts/Archivo-Regular.ttf", 80);			// Font for Credits

	// Load Game Parameters (File Read)
	Load_Game_Parameters();

	// Check User Data
	tutorial_done = Check_User_Data();

	// Load Audio
	music_group = AEAudioCreateGroup(); // Generic Music Group
	sound_group = AEAudioCreateGroup(); // Generic Sound Group
	Load_Music();
	Load_Sound();

	// Mesh Creation
	AEGfxMeshStart(); // Start Mesh creation
	AEGfxTriAdd(-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f, 0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f); // Triangle 1
	AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f, 0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f, -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f); // Triangle 2
	p_mesh = AEGfxMeshEnd(); // End Mesh, Save address of Mesh
}

/**************************************************************************/
/*!
	This function is called on application exit. It handles the freeing of
	memory and fonts.
*/
/**************************************************************************/
void System_Exit()
{
	// Unload Fonts
	AEGfxDestroyFont(p_font);
	AEGfxDestroyFont(p_smaller_font);
	AEGfxDestroyFont(p_spell_ui_font);
	AEGfxDestroyFont(p_tutorial_font);
	AEGfxDestroyFont(p_button_font);
	AEGfxDestroyFont(p_title_font);
	AEGfxDestroyFont(p_objectives_font);
	AEGfxDestroyFont(p_tool_tips_font);
	AEGfxDestroyFont(p_splash_font);
	AEGfxDestroyFont(p_credits_font);

	// Unload Audio
	Unload_Music();
	Unload_Sound();
	AEAudioUnloadAudioGroup(music_group);
	AEAudioUnloadAudioGroup(sound_group);

	// Unload Mesh
	AEGfxMeshFree(p_mesh);

	// End Alpha Engine
	AESysExit();
}