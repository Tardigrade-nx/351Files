#ifndef _TEXTVIEWER_H_
#define _TEXTVIEWER_H_

#include <string>
#include <vector>
#include <SDL.h>
#include "window.h"

class TextViewer : public IWindow
{
   public:

   // Constructor
   TextViewer(const std::string &p_title);

   // Destructor
   virtual ~TextViewer(void);

   // Draw window
   virtual void render(const bool p_focus);

   private:

   // Forbidden
   TextViewer(const TextViewer &p_source);
   const TextViewer &operator =(const TextViewer &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

   // Move camera
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // List of lines
   std::vector<std::string> m_lines;

   // Camera
   SDL_Point m_camera;

};


#endif
