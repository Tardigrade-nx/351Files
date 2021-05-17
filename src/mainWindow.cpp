#include "mainWindow.h"
#include <iostream>
#include "def.h"
#include "sdlutils.h"

// Destructor
MainWindow::~MainWindow(void)
{
}

// Constructor
MainWindow::MainWindow(const std::string &p_title):
   IWindow(true, p_title)
{
   INHIBIT(std::cout << "Path: " << p_title << "\n";)
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
}
