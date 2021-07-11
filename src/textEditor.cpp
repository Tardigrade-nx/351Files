#include <iostream>
#include <fstream>
#include "textEditor.h"
#include "def.h"
#include "sdlutils.h"
#include "keyboard.h"

//------------------------------------------------------------------------------

// Constructor
TextEditor::TextEditor(const std::string &p_title):
   IWindow(true, p_title),
   m_oldX(0)
{
   // Init cursor
   m_inputTextCursor.x = 0;
   m_inputTextCursor.y = 0;
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
   }
   ifs.close();
   // Number of lines
   m_nbItems = m_lines.size();
   // Init scrollbar
   adjustScrollbar();
   // Number of visible chars
   m_nbVisibleChars = round(static_cast<double>(SCREEN_WIDTH - 2*MARGIN_X - m_scrollbar.w) / g_charW);
}

//------------------------------------------------------------------------------

// Destructor
TextEditor::~TextEditor(void)
{
}

//------------------------------------------------------------------------------

// Draw window
void TextEditor::render(const bool p_focus)
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
   SDLUtils::renderTexture(g_iconEdit, MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
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

   // Render cursor
   SDL_SetRenderDrawColor(g_renderer, COLOR_TEXT_NORMAL, 255);
   rect.w = 1;
   rect.h = LINE_HEIGHT;
   rect.x = MARGIN_X + (m_inputTextCursor.x - m_camera.x) * g_charW;
   rect.y = LINE_HEIGHT + (m_inputTextCursor.y - m_camera.y) * LINE_HEIGHT;
   SDL_RenderFillRect(g_renderer, &rect);
}

//------------------------------------------------------------------------------

// Key pressed
void TextEditor::keyPressed(const SDL_Event &event)
{
   // Button validate => open keyboard
   if (BUTTON_PRESSED_VALIDATE)
   {
      // Reset timer
      resetTimer();
      // Open keyboard
      Keyboard keyboard(this, false);
      keyboard.execute();
      return;
   }
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

// Move input cursor

void TextEditor::moveCursorUp(const int p_step, bool p_loop)
{
   if (m_inputTextCursor.y <= 0)
      return;
   // Previous line
   m_inputTextCursor.y -= p_step;
   if (m_inputTextCursor.y < 0)
      m_inputTextCursor.y = 0;
   // Adjust x
   m_inputTextCursor.x = m_oldX;
   if (m_inputTextCursor.x > static_cast<int>(m_lines[m_inputTextCursor.y].size()))
      m_inputTextCursor.x = m_lines[m_inputTextCursor.y].size();
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
   // Redraw
   g_hasChanged = true;
}

void TextEditor::moveCursorDown(const int p_step, bool p_loop)
{
   if (m_inputTextCursor.y >= static_cast<int>(m_lines.size()) - 1)
      return;
   // Next line
   m_inputTextCursor.y += p_step;
   if (m_inputTextCursor.y > m_nbItems - 1)
      m_inputTextCursor.y = m_nbItems - 1;
   // Adjust x
   m_inputTextCursor.x = m_oldX;
   if (m_inputTextCursor.x > static_cast<int>(m_lines[m_inputTextCursor.y].size()))
      m_inputTextCursor.x = m_lines[m_inputTextCursor.y].size();
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
   // Redraw
   g_hasChanged = true;
}

void TextEditor::moveCursorLeft(const int p_step, bool p_loop)
{
   if (m_inputTextCursor.x <= 0)
   {
      if (m_inputTextCursor.y <= 0)
         return;
      // Go to the end of previous line
      --m_inputTextCursor.y;
      m_inputTextCursor.x = m_lines[m_inputTextCursor.y].size();
   }
   else
   {
      --m_inputTextCursor.x;
   }
   m_oldX = m_inputTextCursor.x;
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
   // Redraw
   g_hasChanged = true;
}

void TextEditor::moveCursorRight(const int p_step, bool p_loop)
{
   if (m_inputTextCursor.x >= static_cast<int>(m_lines[m_inputTextCursor.y].size()))
   {
      // Go to the start of next line
      if (m_inputTextCursor.y >= static_cast<int>(m_lines.size()) - 1)
         return;
      ++m_inputTextCursor.y;
      m_inputTextCursor.x = 0;
   }
   else
   {
      ++m_inputTextCursor.x;
   }
   m_oldX = m_inputTextCursor.x;
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
   // Redraw
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Adjust camera
void TextEditor::adjustCamera(void)
{
   // Adjust camera Y
   if (m_inputTextCursor.y < m_camera.y)
      m_camera.y = m_inputTextCursor.y;
   else if (m_inputTextCursor.y > m_camera.y + m_nbVisibleLines - 1)
      m_camera.y = m_inputTextCursor.y - m_nbVisibleLines + 1;

   // Adjust camera X
   if (MARGIN_X + (m_inputTextCursor.x - m_camera.x) * g_charW > SCREEN_WIDTH - m_scrollbar.w - MARGIN_X)
      m_camera.x = m_inputTextCursor.x - ((SCREEN_WIDTH - m_scrollbar.w - 2*MARGIN_X) / g_charW);
   else if ((m_inputTextCursor.x - m_camera.x) * g_charW < 0)
      m_camera.x = m_inputTextCursor.x;
}

//------------------------------------------------------------------------------

// Callbacks for virtual keyboard
void TextEditor::keyboardInputChar(const std::string &p_string)
{
   m_lines[m_inputTextCursor.y].insert(m_inputTextCursor.x, p_string);
   ++m_inputTextCursor.x;
   m_oldX = m_inputTextCursor.x;
   adjustCamera();
   g_hasChanged = true;
}

void TextEditor::keyboardInputEnter(void)
{
   // Insert new line
   m_lines.insert(m_lines.begin() + m_inputTextCursor.y + 1, m_lines[m_inputTextCursor.y].substr(m_inputTextCursor.x));
   // Cut current line
   m_lines[m_inputTextCursor.y].erase(m_inputTextCursor.x);
   // Number of lines
   m_nbItems = m_lines.size();
   // Init scrollbar
   adjustScrollbar();
   // Cursor
   ++m_inputTextCursor.y;
   m_inputTextCursor.x = 0;
   m_oldX = m_inputTextCursor.x;
   adjustCamera();
   g_hasChanged = true;
}

void TextEditor::keyboardBackspace(void)
{
   if (m_inputTextCursor.x <= 0)
   {
      if (m_inputTextCursor.y <= 0)
         return;
      // Move cursor
      --m_inputTextCursor.y;
      m_inputTextCursor.x = m_lines[m_inputTextCursor.y].size();
      m_oldX = m_inputTextCursor.x;
      // Append current line to the previous one
      m_lines[m_inputTextCursor.y].append(m_lines[m_inputTextCursor.y + 1]);
      // Remove current line
      m_lines.erase(m_lines.begin() + m_inputTextCursor.y + 1);
      m_nbItems = m_lines.size();
      adjustScrollbar();
   }
   else
   {
      // Remove previous character in the line
      m_lines[m_inputTextCursor.y].erase(m_inputTextCursor.x - 1, 1);
      --m_inputTextCursor.x;
      m_oldX = m_inputTextCursor.x;
   }
   adjustCamera();
   g_hasChanged = true;
}

void TextEditor::keyboardMoveLeft(void)
{
   moveCursorLeft(1, false);
}

void TextEditor::keyboardMoveRight(void)
{
   moveCursorRight(1, false);
}
