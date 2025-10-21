/******************************************************************************/
/*!
\file		Splash_Screen.cpp
\author 	Titus Kwong Wen Shuen
\par    	Email: tituswenshuen.kwong@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		Contains functions that implements splash screen.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "Splash_Screen.hpp"

static f32 font_width{}, font_height{};
static AEGfxTexture* p_digipen{ nullptr };
static AEMtx33 image_transform{}, scale{}, translate{};

static const int IMAGE_LENGTH = 1525;
static const int IMAGE_WIDTH = 445;

static const char* SPLASH_TEXT = "AVATAR";
static const f32 SPLASH_TEXT_X = 0;
static const f32 SPLASH_TEXT_Y = 0;

static f64 counter{};
static f32 logo_alpha{};
static f32 avatar_alpha = 255.0f;

bool render_text = true;

/**************************************************************************/
/*!
    Loads textures
    Calculates scaling factors based on window dimensions
*/
/**************************************************************************/
void Splash_Screen_Load()
{
    // Splash screen
    p_digipen = AEGfxTextureLoad("Assets/Menu/digipen_logo.png");

    f32 scale_x = static_cast<f32>(AEGfxGetWindowWidth()) / IMAGE_LENGTH;
    f32 scale_y = static_cast<f32>(AEGfxGetWindowHeight()) / IMAGE_WIDTH;

    AEMtx33Scale(&scale, scale_x, scale_y);
    AEMtx33Trans(&translate, 0, 0);
    AEMtx33Concat(&image_transform, &translate, &scale);
}

/**************************************************************************/
/*!
    NA
*/
/**************************************************************************/
void Splash_Screen_Initialize()
{

}

/**************************************************************************/
/*!
    Updates alpha value of logo
    Transitions to main menu after certain time or user input
*/
/**************************************************************************/
void Splash_Screen_Update()
{
    counter += delta_time;
    logo_alpha = static_cast<f32>(counter < 2.0f ? logo_alpha + 255.0f / (2.0f / delta_time) : logo_alpha - 255.0f / (2.0f / delta_time));

    if (AEInputCheckTriggered(AEVK_ESCAPE) || AEInputCheckTriggered(AEVK_LBUTTON) || 
        AEInputCheckTriggered(AEVK_SPACE) || AEInputCheckTriggered(AEVK_RBUTTON) || 
        AEInputCheckTriggered(AEVK_RETURN) || counter > 8.0f) {
        next = GS_MAINMENU;
    }
}

/**************************************************************************/
/*!
    Renders logo, then displays team name
*/
/**************************************************************************/
void Splash_Screen_Draw()
{
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // DigiPen logo
    AEMtx33Scale(&scale, IMAGE_LENGTH, IMAGE_WIDTH);
    AEMtx33Trans(&translate, 0, 0);
    AEMtx33Concat(&image_transform, &translate, &scale);

    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(logo_alpha / 255.0f); // Set alpha value for logo
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxTextureSet(p_digipen, 0, 0);

    // Draw mesh
    AEGfxSetTransform(image_transform.m);
    AEGfxMeshDraw(p_mesh, AE_GFX_MDM_TRIANGLES);

    if (counter > 4.0f) {
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(avatar_alpha / 255.0f);

        // 'AVATAR' Text
        // Calculate alpha value for the text
        f32 target_alpha = 255.0f;
        f64 fade_time = 2.0; 
        f64 fade_start = 4.0; 
        f64 alpha_prog;
        if (counter > fade_start) {
            f64 elapsed_time = counter - fade_start;
            if (elapsed_time < fade_time) {
                alpha_prog = elapsed_time / fade_time;
                target_alpha *= static_cast<f32>(alpha_prog);
            }
            else if (elapsed_time < fade_time * 2.0f) { // Fade out after fade in
                alpha_prog = 1.0f - (elapsed_time - fade_time) / fade_time;
                target_alpha *= static_cast<f32>(alpha_prog);
            }
            else {
                render_text = false;
            }
        }

        // Render the text with the calculated alpha value
        if (render_text) {
            AEGfxGetPrintSize(p_splash_font, SPLASH_TEXT, 1.0f, &font_width, &font_height);
            AEGfxPrint(p_splash_font, SPLASH_TEXT, SPLASH_TEXT_X - font_width / 2.0f, SPLASH_TEXT_Y - font_height / 2.0f, 1.0f, 0, 1, 0, target_alpha / 255.0f);
        }
    }
}

/**************************************************************************/
/*!
    NA
*/
/**************************************************************************/
void Splash_Screen_Free()
{

}

/**************************************************************************/
/*!
    Unloads textures
*/
/**************************************************************************/
void Splash_Screen_Unload()
{
    AEGfxTextureUnload(p_digipen);
}