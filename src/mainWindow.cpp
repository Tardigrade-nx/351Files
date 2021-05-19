#include "mainWindow.h"
#include <iostream>
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
   SDL_QueryTexture(m_titleTexture, NULL, NULL, &rect.w, &rect.h);
   rect.x = MARGIN_X;
   rect.y = (LINE_HEIGHT - rect.h) / 2;
   SDL_RenderCopy(g_renderer, m_titleTexture, NULL, &rect);
   // Render file list
   unsigned int l_nbTotal = m_fileLister.getNbTotal();
   int l_y = LINE_HEIGHT + (LINE_HEIGHT / 2);
   rect.x = MARGIN_X + ICON_SIZE + MARGIN_X;
   for (unsigned int l_i = m_camera; l_i < m_camera + m_nbVisibleLines && l_i < l_nbTotal; ++l_i)
   {
      // Icon
      if (l_i == 0)
         SDLUtils::renderTexture(m_iconUp, MARGIN_X, l_y - (ICON_SIZE / 2));
      else
         SDLUtils::renderTexture(m_fileLister.isDirectory(l_i) ? m_iconDir : m_iconFile, MARGIN_X, l_y - (ICON_SIZE / 2));
      // File name
      SDL_QueryTexture(m_fileLister[l_i].m_textureNormal, NULL, NULL, &rect.w, &rect.h);
      rect.y = l_y - (rect.h / 2);
      SDL_RenderCopy(g_renderer, m_fileLister[l_i].m_textureNormal, NULL, &rect);
      // Next line
      l_y += LINE_HEIGHT;
   }

}
