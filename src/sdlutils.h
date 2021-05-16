#ifndef _SDLUTILS_H_
#define _SDLUTILS_H_

#include <SDL.h>
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

   // Render a texture
   void renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y);

   // Render a texture at the center of the screen
   void renderTexture(SDL_Texture * p_texture);

}

#endif
