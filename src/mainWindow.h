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

   // Refresh current directory
   void refresh(void);

   // Select highlighted item
   void selectHighlightedItem(const bool p_step);

   // Open context menu
   void openContextMenu(void);

   // File lister
   CFileLister m_fileLister;

   // Clipboard containing full path of files, for copying or moving
   // Operation: 'c' = copy, 'm' = move
   std::vector<std::string> m_clipboard;
   char m_clipboardOperation;

};

#endif
