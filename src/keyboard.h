#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <SDL.h>
#include <string>
#include "window.h"

class Keyboard : public IWindow
{

   public:

   // Destructor
   virtual ~Keyboard(void);

   // Constructor
   Keyboard(const std::string &p_text = "");

   // Draw window
   virtual void render(const bool p_focus);

   // Get input text
   const std::string &getInputText(void) const;

   private:

   // Forbidden
   Keyboard(const Keyboard &p_source);
   const Keyboard &operator =(const Keyboard &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

   // Create background image and init the keyboard
   void init(void);

   // Move cursor
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // Get background color for the item at the given index
   virtual SDL_Color getBackgroundColor(const int p_i, const bool p_focus) const;

   // Background image for the keyboard
   SDL_Texture *m_background;

   // Size and coordinates of the keys
   SDL_Rect m_key[36];

   // Labels for the keys
   std::string m_keyLabel[3];
   int m_keyLabelCurrent;

   // Size and coordinates of the keyboard
   SDL_Rect m_keyboard;

   // Textures for keys
   SDL_Texture *m_texShiftEmpty;
   SDL_Texture *m_texShiftFull;
   SDL_Texture *m_texEnter;
   SDL_Texture *m_texArrow;
   SDL_Texture *m_texBackspace;

   // Input text
   std::string m_inputText;
   int m_inputTextCursor;

};

#endif
