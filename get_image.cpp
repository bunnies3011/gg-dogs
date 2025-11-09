#include <bits/stdc++.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

using namespace std;

bool init();

bool loadMedia();

void close();
// Cua so ma chung ta render
SDL_Window *gWindow = NULL;

// Be mat duoc chua boi window
SDL_Surface *gScreenSurface = NULL;

// Hinh anh show tren man hinh
SDL_Surface *gHelloWorld = NULL;

bool init()
{
    // Initialization flag
    bool success = true;
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;

    // Initialize SDL
    // SDL_Init returns 0 on success, negative on error. Check for non-zero to detect failure.
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        // Create Window
        gWindow = SDL_CreateWindow("SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL)
        {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError();
            success = false;
        }
        else
        {
            // Get Window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load splash image
    gHelloWorld = SDL_LoadBMP("02_getting_an_image_on_the_screen/hello_world.bmp");
    if (gHelloWorld == NULL)
    {
        cout << "Unable to load image! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    }
    return success;
}

void close()
{
    // Deallocate surface
    SDL_DestroySurface(gHelloWorld);
    gHelloWorld = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char *args[])
{
    // Start up SDL and create window
    if (!init())
    {
        cout << "Failed to initialize! " << endl;
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            cout << "Failed to load media! " << endl;
        }

        // Hack to get window to stay up
        SDL_Event e;
        bool quit = false;
        while (quit == false)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)
                    quit = true;
            }
            // Apply the image
            SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

            // Update the surface
            SDL_UpdateWindowSurface(gWindow);
        }
    }
    close();
    return 0;
}
