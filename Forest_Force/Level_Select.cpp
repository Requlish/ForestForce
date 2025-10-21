/******************************************************************************/
/*!
\file		Level_Select.cpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements level selector.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Level_Select.hpp"
#include "Fileio.hpp"
#include "User_Data.hpp"
#include "Audio.hpp"
#include "Graphics_Utils.hpp"
#include "Click_Particle_System.hpp"

static const f32 BUTTON_SIZE = 150;
static const int MAX_BUTTONS_PER_ROW = 6;
static const f32 IMAGE_POS_X = 0;
static const f32 IMAGE_POS_Y = 0;
static const f32 title_border[4] = { 0.0f , 0.75f, 1.0f, 0.2f };
static const f32 title_border_color[4] = { 0, 0, 1, 1 };
static const char* TITLE_TEXT = "Level Select";

static s32 mouse_X{}, mouse_Y{};
static f32 font_width{}, font_height{};
static AEGfxTexture* p_texture{ nullptr };
static AEGfxTexture* p_background{ nullptr };
static AEMtx33 image_transform{}, scale{}, translate{};
static std::vector<std::string> level_data{};
static int total_buttons = LAST_LEVEL - GS_LEVEL1 + 1;

static std::vector<AEVec2> text_position;
static std::vector<AEVec2> button_position;

/**************************************************************************/
/*!
    Calculates button positions dynamically
*/
/**************************************************************************/
void Calculate_Button_Positions() {
    static const f32 row_height = 200.0f;
    static const f32 button_x = -500.0f;
    static const f32 button_y = 150.0f;
    static const f32 text_x = -0.625f;
    static const f32 text_y = 0.33f;

    static const float text_x_gap = 0.25f;

    text_position.resize(static_cast<size_t>(total_buttons));
    button_position.resize(static_cast<size_t>(total_buttons));

    for (int i = 0; i < total_buttons; ++i) {
        int row = i / MAX_BUTTONS_PER_ROW;
        int col = i % MAX_BUTTONS_PER_ROW;
 
        button_position[i].x = button_x + (col * row_height);
        button_position[i].y = button_y - (row * row_height);
        text_position[i].x = text_x + (text_x_gap * col);
        text_position[i].y = text_y - (row * row_height / (static_cast<f32>(WINDOW_WIDTH / 2.0)));
        if (i == total_buttons - 1)
        {
            button_position[i].x = 0.f;
            text_position[i].x = 0.f;
        }
    }
}

/**************************************************************************/
/*!
    Loads textures
    Sets up transformations for rendering
    Loads particle system
    Gets user data
*/
/**************************************************************************/
void Level_Select_Load() {
    // Button
    p_texture = AEGfxTextureLoad("Assets/Menu/green_border.jpg");
    p_background = AEGfxTextureLoad("Assets/Menu/player_and_arsonist.jpg");
    panel = AEGfxTextureLoad("Assets/UI/Buttons/grey_panel.png");

    AEMtx33Scale(&scale, BUTTON_SIZE, BUTTON_SIZE);
    AEMtx33Trans(&translate, 0, 0);
    AEMtx33Concat(&image_transform, &translate, &scale);

    Calculate_Button_Positions();
    
    // Particle system
    Click_Particles_Load();

    // Get user Data
    Get_Level_Data(level_data);
}

/**************************************************************************/
/*!
    Initializes music
*/
/**************************************************************************/
void Level_Select_Initialize() {
    Start_Music(level_select_music);
}

/**************************************************************************/
/*!
    Handles user input to navigate to level
    Updates particle system
*/
/**************************************************************************/
void Level_Select_Update() {
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        Get_Mouse_World_Coordinates(mouse_X, mouse_Y);
        // Scale button to world co-ordinates
        f32 scaled_button_length = (float(BUTTON_SIZE) / static_cast<f32>(WINDOW_LENGTH)) * static_cast<f32>(AEGfxGetWindowWidth());
        f32 scaled_button_width = (float(BUTTON_SIZE) / static_cast<f32>(WINDOW_WIDTH)) * static_cast<f32>(AEGfxGetWindowHeight());
        
        for (int i = 0; i < total_buttons; ++i) {
            f32 scaled_button_x = (button_position[i].x / WINDOW_LENGTH) * float(WINDOW_LENGTH);
            f32 scaled_button_y = (button_position[i].y / WINDOW_WIDTH) * float(WINDOW_WIDTH);
            if (i == total_buttons - 1)
            {
                scaled_button_length *= 3.f;
     
            }
            if (Is_Point_In_Rect((scaled_button_x - scaled_button_length / 2.0f),
                (scaled_button_x + scaled_button_length / 2.0f),
                (scaled_button_y + scaled_button_width / 2.0f),
                (scaled_button_y - scaled_button_width / 2.0f),
                static_cast<f32>(mouse_X), static_cast<f32>(mouse_Y))) {
                next = GS_LEVEL1 + i;
                break;
            }
        }
    }

    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        next = GS_MAINMENU;
    }

    Click_Particles_Update();
    Click_Particles_Swap();
}

/**************************************************************************/
/*!
    Renders background, buttons, texts
    Draws particle system effects
    Handles colour of level based on level completion
*/
/**************************************************************************/
void Level_Select_Draw() {
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // Background drawing
    AEMtx33Scale(&scale, WINDOW_LENGTH, WINDOW_WIDTH);
    AEMtx33Trans(&translate, IMAGE_POS_X, IMAGE_POS_Y);
    AEMtx33Concat(&image_transform, &translate, &scale);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxTextureSet(p_background, 0, 0);

    // Draw mesh
    AEGfxSetTransform(image_transform.m);
    AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

    // Game Title
    Draw_Panel(title_border[0], title_border[1], title_border[2], title_border[3],
        title_border_color[0], title_border_color[1], title_border_color[2], title_border_color[3]);
    AEGfxGetPrintSize(p_title_font, TITLE_TEXT, 1.0f, &font_width, &font_height);
    AEGfxPrint(p_title_font, TITLE_TEXT, title_border[0] - font_width / 2.0f, title_border[1] - font_height / 2.0f, 1.0f, 1, 1, 1, 1);

    // Button (for every button / level)
    for (int i = 0; i < total_buttons; ++i) {
        AEMtx33Scale(&scale, BUTTON_SIZE, BUTTON_SIZE);

        if (i == total_buttons - 1)
        {
            AEMtx33Scale(&scale, BUTTON_SIZE * 3.f, BUTTON_SIZE);
        }

        AEMtx33Trans(&translate, button_position[i].x, button_position[i].y);
        AEMtx33Concat(&image_transform, &translate, &scale);
 
        // Draw the image (button border)
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        f32 star_color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        switch (static_cast<int>(level_data[i].back() - '0')) {
        case 0:
            break;
        case 1:
            star_color[0] = 205.0f / 255.0f;
            star_color[1] = 127.0f / 255.0f;
            star_color[2] = 50.0f / 255.0f;
            star_color[3] = 1.0f;
            break;
        case 2:
            star_color[0] = 192.0f / 255.0f;
            star_color[1] = 192.0f / 255.0f;
            star_color[2] = 192.0f / 255.0f;
            star_color[3] = 1.0f;
            break;
        case 3:
            star_color[0] = 255.0f / 255.0f;
            star_color[1] = 215.0f / 255.0f;
            star_color[2] = 0.0f / 255.0f;
            star_color[3] = 1.0f;
            break;
        }

        AEGfxSetColorToMultiply(star_color[0], star_color[1], star_color[2], star_color[3]);
        AEGfxTextureSet(p_texture, 0, 0);

        // Draw mesh
        AEGfxSetTransform(image_transform.m);
        AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

        // Button text
        AEVec2 text_position_var = text_position[i];
        std::string level_text = std::to_string(i + 1);
        if (i == total_buttons - 1)
        {
            level_text = "Custom";
        }
        const char* text = level_text.c_str();
        AEGfxGetPrintSize(p_font, text, 1.0f, &font_width, &font_height);
        AEGfxPrint(p_font, text, text_position_var.x - font_width / 2.0f, text_position_var.y - font_height / 2.0f, 1, 0, 0, 0, 1);
    }

    // Particle system
    Click_Particles_Emitter();
}

/**************************************************************************/
/*!
    NA
*/
/**************************************************************************/
void Level_Select_Free() {

}

/**************************************************************************/
/*!
    Unloads textures
*/
/**************************************************************************/
void Level_Select_Unload() {
    AEGfxTextureUnload(p_texture);
    AEGfxTextureUnload(p_background);
    AEGfxTextureUnload(panel);
}