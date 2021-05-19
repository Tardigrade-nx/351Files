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

   // Set title
   void setTitle(const std::string &p_title);

   protected:

   // Constructor
   IWindow(const bool p_fullscreen, const std::string &p_title);

   // Window is fullscreen or not
   bool m_fullscreen;

   // Window title
   std::string m_title;
   SDL_Texture *m_titleTexture;

   // Windows has changed or not
   static bool g_hasChanged;

   private:

   // Forbidden
   IWindow(void);
   IWindow(const IWindow &p_source);
   const IWindow &operator =(const IWindow &p_source);

};

#endif
