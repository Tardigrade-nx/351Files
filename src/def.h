#ifndef _DEF_H_
#define _DEF_H_

#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

//~ #define INHIBIT(X) X
#define INHIBIT(X) /* X */

#define APP_NAME "351Files"

// Parameters for RG351P
#if defined(DEVICE_RG351P)
   #define SCREEN_WIDTH             480
   #define SCREEN_HEIGHT            320
   #define HARDWARE_ACCELERATION    0
   #define FULLSCREEN               1
   #define FONT_NAME                "NotoSans-Regular.ttf"
   #define FONT_NAME_MONO           "NotoSansMono-Regular.ttf"
   #define FONT_SIZE                14
   #define LINE_HEIGHT              24
   #define ICON_SIZE                24
   #define MARGIN_X                 8
   #define KEYBOARD_MARGIN          3
   #define KEYBOARD_KEY_SPACING     3

// Parameters for RG351V
#elif defined(DEVICE_RG351V)
   #define SCREEN_WIDTH             640
   #define SCREEN_HEIGHT            480
   #define HARDWARE_ACCELERATION    0
   #define FULLSCREEN               1
   #define FONT_NAME                "NotoSans-Regular.ttf"
   #define FONT_NAME_MONO           "NotoSansMono-Regular.ttf"
   #define FONT_SIZE                20
   #define LINE_HEIGHT              32
   #define ICON_SIZE                24
   #define MARGIN_X                 10
   #define KEYBOARD_MARGIN          8
   #define KEYBOARD_KEY_SPACING     4

// Parameters for RGB10
#elif defined(DEVICE_RGB10)
   #define SCREEN_WIDTH             480
   #define SCREEN_HEIGHT            320
   #define HARDWARE_ACCELERATION    0
   #define FULLSCREEN               1
   #define FONT_NAME                "NotoSans-Regular.ttf"
   #define FONT_NAME_MONO           "NotoSansMono-Regular.ttf"
   #define FONT_SIZE                14
   #define LINE_HEIGHT              24
   #define ICON_SIZE                24
   #define MARGIN_X                 8
   #define KEYBOARD_MARGIN          3
   #define KEYBOARD_KEY_SPACING     3

// Parameters for RK2020
#elif defined(DEVICE_RK2020)
   #define SCREEN_WIDTH             480
   #define SCREEN_HEIGHT            320
   #define HARDWARE_ACCELERATION    0
   #define FULLSCREEN               1
   #define FONT_NAME                "NotoSans-Regular.ttf"
   #define FONT_NAME_MONO           "NotoSansMono-Regular.ttf"
   #define FONT_SIZE                14
   #define LINE_HEIGHT              24
   #define ICON_SIZE                24
   #define MARGIN_X                 8
   #define KEYBOARD_MARGIN          3
   #define KEYBOARD_KEY_SPACING     3

// Parameters for CHI
#elif defined(DEVICE_CHI)
   #define SCREEN_WIDTH             640
   #define SCREEN_HEIGHT            480
   #define HARDWARE_ACCELERATION    0
   #define FULLSCREEN               1
   #define FONT_NAME                "NotoSans-Regular.ttf"
   #define FONT_NAME_MONO           "NotoSansMono-Regular.ttf"
   #define FONT_SIZE                20
   #define LINE_HEIGHT              32
   #define ICON_SIZE                24
   #define MARGIN_X                 10
   #define KEYBOARD_MARGIN          8
   #define KEYBOARD_KEY_SPACING     4

// Paramaters for desktop PC
#else
   #define SCREEN_WIDTH             640
   #define SCREEN_HEIGHT            480
   #define HARDWARE_ACCELERATION    1
   #define FULLSCREEN               0
   #define FONT_NAME                "NotoSans-Regular.ttf"
   #define FONT_NAME_MONO           "NotoSansMono-Regular.ttf"
   #define FONT_SIZE                20
   #define LINE_HEIGHT              32
   #define ICON_SIZE                24
   #define MARGIN_X                 10
   #define KEYBOARD_MARGIN          8
   #define KEYBOARD_KEY_SPACING     4
#endif

// Colors
#define COLOR_TITLE_BG           68,  68,  68
#define COLOR_TEXT_NORMAL        255, 255, 255
#define COLOR_TEXT_SELECTED      255, 0, 0
#define COLOR_BODY_BG            45,  45,  45
#define COLOR_KEYBOARD_DARK      35,  35,  35
#define COLOR_CURSOR_FOCUS       21,  83,  158
#define COLOR_CURSOR_NO_FOCUS    80,  80,  80

// Framerate = 30fps
#define MS_PER_FRAME          16

// Timer for key holds
#define KEYHOLD_TIMER_FIRST   16
#define KEYHOLD_TIMER         4

// Size of dialog border
#define DIALOG_BORDER         4

// Scroll speed in image viewer
#define VIEWER_SCROLL_SPEED      20

// Button events
#if defined(DEVICE_RG351P)
   #define BUTTON_PRESSED_UP              event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_UP
   #define BUTTON_PRESSED_DOWN            event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_DOWN
   #define BUTTON_PRESSED_LEFT            event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_LEFT
   #define BUTTON_PRESSED_RIGHT           event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_RIGHT
   #define BUTTON_PRESSED_PAGEUP          event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 4 || event.jbutton.button == 10)
   #define BUTTON_PRESSED_PAGEDOWN        event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 5 || event.jbutton.button == 11)
   #define BUTTON_PRESSED_VALIDATE        event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 0
   #define BUTTON_PRESSED_BACK            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 1
   #define BUTTON_PRESSED_MENU_CONTEXT    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 2
   #define BUTTON_PRESSED_SELECT          event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 3
   #define BUTTON_HELD_UP                 SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_UP
   #define BUTTON_HELD_DOWN               SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_DOWN
   #define BUTTON_HELD_LEFT               SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_LEFT
   #define BUTTON_HELD_RIGHT              SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_RIGHT
   #define BUTTON_HELD_PAGEUP             SDL_JoystickGetButton(g_joystick, 4) || SDL_JoystickGetButton(g_joystick, 10)
   #define BUTTON_HELD_PAGEDOWN           SDL_JoystickGetButton(g_joystick, 5) || SDL_JoystickGetButton(g_joystick, 11)
   #define BUTTON_HELD_SELECT             SDL_JoystickGetButton(g_joystick, 3)
#elif defined(DEVICE_RG351V)
   #define BUTTON_PRESSED_UP              event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_UP
   #define BUTTON_PRESSED_DOWN            event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_DOWN
   #define BUTTON_PRESSED_LEFT            event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_LEFT
   #define BUTTON_PRESSED_RIGHT           event.type == SDL_JOYHATMOTION && event.jhat.value == SDL_HAT_RIGHT
   #define BUTTON_PRESSED_PAGEUP          event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 4 || event.jbutton.button == 10)
   #define BUTTON_PRESSED_PAGEDOWN        event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 5 || event.jbutton.button == 11)
   #define BUTTON_PRESSED_VALIDATE        event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 0
   #define BUTTON_PRESSED_BACK            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 1
   #define BUTTON_PRESSED_MENU_CONTEXT    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 2
   #define BUTTON_PRESSED_SELECT          event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 3
   #define BUTTON_HELD_UP                 SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_UP
   #define BUTTON_HELD_DOWN               SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_DOWN
   #define BUTTON_HELD_LEFT               SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_LEFT
   #define BUTTON_HELD_RIGHT              SDL_JoystickGetHat(g_joystick, 0) == SDL_HAT_RIGHT
   #define BUTTON_HELD_PAGEUP             SDL_JoystickGetButton(g_joystick, 4) || SDL_JoystickGetButton(g_joystick, 10)
   #define BUTTON_HELD_PAGEDOWN           SDL_JoystickGetButton(g_joystick, 5) || SDL_JoystickGetButton(g_joystick, 11)
   #define BUTTON_HELD_SELECT             SDL_JoystickGetButton(g_joystick, 3)
#elif defined(DEVICE_RGB10)
   #define BUTTON_PRESSED_UP              event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 8
   #define BUTTON_PRESSED_DOWN            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 9
   #define BUTTON_PRESSED_LEFT            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 10
   #define BUTTON_PRESSED_RIGHT           event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 11
   #define BUTTON_PRESSED_PAGEUP          event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 4 || event.jbutton.button == 14)
   #define BUTTON_PRESSED_PAGEDOWN        event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 5 || event.jbutton.button == 15)
   #define BUTTON_PRESSED_VALIDATE        event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 1
   #define BUTTON_PRESSED_BACK            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 0
   #define BUTTON_PRESSED_MENU_CONTEXT    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 3
   #define BUTTON_PRESSED_SELECT          event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 2
   #define BUTTON_HELD_UP                 SDL_JoystickGetButton(g_joystick, 8)
   #define BUTTON_HELD_DOWN               SDL_JoystickGetButton(g_joystick, 9)
   #define BUTTON_HELD_LEFT               SDL_JoystickGetButton(g_joystick, 10)
   #define BUTTON_HELD_RIGHT              SDL_JoystickGetButton(g_joystick, 11)
   #define BUTTON_HELD_PAGEUP             SDL_JoystickGetButton(g_joystick, 4) || SDL_JoystickGetButton(g_joystick, 14)
   #define BUTTON_HELD_PAGEDOWN           SDL_JoystickGetButton(g_joystick, 5) || SDL_JoystickGetButton(g_joystick, 15)
   #define BUTTON_HELD_SELECT             SDL_JoystickGetButton(g_joystick, 2)
#elif defined(DEVICE_RK2020)
   #define BUTTON_PRESSED_UP              event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 6
   #define BUTTON_PRESSED_DOWN            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 7
   #define BUTTON_PRESSED_LEFT            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 8
   #define BUTTON_PRESSED_RIGHT           event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 9
   #define BUTTON_PRESSED_PAGEUP          event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 4 || event.jbutton.button == 12)
   #define BUTTON_PRESSED_PAGEDOWN        event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 5 || event.jbutton.button == 13)
   #define BUTTON_PRESSED_VALIDATE        event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 1
   #define BUTTON_PRESSED_BACK            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 0
   #define BUTTON_PRESSED_MENU_CONTEXT    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 3
   #define BUTTON_PRESSED_SELECT          event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 2
   #define BUTTON_HELD_UP                 SDL_JoystickGetButton(g_joystick, 6)
   #define BUTTON_HELD_DOWN               SDL_JoystickGetButton(g_joystick, 7)
   #define BUTTON_HELD_LEFT               SDL_JoystickGetButton(g_joystick, 8)
   #define BUTTON_HELD_RIGHT              SDL_JoystickGetButton(g_joystick, 9)
   #define BUTTON_HELD_PAGEUP             SDL_JoystickGetButton(g_joystick, 4) || SDL_JoystickGetButton(g_joystick, 12)
   #define BUTTON_HELD_PAGEDOWN           SDL_JoystickGetButton(g_joystick, 5) || SDL_JoystickGetButton(g_joystick, 13)
   #define BUTTON_HELD_SELECT             SDL_JoystickGetButton(g_joystick, 2)
#elif defined(DEVICE_CHI)
   #define BUTTON_PRESSED_UP              event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 10
   #define BUTTON_PRESSED_DOWN            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 11
   #define BUTTON_PRESSED_LEFT            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 12
   #define BUTTON_PRESSED_RIGHT           event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 13
   #define BUTTON_PRESSED_PAGEUP          event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 4 || event.jbutton.button == 6)
   #define BUTTON_PRESSED_PAGEDOWN        event.type == SDL_JOYBUTTONDOWN && (event.jbutton.button == 5 || event.jbutton.button == 7)
   #define BUTTON_PRESSED_VALIDATE        event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 1
   #define BUTTON_PRESSED_BACK            event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 0
   #define BUTTON_PRESSED_MENU_CONTEXT    event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 3
   #define BUTTON_PRESSED_SELECT          event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 2
   #define BUTTON_HELD_UP                 SDL_JoystickGetButton(g_joystick, 10)
   #define BUTTON_HELD_DOWN               SDL_JoystickGetButton(g_joystick, 11)
   #define BUTTON_HELD_LEFT               SDL_JoystickGetButton(g_joystick, 12)
   #define BUTTON_HELD_RIGHT              SDL_JoystickGetButton(g_joystick, 13)
   #define BUTTON_HELD_PAGEUP             SDL_JoystickGetButton(g_joystick, 4) || SDL_JoystickGetButton(g_joystick, 6)
   #define BUTTON_HELD_PAGEDOWN           SDL_JoystickGetButton(g_joystick, 5) || SDL_JoystickGetButton(g_joystick, 7)
   #define BUTTON_HELD_SELECT             SDL_JoystickGetButton(g_joystick, 2)
#else
   #define BUTTON_PRESSED_UP              event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_UP
   #define BUTTON_PRESSED_DOWN            event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_DOWN
   #define BUTTON_PRESSED_LEFT            event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_LEFT
   #define BUTTON_PRESSED_RIGHT           event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_RIGHT
   #define BUTTON_PRESSED_PAGEUP          event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_PAGEUP
   #define BUTTON_PRESSED_PAGEDOWN        event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_PAGEDOWN
   #define BUTTON_PRESSED_VALIDATE        event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_RETURN
   #define BUTTON_PRESSED_BACK            event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_BACKSPACE
   #define BUTTON_PRESSED_MENU_CONTEXT    event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_x
   #define BUTTON_PRESSED_SELECT          event.type == SDL_KEYDOWN && event.key.repeat == 0 && event.key.keysym.sym == SDLK_INSERT
   #define BUTTON_HELD_UP                 SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP]
   #define BUTTON_HELD_DOWN               SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN]
   #define BUTTON_HELD_LEFT               SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT]
   #define BUTTON_HELD_RIGHT              SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT]
   #define BUTTON_HELD_PAGEUP             SDL_GetKeyboardState(NULL)[SDL_SCANCODE_PAGEUP]
   #define BUTTON_HELD_PAGEDOWN           SDL_GetKeyboardState(NULL)[SDL_SCANCODE_PAGEDOWN]
   #define BUTTON_HELD_SELECT             SDL_GetKeyboardState(NULL)[SDL_SCANCODE_INSERT]
#endif

// Globals
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern SDL_Joystick* g_joystick;
extern TTF_Font *g_font;                     // Variable size font
extern TTF_Font *g_fontMono;                 // Mono size font
extern int g_charW;                          // Width of one character in monospace font
class IWindow;
extern std::vector<IWindow *> g_windows;

// Textures for icons
extern SDL_Texture *g_iconFile;
extern SDL_Texture *g_iconDir;
extern SDL_Texture *g_iconUp;
extern SDL_Texture *g_iconCopy;
extern SDL_Texture *g_iconCut;
extern SDL_Texture *g_iconPaste;
extern SDL_Texture *g_iconTrash;
extern SDL_Texture *g_iconDisk;
extern SDL_Texture *g_iconSelect;
extern SDL_Texture *g_iconNone;
extern SDL_Texture *g_iconNewDir;
extern SDL_Texture *g_iconQuit;
extern SDL_Texture *g_iconEdit;
extern SDL_Texture *g_iconCancel;
extern SDL_Texture *g_iconFloppy;
extern SDL_Texture *g_iconImage;
extern SDL_Texture *g_iconFileText;

#endif
