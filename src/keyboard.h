#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <SDL.h>
#include <string>
#include "window.h"

#define KEYBOARD_MARGIN          8
#define KEYBOARD_KEY_SPACING     4

class Keyboard : public IWindow
{

   public:

   // Destructor
   virtual ~Keyboard(void);

   // Constructor
   Keyboard(void);

   // Draw window
   virtual void render(const bool p_focus);

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
   SDL_Texture *m_texEnter;
   SDL_Texture *m_texArrow;
   SDL_Texture *m_texBackspace;

};

#endif
