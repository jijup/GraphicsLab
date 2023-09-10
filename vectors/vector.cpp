#include "vector.h"

using namespace std;


Vector& Vector::operator =(const Vector &v){
    //TODO: Assign x, y, z values of 'v' to 'this' vector
    return *this;
}

bool Vector::operator ==(const Vector &v) const {
	//TODO: Check whether each (x, y, z) component of 'v' is equal to 'this' components.
    
}

void Vector::Zero(){
    //TODO: Initialize 'thos' components to 0.0
}

Vector Vector::operator-() const {
	//TODO: return a new vector with negated components, i.e., -x, -y, -z
  
}

Vector Vector::operator +(const Vector &v) const {
	//TODO: add respective components (x+v.x,...) and return the resultant vector.
    
}

Vector Vector::operator-(const Vector &v) const {
    //TODO: subtract respective components (x-v.x,...) and return the resultant vector.
}


Vector Vector::operator *(float s) const {
	//TODO: multiply 'this' vector with scalar 's' and return the scaled vector
    
}
Vector Vector::operator /(float s) const {
	//TODO: a variant of scalar multiplication where you multiply 'this' vector with 1/s
    
}

float Vector::VectorNorm() {
	//TODO: Use the formula discussed in the class to compute the norm of 'this' vector
}

void Vector::Normalize() {
	//TODO: normalize 'this' vector using the formula. Check for divide by zero.    
}


float Vector::operator *(const Vector &v) const {
    //TODO: compute the dot product of 'this' and 'v'. Return the scalar value.
}


Vector Vector::Cross(const Vector &v) {
    //TODO: Compute the cross product of 'this' and 'v' vectors. Return the resultant vector. Refer to the slides for the formula.
}



std::ostream& operator<< (std::ostream& os, const Vector& v){
    os<<v.x<<" "<<v.y<<" "<<v.z<<endl;
    return os;
}
