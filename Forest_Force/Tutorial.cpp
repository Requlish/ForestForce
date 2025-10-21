/******************************************************************************/
/*!
\file		Tutorial.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for functions that are used for tutorial.
			Each function here is a specific tutorial panel. Should a 
			tutorial be added, the MAX_TUTORIAL should be incremented and 
			their fucntion should be added to tutorial_draw.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "pch.hpp"
#include "Tutorial.hpp"
#include "Utils.hpp"
#include "User_Data.hpp"
#include "Graphics.hpp"
#include "Graphics_Utils.hpp"

static const int MAX_TUTORIALS = 4;
static const f32 next_button[4] = { 0.0f, -0.60f, 0.6f, 0.3f };
static const f32 next_button_color[4] = { 205.0f / 255.0f, 127.0f / 255.0f, 50.0f / 255.0f ,1.0f };
static const char* next_button_text = "Continue";
const char* change_spell_text1 = "Press Q or E";
const char* change_spell_text2 = "to switch spells";
const char* cast_spell_text = "Click to cast!";
const char* water_spell_text1 = "Use Water to";
const char* water_spell_text2 = "put out Fire";
const char* earth_spell_text1 = "Use Earth to create";
const char* earth_spell_text2 = "mountains that decay";
const char* air_spell_text1 = "Use Wind to change";
const char* air_spell_text2 = "fire spread direction";
const char* zoom_text = "Press Space to Zoom";
const char* movement_text = "Press WASD to Move";

static int counter{};	// For counting tutorials

void Draw_Spell_Tutorial() {
	const f32 panel1_coords[4] = { 0.675f, -0.4f, 0.4f, 0.2f };
	const f32 panel1_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };
	const f32 panel2_coords[4] = { 0.4f, 0.4f, 0.4f, 0.2f };
	const f32 panel2_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };

	const f32 normalized_text_gap = 0.05f;

	f32 w{}, h{};

	Draw_Panel(panel1_coords[0], panel1_coords[1], panel1_coords[2], panel1_coords[3], 
		 panel1_color[0], panel1_color[1], panel1_color[2], panel1_color[3]);

	AEGfxGetPrintSize(p_tutorial_font, change_spell_text1, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, change_spell_text1, -w / 2 + panel1_coords[0], -h / 2 + panel1_coords[1] + normalized_text_gap, 1, 1, 1, 1, 1);
	AEGfxGetPrintSize(p_tutorial_font, change_spell_text2, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, change_spell_text2, -w / 2 + panel1_coords[0], -h / 2 + panel1_coords[1] - normalized_text_gap, 1, 1, 1, 1, 1);

	Draw_Panel(panel2_coords[0], panel2_coords[1], panel2_coords[2], panel2_coords[3],
		 panel2_color[0], panel2_color[1], panel2_color[2], panel2_color[3]);
	
	AEGfxGetPrintSize(p_tutorial_font, cast_spell_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, cast_spell_text, -w / 2 + panel2_coords[0], -h / 2 + panel2_coords[1], 1, 1, 1, 1, 1);

	Draw_Button(next_button[0], next_button[1], next_button[2], next_button[3],
		next_button_color[0], next_button_color[1], next_button_color[2], next_button_color[3]);
	AEGfxGetPrintSize(p_tutorial_font, next_button_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, next_button_text, -w / 2 + next_button[0], -h / 2 + next_button[1], 1, 1, 1, 1, 1);
}

void Draw_Individual_Spell_Tutorial() {
	const f32 panel1_coords[4] = { 0.2f, 0.2f, 0.5f, 0.2f };
	const f32 panel1_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };

	const f32 panel2_coords[4] = { 0.6f, -0.2f, 0.6f, 0.2f };
	const f32 panel2_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };

	const f32 panel3_coords[4] = { -0.2f, -0.2f, 0.6f, 0.2f };
	const f32 panel3_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };

	const f32 normalized_text_gap = 0.05f;

	f32 w{}, h{};

	Draw_Panel(panel1_coords[0], panel1_coords[1], panel1_coords[2], panel1_coords[3],
		 panel1_color[0], panel1_color[1], panel1_color[2], panel1_color[3]);

	AEGfxGetPrintSize(p_tutorial_font, water_spell_text1, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, water_spell_text1, -w / 2 + panel1_coords[0], -h / 2 + panel1_coords[1] + normalized_text_gap, 1, 1, 1, 1, 1);
	AEGfxGetPrintSize(p_tutorial_font, water_spell_text2, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, water_spell_text2, -w / 2 + panel1_coords[0], -h / 2 + panel1_coords[1] - normalized_text_gap, 1, 1, 1, 1, 1);

	Draw_Panel(panel2_coords[0], panel2_coords[1], panel2_coords[2], panel2_coords[3],
		 panel2_color[0], panel2_color[1], panel2_color[2], panel2_color[3]);

	AEGfxGetPrintSize(p_tutorial_font, earth_spell_text1, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, earth_spell_text1, -w / 2 + panel2_coords[0], -h / 2 + panel2_coords[1] + normalized_text_gap, 1, 1, 1, 1, 1);
	AEGfxGetPrintSize(p_tutorial_font, earth_spell_text2, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, earth_spell_text2, -w / 2 + panel2_coords[0], -h / 2 + panel2_coords[1] - normalized_text_gap, 1, 1, 1, 1, 1);

	Draw_Panel(panel3_coords[0], panel3_coords[1], panel3_coords[2], panel3_coords[3],
		panel3_color[0], panel3_color[1], panel3_color[2], panel3_color[3]);

	AEGfxGetPrintSize(p_tutorial_font, air_spell_text1, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, air_spell_text1, -w / 2 + panel3_coords[0], -h / 2 + panel3_coords[1] + normalized_text_gap, 1, 1, 1, 1, 1);
	AEGfxGetPrintSize(p_tutorial_font, air_spell_text2, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, air_spell_text2, -w / 2 + panel3_coords[0], -h / 2 + panel3_coords[1] - normalized_text_gap, 1, 1, 1, 1, 1);

	// Continue Button
	Draw_Button(next_button[0], next_button[1], next_button[2], next_button[3],
		 next_button_color[0], next_button_color[1], next_button_color[2], next_button_color[3]);
	AEGfxGetPrintSize(p_tutorial_font, next_button_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, next_button_text, -w / 2 + next_button[0], -h / 2 + next_button[1], 1, 1, 1, 1, 1);
}

void Draw_Zoom_Tutorial() {
	const f32 panel1_coords[4] = { -0.4f, -0.2f, 0.6f, 0.2f };
	const f32 panel1_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };

	f32 w{}, h{};

	Draw_Panel(panel1_coords[0], panel1_coords[1], panel1_coords[2], panel1_coords[3],
		 panel1_color[0], panel1_color[1], panel1_color[2], panel1_color[3]);
	AEGfxGetPrintSize(p_tutorial_font, zoom_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, zoom_text, -w / 2 + panel1_coords[0], -h / 2 + panel1_coords[1], 1, 1, 1, 1, 1);

	Draw_Button(next_button[0], next_button[1], next_button[2], next_button[3],
		next_button_color[0], next_button_color[1], next_button_color[2], next_button_color[3]);
	AEGfxGetPrintSize(p_tutorial_font, next_button_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, next_button_text, -w / 2 + next_button[0], -h / 2 + next_button[1], 1, 1, 1, 1, 1);

}

void Draw_Movement_Tutorial() {
	const f32 panel1_coords[4] = { -0.4f, 0.4f, 0.6f, 0.2f };
	const f32 panel1_color[4] = { 120.0f / 255.0f, 170.0f / 255.0f, 200.0f / 255.0f, 1.0f };

	f32 w{}, h{};

	Draw_Panel(panel1_coords[0], panel1_coords[1], panel1_coords[2], panel1_coords[3],
		 panel1_color[0], panel1_color[1], panel1_color[2], panel1_color[3]);
	AEGfxGetPrintSize(p_tutorial_font, movement_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, movement_text, -w / 2 + panel1_coords[0], -h / 2 + panel1_coords[1], 1, 1, 1, 1, 1);

	Draw_Button(next_button[0], next_button[1], next_button[2], next_button[3], 
		 next_button_color[0], next_button_color[1], next_button_color[2], next_button_color[3]);
	AEGfxGetPrintSize(p_tutorial_font, next_button_text, 1.f, &w, &h);
	AEGfxPrint(p_tutorial_font, next_button_text, -w / 2 + next_button[0], -h / 2 + next_button[1], 1, 1, 1, 1, 1);

}

/**************************************************************************/
/*!
	This function handles the checks input for the continue button of
	the tutorial.
*/
/**************************************************************************/
void Tutorials_Update() {
	f32 mouse_x{}, mouse_y{};
	Get_Mouse_Normalized_Coordinates(mouse_x, mouse_y);
	bool button_clicked = Is_Point_In_Rect( // Check if Resume Button is clicked
		next_button[0] - next_button[2] / 2.0f, // pos.x - scale.x / 2
		next_button[0] + next_button[2] / 2.0f, // pos.x + scale.x / 2
		next_button[1] + next_button[3] / 2.0f, // pos.y + scale.y / 2
		next_button[1] - next_button[3] / 2.0f, // pos.y - scale.y / 2
		mouse_x, mouse_y) && AEInputCheckTriggered(AEVK_LBUTTON);

	if (button_clicked && counter < MAX_TUTORIALS) counter++;

	if (counter == MAX_TUTORIALS) {
		tutorial_done = true;
		Complete_Tutorial();
	}
}

/**************************************************************************/
/*!
	This function draws the appropriate tutorial based on counter.
*/
/**************************************************************************/
void Tutorials_Draw() {
	switch (counter) {
	case 0:
		// Draw Movement Tutorial
		Draw_Movement_Tutorial();
		break;
	case 1:
		// Draw Spell Tutorial
		Draw_Spell_Tutorial();
		break;
	case 2:
		// Draw Zoom Tutorial
		Draw_Individual_Spell_Tutorial();
		break;
	case 3:
		// Draw Spell Tutorial 2
		Draw_Zoom_Tutorial();
		break;
	}
}

/**************************************************************************/
/*!
	This function resets the tutorial.
*/
/**************************************************************************/
void Tutorials_Reset() {
	tutorial_done = false;
	counter = 0;
}