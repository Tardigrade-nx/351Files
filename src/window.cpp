#include "window.h"
#include "def.h"
#include "sdlutils.h"
#include <iostream>

// Buttons with timer
enum {
   BUTTON_UP = 0,
   BUTTON_PAGEUP,
   BUTTON_DOWN,
   BUTTON_PAGEDOWN,
   BUTTON_LEFT,
   BUTTON_RIGHT
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
   m_title(p_title),
   m_cursor(0),
   m_cursorLoop(false),
   m_nbItems(0),
   m_nbVisibleLines(0),
   m_timer(0),
   m_lastPressed(-1),
   m_camera(0),
   m_retVal(-1)
{
   m_nbVisibleLines = (SCREEN_HEIGHT - LINE_HEIGHT) / LINE_HEIGHT;
   // Add window to the list
   g_windows.push_back(this);
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Is window full screen?
bool IWindow::isFullScreen(void) const
{
   return m_fullscreen;
}

//------------------------------------------------------------------------------

// Execute window and return result
int IWindow::execute(void)
{
   // Main loop
   Uint32 l_time(0);
   SDL_Event event;
   while (m_retVal == -1)
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
            moveCursorUp(m_nbVisibleLines - 1, m_cursorLoop);
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
            moveCursorDown(m_nbVisibleLines - 1, m_cursorLoop);
            m_lastPressed = BUTTON_PAGEDOWN;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
         if (BUTTON_PRESSED_LEFT)
         {
            moveCursorLeft(1, m_cursorLoop);
            m_lastPressed = BUTTON_LEFT;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
         if (BUTTON_PRESSED_RIGHT)
         {
            moveCursorRight(1, m_cursorLoop);
            m_lastPressed = BUTTON_RIGHT;
            m_timer = KEYHOLD_TIMER_FIRST;
            continue;
         }
         // Specific reaction to event
         keyPressed(event);
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
            moveCursorUp(m_nbVisibleLines - 1, false);
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
            moveCursorDown(m_nbVisibleLines - 1, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else if (BUTTON_HELD_LEFT)
      {
         if (m_lastPressed == BUTTON_LEFT && m_timer > 0 && --m_timer == 0)
         {
            moveCursorLeft(1, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else if (BUTTON_HELD_RIGHT)
      {
         if (m_lastPressed == BUTTON_RIGHT && m_timer > 0 && --m_timer == 0)
         {
            moveCursorRight(1, false);
            m_timer = KEYHOLD_TIMER;
         }
      }
      else
      {
         resetTimer();
      }

      // Close window now
      if (m_retVal != -1)
         return m_retVal;

      // Render windows if necessary
      if (g_hasChanged)
      {
         renderAll();
         SDL_RenderPresent(g_renderer);
         #ifndef DEVICE_PC
         SDL_RenderPresent(g_renderer);
         SDL_RenderPresent(g_renderer);
         #endif
         g_hasChanged = false;
      }

      // Cap the framerate
      l_time = MS_PER_FRAME - (SDL_GetTicks() - l_time);
      if (l_time <= MS_PER_FRAME)
         SDL_Delay(l_time);
   }
   return m_retVal;
}

//------------------------------------------------------------------------------

// Render all windows
void IWindow::renderAll(void)
{
   //~ INHIBIT(std::cout << "renderAll - " << g_windows.size() << " windows\n";)
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
void IWindow::moveCursorUp(const int p_step, bool p_loop)
{
   if (m_nbItems == 0)
      return;
   // Cursor already at the top
   if (m_cursor == 0)
   {
      if (p_loop)
      {
         m_cursor = m_nbItems - 1;
         g_hasChanged = true;
      }
   }
   // Cursor can move up
   else
   {
      if (m_cursor > p_step)
         m_cursor -= p_step;
      else
         m_cursor = 0;
      g_hasChanged = true;
   }
   // Adjust camera
   adjustCamera();
}

//------------------------------------------------------------------------------

// Move cursor down
void IWindow::moveCursorDown(const int p_step, bool p_loop)
{
   if (m_nbItems == 0)
      return;
   // Cursor can move down
   if (m_cursor < m_nbItems - 1)
   {
     if (m_cursor + p_step > m_nbItems - 1)
         m_cursor = m_nbItems - 1;
     else
         m_cursor += p_step;
      g_hasChanged = true;
   }
   // Cursor already at the bottom
   else
   {
      if (p_loop)
      {
         m_cursor = 0;
         g_hasChanged = true;
      }
   }
   // Adjust camera
   adjustCamera();
}

//------------------------------------------------------------------------------

   // Move cursor left
   void IWindow::moveCursorLeft(const int p_step, bool p_loop)
   {
   }

//------------------------------------------------------------------------------

   // Move cursor right
   void IWindow::moveCursorRight(const int p_step, bool p_loop)
   {
   }

//------------------------------------------------------------------------------

// Adjust camera
void IWindow::adjustCamera(void)
{
   if (m_nbItems <= m_nbVisibleLines)
      m_camera = 0;
   else if (m_cursor < m_camera)
      m_camera = m_cursor;
   else if (m_cursor > m_camera + m_nbVisibleLines - 1)
      m_camera = m_cursor - m_nbVisibleLines + 1;
}

//------------------------------------------------------------------------------

// Get background color for the item at the given index
SDL_Color IWindow::getBackgroundColor(const int p_i, const bool p_focus) const
{
   // Background color for the line
   if (m_cursor == p_i)
   {
      if (p_focus)
         return {COLOR_CURSOR_FOCUS};
      return {COLOR_CURSOR_NO_FOCUS};
   }
   return {COLOR_BODY_BG};
}

//------------------------------------------------------------------------------

// Reset timer for key hold
void IWindow::resetTimer(void)
{
   m_lastPressed = -1;
   m_timer = 0;
}
