#include "window.h"
#include "def.h"
#include "sdlutils.h"
#include <iostream>

// Destructor
IWindow::~IWindow(void)
{
   // Remove last window
   g_windows.pop_back();
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Constructor
IWindow::IWindow(const bool p_fullscreen, const std::string &p_title) :
   m_fullscreen(p_fullscreen),
   m_title(""),
   m_highlightedLine(0)
{
   // Add window to the list
   g_windows.push_back(this);
   // Set title
   setTitle(p_title);
}

//------------------------------------------------------------------------------

// Is window full screen?
bool IWindow::isFullScreen(void) const
{
   return m_fullscreen;
}

//------------------------------------------------------------------------------

// Set title
void IWindow::setTitle(const std::string &p_title)
{
   m_title = p_title;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Execute window and return result
int IWindow::execute(void)
{
   // Main loop
   Uint32 l_time(0);
   SDL_Event event;
   while (1)
   {
      l_time = SDL_GetTicks();

      // Handle events
      while (SDL_PollEvent(&event) != 0 )
      {
         // Quit
         if (event.type == SDL_QUIT)
            return 0;
      }

      // Render windows if necessary
      if (g_hasChanged)
      {
         renderAll();
         SDL_RenderPresent(g_renderer);
         g_hasChanged = false;
      }

      // Cap the framerate
      l_time = MS_PER_FRAME - (SDL_GetTicks() - l_time);
      if (l_time <= MS_PER_FRAME)
         SDL_Delay(l_time);
   }
   return 0;
}

//------------------------------------------------------------------------------

// Render all windows
void IWindow::renderAll(void)
{
   std::cout << "renderAll()\n";
   if (g_windows.empty())
      return;
   // First window to draw is the last that is fullscreen
   int ind = g_windows.size() - 1;
   while (ind > 0 && !g_windows[ind]->isFullScreen())
      --ind;
   // Draw windows
   for (std::vector<IWindow *>::iterator it = g_windows.begin() + ind; it != g_windows.end(); ++it)
      (*it)->render(it + 1 == g_windows.end());
}
