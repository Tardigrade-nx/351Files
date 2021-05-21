#include "dialog.h"

// Constructor
Dialog::Dialog(const std::string &p_title):
   IWindow(false, p_title)
{
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
}

//------------------------------------------------------------------------------

// Key pressed: validate
void Dialog::keyPressedValidate(void)
{
}

//------------------------------------------------------------------------------

// Key pressed: back
void Dialog::keyPressedBack(void)
{
}
