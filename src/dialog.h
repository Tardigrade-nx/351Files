#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <string>
#include <vector>
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
   void addLabel(const std::string &p_label);

   // Add a menu option
   void addOption(const std::string &p_option);

   private:

   // List of labels
   std::vector<std::string> m_labels;

   // List of options
   std::vector<std::string> m_options;

   // Forbidden
   Dialog(const Dialog &p_source);
   const Dialog &operator =(const Dialog &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

};

#endif
