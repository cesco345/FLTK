// From his CPP course http://epweb2.ph.bham.ac.uk/user/hillier/course3cpp/fltkegs/
// Skeleton FLTK program

#include <FL/Fl.H>
#include <FL/Fl_Window.H>   // One include for each class of GUI object
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Output.H>

// Global pointers for the GUI objects
Fl_Window* mywindow;
Fl_Button* spiderbutton;
Fl_Button* dogbutton;
Fl_Value_Output* myoutputbox;

// Callback function
void mybutton_cb(Fl_Widget* w, long int data) {
    myoutputbox->value(double(data));
}

// Execution starts here
int main() {
    // The main window (will contain everything up to mywindow->end() )
    mywindow = new Fl_Window(300,180,"FLTK skeleton program");

    // Two buttons, sharing one callback
    spiderbutton = new Fl_Button(50, 50, 50, 30, "Spider");
    dogbutton = new Fl_Button(200, 50, 50, 30, "Dog");
    spiderbutton->callback(mybutton_cb, 8);
    dogbutton->callback(mybutton_cb, 4);	

    // An output box
    myoutputbox = new Fl_Value_Output(50, 100, 200, 30, "Legs =");
    myoutputbox->color(FL_YELLOW);

    // Make the window visible and start processing events
    mywindow->end();
    mywindow->show();  
    return Fl::run();
}
