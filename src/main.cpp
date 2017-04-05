#include "ofMain.h"
#include "ofApp.h"
#include "constant.h"

int main( ){
	ofSetupOpenGL(WIDTH,HEIGHT,OF_WINDOW);			// <-------- setup the GL context
	ofRunApp(new ofApp());
}
