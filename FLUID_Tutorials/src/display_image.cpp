
#include <iostream>

#include "CMainWindow.h"
#include "CMainWindow.cpp"

// Use standard main to have console background:
int main()
{
    CMainWindow window;
    window.show();
    return Fl::run();
}
