#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

bool init();

bool loadMedia();

void close();

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

class LTexture
{
public:
    // Initializes variables;
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(string path);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Renders texture at given point
    void render(float x, float y, SDL_FRect *clip = NULL);

    // Get image dimensions
    float getWidth();
    float getHeight();

private:
    SDL_Texture *mTexture;

    float mWidth;
    float mHeight;
};

LTexture gModulatedTexture;

LTexture::LTexture()
{
    // Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
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

void LTexture ::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

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

float LTexture ::getWidth()
{
    return mWidth;
}

float LTexture ::getHeight()
{
    return mHeight;
}

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) == false)
    {
        cout << "Failed to initialize " << endl;
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL_Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (gWindow == NULL)
        {
            cout << "Window could not be created. Error " << SDL_GetError() << endl;
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, NULL);
            if (gRenderer == NULL)
            {
                cout << "Could not render. Error: " << SDL_GetError() << endl;
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

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load texture
    if (!gModulatedTexture.loadFromFile("src/12_color_modulation/colors.png"))
    {
        printf("Failed to load colors texture!\n");
        success = false;
    }

    return success;
}

void close()
{
    gModulatedTexture.free();

    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);

    SDL_Quit();
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
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;
            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_EVENT_QUIT)
                    {
                        quit = true;
                    }
                    else if (e.type == SDL_EVENT_KEY_DOWN)
                    {
                        switch (e.key.key)
                        {
                        case SDLK_Q:
                            r += 32;
                            break;

                        case SDLK_W:
                            g += 32;
                            break;

                        case SDLK_E:
                            b += 32;
                            break;

                        case SDLK_S:
                            g -= 32;
                            break;

                        case SDLK_D:
                            b -= 32;
                            break;
                        }
                    }
                }
                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                gModulatedTexture.setColor(r, g, b);
                gModulatedTexture.render(0, 0);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
    close();
    return 0;
}