#include <string>
#include <vector>
#include <sstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "def.h"
#include "sdlutils.h"
#include "mainWindow.h"

//------------------------------------------------------------------------------

// Globals
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
SDL_Joystick* g_joystick = NULL;
TTF_Font *g_font = NULL;
TTF_Font *g_fontMono = NULL;
int g_charW = 0;
std::vector<IWindow *> g_windows;
bool IWindow::g_hasChanged = true;

// Textures for icons
SDL_Texture *g_iconFile = NULL;
SDL_Texture *g_iconDir = NULL;
SDL_Texture *g_iconUp = NULL;
SDL_Texture *g_iconCopy = NULL;
SDL_Texture *g_iconCut = NULL;
SDL_Texture *g_iconPaste = NULL;
SDL_Texture *g_iconTrash = NULL;
SDL_Texture *g_iconDisk = NULL;
SDL_Texture *g_iconSelect = NULL;
SDL_Texture *g_iconNone = NULL;
SDL_Texture *g_iconNewDir = NULL;
SDL_Texture *g_iconQuit = NULL;
SDL_Texture *g_iconEdit = NULL;
SDL_Texture *g_iconCancel = NULL;
SDL_Texture *g_iconFloppy = NULL;
SDL_Texture *g_iconImage = NULL;
SDL_Texture *g_iconFileText = NULL;
SDL_Texture *g_iconPlus = NULL;

//------------------------------------------------------------------------------

int main(int argc, char* args[])
{
   // Init SDL
   if (! SDLUtils::init())
   {
      SDLUtils::close();
      return 1;
   }

   // Load icon textures
   std::ostringstream oss;
   oss << '/' << ICON_SIZE;
   g_iconFile =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/file.png");
   g_iconDir =       SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/folder.png");
   g_iconUp =        SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/up.png");
   g_iconCopy =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/edit-copy.png");
   g_iconCut =       SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/edit-cut.png");
   g_iconPaste =     SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/edit-paste.png");
   g_iconTrash =     SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/trash.png");
   g_iconDisk =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/disk.png");
   g_iconSelect =    SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/select.png");
   g_iconNone =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/none.png");
   g_iconNewDir =    SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/folder-new.png");
   g_iconQuit =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/quit.png");
   g_iconEdit =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/edit.png");
   g_iconCancel =    SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/cancel.png");
   g_iconFloppy =    SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/floppy.png");
   g_iconImage =     SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/image.png");
   g_iconFileText =  SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/file-text.png");
   g_iconPlus =      SDLUtils::loadTexture(std::string(RES_PATH) + oss.str() + "/plus.png");

   // Load fonts
   g_font = SDLUtils::loadFont(std::string(RES_PATH) + "/" + FONT_NAME, FONT_SIZE);
   g_fontMono = SDLUtils::loadFont(std::string(RES_PATH) + "/" + FONT_NAME_MONO, FONT_SIZE);
   if (g_font == NULL || g_fontMono == NULL)
   {
      SDLUtils::close();
      return 1;
   }

   // Width of one character in monospace font
   g_charW = SDLUtils::getCharWidthMono();

   // Execute main window
   {
      MainWindow mainWindow(START_PATH);
      mainWindow.execute();
   }

   // Clean up font
   TTF_CloseFont(g_font);
   g_font = NULL;
   TTF_CloseFont(g_fontMono);
   g_fontMono = NULL;

   // Free textures
   if (g_iconFile != NULL)       { SDL_DestroyTexture(g_iconFile);      g_iconFile = NULL; }
   if (g_iconDir != NULL)        { SDL_DestroyTexture(g_iconDir);       g_iconDir = NULL; }
   if (g_iconUp != NULL)         { SDL_DestroyTexture(g_iconUp);        g_iconUp = NULL; }
   if (g_iconCopy != NULL)       { SDL_DestroyTexture(g_iconCopy);      g_iconCopy = NULL; }
   if (g_iconCut != NULL)        { SDL_DestroyTexture(g_iconCut);       g_iconCut = NULL; }
   if (g_iconPaste != NULL)      { SDL_DestroyTexture(g_iconPaste);     g_iconPaste = NULL; }
   if (g_iconTrash != NULL)      { SDL_DestroyTexture(g_iconTrash);     g_iconTrash = NULL; }
   if (g_iconDisk != NULL)       { SDL_DestroyTexture(g_iconDisk);      g_iconDisk = NULL; }
   if (g_iconSelect != NULL)     { SDL_DestroyTexture(g_iconSelect);    g_iconSelect = NULL; }
   if (g_iconNone != NULL)       { SDL_DestroyTexture(g_iconNone);      g_iconNone = NULL; }
   if (g_iconNewDir != NULL)     { SDL_DestroyTexture(g_iconNewDir);    g_iconNewDir = NULL; }
   if (g_iconQuit != NULL)       { SDL_DestroyTexture(g_iconQuit);      g_iconQuit = NULL; }
   if (g_iconEdit != NULL)       { SDL_DestroyTexture(g_iconEdit);      g_iconEdit = NULL; }
   if (g_iconCancel != NULL)     { SDL_DestroyTexture(g_iconCancel);    g_iconCancel = NULL; }
   if (g_iconFloppy != NULL)     { SDL_DestroyTexture(g_iconFloppy);    g_iconFloppy = NULL; }
   if (g_iconImage != NULL)      { SDL_DestroyTexture(g_iconImage);     g_iconImage = NULL; }
   if (g_iconFileText != NULL)   { SDL_DestroyTexture(g_iconFileText);  g_iconFileText = NULL; }
   if (g_iconPlus != NULL)       { SDL_DestroyTexture(g_iconPlus);      g_iconPlus = NULL; }

   // Quit SDL
   SDLUtils::close();

   return 0;
}
