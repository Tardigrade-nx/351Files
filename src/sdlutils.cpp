#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
   int flags = IMG_INIT_JPG | IMG_INIT_PNG;
   if ((IMG_Init(flags) & flags) != flags)
   {
      std::cerr << "SDL_image could not initialize! IMG_GetError: " << IMG_GetError() << std::endl;
      return false;
   }

   // Initialize SDL_ttf
   if (TTF_Init() == -1)
   {
      std::cerr << "SDL_ttf could not initialize! TTF_GetError: " << TTF_GetError() << std::endl;
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
   #if FULLSCREEN == 1
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
   #if HARDWARE_ACCELERATION == 1
   g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
   #else
   g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_SOFTWARE);
   #endif
   if (g_renderer == NULL)
   {
      std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------

// Close SDL
void SDLUtils::close()
{
   INHIBIT(std::cout << "SDLUtils::close()" << std::endl;)

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

   // SDL_ttf
   TTF_Quit();

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
      std::cerr << "Unable to create texture from '" << p_path << "'! SDL_Error: " << SDL_GetError() << std::endl;
   SDL_FreeSurface(surface);
   return texture;
}

//------------------------------------------------------------------------------

// Load a TTF font
TTF_Font *SDLUtils::loadFont(const std::string &p_path, const int p_size)
{
   INHIBIT(std::cout << "SDLutils::loadFont(" << p_path << ", " << p_size << ")" << std::endl;)
   TTF_Font *font = TTF_OpenFont(p_path.c_str(), p_size);
   if (font == NULL)
      std::cerr << "Unable to load font " << p_path << ": " << SDL_GetError() << std::endl;
   return font;
}

//------------------------------------------------------------------------------

// Render a texture
void SDLUtils::renderTexture(SDL_Texture * p_texture, const int p_x, const int p_y, const T_ALIGN_H p_alignH, const T_ALIGN_V p_alignV, const SDL_RendererFlip p_flip, const SDL_Rect *p_clip)
{
   // Destination rectangle
   SDL_Rect destRect;
   destRect.x = p_x;
   destRect.y = p_y;
   if (p_clip == NULL)
   {
      SDL_QueryTexture(p_texture, NULL, NULL, &destRect.w, &destRect.h);
   }
   else
   {
      destRect.w = p_clip->w;
      destRect.h = p_clip->h;
   }
   switch (p_alignH)
   {
      case T_ALIGN_RIGHT:
         destRect.x -= destRect.w;
         break;
      case T_ALIGN_CENTER:
         destRect.x -= destRect.w / 2;
         break;
      default:
         break;
   }
   switch (p_alignV)
   {
      case T_ALIGN_BOTTOM:
         destRect.y -= destRect.h;
         break;
      case T_ALIGN_MIDDLE:
         destRect.y -= destRect.h / 2;
         break;
      default:
         break;
   }
   // Render copy
   if (p_flip == SDL_FLIP_NONE)
      SDL_RenderCopy(g_renderer, p_texture, p_clip, &destRect);
   else
      SDL_RenderCopyEx(g_renderer, p_texture, p_clip, &destRect, 0.0, NULL, p_flip);
}

//------------------------------------------------------------------------------

// Render text on the screen
int SDLUtils::renderText(const std::string &p_text, TTF_Font *p_font, const int p_x, const int p_y, const SDL_Color &p_fg, const SDL_Color &p_bg, const T_ALIGN_H p_alignH, const T_ALIGN_V p_alignV, const int p_maxWidth, const T_ALIGN_H p_alignHClip)
{
   SDL_Texture *texture = renderText(p_text, p_font, p_fg, p_bg);
   if (texture == NULL)
   {
      std::cerr << "Unable to create texture from surface. SDL_Error: " << SDL_GetError() << std::endl;
      return 0;
   }
   int w = 0, h = 0;
   SDL_QueryTexture(texture, NULL, NULL, &w, &h);
   // Check size
   if (p_maxWidth != -1 && w > p_maxWidth)
   {
      // Clip texture width
      SDL_Rect rect = { 0, 0, p_maxWidth, h};
      if (p_alignHClip == T_ALIGN_RIGHT)
         rect.x = w - rect.w;
      renderTexture(texture, p_x, p_y, p_alignH, p_alignV, SDL_FLIP_NONE, &rect);
   }
   else
   {
      renderTexture(texture, p_x, p_y, p_alignH, p_alignV);
   }
   // Free texture
   SDL_DestroyTexture(texture);
   return w;
}

//------------------------------------------------------------------------------

SDL_Texture *SDLUtils::renderText(const std::string &p_text, TTF_Font *p_font, const SDL_Color &p_fg, const SDL_Color &p_bg)
{
   // Create surface
   SDL_Surface *surface = TTF_RenderUTF8_Shaded(p_font, p_text.c_str(), p_fg, p_bg);
   if (surface == NULL)
   {
      std::cerr << "TTF_RenderUTF8_Shaded: " << SDL_GetError() << std::endl;
      return NULL;
   }
   // Create texture from surface
   SDL_Texture *texture = SDL_CreateTextureFromSurface(g_renderer, surface);
   SDL_FreeSurface(surface);
   if (texture == NULL)
   {
      std::cerr << "Unable to create texture from surface. SDL_Error: " << SDL_GetError() << std::endl;
      return NULL;
   }
   return texture;
}

//------------------------------------------------------------------------------

// Render text on the screen, with clip scrolling
// Return: width of the texture
int SDLUtils::renderTextScrolling(const std::string &p_text, TTF_Font *p_font, const int p_x, const int p_y, const SDL_Color &p_fg, const SDL_Color &p_bg, const T_ALIGN_H p_alignH, const T_ALIGN_V p_alignV, const int p_maxWidth, const int p_clipX)
{
   SDL_Texture *texture = renderText(p_text, p_font, p_fg, p_bg);
   if (texture == NULL)
   {
      std::cerr << "Unable to create texture from surface. SDL_Error: " << SDL_GetError() << std::endl;
      return 0;
   }
   int w = 0, h = 0;
   SDL_QueryTexture(texture, NULL, NULL, &w, &h);
   // Check size
   if (w > p_maxWidth)
   {
      // Clip texture width
      SDL_Rect rect = { p_clipX, 0, p_maxWidth, h};
      if (p_clipX + p_maxWidth > w)
         rect.w = w - p_clipX;
      renderTexture(texture, p_x, p_y, p_alignH, p_alignV, SDL_FLIP_NONE, &rect);
   }
   else
   {
      renderTexture(texture, p_x, p_y, p_alignH, p_alignV);
   }
   // Free texture
   SDL_DestroyTexture(texture);
   return w;
}

//------------------------------------------------------------------------------

// Width of one character in monospace font
int SDLUtils::getCharWidthMono(void)
{
   int w(0);
   SDL_Texture *tex = SDLUtils::renderText("a", g_fontMono, {COLOR_TEXT_NORMAL}, {COLOR_BODY_BG});
   SDL_QueryTexture(tex, NULL, NULL, &w, NULL);
   SDL_DestroyTexture(tex);
   return w;
}
