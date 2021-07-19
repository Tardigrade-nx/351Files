#ifndef _TEXTEDITOR_H_
#define _TEXTEDITOR_H_

#include <string>
#include <vector>
#include <SDL.h>
#include "window.h"

class TextEditor : public IWindow
{
   public:

   // Constructor
   TextEditor(const std::string &p_title);

   // Destructor
   virtual ~TextEditor(void);

   // Draw window
   virtual void render(const bool p_focus);

   private:

   // Forbidden
   TextEditor(const TextEditor &p_source);
   const TextEditor &operator =(const TextEditor &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

   // Callbacks for virtual keyboard
   virtual void keyboardInputChar(const std::string &p_string);
   virtual void keyboardInputEnter(void);
   virtual void keyboardBackspace(void);
   virtual void keyboardMoveLeft(void);
   virtual void keyboardMoveRight(void);

   // Move cursor
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // Adjust camera
   void adjustCamera(void);

   // For a line, get the number of unselected and selected chars
   void getNbSelectedChars(const int p_lineIndex, const int p_lineSize, int &p_nbUnselected, int &p_nbSelected);

   // Text selection
   void unselectText(void);
   void removeSelectedText(void);
   void getSortedSelectionPoints(SDL_Point &p_start, SDL_Point &p_end);

   // Save file
   void save(void);

   // Quit, with a warning if unsaved modifications
   void quit(void);

   // Copy selected text in clipboard
   void copy(void);

   // Cut selected text in clipboard
   void cut(void);

   // Paste clipboard into text
   void paste(void);

   // Delete current line
   void deleteLine(void);

   // Duplicate current line
   void duplicateLine(void);

   // List of lines
   std::vector<std::string> m_lines;

   // Input cursor
   SDL_Point m_inputTextCursor;

   // X saved from previous line
   int m_oldX;

   // Has modifications
   bool m_hasModifications;

   // Start and end for text selection
   SDL_Point m_textSelectionStart;
   SDL_Point m_textSelectionEnd;

   // Clipboard containing lines, for copying or moving
   std::vector<std::string> m_clipboard;
};

#endif
