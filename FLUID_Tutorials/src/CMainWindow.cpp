  GNU nano 2.7.4                            File: CMainWindow.cpp                                      

#include "CMainWindow.h"

CMainWindow::CMainWindow()
    : jpeg_image( nullptr )
{
}

void CMainWindow::show()
{
    // Window callbacks:
    win_app->callback( (Fl_Callback*)CMainWindow::callback_window_closing, (void*)(this) );
    btn_show_jpeg->callback( (Fl_Callback*)CMainWindow::callback_btn_show_jpeg, (void*)(this) );

    // Make the window visible:
    win_app->show();
}

// callback functions
void CMainWindow::callback_window_closing(Fl_Widget* widg, void* userdata_) {
    std::cout << "X button clicked -- exiting the program\n";
    while( Fl::first_window() ) {
        Fl::first_window()->hide();
    }
}

void CMainWindow::callback_btn_show_jpeg(Fl_Widget* widg, void* userdata_)
{
    CMainWindow* win = static_cast< CMainWindow* > ( userdata_ );
    win->click_btn_show_jpeg();
}

void CMainWindow::click_btn_show_jpeg()
{
    std::cout << "btn_show_jpeg click\n";
    Fl_JPEG_Image* jpeg_image_orig = new Fl_JPEG_Image( "lena.jpg" );
    // copy allow us to resize the image:
    jpeg_image = jpeg_image_orig->copy( box_jpeg_image->w(), box_jpeg_image->h() );
    delete jpeg_image_orig;
    box_jpeg_image->image( jpeg_image );
    win_app->redraw();
}
