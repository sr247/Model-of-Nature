#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1600, 800, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	try {
		ofRunApp(new ofApp());	
	}
	catch (exception e) {
		fstream f("error.txt");
		f << "ofMain::main" << e.what() << '\n';
	}

}
