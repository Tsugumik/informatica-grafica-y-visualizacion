#include <cstdlib>

#include "cgvInterface.h"
#include "iostream"

using namespace std;

cgvInterface cgvInterface;
int main (int argc, char** argv) {
	// initialize the display window
    cgvInterface.configure_environment(argc,argv,
	                           500,500, // window size
                               100,100, // window position
                               std::string("CGV. Practice 2b.") // title of the window
                                );

	// define the callbacks to manage the events. 
	cgvInterface.initialize_callbacks();

	// initialize the loop of the OpenGL visualization
	cgvInterface.start_display_loop();

	return(0);
}
