#include <iostream>
#include "textInput.h"
#include "def.h"
#include "sdlutils.h"
#include "keyboard.h"

// Constructor
TextInput::TextInput(const std::string &p_title, SDL_Texture *p_icon, const std::string p_inputText):
   IWindow(false, p_title),
   m_icon(p_icon),
   m_inputText(p_inputText)
{
   // Cursor at the end of the text
   m_cursor = m_inputText.size();
   // Dialog background
   m_dialogBackground.w = SCREEN_WIDTH;
   m_dialogBackground.h = 2*LINE_HEIGHT + DIALOG_BORDER;
   m_dialogBackground.x = (SCREEN_WIDTH - m_dialogBackground.w) / 2;
   m_dialogBackground.y = SCREEN_HEIGHT - Keyboard::getKeyboardH() - m_dialogBackground.h + DIALOG_BORDER;
   // Dialog body
   m_dialogBody.x = m_dialogBackground.x + DIALOG_BORDER;
   m_dialogBody.y = m_dialogBackground.y + LINE_HEIGHT;
   m_dialogBody.w = m_dialogBackground.w - 2*DIALOG_BORDER;
   m_dialogBody.h = m_dialogBackground.h - LINE_HEIGHT - DIALOG_BORDER;
   // Number of visible characters
   m_nbVisibleChars = round(static_cast<double>(m_dialogBody.w - 2*MARGIN_X) / g_charW);
   // Adjust camera
   adjustCamera();
}

//------------------------------------------------------------------------------

// Destructor
TextInput::~TextInput(void)
{
}

//------------------------------------------------------------------------------

// Draw window
void TextInput::render(const bool p_focus)
{
   // Dialog background
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_RenderFillRect(g_renderer, &m_dialogBackground);

   // Dialog icon
   SDLUtils::renderTexture(m_icon, m_dialogBackground.x + MARGIN_X, m_dialogBackground.y + LINE_HEIGHT/2, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Dialog title
   SDLUtils::renderText(m_title, g_font, m_dialogBackground.x + MARGIN_X + ICON_SIZE + MARGIN_X, m_dialogBackground.y + LINE_HEIGHT/2, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Dialog body
   SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
   SDL_RenderFillRect(g_renderer, &m_dialogBody);

   // Input text
   if (! m_inputText.empty())
      SDLUtils::renderText(m_inputText.substr(m_camera.x, m_nbVisibleChars), g_fontMono, m_dialogBody.x + MARGIN_X, m_dialogBody.y + LINE_HEIGHT/2, {COLOR_TEXT_NORMAL}, {COLOR_BODY_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Cursor
   SDL_SetRenderDrawColor(g_renderer, COLOR_TEXT_NORMAL, 255);
   SDL_Rect rect = { m_dialogBody.x + MARGIN_X + (m_cursor - m_camera.x) * g_charW, m_dialogBody.y + 2, 1, LINE_HEIGHT - 4 };
   SDL_RenderFillRect(g_renderer, &rect);
}

//------------------------------------------------------------------------------

// Callbacks for virtual keyboard
void TextInput::keyboardInputChar(const std::string &p_string)
{
   m_inputText.insert(m_cursor, p_string);
   m_cursor++;
   adjustCamera();
   g_hasChanged = true;
}

void TextInput::keyboardBackspace(void)
{
   if (m_cursor > 0 && ! m_inputText.empty())
   {
      m_inputText.erase(m_cursor - 1, 1);
      --m_cursor;
      adjustCamera();
      g_hasChanged = true;
   }
}

void TextInput::keyboardMoveLeft(void)
{
   if (m_cursor > 0)
   {
      --m_cursor;
      adjustCamera();
      g_hasChanged = true;
   }
}

void TextInput::keyboardMoveRight(void)
{
   if (m_cursor < static_cast<int>(m_inputText.size()))
   {
      ++m_cursor;
      adjustCamera();
      g_hasChanged = true;
   }
}

//------------------------------------------------------------------------------

// Execute window and return result
int TextInput::execute(void)
{
   Keyboard keyboard(this, true);
   return keyboard.execute();
}

//------------------------------------------------------------------------------

// Get input text
const std::string &TextInput::getInputText(void) const
{
   return m_inputText;
}

//------------------------------------------------------------------------------

// Adjust camera
void TextInput::adjustCamera(void)
{
   if (static_cast<int>(m_inputText.size()) <= m_nbVisibleChars)
      m_camera.x = 0;
   else if (m_cursor < m_camera.x)
      m_camera.x = m_cursor;
   else if (m_cursor > m_camera.x + m_nbVisibleChars)
      m_camera.x = m_cursor - m_nbVisibleChars;
   else if (m_camera.x > static_cast<int>(m_inputText.size()) - m_nbVisibleChars && m_cursor > static_cast<int>(m_inputText.size()) - m_nbVisibleChars)
      m_camera.x = m_inputText.size() - m_nbVisibleChars;
}
