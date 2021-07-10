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
   SDL_Rect rectBackground;
   rectBackground.w = SCREEN_WIDTH - 2*DIALOG_BORDER;
   rectBackground.h = 2*LINE_HEIGHT + DIALOG_BORDER;
   rectBackground.x = (SCREEN_WIDTH - rectBackground.w) / 2;
   rectBackground.y = SCREEN_HEIGHT - Keyboard::getKeyboardH() - rectBackground.h - DIALOG_BORDER;
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_RenderFillRect(g_renderer, &rectBackground);

   // Dialog icon
   SDLUtils::renderTexture(m_icon, rectBackground.x + MARGIN_X, rectBackground.y + LINE_HEIGHT/2, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Dialog title
   SDLUtils::renderText(m_title, g_font, rectBackground.x + MARGIN_X + ICON_SIZE + MARGIN_X, rectBackground.y + LINE_HEIGHT/2, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Dialog body
   SDL_Rect rectBody;
   rectBody.x = rectBackground.x + DIALOG_BORDER;
   rectBody.y = rectBackground.y + LINE_HEIGHT;
   rectBody.w = rectBackground.w - 2*DIALOG_BORDER;
   rectBody.h = rectBackground.h - LINE_HEIGHT - DIALOG_BORDER;
   SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
   SDL_RenderFillRect(g_renderer, &rectBody);

   // Input text
   if (! m_inputText.empty())
      SDLUtils::renderText(m_inputText, g_fontMono, rectBody.x + MARGIN_X, rectBody.y + LINE_HEIGHT/2, {COLOR_TEXT_NORMAL}, {COLOR_BODY_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE, rectBody.w - 2*MARGIN_X, SDLUtils::T_ALIGN_RIGHT);
}

//------------------------------------------------------------------------------

// Callbacks for virtual keyboard
void TextInput::keyboardInputChar(const std::string &p_string)
{
   m_inputText.append(p_string);
   g_hasChanged = true;
}

void TextInput::keyboardBackspace(void)
{
   if (! m_inputText.empty())
   {
      m_inputText.pop_back();
      g_hasChanged = true;
   }
}

void TextInput::keyboardMoveLeft(void)
{
}

void TextInput::keyboardMoveRight(void)
{
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
