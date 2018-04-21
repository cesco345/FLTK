#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>

// animate.cxx -- Animate playback of three jpg images (0001.jpg, 0002.jpg..)
//                Hold each image for RATE seconds
//

#define RATE    .3             // how long to hold each image
#define TOTAL   13               // total images: /var/tmp/000{1,2,3}.jpg

Fl_Window     *win = 0;         // main window
Fl_JPEG_Image *jpgs[TOTAL];     // loaded images
Fl_Group      *grp = 0;         // group in which images are displayed

// SHOW NEXT IMAGE
//     Slaps next image up on screen, resets frame timer.
//
void ShowNextImage_CB(void*) {
    static int x = 0;
    grp->image(jpgs[x++ % TOTAL]);
    win->redraw();
    // Fl::repeat_timeout(RATE, ShowNextImage_CB);                      // steady rate
    Fl::repeat_timeout(((x%TOTAL)==0)?2.0:RATE, ShowNextImage_CB);      // eye blink: hold 0003.jpg for 2 secs
}

// LOAD ALL IMAGES INTO MEMORY
int LoadImages() {
    for ( int t=0; t<TOTAL; t++ ) {
        char filename[80];
        sprintf(filename, "%03d.jpg", t+1);     // 0001.jpg, 0002.jpg..
        jpgs[t] = new Fl_JPEG_Image(filename);
        if ( jpgs[t]->w() == 0 ) { perror(filename); return(1); }
    }
    return(0);
}

// MAIN -- OPEN DOUBLE BUFFERED WINDOW, LOAD IMAGES, START PLAYBACK
int main() {
    fl_register_images();                               // initialize image lib
    win = new Fl_Double_Window(720,486);                // make a window
    grp = new Fl_Group(0,0,win->w(),win->h());
    grp->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_CLIP);
    win->show();
    if ( LoadImages() ) return(1);
    Fl::add_timeout(RATE, ShowNextImage_CB);
    return(Fl::run());
}
