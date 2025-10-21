/******************************************************************************/
/*!
\file		Graphics_Utils.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for graphical helper functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "Graphics_Utils.hpp"

// Strings
static const std::string POPUP_POPUP_TEXT = "Are you Sure?";
static const std::string POPUP_BUTTON_TRUE_TEXT = "Yes";
static const std::string POPUP_BUTTON_FALSE_TEXT = "No";

// Stored as pos_x, pos_y, scale_x, scale_y. All normalized
static const f32 POPUP_PANEL[4] = { 0.0f, 0.0f, 1.5f, 1.5f };
static const f32 POPUP_WARNING[2] = { 0.0f, 0.2f };					// Only need position
const f32 POPUP_BUTTON_TRUE[4] = { -0.3f, -0.4f, 0.4f, 0.2f };
const f32 POPUP_BUTTON_FALSE[4] = { 0.3f, -0.4f, 0.4f, 0.2f };

// Stored as r, g, b, a. Stored as a value from 0.0f to 1.0f (0 to 255).
static const f32 POPUP_PANEL_COLOR[4] = { 2.0f / 255.0f, 152.0f / 255.0f, 219.0f / 255.0f, 1.0f };
static const f32 POPUP_BUTTON_COLOR[4] = { 241.0f / 255.0f, 196.0f / 255.0f, 15.0f / 255.0f, 1.0f };

static AEMtx33 scale{};
static AEMtx33 translate{};
static AEMtx33 transform{};

AEGfxTexture* button_default{ nullptr };
AEGfxTexture* button_hovered{ nullptr };
AEGfxTexture* button_pressed{ nullptr };
AEGfxTexture* panel{ nullptr };

/**************************************************************************/
/*!
	This function updates a world x and y given a tile x and y. 
*/
/**************************************************************************/
void Get_Tiles_To_World_Coordinates(const s8 &x, const s8 &y, f32& world_x, f32& world_y) { // TODO: THIS GARBAGE
	for (int i = 0; i < TILE_X; i++) {
		if (i != x) continue;											// Ignores all tiles not same x as the x given
		float rect_left = i * GRID_SIZE;								// Left X world coordinate.
		world_x = static_cast<f32>(rect_left + GRID_SIZE / 2.0f);		// Center X world coordinate (what we want)
		for (int j = 0; j < TILE_Y; j++) {
			if (j != y) continue;										// Ignores all tiles not same y as the y given
			float rect_bottom = j * GRID_SIZE;							// Bottom Y world coordinate.
			world_y = static_cast<f32>(rect_bottom + GRID_SIZE / 2.0f); // Center Y world coordinate (what we want)
			break;
		}
		break;
	}
}

/**************************************************************************/
/*!
	This function updates a world x and y given the normalized x and y.
*/
/**************************************************************************/
void Get_Normalized_To_World_Coordinates(f32& world_x, f32& world_y, const f32& norm_x, const f32& norm_y, bool to_scale) {
	// This function assumes the given norm_x and norm_y parameter is between -1.0f to 1.0f
	f32 cam_x{}, cam_y{};

	AEGfxGetCamPosition(&cam_x, &cam_y);
	if (to_scale) cam_x = cam_y = 0; // Set Cam X and Cam Y to 0 when doing scale.
	world_x = norm_x * (AEGfxGetWindowWidth() / 2.0f) + cam_x;
	world_y = norm_y * (AEGfxGetWindowHeight() / 2.0f) + cam_y;
}

/**************************************************************************/
/*!
	This function handles the drawing of a dynamic button. Call this when
	you need a button. This function takes in normalized coordinates as 
	buttons are used with text, which is for UI.
*/
/**************************************************************************/
void Draw_Button(const f32& normalized_x_pos, const f32& normalized_y_pos, const f32& normalized_x_scale,
	const f32& normalized_y_scale,  float r, float g, float b, float a) {

	f32 world_x_pos{}, world_y_pos{}, world_x_scale{}, world_y_scale{};
	Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, normalized_x_pos, normalized_y_pos, false);
	Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, normalized_x_scale, normalized_y_scale, true);
	s32 mouse_x{}, mouse_y{};
	Get_Mouse_World_Coordinates(mouse_x, mouse_y);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(r, g, b, a);

	AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
	AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
	AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
	AEGfxSetTransform(transform.m);

	if (Is_Point_In_Rect(world_x_pos - world_x_scale / 2.0f, world_x_pos + world_x_scale / 2.0f,
		world_y_pos + world_y_scale / 2.0f, world_y_pos - world_y_scale / 2.0f, static_cast<f32>(mouse_x), static_cast<f32>(mouse_y))) {
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			AEGfxTextureSet(button_pressed, 0, 0);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
		}
		else {
			AEGfxTextureSet(button_hovered, 0, 0);
			AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
		}
	}
	else {
		AEGfxTextureSet(button_default, 0, 0);
		AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
	}
}

/**************************************************************************/
/*!
	This function handles the drawing of a panel. Call this when
	you need a panel. This function takes in normalized coordinates as
	it is used for UI.
*/
/**************************************************************************/
void Draw_Panel(const f32& normalized_x_pos, const f32& normalized_y_pos, const f32& normalized_x_scale,
	const f32& normalized_y_scale,  float r, float g, float b, float a) {
	f32 world_x_pos{}, world_y_pos{}, world_x_scale{}, world_y_scale{};
	Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, normalized_x_pos, normalized_y_pos, false);
	Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, normalized_x_scale, normalized_y_scale, true);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND); // Allow Transparency
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(r, g, b, a);

	AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
	AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
	AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
	AEGfxSetTransform(transform.m);

	AEGfxTextureSet(panel, 0, 0);
	AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
}

/**************************************************************************/
/*!
	This function handles the drawing of a popup. Call this when
	you need a poup confirmation. This function will always print at the
	center of the screen.
*/
/**************************************************************************/
void Draw_Popup(const std::string& warning_text) {
	Draw_Panel(POPUP_PANEL[0], POPUP_PANEL[1], POPUP_PANEL[2], POPUP_PANEL[3],
		 POPUP_PANEL_COLOR[0], POPUP_PANEL_COLOR[1], POPUP_PANEL_COLOR[2], POPUP_PANEL_COLOR[3]);

	f32 w{}, h{};

	AEGfxGetPrintSize(p_objectives_font, warning_text.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_objectives_font, warning_text.c_str(), -w / 2 + POPUP_WARNING[0], -h / 2 + POPUP_WARNING[1], 1, 1, 1, 1, 1);

	AEGfxGetPrintSize(p_objectives_font, POPUP_POPUP_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_objectives_font, POPUP_POPUP_TEXT.c_str(), -w / 2, -h / 2, 1, 1, 1, 1, 1);

	Draw_Button(POPUP_BUTTON_TRUE[0], POPUP_BUTTON_TRUE[1], POPUP_BUTTON_TRUE[2], POPUP_BUTTON_TRUE[3],
		 POPUP_BUTTON_COLOR[0], POPUP_BUTTON_COLOR[1], POPUP_BUTTON_COLOR[2], POPUP_BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_objectives_font, POPUP_BUTTON_TRUE_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_objectives_font, POPUP_BUTTON_TRUE_TEXT.c_str(), -w / 2 + POPUP_BUTTON_TRUE[0], -h / 2 + POPUP_BUTTON_TRUE[1], 1, 1, 1, 1, 1);

	Draw_Button(POPUP_BUTTON_FALSE[0], POPUP_BUTTON_FALSE[1], POPUP_BUTTON_FALSE[2], POPUP_BUTTON_FALSE[3],
	 POPUP_BUTTON_COLOR[0], POPUP_BUTTON_COLOR[1], POPUP_BUTTON_COLOR[2], POPUP_BUTTON_COLOR[3]);
	AEGfxGetPrintSize(p_objectives_font, POPUP_BUTTON_FALSE_TEXT.c_str(), 1.f, &w, &h);
	AEGfxPrint(p_objectives_font, POPUP_BUTTON_FALSE_TEXT.c_str(), -w / 2 + POPUP_BUTTON_FALSE[0], -h / 2 + POPUP_BUTTON_FALSE[1], 1, 1, 1, 1, 1);
}