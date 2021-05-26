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
   // Free textures
   if (m_iconFile != NULL)    { SDL_DestroyTexture(m_iconFile);      m_iconFile = NULL; }
   if (m_iconDir != NULL)     { SDL_DestroyTexture(m_iconDir);       m_iconDir = NULL; }
   if (m_iconUp != NULL)      { SDL_DestroyTexture(m_iconUp);        m_iconUp = NULL; }
   if (m_iconCopy != NULL)    { SDL_DestroyTexture(m_iconCopy);      m_iconCopy = NULL; }
   if (m_iconCut != NULL)     { SDL_DestroyTexture(m_iconCut);       m_iconCut = NULL; }
   if (m_iconPaste != NULL)   { SDL_DestroyTexture(m_iconPaste);     m_iconPaste = NULL; }
   if (m_iconTrash != NULL)   { SDL_DestroyTexture(m_iconTrash);     m_iconTrash = NULL; }
   if (m_iconDisk != NULL)    { SDL_DestroyTexture(m_iconDisk);      m_iconDisk = NULL; }
   if (m_iconSelect != NULL)  { SDL_DestroyTexture(m_iconSelect);    m_iconSelect = NULL; }
   if (m_iconNone != NULL)    { SDL_DestroyTexture(m_iconNone);      m_iconNone = NULL; }
   if (m_iconNewDir != NULL)  { SDL_DestroyTexture(m_iconNewDir);    m_iconNewDir = NULL; }
   if (m_iconQuit != NULL)    { SDL_DestroyTexture(m_iconQuit);      m_iconQuit = NULL; }
}

//------------------------------------------------------------------------------

// Constructor
MainWindow::MainWindow(const std::string &p_title):
   IWindow(true, p_title),
   m_fileLister(),
   m_iconFile(NULL),
   m_iconDir(NULL),
   m_iconUp(NULL),
   m_iconCopy(NULL),
   m_iconCut(NULL),
   m_iconPaste(NULL),
   m_iconTrash(NULL),
   m_iconDisk(NULL),
   m_iconSelect(NULL),
   m_iconNone(NULL),
   m_iconNewDir(NULL),
   m_iconQuit(NULL)
{
   // Load textures
   m_iconFile = SDLUtils::loadTexture(std::string(RES_PATH) + "/file.png");
   m_iconDir = SDLUtils::loadTexture(std::string(RES_PATH) + "/folder.png");
   m_iconUp = SDLUtils::loadTexture(std::string(RES_PATH) + "/up.png");
   m_iconCopy = SDLUtils::loadTexture(std::string(RES_PATH) + "/edit-copy.png");
   m_iconCut = SDLUtils::loadTexture(std::string(RES_PATH) + "/edit-cut.png");
   m_iconPaste = SDLUtils::loadTexture(std::string(RES_PATH) + "/edit-paste.png");
   m_iconTrash = SDLUtils::loadTexture(std::string(RES_PATH) + "/trash.png");
   m_iconDisk = SDLUtils::loadTexture(std::string(RES_PATH) + "/disk.png");
   m_iconSelect = SDLUtils::loadTexture(std::string(RES_PATH) + "/select.png");
   m_iconNone = SDLUtils::loadTexture(std::string(RES_PATH) + "/none.png");
   m_iconNewDir = SDLUtils::loadTexture(std::string(RES_PATH) + "/folder-new.png");
   m_iconQuit = SDLUtils::loadTexture(std::string(RES_PATH) + "/quit.png");
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
         SDLUtils::renderTexture(m_iconUp, MARGIN_X, l_y - (ICON_SIZE / 2));
      else
         SDLUtils::renderTexture(m_fileLister.isDirectory(l_i) ? m_iconDir : m_iconFile, MARGIN_X, l_y - (ICON_SIZE / 2));

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
   // Button system menu
   if (BUTTON_PRESSED_MENU_SYSTEM)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // Open system menu
      openSystemMenu();
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

// Open system menu
void MainWindow::openSystemMenu(void)
{
   // Open dialog
   Dialog l_dialog ("System");
   l_dialog.addOption("Select all", 0, m_iconSelect);
   l_dialog.addOption("Select none", 1, m_iconNone);
   l_dialog.addOption("New directory", 2, m_iconNewDir);
   l_dialog.addOption("Disk info", 3, m_iconDisk);
   l_dialog.addOption("Quit", 4, m_iconQuit);
   switch(l_dialog.execute())
   {
      // Select all
      case 0:
         m_fileLister.setSelectedAll(true);
         g_hasChanged = true;
         break;
      // Select none
      case 1:
         m_fileLister.setSelectedAll(false);
         g_hasChanged = true;
         break;
      case 2:
      case 3:
      {
         Dialog l_dialog2 ("Info");
         l_dialog2.addLabel("Not yet implemented!");
         l_dialog2.addOption("OK", 0);
         l_dialog2.execute();
      }
      break;
      // Quit
      case 4:
         m_retVal = 0;
         break;
      default:
         break;
   }
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
   l_dialog.addOption("Copy", 0, m_iconCopy);
   l_dialog.addOption("Cut", 1, m_iconCut);
   l_dialog.addOption("Paste", 2, m_iconPaste);
   l_dialog.addOption("Delete", 3, m_iconTrash);
   l_dialog.addOption("Disk used", 4, m_iconDisk);
   switch(l_dialog.execute())
   {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      {
         Dialog l_dialog2 ("Info");
         l_dialog2.addLabel("Not yet implemented!");
         l_dialog2.addOption("OK", 0);
         l_dialog2.execute();
      }
      break;
      default:
         break;
   }
}
