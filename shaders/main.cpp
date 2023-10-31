#include "Helpers/GlError.h"
#include "Triangle/Triangle.h"
#include "Quad/Quad.h"


Triangle triangle;
//Quad quad; //Uncomment for Task2, comment for Task 1

///--- Vertex position of the triangle
const GLfloat vpoint[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,};


void init(){
    ///--- Sets background color
    glClearColor(/*white*/ 1.0, 1.0, 1.0, /*solid*/1.0 );
    triangle.init(); //Uncomment for Task1, comment for Task 2
    //quad.init();  //Uncomment for Task2, comment for Task 1

}

void display(){

    glClear(GL_COLOR_BUFFER_BIT);
    triangle.draw(); //Uncomment for Task1, comment for Task 2
    //quad.draw();  //Uncomment for Task2, comment for Task 1 

}

int main(int, char**){
	GLFWwindow* window;
    
	glewExperimental = true; // for core profile

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//versions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

	// Open a window and create its OpenGL context

	window = glfwCreateWindow(1024, 768, "Shaders", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Initialize GLEW

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	init();

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	do {
		// Clear the screen. 
		glClear(GL_COLOR_BUFFER_BIT);
		display();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

    triangle.cleanup(); //Uncomment for Task1, comment for Task 2
    //quad.cleanup();     //Uncomment for Task 2, comment for Task 1

    return EXIT_SUCCESS;
}
