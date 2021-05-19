#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <string>
#include <SDL.h>
#include "window.h"
#include "fileLister.h"

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

   // File lister
   CFileLister m_fileLister;

   // Number of visible lines
   unsigned int m_nbVisibleLines;

   // Index of the first displayed line
   unsigned int m_camera;

   // Textures for icons
   SDL_Texture *m_iconFile;
   SDL_Texture *m_iconDir;
   SDL_Texture *m_iconUp;

};

#endif
