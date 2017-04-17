#include "ofMain.h"
#include "ofApp.h"
#include "constant.h"
#include "ofGLProgrammableRenderer.h"

int main( ){
	ofGLFWWindowSettings settings;
	settings.setGLVersion(2, 1); //we define the OpenGL version we want to use
	settings.width = WIDTH;
	settings.height = HEIGHT;
	ofCreateWindow(settings);

//	ofSetupOpenGL(WIDTH,HEIGHT,OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new ofApp());
}
