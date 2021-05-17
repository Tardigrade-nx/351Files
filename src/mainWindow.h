#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <string>
#include "window.h"

class MainWindow : public IWindow
{

   public:

   // Destructor
   virtual ~MainWindow(void);

   // Constructor
   MainWindow(const std::string &p_title);

   // Draw window
   virtual void render(const bool p_focus);

   private:

   // Forbidden
   MainWindow(const MainWindow &p_source);
   const MainWindow &operator =(const MainWindow &p_source);

};

#endif
