#include "imageViewer.h"
#include "def.h"
#include "sdlutils.h"

//------------------------------------------------------------------------------

// Supported extensions
bool ImageViewer::extensionIsSupported(const std::string &p_ext)
{
   return p_ext == "jpg" || p_ext == "jpeg" || p_ext == "png" || p_ext == "ico" || p_ext == "bmp";
}

//------------------------------------------------------------------------------

// Constructor
ImageViewer::ImageViewer(const std::string &p_dir, CFileLister *p_fileLister, const int p_fileIndex):
   IWindow(true, ""),
   m_image(NULL),
   m_imageW(0),
   m_imageH(0),
   m_fitToScreen(true),
   m_dir(p_dir),
   m_fileLister(p_fileLister),
   m_fileIndex(0)
{
   loadImage(p_fileIndex);
}

//------------------------------------------------------------------------------

// Destructor
ImageViewer::~ImageViewer(void)
{
   if (m_image != NULL)
   {
      SDL_DestroyTexture(m_image);
      m_image = NULL;
   }
}

//------------------------------------------------------------------------------

// Load an image
void ImageViewer::loadImage(const int p_fileIndex)
{
   // Destroy old image
   if (m_image != NULL)
   {
      SDL_DestroyTexture(m_image);
      m_image = NULL;
   }
   // Load new image
   m_fileIndex = p_fileIndex;
   m_title = m_dir + (m_dir == "/" ? "" : "/") + (*m_fileLister)[m_fileIndex].m_name;
   m_image = SDLUtils::loadTexture(m_title);
   if (m_image != NULL)
      SDL_QueryTexture(m_image, NULL, NULL, &m_imageW, &m_imageH);
}

//------------------------------------------------------------------------------

// Draw window
void ImageViewer::render(const bool p_focus)
{
   // Clear screen
   SDL_SetRenderDrawColor(g_renderer, COLOR_BODY_BG, 255);
   SDL_RenderClear(g_renderer);

   // Display image
   if (m_image != NULL)
   {
      if (m_fitToScreen && (m_imageW > SCREEN_WIDTH || m_imageH > SCREEN_HEIGHT - LINE_HEIGHT))
      {
         // Resize image to fit the screen
         double aspectRatioScreen = static_cast<double>(SCREEN_WIDTH) / (SCREEN_HEIGHT - LINE_HEIGHT);
         double aspectRatioImage = static_cast<double>(m_imageW) / m_imageH;
         double resizeFactor = 0.0;
         if (aspectRatioImage > aspectRatioScreen)
            resizeFactor = static_cast<double>(m_imageW) / SCREEN_WIDTH;
         else
            resizeFactor = static_cast<double>(m_imageH) / (SCREEN_HEIGHT - LINE_HEIGHT);
         int imageFitW = static_cast<double>(m_imageW) / resizeFactor;
         int imageFitH = static_cast<double>(m_imageH) / resizeFactor;
         SDL_Rect destRect = { (SCREEN_WIDTH - imageFitW) / 2, LINE_HEIGHT + ((SCREEN_HEIGHT - LINE_HEIGHT - imageFitH) / 2), imageFitW, imageFitH };
         SDL_RenderCopy(g_renderer, m_image, NULL, &destRect);
      }
      else
      {
         // Display image as it is
         SDLUtils::renderTexture(m_image, (m_imageW > SCREEN_WIDTH ? m_camera.x : 0) + (SCREEN_WIDTH / 2), (m_imageH > SCREEN_HEIGHT - LINE_HEIGHT ? m_camera.y : 0) + ((SCREEN_HEIGHT + LINE_HEIGHT) / 2), SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
      }
   }
   else
   {
      // Error
      SDLUtils::renderText("Unable to load image", g_font, SCREEN_WIDTH / 2, (SCREEN_HEIGHT + LINE_HEIGHT) / 2, {COLOR_TEXT_NORMAL}, {COLOR_BODY_BG}, SDLUtils::T_ALIGN_CENTER, SDLUtils::T_ALIGN_MIDDLE);
   }

   // Render title background
   SDL_SetRenderDrawColor(g_renderer, COLOR_TITLE_BG, 255);
   SDL_Rect rect { 0, 0, SCREEN_WIDTH, LINE_HEIGHT };
   SDL_RenderFillRect(g_renderer, &rect);

   // Render title
   SDLUtils::renderTexture(g_iconImage, MARGIN_X, LINE_HEIGHT / 2, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
   SDLUtils::renderText(m_title, g_font, MARGIN_X + ICON_SIZE + MARGIN_X, LINE_HEIGHT / 2, {COLOR_TEXT_NORMAL}, {COLOR_TITLE_BG}, SDLUtils::T_ALIGN_LEFT, SDLUtils::T_ALIGN_MIDDLE);
}

//------------------------------------------------------------------------------

// Key pressed
void ImageViewer::keyPressed(const SDL_Event &event)
{
   // Button Validate : change original / fit
   if (BUTTON_PRESSED_VALIDATE)
   {
      // Reset timer
      resetTimer();
      // Change original / fit
      m_fitToScreen = ! m_fitToScreen;
      m_camera.x = 0;
      m_camera.y = 0;
      g_hasChanged = true;
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

// Move camera
void ImageViewer::moveCursorUp(const int p_step, bool p_loop)
{
   if (m_fitToScreen)
   {
      // Previous image
      int ind = m_fileLister->getPreviousImage(m_fileIndex);
      if (ind != -1)
      {
         loadImage(ind);
         g_hasChanged = true;
      }
   }
   else
   {
      // Scroll image
      m_camera.y += VIEWER_SCROLL_SPEED;
      g_hasChanged = true;
   }
}

void ImageViewer::moveCursorDown(const int p_step, bool p_loop)
{
   if (m_fitToScreen)
   {
      // Next image
      int ind = m_fileLister->getNextImage(m_fileIndex);
      if (ind != -1)
      {
         loadImage(ind);
         g_hasChanged = true;
      }
   }
   else
   {
      // Scroll image
      m_camera.y -= VIEWER_SCROLL_SPEED;
      g_hasChanged = true;
   }
}

void ImageViewer::moveCursorLeft(const int p_step, bool p_loop)
{
   if (m_fitToScreen)
   {
      // Previous image
      int ind = m_fileLister->getPreviousImage(m_fileIndex);
      if (ind != -1)
      {
         loadImage(ind);
         g_hasChanged = true;
      }
   }
   else
   {
      // Scroll image
      m_camera.x += VIEWER_SCROLL_SPEED;
      g_hasChanged = true;
   }
}

void ImageViewer::moveCursorRight(const int p_step, bool p_loop)
{
   if (m_fitToScreen)
   {
      // Next image
      int ind = m_fileLister->getNextImage(m_fileIndex);
      if (ind != -1)
      {
         loadImage(ind);
         g_hasChanged = true;
      }
   }
   else
   {
      // Scroll image
      m_camera.x -= VIEWER_SCROLL_SPEED;
      g_hasChanged = true;
   }
}
