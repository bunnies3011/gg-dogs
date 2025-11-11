#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

class LTexture
{
public:
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loadds image at specified path
    bool loadFromFile(string path);

    // Deallocates texture
    void free();

    // Renders texture at given point.
    void render(float x, float y);

    // Gets image dimensions
    float getWidth();
    float getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;

    // Image dimensions
    float mWidth;
    float mHeight;
};
// The window we'll be rendering to
SDL_Window *gWindow = NULL;

// The window renderer
SDL_Renderer *gRenderer = NULL;

// Scene textures
LTexture gFooTexture;
LTexture gBackgroundTexture;

LTexture::LTexture()
{
    // Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    // Deallocate
    free();
}

bool LTexture::loadFromFile(string path)
{
    // Get rid of preexisting texture
    free();

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
        // Uint32 colorKey = SDL_MapSurfaceRGB(loadedSurface, 0x00, 0xFF, 0xFF);
        SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0x00, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Unable to create texture! Error:  " << SDL_GetError() << endl;
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_DestroySurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    // Free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture ::render(float x, float y)
{
    // Set rendering space and render to screen
    SDL_FRect renderQuad = {x, y, mWidth, mHeight};
    SDL_RenderTexture(gRenderer, mTexture, NULL, &renderQuad);
}

float LTexture::getWidth()
{
    return mWidth;
}

float LTexture::getHeight()
{
    return mHeight;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load Foo texture
    if (!gFooTexture.loadFromFile("src/10_color_keying/foo.png"))
    {
        cout << "Failed to load Foo" << endl;
        success = false;
    }

    // Load background texture
    if (!gBackgroundTexture.loadFromFile("src/10_color_keying/background.png"))
    {
        cout << "Failed to load background texture" << endl;
        success = false;
    }
    return success;
}

void close()
{
    // Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        cout << "Failed to initialize" << endl;
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Tutorial", 1280, 720, SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL)
        {
            cout << "Window could not be created! Error: " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, NULL);
            if (gRenderer == NULL)
            {
                cout << "Renderer could not be created! " << endl;
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}

int main(int argc, char *args[])
{
    if (!init())
    {
        cout << "Failed to initialize " << endl;
    }
    else
    {
        if (!loadMedia())
        {
            cout << "Failed to load Media" << endl;
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            while (!quit)
            {
                // Handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    // User request quit
                    if (e.type == SDL_EVENT_QUIT)
                    {
                        quit = true;
                    }
                }
                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render background texture to screen
                gBackgroundTexture.render(0, 0);

                // Render Foo to the screen
                gFooTexture.render(240, 190);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}