#include <iostream>
#include <SDL_image.h>
#include "sdlutils.h"

//------------------------------------------------------------------------------

// Init SDL
bool SDLUtils::init()
{
   INHIBIT(std::cout << "SDLUtils::Init()" << std::endl;)

   // Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
   {
      std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
   }

   // Intialize SDL_image
   int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
   if ((IMG_Init(flags) & flags) != flags)
   {
      std::cerr << "SDL_image could not initialize! IMG_GetError: " << IMG_GetError() << std::endl;
      return false;
   }

   // Initialize joystick
   INHIBIT(std::cout << "SDL_NumJoysticks: '" << SDL_NumJoysticks() << "'" << std::endl;)
   if (SDL_NumJoysticks() >= 1)
   {
      g_joystick = SDL_JoystickOpen(0);
      if (g_joystick == NULL)
      {
         std::cerr << "Unable to open joystick." << std::endl;
         return false;
      }
      INHIBIT(std::cout << "SDL_JoystickOpen OK" << std::endl;)
   }

   // Create window
   #if defined(DEVICE_RG351P) || defined(DEVICE_RG351V)
   g_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
   #else
   g_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
   #endif
   if (g_window == NULL)
   {
      std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
   }

   // Create renderer
   g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if (g_renderer == NULL)
   {
      std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
   }

   // Set renderer's alpha mode
   //~ SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

   return true;
}

//------------------------------------------------------------------------------

// Close SDL
void SDLUtils::close()
{
   INHIBIT(std::cout << "SDLUtils::close()" << std::endl;)

   // Textures
   for (std::map<T_TEXTURE_ID, SDL_Texture*>::iterator it = g_textures.begin(); it != g_textures.end(); ++it)
      SDL_DestroyTexture(it->second);

   // Renderer
   if (g_renderer != NULL)
   {
      SDL_DestroyRenderer(g_renderer);
      g_renderer = NULL;
   }

   // Window
   if (g_window != NULL)
   {
      SDL_DestroyWindow(g_window);
      g_window = NULL;
   }

   // Joystick
   if (g_joystick != NULL)
   {
      SDL_JoystickClose(g_joystick);
      g_joystick = NULL;
   }

   // SDL_image
   IMG_Quit();

   // SDL
   SDL_Quit();
}

//------------------------------------------------------------------------------

// Load a texture
SDL_Texture* SDLUtils::loadTexture(const std::string &p_path)
{
   INHIBIT(std::cout << "SDLUtils::loadTexture(" << p_path << ")" << std::endl;)
   SDL_Texture* texture = NULL;
   SDL_Surface* surface = IMG_Load(p_path.c_str());
   if (surface == NULL)
   {
      std::cerr << "Unable to load image '" << p_path << "'! SDL_Error: " << SDL_GetError() << std::endl;
      return NULL;
   }
   texture = SDL_CreateTextureFromSurface(g_renderer, surface);
   if (texture == NULL)
   {
      std::cerr << "Unable to create texture from '" << p_path << "'! SDL_Error: " << SDL_GetError() << std::endl;
      return NULL;
   }
   SDL_FreeSurface(surface);
   return texture;
}

//------------------------------------------------------------------------------

// Render a texture
void SDLUtils::renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y)
{
   SDL_Rect destRect;
   destRect.x = p_x;
   destRect.y = p_y;
   SDL_QueryTexture(p_texture, NULL, NULL, &destRect.w, &destRect.h);
   SDL_RenderCopy(g_renderer, p_texture, NULL, &destRect);
}

//------------------------------------------------------------------------------

// Render a texture at the center off the screen
void SDLUtils::renderTexture(SDL_Texture * p_texture)
{
   SDL_Rect destRect;
   SDL_QueryTexture(p_texture, NULL, NULL, &destRect.w, &destRect.h);
   destRect.x = (SCREEN_WIDTH - destRect.w) / 2;
   destRect.y = (SCREEN_HEIGHT - destRect.h) / 2;
   SDL_RenderCopy(g_renderer, p_texture, NULL, &destRect);
}
