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

   // Horizontal alignment
   typedef enum T_ALIGN_H {
      T_ALIGN_LEFT = 0,
      T_ALIGN_RIGHT,
      T_ALIGN_CENTER
   } T_ALIGN_H;

   // Vertical alignment
   typedef enum T_ALIGN_V {
      T_ALIGN_TOP = 0,
      T_ALIGN_BOTTOM,
      T_ALIGN_MIDDLE
   } T_ALIGN_V;

   // Render a texture on the screen
   void renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y, const T_ALIGN_H p_alignH = T_ALIGN_LEFT, const T_ALIGN_V p_alignV = T_ALIGN_TOP, const SDL_RendererFlip p_flip = SDL_FLIP_NONE, const SDL_Rect *p_clip = NULL);

   // Render text on the screen
   // Return: width of the texture
   int renderText(const std::string &p_text, TTF_Font *p_font, const int p_x, const int p_y, const SDL_Color &p_fg, const SDL_Color &p_bg, const T_ALIGN_H p_alignH = T_ALIGN_LEFT, const T_ALIGN_V p_alignV = T_ALIGN_TOP, const int p_maxWidth = -1, const T_ALIGN_H p_alignHClip = T_ALIGN_LEFT);

   // Render text on the screen, with clip scrolling
   // Return: width of the texture
   int renderTextScrolling(const std::string &p_text, TTF_Font *p_font, const int p_x, const int p_y, const SDL_Color &p_fg, const SDL_Color &p_bg, const T_ALIGN_H p_alignH, const T_ALIGN_V p_alignV, const int p_maxWidth, const int p_clipX);

   // Render text and return the texture
   SDL_Texture *renderText(const std::string &p_text, TTF_Font *p_font, const SDL_Color &p_fg, const SDL_Color &p_bg);

   // Width of one character in monospace font
   int getCharWidthMono(void);

}

#endif
