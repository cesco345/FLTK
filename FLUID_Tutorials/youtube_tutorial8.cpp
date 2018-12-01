#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "Fl_ViewerCV.h"

using namespace cv;
using namespace std;

cv::Mat dst, detected_edges;
cv::Mat src, src_gray;

int lowThreshold = 75;
const int ratio = 3;
const int kernel_size = 3;
static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(0.5,0.5) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
}


int main()
{
    Fl_Window * mw = new Fl_Window(900, 750, "OpenCV & FLTK Demo");
    Fl_ViewerCV * v = new Fl_ViewerCV(100, 0, 350, 350);
    Fl_ViewerCV * w = new Fl_ViewerCV(450,0, 350, 350);
    Fl_ViewerCV * z = new Fl_ViewerCV(300,400, 350, 350);
    mw->end();
    mw->resizable(v);
    mw->resizable(w);
    mw->resizable(z);
    cv::Mat mat=cv::imread("volks.jpeg");
    cvtColor( mat, src_gray, CV_BGR2GRAY );

    blur( src_gray, detected_edges, Size(3,3) ); 
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    v->SetImage(&mat);
    w->SetImage(&src_gray);
    z->SetImage(&detected_edges);
    v->UpdateView();
    w->UpdateView();
    z->UpdateView();


    mw->show();

    Fl::run();

    return 0;
}
