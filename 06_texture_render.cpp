#include <bits/stdc++.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

using namespace std;

// Load individual image as texture
SDL_Texture *loadTexture(string path);

// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Current displayed texture
SDL_Texture *gTexture = NULL;

bool init()
{
    // Initialize flag
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        cout << "Failed to initialize. SDL Error: " << SDL_GetError() << endl;
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Tutorial", 1280, 720, SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL)
        {
            cout << "Failed to load Window. Get Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, NULL);
            if (gRenderer == NULL)
            {
                cout << "Renderer could not be created! Error: " << SDL_GetError() << endl;
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}

SDL_Texture *loadTexture(string path)
{
    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL)
    {
        cout << "Unable to load image! Error: " << SDL_GetError() << endl;
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Unable to create texture. Error: " << SDL_GetError() << endl;
        }

        // Get rid of old loaded surface
        SDL_DestroySurface(loadedSurface);
    }
    return newTexture;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load PNG texture
    gTexture = loadTexture("src/09_the_viewport/viewport.png");
    if (gTexture == NULL)
    {
        cout << "Failed to load texture image" << endl;
        success = false;
    }
    return success;
}

void close()
{
    // Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

int main(int argc, char *args[])
{
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    if (!init())
    {
        cout << "Failed to initialize" << endl;
    }
    else
    {
        if (!loadMedia())
        {
            cout << "Failed to load Media!" << endl;
        }
        else
        {
            bool quit = false;
            SDL_Event e;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    // User requests quit
                    if (e.type == SDL_EVENT_QUIT)
                    {
                        quit = true;
                    }
                }
                // 09-the-viewport
                //  Clear Screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Top left corner viewport
                SDL_Rect topLeftViewport;
                topLeftViewport.x = 0;
                topLeftViewport.y = 0;
                topLeftViewport.w = SCREEN_WIDTH / 2;
                topLeftViewport.h = SCREEN_HEIGHT / 2;
                SDL_SetRenderViewport(gRenderer, &topLeftViewport);
                // Render texture to screen
                SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);

                // Top right viewport
                SDL_Rect topRightViewport;
                topRightViewport.x = SCREEN_WIDTH / 2;
                topRightViewport.y = 0;
                topRightViewport.w = SCREEN_WIDTH / 2;
                topRightViewport.h = SCREEN_HEIGHT / 2;
                SDL_SetRenderViewport(gRenderer, &topRightViewport);
                // Render texture to screen
                SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);

                // Bottom viewport
                SDL_Rect bottomViewport;
                bottomViewport.x = 0;
                bottomViewport.y = SCREEN_HEIGHT / 2;
                bottomViewport.w = SCREEN_WIDTH;
                bottomViewport.h = SCREEN_HEIGHT / 2;
                SDL_SetRenderViewport(gRenderer, &bottomViewport);
                // Render texture to screen
                SDL_RenderTexture(gRenderer, gTexture, NULL, NULL);
                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}
