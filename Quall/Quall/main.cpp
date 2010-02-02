/**
 * Glowny plik rozruchowy.
 *
 * @author marcin.kanclerz
 */
#define WIN32_LEAN_AND_MEAN

#include "windows.h"

#include "Application.hpp"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Application app;
    app.go();

    return 0;
}