#include <stdio.h>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>


//
// Demonstrate overlapping alpha blended images -- erco 06/09/09
//
class MyWindow : public Fl_Window {
    Fl_PNG_Image *left;
    Fl_PNG_Image *right;
    void GetFLTKVersion(char *s) {            // get fltk version info for demo -- optional
        sprintf(s, "FLTK %d.%d.%d", FL_MAJOR_VERSION, FL_MINOR_VERSION, FL_PATCH_VERSION);
    }
public:
    void draw() {
        Fl_Window::draw();                    // Draw window widget first
        fl_font(FL_HELVETICA, 25);            // set font
        fl_color(FL_BLACK+FL_BOLD+FL_ITALIC);                   // set color
        fl_draw("Hello YouTube, Alpha Blend", 0, 40);   // draw a text string
       
        left->draw(75,100);                      // draw left alpha image over the above
        right->draw(75,100);                     // draw right alpha image over the above
    }
    MyWindow(int W, int H) : Fl_Window(W,H) {
        char s[80]; GetFLTKVersion(s); copy_label(s);        // (show fltk version -- optional)
        left  = new Fl_PNG_Image("./black.png");        // assumes images in cwd
        right = new Fl_PNG_Image("./ubuntu.png");       // assumes images in cwd
        show();
    }
};
int main() {

    fl_register_images();
    MyWindow win(400,400);
    win.show();
    return(Fl::run());
}

