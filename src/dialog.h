#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <string>
#include <vector>
#include <SDL.h>
#include "window.h"

class Dialog : public IWindow
{
   public:

   // Constructor
   Dialog(const std::string &p_title);

   // Destructor
   virtual ~Dialog(void);

   // Draw window
   virtual void render(const bool p_focus);

   // Add a label
   void addLabel(const std::string &p_label, SDL_Texture *p_icon = NULL);

   // Add a menu option
   void addOption(const std::string &p_option, const int p_retVal, SDL_Texture *p_icon = NULL);

   private:

   // List of labels
   std::vector<std::string> m_labels;
   std::vector<SDL_Texture *> m_labelIcons;

   // List of options
   std::vector<std::string> m_options;
   std::vector<SDL_Texture *> m_optionIcons;
   std::vector<int> m_optionRetVals;

   // True if at least 1 icon is present
   bool m_iconPresent;

   // Forbidden
   Dialog(const Dialog &p_source);
   const Dialog &operator =(const Dialog &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

};

#endif
