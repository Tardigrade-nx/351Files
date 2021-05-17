#ifndef _DEF_H_
#define _DEF_H_

#include <map>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

#define INHIBIT(X) X
//~ #define INHIBIT(X) /* X */

#define APP_NAME "351Files"

#if defined(DEVICE_RG351P)
   #define SCREEN_WIDTH    480
   #define SCREEN_HEIGHT   320
   #define START_PATH      "/"
   #define RES_PATH        "./res"
   #define FONT_NAME       "NotoSans-Regular.ttf"
   #define FONT_SIZE       14
   #define LINE_HEIGHT     32
#elif defined(DEVICE_RG351V)
   #define SCREEN_WIDTH    640
   #define SCREEN_HEIGHT   480
   #define START_PATH      "/"
   #define RES_PATH        "./res"
   #define FONT_NAME       "NotoSans-Regular.ttf"
   #define FONT_SIZE       14
   #define LINE_HEIGHT     32
#else
   #define SCREEN_WIDTH    640
   #define SCREEN_HEIGHT   480
   #define START_PATH      getenv("PWD")
   #define RES_PATH        "./res"
   #define FONT_NAME       "NotoSans-Regular.ttf"
   #define FONT_SIZE       14
   #define LINE_HEIGHT     32
#endif

// Colors
#define COLOR_TITLE_BG         68,  68,  68
#define COLOR_TEXT_NORMAL     255, 255, 255
#define COLOR_BODY_BG          45,  45,  45

// Margins
#define MARGIN_X           8

// Texture ID
typedef enum {
   UNDEFINED = 0
} T_TEXTURE_ID;

// Globals
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern SDL_Joystick* g_joystick;
extern std::map<T_TEXTURE_ID, SDL_Texture*> g_textures;
extern TTF_Font *g_font;
class IWindow;
extern std::vector<IWindow *> g_windows;

#endif
