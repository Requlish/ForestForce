/******************************************************************************/
/*!
\file		Main.cpp
\author 	Ho Zhen Hao
\par    	Email: zhenhao.ho@digipen.edu
\par    	Project: Forest Force
\date   	April 06, 2024
\brief		This file contains the core game loop.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "pch.hpp"
#include "System.hpp"
#include "GameStateManager.hpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Enable run-time memory check for debug builds.
    #if defined(DEBUG) | defined(_DEBUG)
    	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif

    /// System Init Code
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // Using custom window procedure (also determines console spawning)
    AESysInit(hInstance, nCmdShow, WINDOW_LENGTH, WINDOW_WIDTH, 0, 60, true, NULL);
    System_Initialize();
    /// System Init End

    GSM_Initialize(GS_SPLASHSCREEN);
    while (current != GS_QUIT)
    {
        if (current == GS_RESTART)
        {
            current = previous;
            next = previous;
        }
        else
        {
            GSM_Update();
            fp_load();
        }

        fp_initialize();

        while (current == next)
        {
            if (0 == AESysDoesWindowExist()) next = GS_QUIT;
            AESysFrameStart();
            Update_Delta_Time(delta_time);
            fp_update();
            fp_draw();
            AESysFrameEnd();
        }

        fp_free();

        if (next != GS_RESTART) {
            fp_unload();
        }
        previous = current;
        current = next;

    }
    System_Exit();
}