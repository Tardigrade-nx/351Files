#include <iostream>
#include <fstream>
#include "textEditor.h"
#include "def.h"
#include "sdlutils.h"
#include "keyboard.h"
#include "dialog.h"

//------------------------------------------------------------------------------

// Constructor
TextEditor::TextEditor(const std::string &p_title):
   IWindow(true, p_title),
   m_oldX(0),
   m_hasModifications(false)
{
   // Init cursor
   m_inputTextCursor.x = 0;
   m_inputTextCursor.y = 0;
   // Text selection
   unselectText();
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
   SDL_Color l_bgColorSelect = {COLOR_CURSOR_FOCUS};
   std::string subLine = "";
   int nbCharPart1 = 0, nbCharPart2 = 0, nbCharPart3 = 0;
   for (int l_i = m_camera.y; l_i < m_camera.y + m_nbVisibleLines && l_i < m_nbItems; ++l_i, l_y += LINE_HEIGHT)
   {
      // Case : nothing visible on this line
      if (m_camera.x >= static_cast<int>(m_lines[l_i].size()))
         continue;
      // Case: no text selection
      if (m_textSelectionStart.y == -1 || m_textSelectionEnd.y == -1)
      {
         SDLUtils::renderText(m_lines[l_i].substr(m_camera.x, m_nbVisibleChars), g_fontMono, MARGIN_X, l_y, l_fgColor, l_bgColor, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
         continue;
      }
      // Case: text selection
      subLine = m_lines[l_i].substr(m_camera.x, m_nbVisibleChars);
      nbCharPart1 = 0;
      nbCharPart2 = 0;
      nbCharPart3 = 0;
      getNbSelectedChars(l_i, subLine.size(), nbCharPart1, nbCharPart2);
      nbCharPart3 = subLine.size() - nbCharPart1 - nbCharPart2;
      // Render line in 3 parts : unselected / selected / unselected
      if (nbCharPart1 > 0)
         SDLUtils::renderText(subLine.substr(0, nbCharPart1), g_fontMono, MARGIN_X, l_y, l_fgColor, l_bgColor, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      if (nbCharPart2 > 0 && ! subLine.substr(nbCharPart1, nbCharPart2).empty())
         SDLUtils::renderText(subLine.substr(nbCharPart1, nbCharPart2), g_fontMono, MARGIN_X + nbCharPart1 * g_charW, l_y, l_fgColor, l_bgColorSelect, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      if (nbCharPart3 > 0 && ! subLine.substr(nbCharPart1 + nbCharPart2, nbCharPart3).empty())
         SDLUtils::renderText(subLine.substr(nbCharPart1 + nbCharPart2, nbCharPart3), g_fontMono, MARGIN_X + (nbCharPart1 + nbCharPart2) * g_charW, l_y, l_fgColor, l_bgColor, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
   }

   // Render cursor
   SDL_SetRenderDrawColor(g_renderer, COLOR_TEXT_NORMAL, 255);
   rect.w = 1;
   rect.h = LINE_HEIGHT - 4;
   rect.x = MARGIN_X + (m_inputTextCursor.x - m_camera.x) * g_charW;
   rect.y = LINE_HEIGHT + 2 +(m_inputTextCursor.y - m_camera.y) * LINE_HEIGHT;
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
      // If the keyboard hides the cursor, move the camera to make it visible
      if ((m_inputTextCursor.y - m_camera.y + 2) * LINE_HEIGHT > SCREEN_HEIGHT - Keyboard::getKeyboardH())
      {
         m_camera.y = m_inputTextCursor.y - 2;
         adjustScrollbarPosition();
      }
      // Open keyboard
      Keyboard keyboard(this, false);
      keyboard.execute();
      return;
   }
   // Button context menu
   if (BUTTON_PRESSED_MENU_CONTEXT)
   {
      // Reset timer
      resetTimer();
      // Open action menu
      Dialog dialog("Actions:");
      dialog.addOption("Save", 0, g_iconFloppy);
      if (m_textSelectionStart.y != -1 && m_textSelectionEnd.y != -1 && (m_textSelectionStart.x != m_textSelectionEnd.x || m_textSelectionStart.y != m_textSelectionEnd.y))
      {
         dialog.addOption("Copy", 4, g_iconCopy);
         dialog.addOption("Cut", 5, g_iconCut);
      }
      if (! m_clipboard.empty())
         dialog.addOption("Paste", 6, g_iconPaste);
      dialog.addOption("Delete line", 1, g_iconTrash);
      dialog.addOption("Duplicate line", 2, g_iconPlus);
      dialog.addOption("Quit", 3, g_iconQuit);
      switch (dialog.execute())
      {
         case 0: save(); break;
         case 1: deleteLine(); break;
         case 2: duplicateLine(); break;
         case 3: quit(); break;
         case 4: copy(); break;
         case 5: cut(); break;
         case 6: paste(); break;
         default: break;
      }
      return;
   }
   // Button Back
   if (BUTTON_PRESSED_BACK)
   {
      // Reset timer
      resetTimer();
      // Quit
      quit();
      return;
   }
   // Button select
   if (BUTTON_PRESSED_SELECT)
   {
      resetTimer();
      unselectText();
      m_textSelectionStart = m_inputTextCursor;
      g_hasChanged = true;
      return;
   }
}

//------------------------------------------------------------------------------

// Move input cursor

void TextEditor::moveCursorUp(const int p_step, bool p_loop)
{
   // Selection = none
   if (! BUTTON_HELD_SELECT)
      unselectText();
   g_hasChanged = true;
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
   // Text selection
   if (BUTTON_HELD_SELECT)
      m_textSelectionEnd = m_inputTextCursor;
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
}

void TextEditor::moveCursorDown(const int p_step, bool p_loop)
{
   // Selection = none
   if (! BUTTON_HELD_SELECT)
      unselectText();
   g_hasChanged = true;
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
   // Text selection
   if (BUTTON_HELD_SELECT)
      m_textSelectionEnd = m_inputTextCursor;
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
}

void TextEditor::moveCursorLeft(const int p_step, bool p_loop)
{
   // Selection = none
   if (! BUTTON_HELD_SELECT)
      unselectText();
   g_hasChanged = true;
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
   // Text selection
   if (BUTTON_HELD_SELECT)
      m_textSelectionEnd = m_inputTextCursor;
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
}

void TextEditor::moveCursorRight(const int p_step, bool p_loop)
{
   // Selection = none
   if (! BUTTON_HELD_SELECT)
      unselectText();
   g_hasChanged = true;
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
   // Text selection
   if (BUTTON_HELD_SELECT)
      m_textSelectionEnd = m_inputTextCursor;
   // Camera
   adjustCamera();
   // Scrollbar
   adjustScrollbarPosition();
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
   // Remove selected text if any
   if (m_textSelectionStart.y != -1 && m_textSelectionEnd.y != -1 && (m_textSelectionStart.x != m_textSelectionEnd.x || m_textSelectionStart.y != m_textSelectionEnd.y))
      removeSelectedText();
   // Selection = none
   unselectText();
   // Insert character
   m_lines[m_inputTextCursor.y].insert(m_inputTextCursor.x, p_string);
   ++m_inputTextCursor.x;
   m_oldX = m_inputTextCursor.x;
   m_hasModifications = true;
   adjustCamera();
   g_hasChanged = true;
}

void TextEditor::keyboardInputEnter(void)
{
   // Remove selected text if any
   if (m_textSelectionStart.y != -1 && m_textSelectionEnd.y != -1 && (m_textSelectionStart.x != m_textSelectionEnd.x || m_textSelectionStart.y != m_textSelectionEnd.y))
      removeSelectedText();
   // Selection = none
   unselectText();
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
   m_hasModifications = true;
   adjustCamera();
   g_hasChanged = true;
}

void TextEditor::keyboardBackspace(void)
{
   // Remove selected text if any
   if (m_textSelectionStart.y != -1 && m_textSelectionEnd.y != -1 && (m_textSelectionStart.x != m_textSelectionEnd.x || m_textSelectionStart.y != m_textSelectionEnd.y))
   {
      removeSelectedText();
      return;
   }
   unselectText();
   g_hasChanged = true;
   if (m_inputTextCursor.x <= 0)
   {
      if (m_inputTextCursor.y <= 0)
         return;
      // Move cursor
      --m_inputTextCursor.y;
      m_inputTextCursor.x = m_lines[m_inputTextCursor.y].size();
      // Append current line to the previous one
      m_lines[m_inputTextCursor.y].append(m_lines[m_inputTextCursor.y + 1]);
      // Remove current line
      m_lines.erase(m_lines.begin() + m_inputTextCursor.y + 1);
      m_nbItems = m_lines.size();
   }
   else
   {
      // Remove previous character in the line
      m_lines[m_inputTextCursor.y].erase(m_inputTextCursor.x - 1, 1);
      --m_inputTextCursor.x;
   }
   m_oldX = m_inputTextCursor.x;
   m_hasModifications = true;
   adjustScrollbar();
   adjustCamera();
}

void TextEditor::keyboardMoveLeft(void)
{
   moveCursorLeft(1, false);
}

void TextEditor::keyboardMoveRight(void)
{
   moveCursorRight(1, false);
}

//------------------------------------------------------------------------------

// Save file
void TextEditor::save(void)
{
   INHIBIT(std::cout << "Saving file '" << m_title << "'...\n";)
   // Open file
   std::ofstream ofs(m_title);
   if (! ofs.is_open())
   {
      Dialog dialog("Error");
      dialog.addLabel("Unable to write file.");
      dialog.addOption("OK", 0, g_iconSelect);
      return;
   }
   // Write new file
   for (auto it = m_lines.begin(); it != m_lines.end(); ++it)
   {
      if (it != m_lines.begin())
         ofs << '\n';
      ofs << *it;
   }
   ofs.close();
   m_hasModifications = false;
}

//------------------------------------------------------------------------------

// Quit, with a warning if unsaved modifications
void TextEditor::quit(void)
{
   if (m_hasModifications)
   {
      Dialog dialog("Warning:");
      dialog.addLabel("The file is not saved.");
      dialog.addOption("Save", 0, g_iconFloppy);
      dialog.addOption("Don't save", 1, g_iconNone);
      dialog.addOption("Cancel", 2, g_iconCancel);
      switch (dialog.execute())
      {
         case 0: save(); break;
         case 1: break;
         default: return;
      }
   }
   // Close window with no return value
   m_retVal = -2;
}

//------------------------------------------------------------------------------

// Copy selected text in clipboard
void TextEditor::copy(void)
{
   // If start point is after end point, swap them
   SDL_Point start, end;
   getSortedSelectionPoints(start, end);
   // Copy selected text to clipboard
   m_clipboard.clear();
   for (int indLine = start.y; indLine <= end.y; ++indLine)
   {
      if (start.y == indLine && end.y == indLine)
         m_clipboard.push_back(m_lines[indLine].substr(start.x, end.x - start.x));
      else if (start.y == indLine)
         m_clipboard.push_back(m_lines[indLine].substr(start.x));
      else if (end.y == indLine)
         m_clipboard.push_back(m_lines[indLine].substr(0, end.x));
      else
         m_clipboard.push_back(m_lines[indLine]);
   }
}

//------------------------------------------------------------------------------

// Cut selected text in clipboard
void TextEditor::cut(void)
{
   // Cut = copy then remove
   copy();
   removeSelectedText();
}

//------------------------------------------------------------------------------

// Paste clipboard into text
void TextEditor::paste(void)
{
   // Remove selected text if any
   if (m_textSelectionStart.y != -1 && m_textSelectionEnd.y != -1 && (m_textSelectionStart.x != m_textSelectionEnd.x || m_textSelectionStart.y != m_textSelectionEnd.y))
      removeSelectedText();
   // Rest of the line after insertion
   std::string rest = m_lines[m_inputTextCursor.y].substr(m_inputTextCursor.x);
   m_lines[m_inputTextCursor.y].erase(m_inputTextCursor.x);
   // Insert clipboard lines into text
   int indLine = m_inputTextCursor.y;
   for (auto it = m_clipboard.begin(); it != m_clipboard.end(); ++it)
   {
      if (it == m_clipboard.begin())
      {
         m_lines[indLine].append(*it);
      }
      else
      {
         ++indLine;
         m_lines.insert(m_lines.begin() + indLine, *it);
      }
   }
   // Move cursor to the end of the pasted part
   m_inputTextCursor.y = indLine;
   m_inputTextCursor.x = m_lines[indLine].size();
   m_oldX = m_inputTextCursor.x;
   // Restore end of first line
   m_lines[indLine].append(rest);
   // Update everything
   unselectText();
   m_nbItems = m_lines.size();
   adjustScrollbar();
   adjustCamera();
   m_hasModifications = true;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Delete current line
void TextEditor::deleteLine(void)
{
   unselectText();
   m_lines.erase(m_lines.begin() + m_inputTextCursor.y);
   m_nbItems = m_lines.size();
   adjustScrollbar();
   adjustCamera();
   m_hasModifications = true;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Duplicate current line
void TextEditor::duplicateLine(void)
{
   unselectText();
   m_lines.insert(m_lines.begin() + m_inputTextCursor.y + 1, m_lines[m_inputTextCursor.y]);
   m_nbItems = m_lines.size();
   adjustScrollbar();
   adjustCamera();
   m_hasModifications = true;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// For a line, get the number of unselected and selected chars
void TextEditor::getNbSelectedChars(const int p_lineIndex, const int p_lineSize, int &p_nbUnselected, int &p_nbSelected)
{
   // If start point is after end point, swap them
   SDL_Point start, end;
   getSortedSelectionPoints(start, end);
   // Case: selection starts after or ends before the line
   if (start.y > p_lineIndex || end.y < p_lineIndex)
   {
      p_nbUnselected = p_lineSize;
      p_nbSelected = 0;
      return;
   }
   // Case: selection starts before the line and ends after the line
   if (start.y < p_lineIndex && end.y > p_lineIndex)
   {
      p_nbUnselected = 0;
      p_nbSelected = p_lineSize;
      return;
   }
   // Case: selection starts at the line and ends after the line
   if (start.y == p_lineIndex && end.y > p_lineIndex)
   {
      p_nbUnselected = start.x - m_camera.x;
      if (p_nbUnselected < 0) p_nbUnselected = 0;
      else if (p_nbUnselected > p_lineSize) p_nbUnselected = p_lineSize;
      p_nbSelected = p_lineSize - p_nbUnselected;
      return;
   }
   // Case: selection starts before the line and ends at the line
   if (start.y < p_lineIndex && end.y == p_lineIndex)
   {
      p_nbUnselected = 0;
      p_nbSelected = end.x - m_camera.x;
      if (p_nbSelected < 0) p_nbSelected = 0;
      else if (p_nbSelected > p_lineSize) p_nbSelected = p_lineSize;
      return;
   }
   // Case: selection starts and ends at the line
   if (start.y == p_lineIndex && end.y == p_lineIndex)
   {
      p_nbUnselected = start.x - m_camera.x;
      if (p_nbUnselected < 0) p_nbUnselected = 0;
      else if (p_nbUnselected > p_lineSize) p_nbUnselected = p_lineSize;
      p_nbSelected = end.x - start.x;
      if (start.x < m_camera.x)
         p_nbSelected -= m_camera.x - start.x;
      if (p_nbSelected < 0) p_nbSelected = 0;
      else if (p_nbSelected > p_lineSize) p_nbSelected = p_lineSize;
      return;
   }
}

//------------------------------------------------------------------------------

// Unselect text
void TextEditor::unselectText(void)
{
   m_textSelectionStart.x = -1;
   m_textSelectionStart.y = -1;
   m_textSelectionEnd.x = -1;
   m_textSelectionEnd.y = -1;
}

//------------------------------------------------------------------------------

// Remove selected text, if any
void TextEditor::removeSelectedText(void)
{
   // If start point is after end point, swap them
   SDL_Point start, end;
   getSortedSelectionPoints(start, end);
   // Remove selected text
   for (int indLine = start.y, nbErased = 0; indLine <= end.y; ++indLine)
   {
      // Case: remove line completely
      if ((start.y < indLine || (start.y == indLine && start.x == 0)) && end.y > indLine)
      {
         m_lines.erase(m_lines.begin() + indLine - nbErased);
         ++nbErased;
         continue;
      }
      // Case: remove a part of the line
      if (start.y == indLine && end.y == indLine)
      {
         // Remove only part of the line
         m_lines[indLine - nbErased].erase(start.x, end.x - start.x);
         continue;
      }
      // Case: remove end of line
      if (start.y == indLine)
         m_lines[indLine - nbErased].erase(start.x);
      // Case: remove beginning of line
      if (end.y == indLine)
      {
         m_lines[indLine - nbErased].erase(0, end.x);
         if (start.y < indLine - nbErased)
         {
            // Append the rest of the line to the previous line
            m_lines[indLine - nbErased - 1].append(m_lines[indLine - nbErased]);
            // Remove line
            m_lines.erase(m_lines.begin() + indLine - nbErased);
            ++nbErased;
         }
      }
   }
   // Update everything
   m_inputTextCursor = start;
   m_oldX = m_inputTextCursor.x;
   unselectText();
   m_nbItems = m_lines.size();
   adjustScrollbar();
   adjustCamera();
   m_hasModifications = true;
   g_hasChanged = true;
}

//------------------------------------------------------------------------------

// Get start and end points, with start < end
void TextEditor::getSortedSelectionPoints(SDL_Point &p_start, SDL_Point &p_end)
{
   if (m_textSelectionStart.y > m_textSelectionEnd.y || (m_textSelectionStart.y == m_textSelectionEnd.y && m_textSelectionStart.x > m_textSelectionEnd.x))
   {
      // Flip start and end
      p_start = m_textSelectionEnd;
      p_end = m_textSelectionStart;
   }
   else
   {
      p_start = m_textSelectionStart;
      p_end = m_textSelectionEnd;
   }
}
