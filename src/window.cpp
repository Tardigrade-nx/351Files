#include "window.h"
#include "def.h"
#include "sdlutils.h"

// Destructor
IWindow::~IWindow(void)
{
   // Remove last window
   g_windows.pop_back();
   // Clean up texture
   if (m_titleTexture != NULL)
   {
      SDL_DestroyTexture(m_titleTexture);
      m_titleTexture = NULL;
   }
}

//------------------------------------------------------------------------------

// Constructor
IWindow::IWindow(const bool p_fullscreen, const std::string &p_title) :
   m_fullscreen(p_fullscreen),
   m_title(""),
   m_titleTexture(NULL)
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
   if (m_titleTexture != NULL)
   {
      SDL_DestroyTexture(m_titleTexture);
      m_titleTexture = NULL;
   }
   m_titleTexture = SDLUtils::renderText(p_title, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG});
}

//------------------------------------------------------------------------------

// Execute window and return result
int IWindow::execute(void)
{
   // Main loop
   SDL_Event event;
   while (1)
   {
      // Handle events
      while (SDL_PollEvent(&event) != 0 )
      {
         // Quit
         if (event.type == SDL_QUIT)
            return 0;
      }
      // Render all windows
      renderAll();
      SDL_RenderPresent(g_renderer);
   }
   return 0;
}

//------------------------------------------------------------------------------

// Render all windows
void IWindow::renderAll(void)
{
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
