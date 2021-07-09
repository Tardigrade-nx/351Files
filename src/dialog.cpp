#include "dialog.h"
#include "def.h"
#include "sdlutils.h"

// Constructor
Dialog::Dialog(const std::string &p_title):
   IWindow(false, p_title),
   m_iconPresent(false)
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
   SDL_Rect l_dialogDim = {0, 0, 0, 0};
   int l_w = 0;

   // Render title
   SDL_Texture *l_textureTitle = SDLUtils::renderText(m_title, g_font, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG});
   SDL_QueryTexture(l_textureTitle, NULL, NULL, &l_dialogDim.w, NULL);

   // Render labels
   std::vector<std::string>::iterator l_it;
   std::vector<SDL_Texture *> l_texLabels;
   for (l_it = m_labels.begin(); l_it != m_labels.end(); ++l_it)
   {
      // Render text
      l_texLabels.push_back(SDLUtils::renderText(*l_it, g_font, {COLOR_TEXT_NORMAL}, {COLOR_BODY_BG}));
      SDL_QueryTexture(l_texLabels.back(), NULL, NULL, &l_w, NULL);
      // Remember largest width
      if (l_w > l_dialogDim.w)
         l_dialogDim.w = l_w;
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
      l_texOptions.push_back(SDLUtils::renderText(*l_it, g_font, {COLOR_TEXT_NORMAL}, l_bgColor));
      SDL_QueryTexture(l_texOptions.back(), NULL, NULL, &l_w, NULL);
      // Remember largest width
      if (l_w > l_dialogDim.w)
         l_dialogDim.w = l_w;
   }

   // Largest line of the dialog
   l_dialogDim.w = DIALOG_BORDER + MARGIN_X + l_dialogDim.w + MARGIN_X + DIALOG_BORDER;
   if (m_iconPresent)
      l_dialogDim.w += ICON_SIZE + MARGIN_X;
   if (l_dialogDim.w > SCREEN_WIDTH)
      l_dialogDim.w = SCREEN_WIDTH;

   // Render dialog background
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
   std::vector<SDL_Texture *>::iterator l_tex, l_icon;
   for (l_tex = l_texLabels.begin(), l_icon = m_labelIcons.begin(); l_tex != l_texLabels.end(); ++l_tex, ++l_icon)
   {
      // Icon
      if (*l_icon != NULL)
         SDLUtils::renderTexture(*l_icon, l_dialogDim.x + DIALOG_BORDER + MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      // Text
      SDLUtils::renderTexture(*l_tex, l_dialogDim.x + DIALOG_BORDER + MARGIN_X + (*l_icon == NULL ? 0 : ICON_SIZE + MARGIN_X), l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      l_y += LINE_HEIGHT;
   }

   // Render cursor
   if (m_options.size() > 0)
   {
      if (p_focus)
         SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_FOCUS, 255);
      else
         SDL_SetRenderDrawColor(g_renderer, COLOR_CURSOR_NO_FOCUS, 255);
      l_rect.x = l_dialogDim.x + DIALOG_BORDER;
      l_rect.y = l_dialogDim.y + LINE_HEIGHT + (m_cursor + m_labels.size()) * LINE_HEIGHT;
      l_rect.w = l_dialogDim.w - 2 * DIALOG_BORDER;
      l_rect.h = LINE_HEIGHT;
      SDL_RenderFillRect(g_renderer, &l_rect);
   }

   // Display options
   for (l_tex = l_texOptions.begin(), l_icon = m_optionIcons.begin(); l_tex != l_texOptions.end(); ++l_tex, ++l_icon)
   {
      // Icon
      if (*l_icon != NULL)
         SDLUtils::renderTexture(*l_icon, l_dialogDim.x + DIALOG_BORDER + MARGIN_X, l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
      // Text
      SDLUtils::renderTexture(*l_tex, l_dialogDim.x + DIALOG_BORDER + MARGIN_X + (*l_icon == NULL ? 0 : ICON_SIZE + MARGIN_X), l_y, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
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
      resetTimer();
      // Close window with return value
      m_retVal = m_optionRetVals[m_cursor];
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

// Add a label
void Dialog::addLabel(const std::string &p_label, SDL_Texture *p_icon)
{
   m_labels.push_back(p_label);
   m_labelIcons.push_back(p_icon);
   if (p_icon != NULL && !m_iconPresent)
      m_iconPresent = true;
}

//------------------------------------------------------------------------------

// Add a menu option
void Dialog::addOption(const std::string &p_option, const int p_retVal, SDL_Texture *p_icon)
{
   m_options.push_back(p_option);
   m_optionRetVals.push_back(p_retVal);
   m_optionIcons.push_back(p_icon);
   ++m_nbItems;
   if (p_icon != NULL && !m_iconPresent)
      m_iconPresent = true;
}
