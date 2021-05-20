#include "window.h"
#include "def.h"
#include "sdlutils.h"
#include <iostream>

// Buttons with timer
enum {
   BUTTON_UP = 0,
   BUTTON_PAGEUP,
   BUTTON_DOWN,
   BUTTON_PAGEDOWN
};

//------------------------------------------------------------------------------

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
   m_highlightedLine(0),
   m_cursorLoop(false),
   m_nbItems(0),
   m_nbVisibleItems(0),
   m_timer(0),
   m_lastPressed(-1)
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
         // Move cursor
         if (BUTTON_PRESSED_UP)
         {
            moveCursorUp(1, m_cursorLoop);
            m_lastPressed = BUTTON_UP;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
         if (BUTTON_PRESSED_PAGEUP)
         {
            moveCursorUp(m_nbVisibleItems, m_cursorLoop);
            m_lastPressed = BUTTON_PAGEUP;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
         if (BUTTON_PRESSED_DOWN)
         {
            moveCursorDown(1, m_cursorLoop);
            m_lastPressed = BUTTON_DOWN;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
         if (BUTTON_PRESSED_PAGEDOWN)
         {
            moveCursorDown(m_nbVisibleItems, m_cursorLoop);
            m_lastPressed = BUTTON_PAGEDOWN;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
      }

      // Handle key holds
      if (BUTTON_HELD_UP)
      {
         if (m_lastPressed == BUTTON_UP && m_timer > 0 && --m_timer == 0)
         {
            moveCursorUp(1, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else if (BUTTON_HELD_PAGEUP)
      {
         if (m_lastPressed == BUTTON_PAGEUP && m_timer > 0 && --m_timer == 0)
         {
            moveCursorUp(m_nbVisibleItems, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else if (BUTTON_HELD_DOWN)
      {
         if (m_lastPressed == BUTTON_DOWN && m_timer > 0 && --m_timer == 0)
         {
            moveCursorDown(1, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else if (BUTTON_HELD_PAGEDOWN)
      {
         if (m_lastPressed == BUTTON_PAGEDOWN && m_timer > 0 && --m_timer == 0)
         {
            moveCursorDown(m_nbVisibleItems, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else
      {
         m_lastPressed = -1;
         m_timer = 0;
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
   INHIBIT(std::cout << "renderAll - " << g_windows.size() << " windows\n";)
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

//------------------------------------------------------------------------------

// Move cursor up
void IWindow::moveCursorUp(const unsigned int p_step, bool p_loop)
{
   if (m_nbItems == 0)
      return;
   // Cursor already at the top
   if (m_highlightedLine == 0)
   {
      if (p_loop)
      {
         m_highlightedLine = m_nbItems - 1;
         g_hasChanged = true;
      }
   }
   // Cursor can move up
   else
   {
      if (m_highlightedLine > p_step)
         m_highlightedLine -= p_step;
      else
         m_highlightedLine = 0;
      g_hasChanged = true;
   }
}

//------------------------------------------------------------------------------

// Move cursor down
void IWindow::moveCursorDown(const unsigned int p_step, bool p_loop)
{
   if (m_nbItems == 0)
      return;
   // Cursor can move down
   if (m_highlightedLine < m_nbItems - 1)
   {
     if (m_highlightedLine + p_step > m_nbItems - 1)
         m_highlightedLine = m_nbItems - 1;
     else
         m_highlightedLine += p_step;
      g_hasChanged = true;
   }
   // Cursor already at the bottom
   else
   {
      if (p_loop)
      {
         m_highlightedLine = 0;
         g_hasChanged = true;
      }
   }
}
