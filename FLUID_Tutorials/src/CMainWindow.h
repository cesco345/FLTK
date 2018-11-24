// CMainWindow.h

#ifndef _CMAINWINDOW_H_INCLUDED_
#define _CMAINWINDOW_H_INCLUDED_

#include "../fluid_project/CFluidWindow.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>

class CMainWindow : public CFluidWindow {
    Fl_Image* jpeg_image;
public:
    CMainWindow();

    void show();
    void click_btn_show_jpeg();

    // callback functions
    static void callback_window_closing(Fl_Widget* widg, void* userdata_);
    static void callback_btn_show_jpeg(Fl_Widget* widg, void* userdata_);
};//class CMainWindow

#endif // _CMAINWINDOW_H_INCLUDED_
