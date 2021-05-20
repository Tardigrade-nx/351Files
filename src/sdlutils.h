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

   // Text horizontal alignment
   typedef enum T_TEXT_ALIGN_H {
      T_TEXT_ALIGN_LEFT = 0,
      T_TEXT_ALIGN_RIGHT,
      T_TEXT_ALIGN_CENTER
   } T_TEXT_ALIGN_H;

   // Text vertical alignment
   typedef enum T_TEXT_ALIGN_V {
      T_TEXT_ALIGN_TOP = 0,
      T_TEXT_ALIGN_BOTTOM,
      T_TEXT_ALIGN_MIDDLE
   } T_TEXT_ALIGN_V;

   // Render text on the screen
   void renderText(const std::string &p_text, const int p_x, const int p_y, const SDL_Color &p_fg, const SDL_Color &p_bg, const T_TEXT_ALIGN_H p_textAlignH, const T_TEXT_ALIGN_V p_textAlignV);

}

#endif
