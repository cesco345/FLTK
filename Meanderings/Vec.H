// This header file describes a minimal Vec class that represents
// vectors in 2-dimensional space.  It has nothing to do with the
// "vector" data structure in the C++ standard library, despite the
// unfortunate similarity of names.

class Vec {
  public:
    // The only internal data that a Vec has is its x,y components.
    // It is OK to give public access to them because changing them
	// directly will not make the Vec internally inconsistent.
	double x;
	double y;
	
	// Construct a Vec from the x,y components
	Vec(double x_in=0, double y_in=0): x(x_in), y(y_in) {}
	
	// Define Vec addition
	Vec operator+(const Vec& v) const {return Vec(x+v.x, y+v.y);}
	
	// Vec subtraction
	Vec operator-(const Vec& v) const {return Vec(x-v.x, y-v.y);}
	
	// Vec magnitude
	double mag() const {return sqrt(x*x + y*y);}
	
	// Division by a constant
	Vec operator/(const double d) const {return Vec(x/d, y/d);}
	
	// Is the difference of two Vecs less than a certain separation?
	bool closeto(const Vec& v, const double sep = 5.0) const {
		return (*this - v).mag() < sep;
	}
	
	// Unit vector having same direction as original
	Vec unit() const {return *this/this->mag();} 	 	 
};

	
// Multiplication by a constant (both ways round, so place outside class)
Vec operator*(const double d, const Vec& v) {return Vec(v.x*d, v.y*d);}
Vec operator*(const Vec& v, const double d) {return Vec(v.x*d, v.y*d);}
