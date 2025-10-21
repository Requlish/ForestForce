/******************************************************************************/
/*!
\file		Settings.cpp
\author 	Lim Zhen Eu Damon
\par    	Email: l.zheneudamon@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This is the source file for Settings / Options related functions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Settings.hpp"             // Includes Font for Button and Title
#include "User_Data.hpp"
#include "Graphics_Utils.hpp"
#include "Tutorial.hpp"
#include "Audio.hpp"
#include "Click_Particle_System.hpp"

static s32 mouse_X{}, mouse_Y{};
static f32 mouse_Xf{}, mouse_Yf{};
static f32 font_width{}, font_height{};
static AEGfxTexture* p_background{ nullptr };
static AEGfxTexture* right_slider{ nullptr };
static AEGfxTexture* left_slider{ nullptr };
static AEMtx33 image_transform{}, scale{}, translate{}, transform{};
static const int INITIAL_WINDOW_LENGTH = 1600;
static const int INITIAL_WINDOW_WIDTH = 900;
static const int BUTTON_LENGTH = 350;
static const int BUTTON_WIDTH = 100;

static const char* TITLE_TEXT = "Options";

static const f32 IMAGE_POS_X = 0;
static const f32 IMAGE_POS_Y = 0;

static std::string music_text;
static std::string sound_text;
static const char* tutorial_text = "Restart Tutorial";
static const char* clear_data_text = "Clear Data";
static const char* menu_text = "To Menu";

// X divide by 800
// Y divide by 450
static AEVec2 text_position[] = {
    { -0.5f, 0.5f },
    { -0.5f, 0.255f },
    { -0.5f, 0.011f },
    { -0.5f, -0.233f },
    { -0.5f, -0.477f },
};

static float accumulated_time_menu = 0.0f;

// Stored in normalized pos_x, pos_y, scale_x, scale_y
static const f32 title_border[4] = { 0.0f , 0.75f, 1.0f, 0.2f };
static const f32 music_panel[4]= { -0.5f, 0.5f, 0.5f, 0.2f };
static const f32 music_panel_left_slider[4] = { -0.5f - 0.2f, 0.5f, 0.075f, 0.075f };
static const f32 music_panel_right_slider[4] = { -0.5f + 0.2f, 0.5f, 0.075f, 0.075f };
static const f32 sound_panel[4] = { -0.5f, 0.255f, 0.5f, 0.2f };
static const f32 sound_panel_left_slider[4] = { -0.5f - 0.2f, 0.255f, 0.075f, 0.075f };
static const f32 sound_panel_right_slider[4] = { -0.5f + 0.2f, 0.255f, 0.075f, 0.075f };
static const f32 tutorial_button[4] = { -0.5f, 0.011f, 0.5f, 0.2f };
static const f32 clear_data_button[4] = { -0.5f, -0.233f, 0.5f, 0.2f };
static const f32 exit_button[4] = { -0.5f, -0.477f, 0.5f, 0.2f };
static const f32 button_color[4] = { 241.0f / 255.0f, 196.0f / 255.0f, 15.0f / 255.0f, 1.0f };
static const f32 title_border_color[4] = { 0, 0, 1, 1};

static const f32 pop_up[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
static const f32 pop_up_color[4] = { 200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 1.0f };
static const char* POP_UP_TEXT1 = "Settings have been updated!";
static const char* POP_UP_TEXT2 = "Press any key to continue!";
static const f32 pop_up_text_color[4] = { 50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 1.0f };

static bool confirmation_pop_up_triggered = false;
static bool any_key_pop_up_triggered = false;
static const std::string CONFIRMATION_CLEAR_DATA_TEXT = "You are about to delete your play data";
static std::string warning_text;

void Settings_Load()
{
    // Button
    button_default = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_1.png");
    button_hovered = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_2.png");
    button_pressed = AEGfxTextureLoad("Assets/UI/Buttons/grey_button_3.png");
    panel = AEGfxTextureLoad("Assets/UI/Buttons/grey_panel.png");
    p_background = AEGfxTextureLoad("Assets/Menu/house_in_forest.jpg");
    right_slider = AEGfxTextureLoad("Assets/UI/Buttons/blue_slider_right.png");
    left_slider = AEGfxTextureLoad("Assets/UI/Buttons/blue_slider_left.png");

    AEMtx33Scale(&scale, BUTTON_LENGTH, BUTTON_WIDTH);
    AEMtx33Trans(&translate, 0, 0);
    AEMtx33Concat(&image_transform, &translate, &scale);

    // Particle system
    Click_Particles_Load();
}

void Settings_Initialize()
{

}

void Settings_Update()
{
    static const s8 inc_dec_amount = 10; // Percentage increase or decrease per slider click
    Get_Mouse_Normalized_Coordinates(mouse_Xf, mouse_Yf);

    if (any_key_pop_up_triggered) {
        if (Any_Key_Was_Pressed()) {
            any_key_pop_up_triggered = false;
        }
    }
    else if (confirmation_pop_up_triggered) {
        if (AEInputCheckTriggered(AEVK_ESCAPE)) {
            confirmation_pop_up_triggered = false;
        }
        else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
            if (Is_Point_In_Rect(
                (POPUP_BUTTON_FALSE[0] - POPUP_BUTTON_FALSE[2] / 2.0f),
                (POPUP_BUTTON_FALSE[0] + POPUP_BUTTON_FALSE[2] / 2.0f),
                (POPUP_BUTTON_FALSE[1] + POPUP_BUTTON_FALSE[3] / 2.0f),
                (POPUP_BUTTON_FALSE[1] - POPUP_BUTTON_FALSE[3] / 2.0f),
                mouse_Xf, mouse_Yf))
            {
                confirmation_pop_up_triggered = false;
            }
            else if (Is_Point_In_Rect(
                (POPUP_BUTTON_TRUE[0] - POPUP_BUTTON_TRUE[2] / 2.0f),
                (POPUP_BUTTON_TRUE[0] + POPUP_BUTTON_TRUE[2] / 2.0f),
                (POPUP_BUTTON_TRUE[1] + POPUP_BUTTON_TRUE[3] / 2.0f),
                (POPUP_BUTTON_TRUE[1] - POPUP_BUTTON_TRUE[3] / 2.0f),
                mouse_Xf, mouse_Yf))
            {
                confirmation_pop_up_triggered = false;
                Clear_User_Data();
                Tutorials_Reset();
                any_key_pop_up_triggered = true;
            }
        }
    }
    else if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        next = GS_MAINMENU;
    }
    else if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        // Music
        if (Is_Point_In_Rect(
            (music_panel_left_slider[0] - music_panel_left_slider[2] / 2.0f),
            (music_panel_left_slider[0] + music_panel_left_slider[2] / 2.0f),
            (music_panel_left_slider[1] + music_panel_left_slider[3] / 2.0f),
            (music_panel_left_slider[1] - music_panel_left_slider[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            Adjust_Music(-inc_dec_amount);
        }
        else if (Is_Point_In_Rect(
            (music_panel_right_slider[0] - music_panel_right_slider[2] / 2.0f),
            (music_panel_right_slider[0] + music_panel_right_slider[2] / 2.0f),
            (music_panel_right_slider[1] + music_panel_right_slider[3] / 2.0f),
            (music_panel_right_slider[1] - music_panel_right_slider[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            Adjust_Music(inc_dec_amount);
        }
         
        // Sound
        else if (Is_Point_In_Rect(
            (sound_panel_left_slider[0] - sound_panel_left_slider[2] / 2.0f),
            (sound_panel_left_slider[0] + sound_panel_left_slider[2] / 2.0f),
            (sound_panel_left_slider[1] + sound_panel_left_slider[3] / 2.0f),
            (sound_panel_left_slider[1] - sound_panel_left_slider[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            Adjust_Sound(-inc_dec_amount);
        }
        else if (Is_Point_In_Rect(
            (sound_panel_right_slider[0] - sound_panel_right_slider[2] / 2.0f),
            (sound_panel_right_slider[0] + sound_panel_right_slider[2] / 2.0f),
            (sound_panel_right_slider[1] + sound_panel_right_slider[3] / 2.0f),
            (sound_panel_right_slider[1] - sound_panel_right_slider[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            Adjust_Sound(inc_dec_amount);
        }        

        // Reset Tutorial
        else if (Is_Point_In_Rect(
            (tutorial_button[0] - tutorial_button[2] / 2.0f),
            (tutorial_button[0] + tutorial_button[2] / 2.0f),
            (tutorial_button[1] + tutorial_button[3] / 2.0f),
            (tutorial_button[1] - tutorial_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            Tutorials_Reset();
            any_key_pop_up_triggered = true;
        }        

        // Clear Data
        else if (Is_Point_In_Rect(
            (clear_data_button[0] - clear_data_button[2] / 2.0f),
            (clear_data_button[0] + clear_data_button[2] / 2.0f),
            (clear_data_button[1] + clear_data_button[3] / 2.0f),
            (clear_data_button[1] - clear_data_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            confirmation_pop_up_triggered = true;
            warning_text = CONFIRMATION_CLEAR_DATA_TEXT;
        }

        // To Menu
        else if (Is_Point_In_Rect(
            (exit_button[0] - exit_button[2] / 2.0f),
            (exit_button[0] + exit_button[2] / 2.0f),
            (exit_button[1] + exit_button[3] / 2.0f),
            (exit_button[1] - exit_button[3] / 2.0f),
            mouse_Xf, mouse_Yf))
        {
            next = GS_MAINMENU;
        }
    }

    // Particle system
    Click_Particles_Update();
    Click_Particles_Swap();
}

void Settings_Draw()
{
    f32 world_x_pos{}, world_y_pos{}, world_x_scale{}, world_y_scale{};

    music_text = "Music: ";
    music_text += std::to_string(music_volume);
    sound_text = "Sound: "; 
    sound_text += std::to_string(sound_volume);

    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    // Background drawing
    AEMtx33Scale(&scale, WINDOW_LENGTH, WINDOW_WIDTH);
    AEMtx33Trans(&translate, IMAGE_POS_X, IMAGE_POS_Y);
    AEMtx33Concat(&image_transform, &translate, &scale);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxTextureSet(p_background, 0, 0);

    // Draw image
    AEGfxSetTransform(image_transform.m);
    AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

    // Buttons
    // Music
    Draw_Panel(music_panel[0], music_panel[1], music_panel[2], music_panel[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_smaller_font, music_text.c_str(), 1.f, &font_width, &font_height);
    AEGfxPrint(p_smaller_font, music_text.c_str(), text_position[0].x - font_width / 2.0f, text_position[0].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Draw Music Sliders
    {
        Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, music_panel_left_slider[0], music_panel_left_slider[1], false);
        Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, music_panel_left_slider[2], music_panel_left_slider[3], true);

        AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
        AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
        AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
        AEGfxSetTransform(transform.m);
        AEGfxTextureSet(left_slider, 0, 0);
        AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

        Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, music_panel_right_slider[0], music_panel_right_slider[1], false);
        Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, music_panel_right_slider[2], music_panel_right_slider[3], true);

        AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
        AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
        AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
        AEGfxSetTransform(transform.m);
        AEGfxTextureSet(right_slider, 0, 0);
        AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
    }

    // Sound
    Draw_Panel(sound_panel[0], sound_panel[1], sound_panel[2], sound_panel[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_smaller_font, sound_text.c_str(), 1.f, &font_width, &font_height);
    AEGfxPrint(p_smaller_font, sound_text.c_str(), text_position[1].x - font_width / 2.0f, text_position[1].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Draw Sound Sliders
    {
        Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, sound_panel_left_slider[0], sound_panel_left_slider[1], false);
        Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, sound_panel_left_slider[2], sound_panel_left_slider[3], true);

        AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
        AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
        AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
        AEGfxSetTransform(transform.m);
        AEGfxTextureSet(left_slider, 0, 0);
        AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

        Get_Normalized_To_World_Coordinates(world_x_pos, world_y_pos, sound_panel_right_slider[0], sound_panel_right_slider[1], false);
        Get_Normalized_To_World_Coordinates(world_x_scale, world_y_scale, sound_panel_right_slider[2], sound_panel_right_slider[3], true);

        AEMtx33Scale(&scale, world_x_scale, world_y_scale); // Scaling Factors (Size of Squares)
        AEMtx33Trans(&translate, world_x_pos, world_y_pos); // Sets Position
        AEMtx33Concat(&transform, &translate, &scale); // Puts them into a transform
        AEGfxSetTransform(transform.m);
        AEGfxTextureSet(right_slider, 0, 0);
        AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);
    }

    // Reset Tutorial
    Draw_Button(tutorial_button[0], tutorial_button[1], tutorial_button[2], tutorial_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_smaller_font, tutorial_text, 1.f, &font_width, &font_height);
    AEGfxPrint(p_smaller_font, tutorial_text, text_position[2].x - font_width / 2.0f, text_position[2].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Clear Data
    Draw_Button(clear_data_button[0], clear_data_button[1], clear_data_button[2], clear_data_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_smaller_font, clear_data_text, 1.f, &font_width, &font_height);
    AEGfxPrint(p_smaller_font, clear_data_text, text_position[3].x - font_width / 2.0f, text_position[3].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // To Menu (Exit)
    Draw_Button(exit_button[0], exit_button[1], exit_button[2], exit_button[3],
        button_color[0], button_color[1], button_color[2], button_color[3]);
    AEGfxGetPrintSize(p_smaller_font, menu_text, 1.f, &font_width, &font_height);
    AEGfxPrint(p_smaller_font, menu_text, text_position[4].x - font_width / 2.0f, text_position[4].y - font_height / 2.0f, 1, 1, 1, 1, 1);

    // Game Title
    Draw_Panel(title_border[0], title_border[1], title_border[2], title_border[3],
        title_border_color[0], title_border_color[1], title_border_color[2], title_border_color[3]);
    AEGfxGetPrintSize(p_title_font, TITLE_TEXT, 1.0f, &font_width, &font_height);
    AEGfxPrint(p_title_font, TITLE_TEXT, title_border[0] - font_width / 2.0f, title_border[1] - font_height / 2.0f, 1.0f, 1, 1, 1, 1);

    // Pop Up
    if (any_key_pop_up_triggered) {
        f32 text_offset = 0.05f; // Distance between the 2 lines of text (halfed)
        Draw_Panel(pop_up[0], pop_up[1], pop_up[2], pop_up[3],
            pop_up_color[0], pop_up_color[1], pop_up_color[2], pop_up_color[3]);

        AEGfxGetPrintSize(p_button_font, POP_UP_TEXT1, 1.0f, &font_width, &font_height);
        AEGfxPrint(p_button_font, POP_UP_TEXT1, pop_up[0] - font_width / 2.0f, pop_up[1] - font_height / 2.0f + text_offset, 1.0f,
            pop_up_text_color[0], pop_up_text_color[1], pop_up_text_color[2], pop_up_text_color[3]);

        AEGfxGetPrintSize(p_button_font, POP_UP_TEXT2, 1.0f, &font_width, &font_height);
        AEGfxPrint(p_button_font, POP_UP_TEXT2, pop_up[0] - font_width / 2.0f, pop_up[1] - font_height / 2.0f - text_offset, 1.0f,
            pop_up_text_color[0], pop_up_text_color[1], pop_up_text_color[2], pop_up_text_color[3]);
    }
    else if (confirmation_pop_up_triggered) {
        Draw_Popup( warning_text);
    }

    // Particle system
    Click_Particles_Emitter();
}

void Settings_Free()
{
        
}

void Settings_Unload()
{
    AEGfxTextureUnload(button_default);
    AEGfxTextureUnload(button_hovered);
    AEGfxTextureUnload(button_pressed);
    AEGfxTextureUnload(panel);
    AEGfxTextureUnload(p_background);
    AEGfxTextureUnload(right_slider);
    AEGfxTextureUnload(left_slider);
    
}