#include <iostream>
#include <fstream>
#include <algorithm>
#include "textViewer.h"
#include "def.h"
#include "sdlutils.h"

namespace {

inline int UTF8CodePointLen(const char* src) {
  return "\1\1\1\1\1\1\1\1\1\1\1\1\2\2\3\4"[static_cast<unsigned char>(*src)>> 4];
}

void ReplaceTabs(std::string *line) {
   constexpr std::size_t kTabWidth = 4;
   const std::size_t num_tabs = std::count(line->begin(), line->end(), '\t');
   if (num_tabs == 0) return;
   std::string result;
   result.reserve(line->size() + num_tabs * (kTabWidth - 1));
   std::size_t prev_tab_end = 0;
   std::size_t column = 0;
   for (std::size_t i = 0; i < line->size(); i += UTF8CodePointLen(line->data() + i)) {
      if ((*line)[i] == '\t') {
         result.append(*line, prev_tab_end, i - prev_tab_end);
         const std::size_t num_spaces = kTabWidth - (column % kTabWidth);
         result.append(num_spaces, ' ');
         prev_tab_end = i + 1;
         column += num_spaces;
      } else {
         ++column;
      }
   }
   result.append(*line, prev_tab_end, std::string::npos);
   *line = std::move(result);
}

} // namespace

//------------------------------------------------------------------------------

// Constructor
TextViewer::TextViewer(const std::string &p_title):
   IWindow(true, p_title)
{
   // Read file
   std::ifstream ifs(p_title);
   if (! ifs.is_open())
   {
      std::cerr << "Unable to read file: '" << p_title << "'\n";
      return;
   }
   while (! ifs.eof())
   {
      m_lines.emplace_back();
      std::getline(ifs, m_lines.back());
      ReplaceTabs(&m_lines.back());
   }
   ifs.close();
   if (m_lines.back().empty())
      m_lines.pop_back();
   // Number of lines
   m_nbItems = m_lines.size();
   // Init scrollbar
   adjustScrollbar();
   // Number of visible chars
   m_nbVisibleChars = round(static_cast<double>(SCREEN_WIDTH - 2*MARGIN_X - m_scrollbar.w) / g_charW);
}

//------------------------------------------------------------------------------

// Destructor
TextViewer::~TextViewer(void)
{
}

//------------------------------------------------------------------------------

// Draw window
void TextViewer::render(const bool p_focus)
{
   // Clear screen
   SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
   SDL_RenderClear(g_renderer);

   // Render title background
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_Rect rect { 0, 0, SCREEN_WIDTH, LINE_HEIGHT };
   SDL_RenderFillRect(g_renderer, &rect);

   // Render title
   int l_y = LINE_HEIGHT / 2;
   SDLUtils::renderTexture(g_iconFileText, MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
   SDLUtils::renderText(m_title, g_font, MARGIN_X + ICON_SIZE + MARGIN_X, l_y, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Render scrollbar
   if (p_focus)
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_FOCUS, 255);
   else
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_NO_FOCUS, 255);
   if (m_scrollbar.h > 0)
      SDL_RenderFillRect(g_renderer, &m_scrollbar);

   // Render lines
   l_y += LINE_HEIGHT;
   SDL_Color l_fgColor = {COLOR_TEXT_NORMAL};
   SDL_Color l_bgColor = {COLOR_BODY_BG};
   for (int l_i = m_camera.y; l_i < m_camera.y + m_nbVisibleLines && l_i < m_nbItems; ++l_i, l_y += LINE_HEIGHT)
      if (m_camera.x < static_cast<int>(m_lines[l_i].size()))
         SDLUtils::renderText(m_lines[l_i].substr(m_camera.x, m_nbVisibleChars), g_fontMono, MARGIN_X, l_y, l_fgColor, l_bgColor, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
}

//------------------------------------------------------------------------------

// Key pressed
void TextViewer::keyPressed(const SDL_Event &event)
{
   // Button Back
   if (BUTTON_PRESSED_BACK)
   {
      // Reset timer
      resetTimer();
      // Close window with no return value
      m_retVal = -2;
      return;
   }
}

//------------------------------------------------------------------------------

// Move camera
void TextViewer::moveCursorUp(const int p_step, bool p_loop)
{
   if (m_nbItems <= m_nbVisibleLines)
      return;
   m_camera.y -= p_step;
   if (m_camera.y < 0)
      m_camera.y = 0;
   adjustScrollbarPosition();
   g_hasChanged = true;
}

void TextViewer::moveCursorDown(const int p_step, bool p_loop)
{
   if (m_nbItems <= m_nbVisibleLines)
      return;
   m_camera.y += p_step;
   if (m_camera.y > m_nbItems - m_nbVisibleLines)
      m_camera.y = m_nbItems - m_nbVisibleLines;
   adjustScrollbarPosition();
   g_hasChanged = true;
}

void TextViewer::moveCursorLeft(const int p_step, bool p_loop)
{
   if (m_camera.x > 0)
      --m_camera.x;
   g_hasChanged = true;
}

void TextViewer::moveCursorRight(const int p_step, bool p_loop)
{
   ++m_camera.x;
   g_hasChanged = true;
}
