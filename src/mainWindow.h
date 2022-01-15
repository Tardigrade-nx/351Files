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

   // Move cursor
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);

   // Open highlighted dir / file
   void openHighlightedDir(void);
   void openHighlightedFile(void);

   // Refresh current directory
   void refresh(void);

   // Select highlighted item
   void selectHighlightedItem(const bool p_step);

   // Open context menu
   void openContextMenu(void);

   // Adjust camera
   void adjustCamera(void);

   // File lister
   CFileLister m_fileLister;

   // Clipboard containing full path of files, for copying or moving
   // Operation: 'c' = copy, 'm' = move
   std::vector<std::string> m_clipboard;
   char m_clipboardOperation;

   // Scrolling long file names
   bool m_scrollFileNameActive;
   int m_scrollFileNameX;
   int m_scrollFileNameTimer;
   int m_scrollFileNameDir;

};

#endif
