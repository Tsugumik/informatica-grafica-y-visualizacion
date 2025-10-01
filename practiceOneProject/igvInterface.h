#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)

#include <GLUT/glut.h>
//#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>
#include "src/objects/Cone.h"
#include "src/objects/Cube.h"
#include "src/objects/Sphere.h"
#include "src/Camera.h"

/**
 * Class to encapsulate the application's configuration and interface
 */
class igvInterface {
private:
	Cube *cube;
	Cone *cone;
	Sphere *sphere;

	Object3D *selectedObject;
	int currentObject; // 0=none, 1=cube, 2=sphere, 3=cone
	bool transformationMode; // true=RST, false=sequential

	Camera* camera;
	bool cameraMode;


	// Atributos
	int window_width = 0; ///< Initial width of the display window
	int window_height = 0; ///< Initial height of the display window

	// Application of the Singleton pattern
	static igvInterface *_instance; ///< Pointer to the only object of the class

public:
	// Application of the Singleton pattern
	static igvInterface &getInstance();

	igvInterface();

	/// Destroyer
	~igvInterface() = default;

	// Statics methods
	// event callbacks
	static void keyboardFunc(unsigned char key, int x, int y); // method for controlling keyboard events

	static void specialKeyboardFunc(int key, int x, int y); // method for controlling special keyboard events

	static void reshapeFunc(int w, int h); // method that defines the camera view and viewport
	// is automatically called when the window size is changed
	static void displayFunc(); // method for visualizing the scene


	// Methods
	// initializes all parameters to create a display window
	void configure_environment(int argc, char **argv // main parameters
	                           , int _window_width, int _window_height // width and height of the display window
	                           , int _pos_X, int _pos_Y // initial position of the display window
	                           , std::string _title // title of the display window
	);

	void initialize_callbacks(); // initializes all callbacks

	void start_display_loop(); // display the scene and wait for events on the interface

	// get_ and set_ methods for accessing attributes
	int get_window_width();

	int get_window_height();

	void set_window_width(int _window_width);

	void set_window_height(int _window_height);

	void selectObject(int objectNum);
};

#endif   // __IGVINTERFACE
