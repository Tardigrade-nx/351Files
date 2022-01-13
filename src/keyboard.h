#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <SDL.h>
#include <string>
#include "window.h"

class Keyboard : public IWindow
{

   public:

   // Constructor
   Keyboard(IWindow *p_parent, const bool p_quitOnEnter);

   // Destructor
   virtual ~Keyboard(void);

   // Draw window
   virtual void render(const bool p_focus);

   // Key and keyboard size
   static int getKeyW(void);
   static int getKeyH(void);
   static int getKeyboardW(void);
   static int getKeyboardH(void);

   private:

   // Forbidden
   Keyboard(const Keyboard &p_source);
   const Keyboard &operator =(const Keyboard &p_source);

   // Key pressed / held
   virtual void keyPressed(const SDL_Event &event);
   virtual bool keyHeld(void);

   // Create background image and init the keyboard
   void init(void);

   // Move cursor
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // Get background color for the item at the given index
   virtual SDL_Color getBackgroundColor(const int p_i, const bool p_focus) const;

   // Press symbol button
   void keyPressedSymbol(void);

   // Press shift button
   void keyPressedShift(void);

   // Input key
   void input(void);

   // Parent window
   IWindow *m_parent;

   // Background image for the keyboard
   SDL_Texture *m_background;

   // Size and coordinates of the keys
   SDL_Rect m_key[36];

   // Labels for the keys
   std::string m_keyLabel[4];
   int m_keyLabelCurrent;

   // Size and coordinates of the keyboard
   SDL_Rect m_keyboard;

   // Textures for keys
   SDL_Texture *m_texShiftEmpty;
   SDL_Texture *m_texShiftFull;
   SDL_Texture *m_texEnter;
   SDL_Texture *m_texArrow;
   SDL_Texture *m_texBackspace;

   // Behavior for the Enter key
   bool m_quitOnEnter;

};

#endif
