#include "dialog.h"
#include "def.h"
#include "sdlutils.h"

// Constructor
Dialog::Dialog(const std::string &p_title):
   IWindow(false, p_title)
{
   m_cursorLoop = true;
}

//------------------------------------------------------------------------------

// Destructor
Dialog::~Dialog(void)
{
}

//------------------------------------------------------------------------------

// Draw window
void Dialog::render(const bool p_focus)
{
   // Render all textures first, in order to know the dialog size
   int l_w = 0, l_largestW = 0;

   // Render title
   SDL_Texture *l_textureTitle = SDLUtils::renderText(m_title, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG});
   SDL_QueryTexture(l_textureTitle, NULL, NULL, &l_largestW, NULL);

   // Render labels
   std::vector<std::string>::iterator l_it;
   std::vector<SDL_Texture *> l_texLabels;
   for (l_it = m_labels.begin(); l_it != m_labels.end(); ++l_it)
   {
      // Render text
      l_texLabels.push_back(SDLUtils::renderText(*l_it, {COLOR_TEXT_NORMAL}, {COLOR_BODY_BG}));
      SDL_QueryTexture(l_texLabels.back(), NULL, NULL, &l_w, NULL);
      // Remember largest width
      if (l_w > l_largestW)
         l_largestW = l_w;
   }

   // Render options
   std::vector<SDL_Texture *> l_texOptions;
   SDL_Color l_bgColor;
   unsigned int l_i = 0;
   for (l_it = m_options.begin(); l_it != m_options.end(); ++l_it)
   {
      // Background color for the line
      l_bgColor = getBackgroundColor(l_i++, p_focus);
      // Render text
      l_texOptions.push_back(SDLUtils::renderText(*l_it, {COLOR_TEXT_NORMAL}, l_bgColor));
      SDL_QueryTexture(l_texOptions.back(), NULL, NULL, &l_w, NULL);
      // Remember largest width
      if (l_w > l_largestW)
         l_largestW = l_w;
   }

   // Render dialog background
   SDL_Rect l_dialogDim;
   l_dialogDim.w = l_largestW + (MARGIN_X + DIALOG_BORDER) * 2;
   l_dialogDim.h = LINE_HEIGHT + (m_labels.size() + m_options.size()) * LINE_HEIGHT + DIALOG_BORDER;
   l_dialogDim.x = (SCREEN_WIDTH - l_dialogDim.w) / 2;
   l_dialogDim.y = (SCREEN_HEIGHT - l_dialogDim.h) / 2;
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_RenderFillRect(g_renderer, &l_dialogDim);

   // Render dialog body
   SDL_Rect l_rect;
   l_rect.x = l_dialogDim.x + DIALOG_BORDER;
   l_rect.y = l_dialogDim.y + LINE_HEIGHT;
   l_rect.w = l_dialogDim.w - 2 * DIALOG_BORDER;
   l_rect.h = l_dialogDim.h - LINE_HEIGHT - DIALOG_BORDER;
   SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
   SDL_RenderFillRect(g_renderer, &l_rect);

   // Display title text
   int l_y = l_dialogDim.y + LINE_HEIGHT / 2;
   SDLUtils::renderTexture(l_textureTitle, l_dialogDim.x + DIALOG_BORDER + MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);

   // Display labels
   l_y += LINE_HEIGHT;
   std::vector<SDL_Texture *>::iterator l_tex;
   for (l_tex = l_texLabels.begin(); l_tex != l_texLabels.end(); ++l_tex)
   {
      SDLUtils::renderTexture(*l_tex, l_dialogDim.x + DIALOG_BORDER + MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      l_y += LINE_HEIGHT;
   }

   // Render cursor
   if (p_focus)
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_FOCUS, 255);
   else
      SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_NO_FOCUS, 255);
   l_rect.x = l_dialogDim.x + DIALOG_BORDER;
   l_rect.y = l_dialogDim.y + LINE_HEIGHT + (m_highlightedLine - m_camera + m_labels.size()) * LINE_HEIGHT;
   l_rect.w = l_dialogDim.w - 2 * DIALOG_BORDER;
   l_rect.h = LINE_HEIGHT;
   SDL_RenderFillRect(g_renderer, &l_rect);

   // Display options
   for (l_tex = l_texOptions.begin(); l_tex != l_texOptions.end(); ++l_tex)
   {
      SDLUtils::renderTexture(*l_tex, l_dialogDim.x + DIALOG_BORDER + MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      l_y += LINE_HEIGHT;
   }

   // Clean up
   if (l_textureTitle != NULL)
      SDL_DestroyTexture(l_textureTitle);
   for (l_tex = l_texLabels.begin(); l_tex != l_texLabels.end(); ++l_tex)
      if (*l_tex != NULL)
         SDL_DestroyTexture(*l_tex);
   for (l_tex = l_texOptions.begin(); l_tex != l_texOptions.end(); ++l_tex)
      if (*l_tex != NULL)
         SDL_DestroyTexture(*l_tex);
}

//------------------------------------------------------------------------------

// Key pressed
void Dialog::keyPressed(const SDL_Event &event)
{
   // Button Validate
   if (BUTTON_PRESSED_VALIDATE)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // Close window with return value
      m_retVal = m_highlightedLine;
      return;
   }
   // Button Back
   if (BUTTON_PRESSED_BACK)
   {
      // Reset timer
      m_lastPressed = -1;
      m_timer = 0;
      // Close window with no return value
      m_retVal = -2;
      return;
   }
}

//------------------------------------------------------------------------------

// Add a label
void Dialog::addLabel(const std::string &p_label)
{
   m_labels.push_back(p_label);
}

//------------------------------------------------------------------------------

// Add a menu option
void Dialog::addOption(const std::string &p_option)
{
   m_options.push_back(p_option);
   ++m_nbItems;
}
