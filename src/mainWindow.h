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

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

   // Open highlighted dir
   void openHighlightedDir(void);

   // Select highlighted item
   void selectHighlightedItem(const bool p_step);

   // Open system menu
   void openSystemMenu(void);

   // Open context menu
   void openContextMenu(void);

   // File lister
   CFileLister m_fileLister;

   // Textures for icons
   SDL_Texture *m_iconFile;
   SDL_Texture *m_iconDir;
   SDL_Texture *m_iconUp;
   SDL_Texture *m_iconCopy;
   SDL_Texture *m_iconCut;
   SDL_Texture *m_iconPaste;
   SDL_Texture *m_iconTrash;
   SDL_Texture *m_iconDisk;
   SDL_Texture *m_iconSelect;
   SDL_Texture *m_iconNone;
   SDL_Texture *m_iconNewDir;
   SDL_Texture *m_iconQuit;

};

#endif
