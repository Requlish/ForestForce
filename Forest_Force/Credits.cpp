/******************************************************************************/
/*!
\file		Credits.cpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements credits screen.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Credits.hpp"
#include "Graphics_Utils.hpp"
#include "Audio.hpp"

static f32 font_width{}, font_height{};

static AEGfxTexture* p_copyright{ nullptr };
static AEGfxTexture* p_digipen{ nullptr };
static AEMtx33 image_transform{}, scale{}, translate{};
static const int COPYRIGHT_LENGTH = 15;
static const int COPYRIGHT_WIDTH = 15;

static float text_scroll = -0.75f;
static float scroll_speed = 1.0f;
static f32 x_increase = 0.0f;
static f32 y_increase = 0.002f;

static float digipen_logo_pos = 1.1f;
static float speed_increase_text_pos_x = -0.95f;
static float speed_increase_text_pos_y_1 = 0.9f;
static float speed_increase_text_pos_y_2 = 0.85f;
static float digipen_copyright_pos_1 = 0.765f;
static float sound_copyright_pos = 3.085f;
static float digipen_copyright_pos_2 = 3.185f;
static float digipen_copyright_pos_trans = 257.0f;
static float sound_copyright_pos_trans = 99.0f;

static const float IMAGE_LENGTH = (1525 / 2);
static const float IMAGE_WIDTH = (445 / 2);

static const char* credits_text[] = {
	"All Content    2024 DigiPen Institute of Technology Singapore, All Rights Reserved.",	// Copyright			// 0
	"FOREST FORCE",																			// Game and Team Name	// 1
	"by Avatar",																									// 2
	"Developers",																			// Developers			// 3
	"Lim Zhen Eu Damon (l.zheneudamon@digipen.edu)",																// 4
	"Chua Jim Hans (c.jimhans@digipen.edu)",																		// 5
	"Ho Zhen Hao (zhenhao.ho@digipen.edu)",																			// 6
	"Titus Kwong Wen Shuen (tituswenshuen.kwong@digipen.edu)",														// 7
	"Instructors",																			// Instructors			// 8
	"Cheng Ding Xiang",																								// 9
	"Gerald Wong",																									// 10
	"From",																					// DigiPen				// 11
	"DigiPen Institute of Technology Singapore",																	// 12
	"President",																			// President			// 13
	"Claude Comair",																								// 14
	"Executives",																			// Executives			// 15
	"Jason Chu | Christopher Comair",																				// 16
	"Micheal Gats | Michele Comair",																				// 17
	"Raymond Yan | Samir Abou Samra",																				// 18
	"Prasanna Ghali | John Bauer",																					// 19
	"Dr. Erik Mohrmann | Melvin Gonsalvez",																			// 20
	"Anglea Kugler | Dr. Charles Duba",																				// 21
	"Ben Ellinger | Johnny Deek",																					// 22
	"Thank You!",																			// End of credits		// 23
	"Press ESC to exit",																							// 24
	"Hyeongsun IM",																			// Instructors			// 25
	"Hold Down Left Click",																							// 26
	"To Increase Speed",																							// 27
	"FMOD Sound System    FireLight Technologies Pty Ltd (1998 - 2024)",											// 28
};

/**************************************************************************/
/*!
	Loads textures
	Sets up transformations for rendering
*/
/**************************************************************************/
void Credits_Load()
{
	p_copyright = AEGfxTextureLoad("Assets/Menu/copyright.png");
	p_digipen = AEGfxTextureLoad("Assets/Menu/digipen_logo.png");

    AEMtx33Scale(&scale, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));
    AEMtx33Trans(&translate, 0, 0);
    AEMtx33Concat(&image_transform, &translate, &scale);
}

/**************************************************************************/
/*!
	Resets starting position of credits
	Plays credits music
*/
/**************************************************************************/
void Credits_Initialize()
{
	// To reset the starting position of credits back to original
	text_scroll = -0.75f;

	// Play Music
	Start_Music(credits_music);
}

/**************************************************************************/
/*!
	Handles user input to increase scroll speed of change game state
*/
/**************************************************************************/
void Credits_Update()
{
    if (AEInputCheckCurr(AEVK_LBUTTON)) { 
		scroll_speed = 2.0f;
	}
	else { 
		scroll_speed = 1.0f;
	}
	
	text_scroll += 0.004f * scroll_speed;

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        next = GS_MAINMENU;
    }

	// Automatically change back to main menu after all credits shown
	if (text_scroll >= 3.4f)
	{
		next = GS_MAINMENU;
	}
}

/**************************************************************************/
/*!
	Renders background and credits
*/
/**************************************************************************/
void Credits_Draw()
{
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f); 
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

	// DigiPen logo
	y_increase = text_scroll + digipen_logo_pos;
	Get_Normalized_To_World_Coordinates(x_increase, y_increase, x_increase, y_increase, false);

	AEMtx33Scale(&scale, IMAGE_LENGTH, IMAGE_WIDTH);
	AEMtx33Trans(&translate, (-font_width / 2), y_increase);
	AEMtx33Concat(&image_transform, &translate, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(p_digipen, 0, 0);

	AEGfxSetTransform(image_transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	// End of DigiPen logo

	// For user to increase scroll speed if want to
	AEGfxGetPrintSize(p_credits_font, credits_text[26], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[26], speed_increase_text_pos_x, speed_increase_text_pos_y_1, 0.3f, 1.f, 0.f, 0.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[27], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[27], speed_increase_text_pos_x, speed_increase_text_pos_y_2, 0.3f, 1.f, 0.f, 0.f, 1.f);

	// Copyright
	AEGfxGetPrintSize(p_credits_font, credits_text[0], 0.25f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[0], -font_width / 2, text_scroll + 0.75f, 0.25f, 1.f, 1.f, 1.f, 1.f);

	y_increase = text_scroll + digipen_copyright_pos_1;
	Get_Normalized_To_World_Coordinates(x_increase, y_increase, x_increase, y_increase, false);

	// Copyright logo (1st instance)
	AEMtx33Scale(&scale, COPYRIGHT_LENGTH, COPYRIGHT_WIDTH);
	AEMtx33Trans(&translate, (-font_width / 2) - digipen_copyright_pos_trans, y_increase);
	AEMtx33Concat(&image_transform, &translate, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(p_copyright, 0, 0);

	AEGfxSetTransform(image_transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	// End of copyright logo (1st instance)

	// Game and Team name
	AEGfxGetPrintSize(p_credits_font, credits_text[1], 1.0f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[1], -font_width / 2, text_scroll + 0.5f, 1.0f, 1.f, 1.f, 1.f, 1.f);

	AEGfxGetPrintSize(p_credits_font, credits_text[2], 0.7f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[2], -font_width / 2, text_scroll + 0.35f, 0.7f, 0.f, 1.f, 0.f, 1.f);

	// Developers
	AEGfxGetPrintSize(p_credits_font, credits_text[3], 1.0f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[3], -font_width / 2, text_scroll, 1.f, 1.f, 1.f, 1.f, 1.f);

	AEGfxGetPrintSize(p_credits_font, credits_text[4], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[4], -font_width / 2, text_scroll - 0.1f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[5], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[5], -font_width / 2, text_scroll - 0.2f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[6], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[6], -font_width / 2, text_scroll - 0.3f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[7], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[7], -font_width / 2, text_scroll - 0.4f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	// Instructors
	AEGfxGetPrintSize(p_credits_font, credits_text[8], 1.0f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[8], -font_width / 2, text_scroll - 0.7f, 1.f, 1.f, 1.f, 1.f, 1.f);

	AEGfxGetPrintSize(p_credits_font, credits_text[9], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[9], -font_width / 2, text_scroll - 0.8f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[10], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[10], -font_width / 2, text_scroll - 0.9f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[25], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[25], -font_width / 2, text_scroll - 1.0f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	// DigiPen
	AEGfxGetPrintSize(p_credits_font, credits_text[11], 0.7f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[11], -font_width / 2, text_scroll - 1.3f, 0.7f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[12], 0.7f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[12], -font_width / 2, text_scroll - 1.45f, 0.7f, 1.f, 1.f, 1.f, 1.f);

	// President
	AEGfxGetPrintSize(p_credits_font, credits_text[13], 1.0f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[13], -font_width / 2, text_scroll - 1.7f, 1.f, 1.f, 1.f, 1.f, 1.f);

	AEGfxGetPrintSize(p_credits_font, credits_text[14], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[14], -font_width / 2, text_scroll - 1.8f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	// Executives
	AEGfxGetPrintSize(p_credits_font, credits_text[15], 1.0f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[15], -font_width / 2, text_scroll - 2.1f, 1.f, 1.f, 1.f, 1.f, 1.f);

	AEGfxGetPrintSize(p_credits_font, credits_text[16], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[16], -font_width / 2, text_scroll - 2.2f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[17], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[17], -font_width / 2, text_scroll - 2.3f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[18], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[18], -font_width / 2, text_scroll - 2.4f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[19], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[19], -font_width / 2, text_scroll - 2.5f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[20], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[20], -font_width / 2, text_scroll - 2.6f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[21], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[21], -font_width / 2, text_scroll - 2.7f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[22], 0.3f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[22], -font_width / 2, text_scroll - 2.8f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	// Sound copyright
	AEGfxGetPrintSize(p_credits_font, credits_text[28], 0.25f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[28], -font_width / 2, text_scroll - 3.1f, 0.25f, 1.f, 1.f, 1.f, 1.f);

	y_increase = text_scroll - sound_copyright_pos;
	Get_Normalized_To_World_Coordinates(x_increase, y_increase, x_increase, y_increase, false);

	// Copyright logo (2nd instance)
	AEMtx33Scale(&scale, COPYRIGHT_LENGTH, COPYRIGHT_WIDTH);
	AEMtx33Trans(&translate, (-font_width / 2) - sound_copyright_pos_trans, y_increase);
	AEMtx33Concat(&image_transform, &translate, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(p_copyright, 0, 0);

	AEGfxSetTransform(image_transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	// End of copyright logo (2nd instance)

	// DigiPen Copyright
	AEGfxGetPrintSize(p_credits_font, credits_text[0], 0.25f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[0], -font_width / 2, text_scroll - 3.2f, 0.25f, 1.f, 1.f, 1.f, 1.f);

	y_increase = text_scroll - digipen_copyright_pos_2;
	Get_Normalized_To_World_Coordinates(x_increase, y_increase, x_increase, y_increase, false);

	// Copyright logo (3rd instance)
	AEMtx33Scale(&scale, COPYRIGHT_LENGTH, COPYRIGHT_WIDTH);
	AEMtx33Trans(&translate, (-font_width / 2) - digipen_copyright_pos_trans, y_increase);
	AEMtx33Concat(&image_transform, &translate, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxTextureSet(p_copyright, 0, 0);

	AEGfxSetTransform(image_transform.m);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	// End of copyright logo (3rd instance)

	// End of credits
	AEGfxGetPrintSize(p_credits_font, credits_text[23], 0.5f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[23], -font_width / 2, text_scroll - 3.6f, 0.5f, 1.f, 1.f, 1.f, 1.f);
	AEGfxGetPrintSize(p_credits_font, credits_text[24], 0.5f, &font_width, &font_height);
	AEGfxPrint(p_credits_font, credits_text[24] , -font_width / 2, text_scroll - 3.7f, 0.5f, 1.f, 1.f, 1.f, 1.f);
}

/**************************************************************************/
/*!
	NA
*/
/**************************************************************************/
void Credits_Free()
{

}

/**************************************************************************/
/*!
	Unloads textures
*/
/**************************************************************************/
void Credits_Unload()
{
	AEGfxTextureUnload(p_copyright);
	AEGfxTextureUnload(p_digipen);
}