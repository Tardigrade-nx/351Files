#include "mainWindow.h"
#include <iostream>
#include <sstream>
#include "def.h"
#include "sdlutils.h"

// Destructor
MainWindow::~MainWindow(void)
{
   // Free textures
   if (m_iconFile != NULL) { SDL_DestroyTexture(m_iconFile); m_iconFile = NULL; }
   if (m_iconDir != NULL) { SDL_DestroyTexture(m_iconDir); m_iconDir = NULL; }
   if (m_iconUp != NULL) { SDL_DestroyTexture(m_iconUp); m_iconUp = NULL; }
}

// Constructor
MainWindow::MainWindow(const std::string &p_title):
   IWindow(true, p_title),
   m_fileLister(),
   m_nbVisibleLines((SCREEN_HEIGHT - LINE_HEIGHT - LINE_HEIGHT) / LINE_HEIGHT),
   m_camera(0),
   m_iconFile(NULL),
   m_iconDir(NULL),
   m_iconUp(NULL)
{
   // Load textures
   m_iconFile = SDLUtils::loadTexture(std::string(RES_PATH) + "/file.png");
   m_iconDir = SDLUtils::loadTexture(std::string(RES_PATH) + "/folder.png");
   m_iconUp = SDLUtils::loadTexture(std::string(RES_PATH) + "/up.png");
   // List files
   if (! m_fileLister.list(m_title))
   {
      // Path is wrong, fallback to '/'
      setTitle("/");
      m_fileLister.list(m_title);
   }
   INHIBIT(std::cout << "Path: " << m_title << "\n";)
}

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
   SDLUtils::renderText(m_title, MARGIN_X, l_y, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG}, SDLUtils::T_TEXT_ALIGN_LEFT, SDLUtils::T_TEXT_ALIGN_MIDDLE);

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
   std::ostringstream oss;
   l_y += LINE_HEIGHT;
   unsigned int l_nbTotal = m_fileLister.getNbTotal();
   SDL_Color l_bgColor = {COLOR_BODY_BG};
   for (unsigned int l_i = m_camera; l_i < m_camera + m_nbVisibleLines && l_i < l_nbTotal; ++l_i)
   {
      // Background color for the line
      if (m_highlightedLine == l_i)
      {
         if (p_focus)
            l_bgColor = {COLOR_CURSOR_FOCUS};
         else
            l_bgColor = {COLOR_CURSOR_NO_FOCUS};
      }
      else
      {
         l_bgColor = {COLOR_BODY_BG};
      }

      // Icon
      if (l_i == 0 && m_fileLister[l_i].m_name == "..")
         SDLUtils::renderTexture(m_iconUp, MARGIN_X, l_y - (ICON_SIZE / 2));
      else
         SDLUtils::renderTexture(m_fileLister.isDirectory(l_i) ? m_iconDir : m_iconFile, MARGIN_X, l_y - (ICON_SIZE / 2));

      // File name
      SDLUtils::renderText(m_fileLister[l_i].m_name, MARGIN_X + ICON_SIZE + MARGIN_X, l_y, {COLOR_TEXT_NORMAL}, l_bgColor, SDLUtils::T_TEXT_ALIGN_LEFT, SDLUtils::T_TEXT_ALIGN_MIDDLE);

      // File size
      if (! m_fileLister.isDirectory(l_i))
      {
         oss << m_fileLister[l_i].m_size;
         SDLUtils::renderText(oss.str(), SCREEN_WIDTH - 1 - MARGIN_X, l_y, {COLOR_TEXT_NORMAL}, l_bgColor, SDLUtils::T_TEXT_ALIGN_RIGHT, SDLUtils::T_TEXT_ALIGN_MIDDLE);
         oss.str("");
         oss.clear();
      }

      // Next line
      l_y += LINE_HEIGHT;
   }

}
