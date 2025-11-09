#include <bits/stdc++.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

using namespace std;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Load individual image
SDL_Surface *loadSurface(string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The surface contained by the window
SDL_Surface *gScreenSurface = NULL;

// The images that correspond to a keypress
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

// Current displayed image
SDL_Surface *gCurrentSurface = NULL;

bool init()
{
    bool success = true;
    int p_w = 1280;
    int p_h = 720;

    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        // Create Window
        gWindow = SDL_CreateWindow("SDL Tutorial", p_w, p_h, SDL_WINDOW_RESIZABLE);

        if (gWindow == NULL)
        {
            cout << "Window could not be created! " << endl;
            success = false;
        }
        else
        {
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

SDL_Surface *loadSurface(string path)
{
    // Load image at specified path
    SDL_Surface *loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL)
    {
        cout << "Unable to load image. SDL Error: " << SDL_GetError() << endl;
    }
    return loadedSurface;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load default surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("src/04_key_presses/press.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
    {
        cout << "Failed to load image" << endl;
        success = false;
    }

    // Load up surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("src/04_key_presses/up.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
    {
        cout << "Failed to load image" << endl;
        success = false;
    }

    // Load down surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("src/04_key_presses/down.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
    {
        cout << "Failed to load image" << endl;
        success = false;
    }

    // Load left Surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("src/04_key_presses/left.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
    {
        cout << "Failed to load image" << endl;
        success = false;
    }

    // Load right surface
    gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("src/04_key_presses/right.bmp");
    if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
    {
        cout << "Failed to load image " << endl;
        success = false;
    }
    return success;
}

void close()
{
    // Deallocate surface
    SDL_DestroySurface(gCurrentSurface);
    gCurrentSurface = NULL;

    // Destroy Window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char *args[])
{
    if (!init())
    {
        cout << "Failed to initialize, SDL Error: " << SDL_GetError() << endl;
    }
    else
    {
        if (!loadMedia())
        {
            cout << "Failed to load Media" << endl;
        }
        SDL_Event e;
        bool quit = false;
        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                // User requests quit
                if (e.type == SDL_EVENT_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_EVENT_KEY_DOWN)
                {
                    switch (e.key.key)
                    {
                    case SDLK_UP:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
                        break;

                    case SDLK_DOWN:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
                        break;

                    case SDLK_LEFT:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
                        break;

                    case SDLK_RIGHT:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
                        break;

                    default:
                        gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
                        break;
                    }
                }
            }
            // Apply the current image
            SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

            // Update the surface
            SDL_UpdateWindowSurface(gWindow);
        }
    }
    close();
    return 0;
}