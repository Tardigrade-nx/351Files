#ifndef _DIALOG_H_
#define _DIALOG_H_

#include <string>
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

   private:

   // Forbidden
   Dialog(const Dialog &p_source);
   const Dialog &operator =(const Dialog &p_source);

   // Key pressed
   virtual void keyPressedValidate(void);
   virtual void keyPressedBack(void);

};

#endif
