#include<iostream>
#include "vector.h"

using namespace std;

int myFunction(int value) {
	//TODO: increment the 'value' by 1	
	return value;
}

void myfunction_reference(int& val) {
	//TODO: increment the 'value' by 1
	
}


void myfunction_pointer(int* val) {
	//TODO: increment the value by one
	
	// NOTE: val++ will not work, why?

}

int main(int, char**) {
	//Task 1: C++ basics - Weightage: 30%
	
	cout << "Task 1: C++ basics" << endl;
	cout << "------------------" << endl;

	cout << "\nPassing Parameters by Value" << endl;
	//TODO:
	// 1) declare (and initialize!!) a variable "int a"
	
	// 2) implement and call "myfunction(...)"
	// 3) print the original and incremented value
	
	// QUESTION) what happens if you don't initialize a?
	// QUESTION) what happens if you declare "int a" twice?


	cout << "Passing Parameters by Reference" << endl;
	//TODO:    
	// 1) declare (and initialize!!) a variable "int c"
	
	// 2) implement and call "myfunction_reference(...)"
	
	
	// 3) print the original and incremented value


	cout << "Passing Parameters by Pointers" << endl;
	//TODO:    
	// 1) declare (and initialize!!) a variable "int d"
	
	// 2) implement and call "myfunction_pointer(...)"
	// ATTENTION: you need to obtain a pointer from "d" using "&"
	// 3) print the original and incremented value
	

	cout << endl;
	//.................................................................................................
	//Task 2: 3D vectors - Weightage: 55%

	cout << "Task 2: 3D Vector Math Example" << endl;
	cout << "------------------------------" << endl;

	Vector vec1; //vec1 is an object of Vector class
	Vector vec2 = Vector(5.0f, 0.0f, 0.0f); //Constructorfor initializing x, y, z of vec2
	vec1 = Vector();

	cout << "\nInput vectors:" << endl;
	cout << "vec1: " << vec1; //You should see some garbage values for vec1.x, vec1.y, vec1.z
	cout << "vec2: " << vec2;
	cout << endl;

	//set vec1 to a zero vector
	vec1.Zero();

	cout << "Assigned vec1 to zero vector:" << endl;
	cout << vec1 << endl;

	//vector addition
	cout << "Sum of vec1 and vec2:" << endl;
	cout << vec1 + vec2 << endl;

	//vector subtraction
	cout << "Subtract vec1 from vec2:" << endl;
	cout << vec2 - vec1 << endl;


	//scalar multiplication
	cout << "Multipl vec2 with 2:" << endl;
	int s = 2;
	cout << vec2 * s << endl;

	//assignment- set vec1 to vec2
	cout << "Assign vec1 with 2*vec2:" << endl;
	vec1 = vec2 * s;
	cout << vec1 << endl;


	//dot product
	cout << "Now" << endl;
	cout << "vec1:" << vec1;
	cout << "vec2:" << vec2;
	cout << "vec1 dot vec2:" << endl;
	cout << vec1 * vec2 << endl;
	cout << endl;

	//Vector norm
	Vector vec3 = Vector(2.0f, 5.0f, 2.0f);
	cout << "Euclidean norm of vector " << vec3;
	cout << vec3.VectorNorm() << endl;
	cout << endl;

	//cross product
	cout << "vec2:" << vec2;
	cout << "vec3:" << vec3;
	cout << "vec2 cross vec3" << endl;
	cout << vec2.Cross(vec3) << endl;
	
	cout << endl;
	
	//..................................................................................................
	//Task 3: Given two vectors, construct an orthonormal Basis in 3D - Weightage: 15%

	cout << "Task 3: Cross product Method" << endl;
	cout << "----------------------------"<<endl;
	
	Vector x = Vector(1.0f, -1.0f, 0.0f);
	Vector y = Vector(1.0f, 1.0f, 0.0f);
	cout << "x:" << x;
	cout << "y:" << y;
	
	//TODO: set the first basis vector w
	Vector w = Vector(0.0f, 0.0f, 0.0); //NOTE: this is just a dummy. You should get the actual value of w (refer to slides)	
	cout << "w:" << w;
	
	//TODO: create the second basis vector	
	cout << "u:" << w;
	
	//TODO: create the third basis vector	
	cout << "v:" << w;	
	
	cout << endl;

	//..................................................................................................
	//Task 4: Given two vectors, construct a 2D orthonormal Basis using Gram-schmidt's method - Weightage: 15%

	cout << "Task 4: Gram-Schmidt's Method"<<endl;
	cout << "-----------------------------"<<endl;
	Vector u1 = Vector(1.0f, -1.0f, 0.0f);
	Vector u2 = Vector(1.0f, 1.0f, 0.0f);
	
	cout << "\nu1:" << u1;
	cout << "u2:" << u2;

	
	//TODO: Construct the vector e1
	Vector e1 = Vector(0.0f, 0.0f, 0.0); //NOTE: this is just a dummy. You should get the actual value of e1 (refer to slides)

	cout << "e1:" << e1;

	//TODO: Construct the vector u_tilde
	Vector u2_proj = e1 * (u2 * e1);
	//Vector u_tilde = ----------;


	//TODO: Construct e2
	
	cout << "e2:" << e1;

		
	system("pause");

	return EXIT_SUCCESS;
}

