#ifndef _IMAGEVIEWER_H_
#define _IMAGEVIEWER_H_

#include <string>
#include <SDL.h>
#include "window.h"
#include "fileLister.h"

class ImageViewer : public IWindow
{
   public:

   // Constructor
   ImageViewer(const std::string &p_dir, CFileLister *p_fileLister, const int p_fileIndex);

   // Destructor
   virtual ~ImageViewer(void);

   // Draw window
   virtual void render(const bool p_focus);

   // Supported extensions
   static bool extensionIsSupported(const std::string &p_ext);

   private:

   // Forbidden
   ImageViewer(const ImageViewer &p_source);
   const ImageViewer &operator =(const ImageViewer &p_source);

   // Key pressed
   virtual void keyPressed(const SDL_Event &event);

   // Move camera
   virtual void moveCursorUp(const int p_step, bool p_loop);
   virtual void moveCursorDown(const int p_step, bool p_loop);
   virtual void moveCursorLeft(const int p_step, bool p_loop);
   virtual void moveCursorRight(const int p_step, bool p_loop);

   // Load an image
   void loadImage(const int p_fileIndex);

   // The image to view
   SDL_Texture *m_image;
   int m_imageW;
   int m_imageH;

   // Resize image to fit the screen
   bool m_fitToScreen;

   // Current directory
   std::string m_dir;

   // The file lister
   CFileLister *m_fileLister;

   // Current file index
   int m_fileIndex;
};

#endif
