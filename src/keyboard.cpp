#include <iostream>
#include <sstream>
#include "keyboard.h"
#include "def.h"
#include "sdlutils.h"

// Constructor
Keyboard::Keyboard(IWindow *p_parent, const bool p_quitOnEnter):
   IWindow(false, ""),
   m_parent(p_parent),
   m_background(NULL),
   m_keyLabelCurrent(0),
   m_texShiftEmpty(NULL),
   m_texShiftFull(NULL),
   m_texEnter(NULL),
   m_texArrow(NULL),
   m_texBackspace(NULL),
   m_quitOnEnter(p_quitOnEnter)
{
   m_cursorLoop = true;
   m_keyLabel[0] = "qwertyuiop asdfghjkl  zxcvbnm_.";
   m_keyLabel[1] = "QWERTYUIOP ASDFGHJKL  ZXCVBNM-,";
   m_keyLabel[2] = "1234567890 .,:!?/\\\"'  ()[]<>_;$";
   m_keyLabel[3] = "1234567890 ()[]{}~|^  @#%&*-+=`";
   std::ostringstream oss;
   oss << '/' << KEYBOARD_SYMBOL_SIZE;
   m_texShiftEmpty =    SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/keyboard_shift_empty.png");
   m_texShiftFull =     SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/keyboard_shift_full.png");
   m_texEnter =         SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/keyboard_enter.png");
   m_texArrow =         SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/keyboard_arrow.png");
   m_texBackspace =     SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/keyboard_backspace.png");
   init();
}

//------------------------------------------------------------------------------

// Destructor
Keyboard::~Keyboard(void)
{
   if (m_texShiftEmpty != NULL)     { SDL_DestroyTexture(m_texShiftEmpty); m_texShiftEmpty = NULL; }
   if (m_texShiftFull != NULL)      { SDL_DestroyTexture(m_texShiftFull); m_texShiftFull = NULL; }
   if (m_texEnter != NULL)          { SDL_DestroyTexture(m_texEnter); m_texEnter = NULL; }
   if (m_texArrow != NULL)          { SDL_DestroyTexture(m_texArrow); m_texArrow = NULL; }
   if (m_texBackspace != NULL)      { SDL_DestroyTexture(m_texBackspace); m_texBackspace = NULL; }
   if (m_background != NULL)        { SDL_DestroyTexture(m_background); m_background = NULL; }
}

//------------------------------------------------------------------------------

// Draw window
void Keyboard::render(const bool p_focus)
{
   // Keyboard background
   SDL_RenderCopy(g_renderer, m_background, NULL, &m_keyboard);
   // Cursor
   if (p_focus)
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_FOCUS, 255);
   else
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_NO_FOCUS, 255);
   SDL_Rect rect = m_key[m_cursor];
   rect.y += m_keyboard.y;
   SDL_RenderFillRect(g_renderer, &rect);
   // Draw key labels
   for (int ind = 0; ind <= 31; ++ind)
   {
      if (m_keyLabel[m_keyLabelCurrent].substr(ind, 1).empty())
         continue;
      SDLUtils::renderText(m_keyLabel[m_keyLabelCurrent].substr(ind, 1), g_font, m_keyboard.x + m_key[ind].x + m_key[ind].w / 2, m_keyboard.y + m_key[ind].y + m_key[ind].h / 2, {COLOR_TEXT_NORMAL}, getBackgroundColor(ind, p_focus), SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   }
   // Backspace
   SDLUtils::renderTexture(m_texBackspace, m_keyboard.x + m_key[10].x + m_key[10].w / 2, m_keyboard.y + m_key[10].y + m_key[10].h / 2, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   // Enter
   SDLUtils::renderTexture(m_texEnter, m_keyboard.x + m_key[20].x + m_key[20].w / 2, m_keyboard.y + m_key[20].y + m_key[20].h / 2, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   // Shift
   SDLUtils::renderTexture(m_keyLabelCurrent == 1 ? m_texShiftFull : m_texShiftEmpty, m_keyboard.x + m_key[21].x + m_key[21].w / 2, m_keyboard.y + m_key[21].y + m_key[21].h / 2, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   SDLUtils::renderTexture(m_keyLabelCurrent == 1 ? m_texShiftFull : m_texShiftEmpty, m_keyboard.x + m_key[31].x + m_key[31].w / 2, m_keyboard.y + m_key[31].y + m_key[31].h / 2, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   // Arrows
   SDLUtils::renderTexture(m_texArrow, m_keyboard.x + m_key[34].x + m_key[34].w / 2, m_keyboard.y + m_key[34].y + m_key[34].h / 2, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   SDLUtils::renderTexture(m_texArrow, m_keyboard.x + m_key[35].x + m_key[35].w / 2, m_keyboard.y + m_key[35].y + m_key[35].h / 2, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE, SDL_FLIP_HORIZONTAL);
   // Number / symbols
   SDLUtils::renderText(m_keyLabelCurrent == 3 ? "abc" : "&123", g_font, m_keyboard.x + m_key[32].x + m_key[32].w / 2, m_keyboard.y + m_key[32].y + m_key[32].h / 2, {COLOR_TEXT_NORMAL}, getBackgroundColor(32, p_focus), SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
}

//------------------------------------------------------------------------------

// Key pressed
void Keyboard::keyPressed(const SDL_Event &event)
{
   // Button Validate
   if (BUTTON_PRESSED_VALIDATE)
   {
      // Input key
      input();
      m_lastPressed = BUTTON_VALIDATE;
      m_timer = KEYHOLD_TIMER_FIRST;
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

// Specific key held
bool Keyboard::keyHeld(void)
{
   if (BUTTON_HELD_VALIDATE)
   {
      if (m_lastPressed == BUTTON_VALIDATE && m_timer > 0 && --m_timer == 0)
      {
         input();
         m_timer = KEYHOLD_TIMER;
      }
      return true;
   }
   return false;
}

//------------------------------------------------------------------------------

// Input key
void Keyboard::input(void)
{
   // Character key
   if ((m_cursor >= 0 && m_cursor <= 9) || (m_cursor >= 11 && m_cursor <= 19) || (m_cursor >= 22 && m_cursor <= 30))
   {
      // Call parent callback
      m_parent->keyboardInputChar(m_keyLabel[m_keyLabelCurrent].substr(m_cursor, 1));
   }
   // Backspace
   else if (m_cursor == 10)
   {
      // Call parent callback
      m_parent->keyboardBackspace();
   }
   // Enter
   else if (m_cursor == 20)
   {
      if (m_quitOnEnter)
         m_retVal = 0;
      else
         // Call parent callback
         m_parent->keyboardInputEnter();
   }
   // Space
   else if (m_cursor == 33)
   {
      // Call parent callback
      m_parent->keyboardInputChar(" ");
   }
   // Shift
   else if (m_cursor == 21 || m_cursor == 31)
   {
      keyPressedShift();
   }
   // Symbol
   else if (m_cursor == 32)
   {
      keyPressedSymbol();
   }
   // Left arrow
   else if (m_cursor == 34)
   {
      // Call parent callback
      m_parent->keyboardMoveLeft();
   }
   // Right arrow
   else if (m_cursor == 35)
   {
      // Call parent callback
      m_parent->keyboardMoveRight();
   }
}

//------------------------------------------------------------------------------

// Create background image and init the keyboard
void Keyboard::init(void)
{
   if (m_background != NULL)
      return;

   // Size and coordinates of the first key
   m_key[0].w = getKeyW();
   m_key[0].h = getKeyH();
   m_key[0].x = round((SCREEN_WIDTH - (11*m_key[0].w + 10*KEYBOARD_KEY_SPACING)) / 2);
   m_key[0].y = KEYBOARD_MARGIN;

   // Height of all the keys
   int indKey = 1;
   for (; indKey < 36; ++indKey)
      m_key[indKey].h = m_key[0].h;

   // Size and coordinates of the keyboard
   m_keyboard.w = getKeyboardW();
   m_keyboard.h = getKeyboardH();
   m_keyboard.x = 0;
   m_keyboard.y = SCREEN_HEIGHT - m_keyboard.h;

   // Keys: 1st line
   for (indKey = 1; indKey <= 10; ++indKey)
   {
      m_key[indKey].x = m_key[indKey - 1].x + m_key[indKey - 1].w + KEYBOARD_KEY_SPACING;
      m_key[indKey].y = m_key[0].y;
      m_key[indKey].w = m_key[0].w;
   }

   // Keys: 2nd line
   m_key[11].w = m_key[0].w;
   m_key[11].x = m_key[0].x + round(m_key[0].w/3);
   m_key[11].y = m_key[0].y + m_key[0].h + KEYBOARD_KEY_SPACING;
   for (indKey = 12; indKey <= 20; ++indKey)
   {
      m_key[indKey].x = m_key[indKey - 1].x + m_key[indKey - 1].w + KEYBOARD_KEY_SPACING;
      m_key[indKey].y = m_key[11].y;
      if (indKey == 20)
         // Enter key
         m_key[indKey].w = 2*m_key[0].w + KEYBOARD_KEY_SPACING - (m_key[11].x - m_key[0].x);
      else
         m_key[indKey].w = m_key[0].w;
   }

   // Keys: 3rd line
   m_key[21].w = m_key[0].w;
   m_key[21].x = m_key[0].x;
   m_key[21].y = m_key[11].y + m_key[0].h + KEYBOARD_KEY_SPACING;
   for (indKey = 22; indKey <= 31; ++indKey)
   {
      m_key[indKey].x = m_key[indKey - 1].x + m_key[indKey - 1].w + KEYBOARD_KEY_SPACING;
      m_key[indKey].y = m_key[21].y;
      m_key[indKey].w = m_key[0].w;
   }

   // Keys: 4th line
   m_key[32].w = m_key[0].w + m_key[11].x - KEYBOARD_MARGIN;
   m_key[32].x = m_key[0].x;
   m_key[32].y = m_key[21].y + m_key[0].h + KEYBOARD_KEY_SPACING;
   m_key[33].w = 9*m_key[0].w + 7*KEYBOARD_KEY_SPACING - m_key[32].w;
   m_key[33].x = m_key[32].x + m_key[32].w + KEYBOARD_KEY_SPACING;
   m_key[33].y = m_key[32].y;
   for (indKey = 34; indKey <= 35; ++indKey)
   {
      m_key[indKey].x = m_key[indKey - 1].x + m_key[indKey - 1].w + KEYBOARD_KEY_SPACING;
      m_key[indKey].y = m_key[32].y;
      m_key[indKey].w = m_key[0].w;
   }

   // Create background image
   m_background = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_keyboard.w, m_keyboard.h);
   SDL_SetRenderTarget(g_renderer, m_background);
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_RenderClear(g_renderer);

   // Draw keys
   for (indKey = 0; indKey < 36; ++indKey)
   {
      if (indKey == 10 || indKey == 20 || indKey == 21 || indKey == 31 || indKey == 32 || indKey == 34 || indKey == 35)
         SDL_SetRenderDrawColor(g_renderer, COLOR_KEYBOARD_DARK, 255);
      else
         SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
      SDL_RenderFillRect(g_renderer, &m_key[indKey]);
   }

   // Restore renderer
   SDL_SetRenderTarget(g_renderer, NULL);
}

//------------------------------------------------------------------------------

// Move cursor up
void Keyboard::moveCursorUp(const int p_step, bool p_loop)
{
   // Page up => symbol button
   if (p_step > 1)
   {
      keyPressedSymbol();
      return;
   }
   // 1st line
   if (m_cursor >= 0 && m_cursor <= 10)
   {
      if (! p_loop) return;
      if (m_cursor == 0) m_cursor = 32;
      else if (m_cursor == 9 || m_cursor == 10) m_cursor += 25;
      else m_cursor = 33;
      g_hasChanged = true;
      return;
   }
   // 2nd line
   if (m_cursor >= 11 && m_cursor <= 20)
   {
      m_cursor -= 11;
      g_hasChanged = true;
      return;
   }
   // 3rd line
   if (m_cursor >= 21 && m_cursor <= 31)
   {
      if (m_cursor == 31) m_cursor = 20;
      else m_cursor -= 10;
      g_hasChanged = true;
      return;
   }
   // 4th line
   if (m_cursor == 32) { m_cursor = 21; g_hasChanged = true; return; }
   if (m_cursor == 33) { m_cursor = 26; g_hasChanged = true; return; }
   if (m_cursor == 34) { m_cursor = 30; g_hasChanged = true; return; }
   if (m_cursor == 35) { m_cursor = 31; g_hasChanged = true; return; }
}

//------------------------------------------------------------------------------

// Move cursor down
void Keyboard::moveCursorDown(const int p_step, bool p_loop)
{
   // Page down => shift button
   if (p_step > 1)
   {
      keyPressedShift();
      return;
   }
   // 1st line
   if (m_cursor >= 0 && m_cursor <= 10)
   {
      if (m_cursor == 10) m_cursor = 20;
      else m_cursor += 11;
      g_hasChanged = true;
      return;
   }
   // 2nd line
   if (m_cursor >= 11 && m_cursor <= 20)
   {
      m_cursor += 10;
      g_hasChanged = true;
      return;
   }
   // 3rd line
   if (m_cursor >= 21 && m_cursor <= 31)
   {
      if (m_cursor == 21) m_cursor = 32;
      else if (m_cursor == 30 || m_cursor == 31) m_cursor += 4;
      else m_cursor = 33;
      g_hasChanged = true;
      return;
   }
   // 4th line
   if (m_cursor >= 32 && m_cursor <= 35)
   {
      if (!p_loop) return;
      if (m_cursor == 32) m_cursor = 0;
      else if (m_cursor == 34 || m_cursor == 35) m_cursor -= 25;
      else m_cursor = 5;
      g_hasChanged = true;
      return;
   }
}

//------------------------------------------------------------------------------

// Move cursor left
void Keyboard::moveCursorLeft(const int p_step, bool p_loop)
{
   if (!p_loop && (m_cursor == 0 || m_cursor == 11 || m_cursor == 21 || m_cursor == 32)) return;
   if (m_cursor == 0) m_cursor = 10;
   else if (m_cursor == 11) m_cursor = 20;
   else if (m_cursor == 21) m_cursor = 31;
   else if (m_cursor == 32) m_cursor = 35;
   else --m_cursor;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Move cursor right
void Keyboard::moveCursorRight(const int p_step, bool p_loop)
{
   if (!p_loop && (m_cursor == 10 || m_cursor == 20 || m_cursor == 31 || m_cursor == 35)) return;
   if (m_cursor == 10) m_cursor = 0;
   else if (m_cursor == 20) m_cursor = 11;
   else if (m_cursor == 31) m_cursor = 21;
   else if (m_cursor == 35) m_cursor = 32;
   else ++m_cursor;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Get background color for the item at the given index
SDL_Color Keyboard::getBackgroundColor(const int p_i, const bool p_focus) const
{
   if (m_cursor == p_i)
   {
      if (p_focus)
         return {COLOR_CURSOR_FOCUS};
      return {COLOR_CURSOR_NO_FOCUS};
   }
   if (p_i == 10 || p_i == 20 || p_i == 21 || p_i == 31 || p_i == 32 || p_i == 34 || p_i == 35)
      return {COLOR_KEYBOARD_DARK};
   return {COLOR_BODY_BG};
}

//------------------------------------------------------------------------------

// Key and keyboard size
int Keyboard::getKeyW(void)
{
   return round((SCREEN_WIDTH - 2*KEYBOARD_MARGIN - 10*KEYBOARD_KEY_SPACING) / 11);
}

int Keyboard::getKeyH(void)
{
   return round(getKeyW() / 1.2);
}

int Keyboard::getKeyboardW(void)
{
   return SCREEN_WIDTH;
}

int Keyboard::getKeyboardH(void)
{
   return 2*KEYBOARD_MARGIN + 3*KEYBOARD_KEY_SPACING + 4*getKeyH();
}

//------------------------------------------------------------------------------

// Press symbol button
void Keyboard::keyPressedSymbol(void)
{
   switch (m_keyLabelCurrent)
   {
      case 2:  m_keyLabelCurrent = 3; break;
      case 3:  m_keyLabelCurrent = 0; break;
      default: m_keyLabelCurrent = 2; break;
   }
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Press shift button
void Keyboard::keyPressedShift(void)
{
   if (m_keyLabelCurrent == 1)
      m_keyLabelCurrent = 0;
   else
      m_keyLabelCurrent = 1;
   g_hasChanged = true;
}
