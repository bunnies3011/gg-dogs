#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

bool init();

bool loadMedia();

void close();

class LTexture
{
public:
    // Initialize variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(string path);

    // Deallocates texture
    void free();

    // Renders texture at given point
    void render(float x, float y, SDL_FRect *clip = NULL);

    // Get image dimensions
    float getWidth();
    float getHeight();

private:
    // The actual hardware texture
    SDL_Texture *mTexture;

    // Image dimensions
    float mWidth;
    float mHeight;
};

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

// Scene sprites
SDL_FRect gSpriteClips[4];
LTexture gSpriteSheetTexture;

void LTexture ::render(float x, float y, SDL_FRect *clip)
{
    // Set rendering space and render to screen
    SDL_FRect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderTexture(gRenderer, mTexture, clip, &renderQuad);
}

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

bool LTexture ::loadFromFile(string path)
{
    // Get rid og preexisting texture
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
        SDL_SetSurfaceColorKey(loadedSurface, true, SDL_MapSurfaceRGB(loadedSurface, 0xFF, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Unable to create texture! Error: " << SDL_GetError() << endl;
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_DestroySurface(loadedSurface);
    }
    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture ::free()
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

float LTexture ::getWidth()
{
    return mWidth;
}

float LTexture ::getHeight()
{
    return mHeight;
}

bool loadMedia()
{
    bool success = true;
    if (!gSpriteSheetTexture.loadFromFile("src/11_clip_rendering_and_sprite_sheets/dots.png"))
    {
        cout << "Failed to load sprite sheet texture" << endl;
        success = false;
    }
    else
    {
        // Set top left sprite
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 100;
        gSpriteClips[0].h = 100;

        // Set top right sprite
        gSpriteClips[1].x = 100;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 100;
        gSpriteClips[1].h = 100;

        // Set bottom left sprite
        gSpriteClips[2].x = 0;
        gSpriteClips[2].y = 100;
        gSpriteClips[2].w = 100;
        gSpriteClips[2].h = 100;

        // Set bottom right sprite
        gSpriteClips[3].x = 100;
        gSpriteClips[3].y = 100;
        gSpriteClips[3].w = 100;
        gSpriteClips[3].h = 100;
    }
    return success;
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
            cout << "Failed to load Window" << endl;
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, NULL);
            if (gRenderer == NULL)
            {
                cout << "Failed to load Renderer " << endl;
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

void close()
{
    gSpriteSheetTexture.free();

    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gWindow = NULL;
    gRenderer = NULL;

    SDL_Quit();
}

int main(int argc, char *args[])
{
    int SCREEN_WIDTH = 1280;
    int SCREEN_HEIGHT = 720;
    if (!init())
    {
        cout << "Failed to initialize " << endl;
    }
    else
    {
        if (!loadMedia())
        {

            cout << "Failed to load Media " << endl;
        }
        else
        {
            bool quit = false;
            SDL_Event e;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_EVENT_QUIT)
                    {
                        quit = true;
                    }
                }
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render top left sprite
                gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);

                // Render top right sprite
                gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

                // Render bottom left sprite
                gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

                // Render bottom right sprite
                gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}