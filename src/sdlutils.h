#ifndef _SDLUTILS_H_
#define _SDLUTILS_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "def.h"

namespace SDLUtils
{

   // Init SDL
   bool init();

   // Close SDL
   void close();

   // Load a texture
   SDL_Texture* loadTexture(const std::string &p_path);

   // Load a TTF font
   TTF_Font *loadFont(const std::string &p_path, const int p_size);

   // Render a texture on the screen
   void renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y);

   // Render a texture at the center of the screen
   void renderTexture(SDL_Texture * p_texture);

   // Render text and return the resulting texture
   SDL_Texture* renderText(const std::string &p_text, const SDL_Color &p_fg, const SDL_Color &p_bg);

}

#endif
