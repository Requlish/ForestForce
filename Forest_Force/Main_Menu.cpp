/******************************************************************************/
/*!
\file		Main_Menu.cpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements main menu.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Main_Menu.hpp"
#include "Graphics.hpp"
#include "Graphics_Utils.hpp"
#include "Audio.hpp"
#include "Click_Particle_System.hpp"

static s32 mouse_X{}, mouse_Y{};
static f32 mouse_Xf{}, mouse_Yf{};
static f32 font_width{}, font_height{};
static AEGfxTexture* p_background{ nullptr };
static AEMtx33 image_transform{}, scale{}, translate{};

static const char* TITLE_TEXT = "Forest Force";
static const std::string CONFIRMATION_EXIT_GAME_TEXT = "You are about to exit the game";

static const f32 IMAGE_POS_X = 0;
static const f32 IMAGE_POS_Y = 0;

static const char* button_text[] = {
    "Start Game",
    "Level Editor",
    "How To Play",
    "Options",
    "Credits",
    "Quit Game"
};

// X divide by 800
// Y divide by 450
static AEVec2 text_position[] = {
    { -0.5f, 0.5f },
    { -0.5f, 0.25f },
    { -0.5f, 0.0f },
    { -0.5f, -0.25f },
    { -0.5f, -0.5f },
    { -0.5f, -0.75f },
};

static float accumulated_time_menu = 0.0f;

static const f32 title_border[4] = { 0.0f , 0.75f, 1.0f, 0.2f };
static const f32 play_button[4]= { -0.5f, 0.5f, 0.5f, 0.2f };
static const f32 editor_button[4] = { -0.5f, 0.25f, 0.5f, 0.2f };
static const f32 how_to_play_button[4] = { -0.5f, 0.0f, 0.5f, 0.2f };
static const f32 settings_button[4] = { -0.5f, -0.25f, 0.5f, 0.2f };
static const f32 credits_button[4] = { -0.5f, -0.5f, 0.5f, 0.2f };
static const f32 exit_button[4] = { -0.5f, -0.75f, 0.5f, 0.2f };
static const f32 button_color[4] = { 241.0f / 255.0f, 196.0f / 255.0f, 15.0f / 255.0f, 1.0f };
static const f32 title_border_color[4] = { 0, 0, 1, 1};

static const f32 button_true[4] = { -0.3f, -0.4f, 0.4f, 0.2f };
static const f32 button_false[4] = { 0.3f, -0.4f, 0.4f, 0.2f };
static bool confirmation_pop_up_triggered = false;
static bool how_to_play_triggered = false;
static std::string warning_text;

/**************************************************************************/
/*!
    Loads textures for main menu UI
    Loads particle system
*/
/**************************************************************************/
void Main_Menu_Load()
{
    // Button
    button_default = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_1.png");
    button_hovered = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_2.png");
    button_pressed = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_3.png");
    panel = AEGfxTextureLoad("Assets/UI/Buttons/grey_panel.png");
    p_background = AEGfxTextureLoad("Assets/Menu/house_in_forest.jpg");
    how_to_play_image = AEGfxTextureLoad("Assets/Menu/how_to_play.png");

    // Particle system
    Click_Particles_Load();
}

/**************************************************************************/
/*!
    Initializes music
*/
/**************************************************************************/
void Main_Menu_Initialize()
{
    Start_Music(menu_music);
}

/**************************************************************************/
/*!
    Handles user inputs to navigate to different game states
    Updates particle system
*/
/**************************************************************************/
void Main_Menu_Update()
{
    Get_Mouse_Normalized_Coordinates(mouse_Xf, mouse_Yf);
    if (confirmation_pop_up_triggered) {
        if (AEInputCheckTriggered(AEVK_ESCAPE)) {
            confirmation_pop_up_triggered = false;
        }
        else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (Is_Point_In_Rect(
                (button_false[0] - button_false[2] / 2.0f),
                (button_false[0] + button_false[2] / 2.0f),
                (button_false[1] + button_false[3] / 2.0f),
                (button_false[1] - button_false[3] / 2.0f),
                mouse_Xf, mouse_Yf))
            {
                confirmation_pop_up_triggered = false;
            }
            else if (Is_Point_In_Rect(
                (button_true[0] - button_true[2] / 2.0f),
                (button_true[0] + button_true[2] / 2.0f),
                (button_true[1] + button_true[3] / 2.0f),
                (button_true[1] - button_true[3] / 2.0f),
                mouse_Xf, mouse_Yf))
            {
                confirmation_pop_up_triggered = false;
                next = GS_QUIT;
            }
        }
    }
    else if (how_to_play_triggered) {
        if (AEInputCheckTriggered(AEVK_ESCAPE)) {
            how_to_play_triggered = false;
        }
    }
    else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        // Play
        if (Is_Point_In_Rect((play_button[0] - play_button[2] / 2.0f),
            (play_button[0] + play_button[2] / 2.0f),
            (play_button[1] + play_button[3] / 2.0f),
            (play_button[1] - play_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            next = MS_LEVELSELECT;
        }

        // Editor
        if (Is_Point_In_Rect((editor_button[0] - editor_button[2] / 2.0f),
            (editor_button[0] + editor_button[2] / 2.0f),
            (editor_button[1] + editor_button[3] / 2.0f),
            (editor_button[1] - editor_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            next = GS_EDITOR;
        }

        // How to Play
        if (Is_Point_In_Rect((how_to_play_button[0] - how_to_play_button[2] / 2.0f),
            (how_to_play_button[0] + how_to_play_button[2] / 2.0f),
            (how_to_play_button[1] + how_to_play_button[3] / 2.0f),
            (how_to_play_button[1] - how_to_play_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            how_to_play_triggered = true;
        }

        // Credits
        if (Is_Point_In_Rect((credits_button[0] - credits_button[2] / 2.0f),
            (credits_button[0] + credits_button[2] / 2.0f),
            (credits_button[1] + credits_button[3] / 2.0f),
            (credits_button[1] - credits_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            next = MS_CREDITS;
        }

        // Settings
        if (Is_Point_In_Rect((settings_button[0] - settings_button[2] / 2.0f),
            (settings_button[0] + settings_button[2] / 2.0f),
            (settings_button[1] + settings_button[3] / 2.0f),
            (settings_button[1] - settings_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            next = MS_SETTINGS;
        }        

        // Exit
        if (Is_Point_In_Rect((exit_button[0] - exit_button[2] / 2.0f),
            (exit_button[0] + exit_button[2] / 2.0f),
            (exit_button[1] + exit_button[3] / 2.0f),
            (exit_button[1] - exit_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            confirmation_pop_up_triggered = true;
            warning_text = CONFIRMATION_EXIT_GAME_TEXT;
        }
    }

    // Particle system
    Click_Particles_Update();
    Click_Particles_Swap();
}

/**************************************************************************/
/*!
    Renders background, buttons, texts
    Draws particle system effects
*/
/**************************************************************************/
void Main_Menu_Draw()
{
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // Background drawing
    AEMtx33Scale(&scale, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));
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

    // Buttons
    // Play
    Draw_Button(play_button[0], play_button[1], play_button[2], play_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_button_font, button_text[0], 1.f, &font_width, &font_height);
    AEGfxPrint(p_button_font, button_text[0], text_position[0].x - font_width / 2.0f, text_position[0].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Editor
    Draw_Button(editor_button[0], editor_button[1], editor_button[2], editor_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_button_font, button_text[1], 1.f, &font_width, &font_height);
    AEGfxPrint(p_button_font, button_text[1], text_position[1].x - font_width / 2.0f, text_position[1].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // How to Play
    Draw_Button(how_to_play_button[0], how_to_play_button[1], how_to_play_button[2], how_to_play_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_button_font, button_text[2], 1.f, &font_width, &font_height);
    AEGfxPrint(p_button_font, button_text[2], text_position[2].x - font_width / 2.0f, text_position[2].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Settings
    Draw_Button(settings_button[0], settings_button[1], settings_button[2], settings_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_button_font, button_text[3], 1.f, &font_width, &font_height);
    AEGfxPrint(p_button_font, button_text[3], text_position[3].x - font_width / 2.0f, text_position[3].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Credits
    Draw_Button(credits_button[0], credits_button[1], credits_button[2], credits_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_button_font, button_text[4], 1.f, &font_width, &font_height);
    AEGfxPrint(p_button_font, button_text[4], text_position[4].x - font_width / 2.0f, text_position[4].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Exit
    Draw_Button(exit_button[0], exit_button[1], exit_button[2], exit_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_button_font, button_text[5], 1.f, &font_width, &font_height);
    AEGfxPrint(p_button_font, button_text[5], text_position[5].x - font_width / 2.0f, text_position[5].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Game Title
    Draw_Panel(title_border[0], title_border[1], title_border[2], title_border[3], 
        title_border_color[0], title_border_color[1], title_border_color[2], title_border_color[3]);
    AEGfxGetPrintSize(p_title_font, TITLE_TEXT, 1.0f, &font_width, &font_height);
    AEGfxPrint(p_title_font, TITLE_TEXT, title_border[0] - font_width / 2.0f, title_border[1] - font_height / 2.0f, 1.0f, 1, 1, 1, 1);
    
    // Popup
    if (confirmation_pop_up_triggered) {
        Draw_Popup( warning_text);
    }
    if (how_to_play_triggered) {
        Draw_How_To_Play();
    }

    // Particle system
    Click_Particles_Emitter();
}

/**************************************************************************/
/*!
    NA
*/
/**************************************************************************/
void Main_Menu_Free()
{
        
}

/**************************************************************************/
/*!
    Unloads textures
*/
/**************************************************************************/
void Main_Menu_Unload()
{
    AEGfxTextureUnload(button_default);
    AEGfxTextureUnload(button_hovered);
    AEGfxTextureUnload(button_pressed);
    AEGfxTextureUnload(panel);
    AEGfxTextureUnload(p_background);
    AEGfxTextureUnload(how_to_play_image);    
}