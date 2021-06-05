#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>
#include <SDL.h>

class IWindow
{
   public:

   // Destructor
   virtual ~IWindow(void);

   // Execute window and return result
   virtual int execute(void);

   // Draw window
   virtual void render(const bool p_focus) = 0;

   // Render all windows
   static void renderAll(void);

   // Is window full screen?
   bool isFullScreen(void) const;

   protected:

   // Constructor
   IWindow(const bool p_fullscreen, const std::string &p_title);

   // Move cursor up or down
   void moveCursorUp(const int p_step, bool p_loop);
   void moveCursorDown(const int p_step, bool p_loop);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event) {};

   // Adjust camera
   void adjustCamera(void);

   // Get background color for the item at the given index
   SDL_Color getBackgroundColor(const int p_i, const bool p_focus) const;

   // Reset timer for key hold
   void resetTimer(void);

   // Window is fullscreen or not
   bool m_fullscreen;

   // Window title
   std::string m_title;

   // Windows has changed or not
   static bool g_hasChanged;

   // Coordinates of the cursor
   SDL_Point m_cursor;

   // Loop or not when moving cursor
   bool m_cursorLoop;

   // Number of selectable items
   int m_nbItems;

   // Number of visible lines
   int m_nbVisibleLines;

   // Timer for key hold
   int m_timer;
   int m_lastPressed;

   // Index of the first displayed line
   int m_camera;

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
