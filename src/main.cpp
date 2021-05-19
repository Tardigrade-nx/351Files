#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "def.h"
#include "sdlutils.h"
#include "mainWindow.h"

//------------------------------------------------------------------------------

// Globals
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
SDL_Joystick* g_joystick = NULL;
TTF_Font *g_font = NULL;
std::vector<IWindow *> g_windows;
bool IWindow::g_hasChanged = true;

//------------------------------------------------------------------------------

int main(int argc, char* args[])
{
   // Init SDL
   if (! SDLUtils::init())
   {
      SDLUtils::close();
      return 1;
   }

   // Load textures
   // ...

   // Load font
   g_font = SDLUtils::loadFont(std::string(RES_PATH) + "/" + FONT_NAME, FONT_SIZE);
   if (g_font == NULL)
      return 1;

   // Execute main window
   {
      MainWindow mainWindow(START_PATH);
      mainWindow.execute();
   }

   // Clean up font
   TTF_CloseFont(g_font);
   g_font = NULL;

   // Quit SDL
   SDLUtils::close();

   return 0;
}
