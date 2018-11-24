// From his CPP course: http://epweb2.ph.bham.ac.uk/user/hillier/course3cpp/fltkegs/
// Simple drawing in FLTK.

#include <vector>
#include <cmath>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Box.H>


using namespace std;

// some data we will calculate and plot
vector<double> ydat(190,0);



// Global pointers for the gui objects
Fl_Window *window;
class Drawing;
Drawing *canvas;
Fl_Value_Slider *carrierslider;
Fl_Value_Slider *signalslider;

// Recalculate ydat (called whenever the sliders change)
void sety(){
	double f1 = carrierslider->value()/3000;
	double f2 = signalslider->value()/3000;
	for (unsigned i = 0; i<ydat.size(); i++){
		ydat[i] = 50*sin(f1*i)*sin(f2*i);
		//ydat[i] = 50*sin(f1*i*sin(f2*i));
	}
}

// This window has a draw() method that knows how to display our data.
// We use a double-buffered window to prevent flickering during rapid updates
class Drawing : public Fl_Double_Window {
  public:
  	Drawing(int X,int Y,int W,int H) : Fl_Double_Window(X,Y,W,H) {}
  private:
	void draw() {
		fl_color(FL_WHITE);		// erase
		fl_rectf(0,0,w(),h());
		fl_color(FL_BLUE);
		fl_line_style(0,2);
		fl_begin_line();		// should do some proper scaling here
		for (unsigned i = 0; i<ydat.size(); i++){
			fl_vertex(2.5*i, h()/2+ydat[i]);
		}
		fl_end_line();
	}
};

// Slider callback function recalculates and then updates display
// Doing this every time is expensive on computation, but makes for
// nice animation if you have the speed.
void doRecalc(Fl_Widget*,void*)
{
	sety();
	canvas->redraw();
}

int main() {
// The main window.
	window = new Fl_Window(500,440,"FLTK data plotting");
// A widget to draw on
	canvas = new Drawing(10,10,480,300);
	canvas->end();  // to stop canvas swallowing sliders
// A couple of controls
	carrierslider = new Fl_Value_Slider(10,320,200,30, "Carrier");
	carrierslider->type(FL_HOR_NICE_SLIDER);
	carrierslider->range(1000,3000);
	carrierslider->value(1000);
	carrierslider->precision(0);
	carrierslider->callback(doRecalc);
	signalslider = new Fl_Value_Slider(10,380,200,30, "Signal");
	signalslider->type(FL_HOR_NICE_SLIDER);
	signalslider->range(100,300);
	signalslider->value(100);
	signalslider->precision(0);
	signalslider->callback(doRecalc);
// A bit of text	
	Fl_Box* label = new Fl_Box(300,350,100,30,"Magic modulator");
	label->labelfont(FL_HELVETICA_BOLD);
	label->labelsize(18);
	Fl_Box* inst = new Fl_Box(300,380,100,50,"Drag the sliders...");
   
// calculate the data based on the initial settings
	sety();

// Make the window visible and start processing events
	window->end();
	window->show();  
	return Fl::run();
}
