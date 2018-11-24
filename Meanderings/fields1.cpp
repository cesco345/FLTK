// From his CPP course: http://epweb2.ph.bham.ac.uk/user/hillier/course3cpp/fltkegs/
// Plot electric fields using the FLTK toolkit              MSC 2003-4
//
// Left click makes a positive charge, right click a negative one.
// Lines are only drawn starting from positive charges, so systems that
// are overall negative look a bit odd.  The lines are started at regular
// intervals around positive charges, which isn't right either, so some
// closely-spaced arrangements look funny too.

// Bring in the necessary features of the FLTK graphical toolkit
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Draw.H>

// These bring in parts of the standard C++ library
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

// This brings in the Vec class for two-dimensional vectors,
// from our other source file.  Vec has nothing to do with the
// standard C++ data structure Class called vector.
#include "Vec.h"


const double PI = 3.141592653;
const Fl_Color BGCOLOR = fl_rgb_color(255,230,170);  // bisque
const Fl_Color POSCOLOR = FL_RED;
const Fl_Color NEGCOLOR = FL_BLUE;
const Fl_Color LINECOLOR = FL_BLACK;
const int CHARGESIZE = 10;	// radius of disc repesenting point charge
const int LINESPERCHARGE = 50;
const double STEPSIZE = 3.0;

void idler();


// Charges have a position and charge.  We also keep track of the total
// charge created so we know whether any field lines will go to infinity.
//
class Charge {
  public:
	int q;	       // size and magnitude of charge	   
	Vec position;
	static int total;
	Charge(int x_in, int y_in, int q_in) : position(x_in, y_in), q(q_in) {
		total += q_in;
	}
	
};
int Charge::total(0);  // storage and initialisation for this class variable


class FieldLine {
  public:
  	vector<Vec> points;
	FieldLine(const Vec& start) {
		points.push_back(start);
	}
	void to(const Vec& nextpt){
		points.push_back(nextpt);
	}	 	 
};


// Global vectors to hold all the charges and all the field lines
// in the problem,
vector <Charge> charges;
vector <FieldLine> fieldlines;


// Calculate the direction of the field at a location by summing over
// the contribution from all charges.
//
Vec fieldDirection(const Vec& locn) {
	Vec efield;	   
	for (unsigned i = 0; i < charges.size(); i++) {
		double rcubed = pow((locn - charges[i].position).mag(), 3);
		efield = efield + charges[i].q * (locn - charges[i].position) / rcubed;
	}
	return efield.unit();	 	 
}

// Perform one Runge-Kutta step along a field line. Given a starting position
// on a field line and the magnitude of the required step, return the next position
// alog the same line.
//
Vec step (const Vec& startpos, const double stepsize ) {
	Vec trialstep = startpos + fieldDirection(startpos) * stepsize;	   
	Vec midpoint = (startpos+trialstep)/2.0;
	return startpos + fieldDirection(midpoint) * stepsize;
}

// Is  specified point close to any of the charges?
//
bool nearcharge(Vec p) {
	for (unsigned i=0; i<charges.size(); i++){
		if (p.closeto(charges[i].position)) {	// (default closeto distance is 5)
			return true;
		}
	}
	return false;
}


// Draw a charge as a disc (0-360 degree pie)
// using a colour depending on sign of charge.
//	  
void drawCharge(Charge& ch) {
	if (ch.q<0) fl_color(NEGCOLOR);
	else fl_color(POSCOLOR);
	fl_pie(int(ch.position.x-CHARGESIZE),int(ch.position.y-CHARGESIZE),
					2*CHARGESIZE, 2*CHARGESIZE, 0,360);
}
	
// Draw all the charges
//
void drawCharges() {
	for (unsigned i = 0; i < charges.size(); i++) {
		drawCharge(charges[i]);
	}
}

// Draw a field line
//
void drawFieldLine(FieldLine& line) {
	fl_color(LINECOLOR);
	fl_begin_line();
	for (unsigned i = 0; i < line.points.size(); i++){
		fl_vertex(line.points[i].x, line.points[i].y);
	}
	fl_end_line();
}	 	 

// This class makes a window for the graphical output.  In it we define
// how it should draw its contents (using data from the program) and how
// it responds to mouse clicks.  It inherits everything else from its
// parent, the fltk Fl_Window class.
// 
class DrawingWindow : public Fl_Window {
  public:
    // The ctor just makes a plain window of the right size 
	DrawingWindow(int x, int y, int w, int h) : Fl_Window(x,y,w,h) {}
	
  private:
  	// Blank out the window with background colour everywhere
    void drawBlank(){
		fl_color(BGCOLOR);
		fl_rectf(0,0,w(),h() );
	}
	  
    // draw() gets called by fltk whenever it needs the window redrawn.
	// Here we specify how to represent our data on the window.  
	void draw() {
		drawBlank();						
		// Draw all the field lines	   	   	   	   	   	     	 	 	   	   	   		
		for (unsigned i = 0; i < fieldlines.size(); i++) {
			drawFieldLine(fieldlines[i]);
		}
		
		// Draw all the charges (do last so they sit on top of lines)
		drawCharges();
	}
	
	// fltk calls handle() with events the window might want.  We use the
	// release of the mouse button to create a new charge.	  
	int handle(int event) {
		if (event == FL_RELEASE) {
			int q = 1;	// positive charge unless...
			if (Fl::event_button()==FL_RIGHT_MOUSE ) q = -1; //  makes it negative
			Charge ch(Fl::event_x(), Fl::event_y(), q);
			charges.push_back(ch);	// save the new charge
			fieldlines.clear();		// delete field lines (now invalid)
			drawBlank();			// erase drawing
			drawCharges();	  	  	// redraw all charges  	   
			Fl::set_idle(idler);	// start recalculating during next idle time
			return 1;	// Signifies we used the event
		}
		return Fl_Window::handle(event); // otherwise, pass event to our parent.
	}
};


// global pointers for the gui widgets

Fl_Button *plotButton;
Fl_Button *clearButton;
DrawingWindow *canvas;


// Start a field line from a positive charge, and follow it until it (almost) hits
// another charge.  If the net charge in the system is positive, we give up on a
// line after 400 steps because it is probably heading to infinity.  If the net
// charge is zero or negative, we allow 20000 steps, because it is sure to be coming
// back eventually.  We always allow 20 steps, to enable the line to get away from
// the originating charge.
//
void calconeline(int charge_i, int line_j) {
	Vec offset(sin(line_j*2*PI/LINESPERCHARGE), cos(line_j*2*PI/LINESPERCHARGE) );
	Vec p = charges[charge_i].position + CHARGESIZE * offset;
	FieldLine line(p);	  	  	  	     
	int nsteps=0;
	int maxsteps = 400;
	if (Charge::total <= 0)  maxsteps = 20000;
	while ( nsteps < 20 || (nsteps < maxsteps && !nearcharge(p))) {
		p = step(p, STEPSIZE);
		line.to(p);
		++nsteps;
	}	 	 	 	 
	fieldlines.push_back(line);
	drawFieldLine(line);
}

// When a new calculation is called for, this is called repeatedly at idle time.
// On each call, it calculates (and draws) one field line.  When it has
// done all lines for all charges, it stops itself from being called any more. 
void idler() {
	static int charge_i(0);
	static int line_j(0);	 
	if (charges[charge_i].q>0) calconeline(charge_i, line_j);
	if(++line_j>=LINESPERCHARGE) {
		line_j = 0;
		if(++charge_i>= charges.size()) {	// Done all charges.
			charge_i = 0;					// Reset for next time.
			drawCharges();					// Put on top of lines.
			Fl::set_idle(0);				// Don't call me any more.
		}
	}
}	 


// Callback functions

// Calculate the fieldlines from the charges, then draw them.
// This is redundant, as it duplicates in one function call what is 
// done in stages by the idle handler whenever a charge is placed.
// It can clean things up when excessive clicking has confused the
// idle handler.
//
void doPlot(Fl_Widget* w=0) {
	fieldlines.clear();
	// calculate lines for each positive charge
	for (unsigned i = 0; i < charges.size(); i++) {
		if (charges[i].q > 0) {
			for (int j = 0; j<LINESPERCHARGE; j++){ 
				calconeline(i,j);
			}
		}
	}
	canvas->redraw();	 
}


void doClear(Fl_Widget* w) {
	charges.clear();
	Charge::total = 0;
	fieldlines.clear();
	canvas->redraw();
}

int main() {
	Fl_Window *window = new Fl_Window(700,600,"Fields 1");
  
	plotButton = new Fl_Button(20,20,100,25, "Plot");	 
	clearButton = new Fl_Button(380,20,100,25, "Clear");
	
	canvas = new DrawingWindow(20,65,660,515);	   
	canvas->end(); // It's a window, but don't want the following widgets in it.	

	plotButton->callback(doPlot); 	    
	clearButton->callback(doClear);
	
	window->end();
	window->show();  
	
	return Fl::run();
}
