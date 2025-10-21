/******************************************************************************/
/*!
\file		Graphics_Utils.hpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the header file for graphical helper functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
extern AEGfxTexture* button_default;
extern AEGfxTexture* button_hovered;
extern AEGfxTexture* button_pressed;
extern AEGfxTexture* panel;

extern const f32 POPUP_BUTTON_TRUE[4];
extern const f32 POPUP_BUTTON_FALSE[4];

extern s8 p_objectives_font;
extern AEGfxVertexList* p_mesh;

void Get_Tiles_To_World_Coordinates(const s8& x, const s8& y, f32& world_x, f32& world_y);

void Get_Normalized_To_World_Coordinates(f32& world_x, f32& world_y, const f32& norm_x, const f32& norm_y, bool scale);

void Draw_Button(const f32& normalized_x_pos, const f32& normalized_y_pos, const f32& normalized_x_scale,
	const f32& normalized_y_scale,  float r, float g, float b, float a);

void Draw_Panel(const f32& normalized_x_pos, const f32& normalized_y_pos, const f32& normalized_x_scale,
	const f32& normalized_y_scale,  float r, float g, float b, float a);

void Draw_Popup(const std::string& warning_text);