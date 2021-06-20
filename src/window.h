#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <SDL.h>

class IWindow
{
   public:

   // Constructor
   IWindow(const bool p_fullscreen, const std::string &p_title);

   // Destructor
   virtual ~IWindow(void);

   // Execute window and return result
   virtual int execute(void);

   // Draw window
   virtual void render(const bool p_focus) = 0;

   // Render all windows
   static void renderAll(void);
   static void renderPresent(void);

   // Is window full screen?
   bool isFullScreen(void) const;

   protected:

   // Move cursor
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event) {};

   // Get background color for the item at the given index
   virtual SDL_Color getBackgroundColor(const int p_i, const bool p_focus) const;

   // Reset timer for key hold
   void resetTimer(void);

   // Window is fullscreen or not
   bool m_fullscreen;

   // Window title
   std::string m_title;

   // Windows has changed or not
   static bool g_hasChanged;

   // Coordinates of the cursor
   int m_cursor;

   // Loop or not when moving cursor
   bool m_cursorLoop;

   // Number of selectable items
   int m_nbItems;

   // Number of visible lines
   int m_nbVisibleLines;

   // Timer for key hold
   int m_timer;
   int m_lastPressed;

   // Return value of the window
   // -1 => executing
   int m_retVal;

   private:

   // Forbidden
   IWindow(void);
   IWindow(const IWindow &p_source);
   const IWindow &operator =(const IWindow &p_source);

};

#endif
