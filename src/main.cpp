#include <string>
#include <SDL.h>
#include "def.h"
#include "sdlutils.h"

//------------------------------------------------------------------------------

// Globals
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
SDL_Joystick* g_joystick = NULL;
std::map<T_TEXTURE_ID, SDL_Texture*> g_textures;

//------------------------------------------------------------------------------

int main(int argc, char* args[])
{
   // Init SDL
   if (! SDLUtils::init())
   {
      SDLUtils::close();
      return 1;
   }

   // Main loop
   SDL_Event event;
   bool loop = true;
   while (loop)
   {
      // Handle events
      while (SDL_PollEvent(&event) != 0 )
      {
         // Quit
         if (event.type == SDL_QUIT)
         {
            loop = false;
            break;
         }
      }
      // Update
      // ...
      // Render
      // ...
      SDL_RenderPresent(g_renderer);
   }

   // Clean up and quit
   SDLUtils::close();
   return 0;
}
