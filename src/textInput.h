#ifndef _TEXTINPUT_H_
#define _TEXTINPUT_H_

#include <string>
#include <SDL.h>
#include "window.h"

class TextInput : public IWindow
{
   public:

   // Constructor
   TextInput(const std::string &p_title, SDL_Texture *p_icon, const std::string p_inputText = "");

   // Destructor
   virtual ~TextInput(void);

   // Draw window
   virtual void render(const bool p_focus);

   // Execute window and return result
   virtual int execute(void);

   // Get input text
   const std::string &getInputText(void) const;

   private:

   // Forbidden
   TextInput(const TextInput &p_source);
   const TextInput &operator =(const TextInput &p_source);

   // Callbacks for virtual keyboard
   virtual void keyboardInputChar(const std::string &p_string);
   virtual void keyboardBackspace(void);
   virtual void keyboardMoveLeft(void);
   virtual void keyboardMoveRight(void);

   // Adjust camera
   void adjustCamera(void);

   // Icon
   SDL_Texture *m_icon;

   // Input text
   std::string m_inputText;

   // Dialog background and body
   SDL_Rect m_dialogBackground;
   SDL_Rect m_dialogBody;
};

#endif
