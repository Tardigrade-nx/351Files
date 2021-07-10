#ifndef _TEXTEDITOR_H_
#define _TEXTEDITOR_H_

#include <string>
#include <vector>
#include <SDL.h>
#include "window.h"

class TextEditor : public IWindow
{
   public:

   // Constructor
   TextEditor(const std::string &p_title);

   // Destructor
   virtual ~TextEditor(void);

   // Draw window
   virtual void render(const bool p_focus);

   private:

   // Forbidden
   TextEditor(const TextEditor &p_source);
   const TextEditor &operator =(const TextEditor &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

   // Move cursor
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // Adjust camera
   void adjustCamera(void);

   // List of lines
   std::vector<std::string> m_lines;

   // Input cursor
   SDL_Point m_inputTextCursor;

   // X saved from previous line
   int m_oldX;
};

#endif
