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
   void moveCursorUp(const unsigned int p_step, bool p_loop);
   void moveCursorDown(const unsigned int p_step, bool p_loop);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event) = 0;

   // Adjust camera
   void adjustCamera(void);

   // Get background color for the item at the given index
   SDL_Color getBackgroundColor(const unsigned int p_i, const bool p_focus);

   // Window is fullscreen or not
   bool m_fullscreen;

   // Window title
   std::string m_title;

   // Windows has changed or not
   static bool g_hasChanged;

   // Index of the highlighted line
   unsigned int m_highlightedLine;

   // Loop or not when moving cursor
   bool m_cursorLoop;

   // Number of items
   unsigned int m_nbItems;

   // Number of visible items
   unsigned int m_nbVisibleItems;

   // Timer for key hold
   unsigned int m_timer;
   int m_lastPressed;

   // Index of the first displayed line
   unsigned int m_camera;

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
