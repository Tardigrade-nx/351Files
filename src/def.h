#ifndef _DEF_H_
#define _DEF_H_

#include <map>
#include <SDL.h>

#define INHIBIT(X) X
//~ #define INHIBIT(X) /* X */

#define APP_NAME "351Files"

#if defined(DEVICE_RG351P)
   #define SCREEN_WIDTH    480
   #define SCREEN_HEIGHT   320
#elif defined(DEVICE_RG351V)
   #define SCREEN_WIDTH    640
   #define SCREEN_HEIGHT   480
#else
   #define SCREEN_WIDTH    640
   #define SCREEN_HEIGHT   480
#endif

// Texture ID
typedef enum {
   UNDEFINED = 0
} T_TEXTURE_ID;

// Globals
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern SDL_Joystick* g_joystick;
extern std::map<T_TEXTURE_ID, SDL_Texture*> g_textures;

#endif
