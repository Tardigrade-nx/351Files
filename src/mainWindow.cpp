#include "mainWindow.h"
#include <iostream>
#include <sstream>
#include "def.h"
#include "sdlutils.h"
#include "fileUtils.h"
#include "dialog.h"

// Destructor
MainWindow::~MainWindow(void)
{
}

//------------------------------------------------------------------------------

// Constructor
MainWindow::MainWindow(const std::string &p_title):
   IWindow(true, p_title)
{
   // List files
   if (! m_fileLister.list(m_title))
   {
      // Path is wrong, fallback to '/'
      m_title = "/";
      m_fileLister.list(m_title);
   }
   m_nbItems = m_fileLister.getNbTotal();
   INHIBIT(std::cout << "Path: " << m_title << " (" << m_nbItems << ") items\n";)
}

//------------------------------------------------------------------------------

// Draw window
void MainWindow::render(const bool p_focus)
{
   // Clear screen
   SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
   SDL_RenderClear(g_renderer);

   // Render title background
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_Rect rect { 0, 0, SCREEN_WIDTH, LINE_HEIGHT };
   SDL_RenderFillRect(g_renderer, &rect);

   // Render title text
   int l_y = LINE_HEIGHT / 2;
   SDLUtils::renderText(m_title, MARGIN_X, l_y, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Render cursor
   if (p_focus)
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_FOCUS, 255);
   else
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_NO_FOCUS, 255);
   rect.x = 0;
   rect.y = LINE_HEIGHT + (m_highlightedLine - m_camera) * LINE_HEIGHT;
   rect.w = SCREEN_WIDTH;
   rect.h = LINE_HEIGHT;
   SDL_RenderFillRect(g_renderer, &rect);

   // Render file list
   l_y += LINE_HEIGHT;
   SDL_Color l_fgColor = {COLOR_TEXT_NORMAL};
   SDL_Color l_bgColor = {COLOR_BODY_BG};
   for (unsigned int l_i = m_camera; l_i < m_camera + m_nbVisibleItems && l_i < m_nbItems; ++l_i)
   {
      // Colors for the line
      if (m_fileLister[l_i].m_selected)
         l_fgColor = {COLOR_TEXT_SELECTED};
      else
         l_fgColor = {COLOR_TEXT_NORMAL};
      l_bgColor = getBackgroundColor(l_i, p_focus);

      // Icon
      if (m_fileLister[l_i].m_name == "..")
         SDLUtils::renderTexture(g_iconUp, MARGIN_X, l_y - (ICON_SIZE / 2));
      else
         SDLUtils::renderTexture(m_fileLister.isDirectory(l_i) ? g_iconDir : g_iconFile, MARGIN_X, l_y - (ICON_SIZE / 2));

      // File name
      SDLUtils::renderText(m_fileLister[l_i].m_name, MARGIN_X + ICON_SIZE + MARGIN_X, l_y, l_fgColor, l_bgColor, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

      // File size
      if (! m_fileLister.isDirectory(l_i))
         SDLUtils::renderText(FileUtils::formatSize(m_fileLister[l_i].m_size), SCREEN_WIDTH - 1 - MARGIN_X, l_y, l_fgColor, l_bgColor, SDLUtils::T_ALIGN_RIGHT, SDLUtils::T_ALIGN_MIDDLE);

      // Next line
      l_y += LINE_HEIGHT;
   }

}

//------------------------------------------------------------------------------

// Key pressed
void MainWindow::keyPressed(const SDL_Event &event)
{
   // Button Validate
   if (BUTTON_PRESSED_VALIDATE)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // If a file is highlighted, do nothing
      if (! m_fileLister.isDirectory(m_highlightedLine))
         return;
      // Open highlighted dir
      openHighlightedDir();
      return;
   }
   // Button Back
   if (BUTTON_PRESSED_BACK)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // If we're already at '/, do nothing
      if (m_title == "/")
         return;
      // Select and open ".."
      m_highlightedLine = 0;
      openHighlightedDir();
      return;
   }
   // Button context menu
   if (BUTTON_PRESSED_MENU_CONTEXT)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // Open context menu
      openContextMenu();
      return;
   }
   // Button select
   if (BUTTON_PRESSED_SELECT)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // Select / unselect highlighted item
      selectHighlightedItem(true);
      return;
   }
}

//------------------------------------------------------------------------------

// Open highlighted dir
void MainWindow::openHighlightedDir(void)
{
   // Build new path
   std::string l_newDir("");
   std::string l_oldDir("");
   if (m_fileLister[m_highlightedLine].m_name == "..")
   {
      // New path = parent
      size_t l_pos = m_title.rfind('/');
      l_newDir = m_title.substr(0, l_pos);
      if (l_newDir.empty())
         l_newDir = "/";
      l_oldDir = m_title.substr(l_pos + 1);
   }
   else
   {
      // New path
      l_newDir = m_title + (m_title == "/" ? "" : "/") + m_fileLister[m_highlightedLine].m_name;
   }

   // List the new path
   if (m_fileLister.list(l_newDir))
   {
      // Path OK
      m_title = l_newDir;
      m_nbItems = m_fileLister.getNbTotal();
     // If it's a back movement, restore old highlighted dir
      if (! l_oldDir.empty())
         m_highlightedLine = m_fileLister.searchDir(l_oldDir);
      else
         m_highlightedLine = 0;
      // Adjust camera
      adjustCamera();
      // New render
      g_hasChanged = true;
      INHIBIT(std::cout << "Path: " << m_title << " (" << m_nbItems << ") items\n";)
   }
}

//------------------------------------------------------------------------------

// Select highlighted item
void MainWindow::selectHighlightedItem(const bool p_step)
{
   // Cannot select '..'
   if (m_fileLister[m_highlightedLine].m_name == "..")
      return;
   // Select/unselect highlighted item
   m_fileLister[m_highlightedLine].m_selected = ! m_fileLister[m_highlightedLine].m_selected;
   // Move 1 step if requested
   if (p_step)
      moveCursorDown(1, false);
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Open context menu
void MainWindow::openContextMenu(void)
{
   // If no file is selected, select current file
   auto nbSelected = m_fileLister.getNbSelected();
   if (nbSelected == 0)
   {
      selectHighlightedItem(false);
      nbSelected = m_fileLister.getNbSelected();
   }
   // Open dialog
   std::ostringstream oss;
   oss << nbSelected << " selected";
   Dialog l_dialog (oss.str());
   if (nbSelected > 0)
   {
      l_dialog.addOption("Copy", 0, g_iconCopy);
      l_dialog.addOption("Cut", 1, g_iconCut);
   }
   if (m_clipboard.size() > 0)
      l_dialog.addOption("Paste", 2, g_iconPaste);
   if (nbSelected > 0)
      l_dialog.addOption("Delete", 3, g_iconTrash);
   if (nbSelected == 1)
      l_dialog.addOption("Rename", 9, g_iconEdit);
   if (nbSelected > 0)
      l_dialog.addOption("Size", 4, g_iconDisk);
   l_dialog.addOption("Select all", 5, g_iconSelect);
   l_dialog.addOption("Select none", 6, g_iconNone);
   l_dialog.addOption("New directory", 7, g_iconNewDir);
   l_dialog.addOption("Quit", 8, g_iconQuit);
   switch(l_dialog.execute())
   {
      // Copy
      case 0:
         m_fileLister.getSelectList(m_title, m_clipboard);
         m_clipboardOperation = 'c';
         INHIBIT(std::cout << m_clipboard.size() << " added to clipboard for copy\n";)
         break;
      // Move
      case 1:
         m_fileLister.getSelectList(m_title, m_clipboard);
         m_clipboardOperation = 'm';
         INHIBIT(std::cout << m_clipboard.size() << " added to clipboard for move\n";)
         break;
      // Paste
      case 2:
         if (m_clipboardOperation == 'c')
            FileUtils::copyFiles(m_clipboard, m_title);
         else if (m_clipboardOperation == 'm')
            FileUtils::moveFiles(m_clipboard, m_title);
         refresh();
         break;
      // Delete
      case 3:
         m_fileLister.getSelectList(m_title, m_clipboard);
         FileUtils::removeFiles(m_clipboard);
         m_clipboard.clear();
         refresh();
         break;
      // Select all
      case 5:
         m_fileLister.setSelectedAll(true);
         g_hasChanged = true;
         break;
      // Select none
      case 6:
         m_fileLister.setSelectedAll(false);
         g_hasChanged = true;
         break;
      // Quit
      case 8:
         m_retVal = 0;
         break;
      case 4:
      case 7:
      case 9:
      {
         Dialog l_dialog2 ("Info");
         l_dialog2.addLabel("Not yet implemented!");
         l_dialog2.addOption("OK", 0, g_iconSelect);
         l_dialog2.execute();
      }
      break;
      default:
         break;
   }
}

//------------------------------------------------------------------------------

// Refresh current directory
void MainWindow::refresh(void)
{
   INHIBIT(std::cout << "MainWindow::refresh\n";)
   // List the current path
   if (! m_fileLister.list(m_title))
   {
      // Path is wrong, fallback to default
      m_title = START_PATH;
      m_fileLister.list(m_title);
   }
   // Update number of items
   m_nbItems = m_fileLister.getNbTotal();
   // Adjust selected line
   if (m_highlightedLine > m_nbItems - 1)
      m_highlightedLine = m_nbItems - 1;
   // Adjust camera
   adjustCamera();
   // New render
   g_hasChanged = true;
}
