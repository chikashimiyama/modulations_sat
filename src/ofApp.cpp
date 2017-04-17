#include "ofApp.h"
#define USE_OPENGL_CONTEXT

void ofApp::setup() {
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	ofEnableSmoothing();
	cameraController.setup();

	opencl.setupFromOpenGL();
	opencl.loadProgramFromFile("MSAOpenCL/Particle.cl");
	kernelUpdate = opencl.loadKernel("updateParticle");

	particleController.setup(&emitter, &worldPhysics, kernelUpdate);
	effector.setup();
	oscInterpreter.setup(15000, &emitter, &particleController, &cameraController, &worldPhysics, &effector);
	glLineWidth(2);
}

void ofApp::update() {
	oscInterpreter.processMessage(); // renew numspawn
	particleController.update();
	worldPhysics.updateCLKernel(kernelUpdate);
	kernelUpdate->run1D(NUM_PARTICLES);
	ofLog() << ofGetFrameRate();
}

void ofApp::draw() {

	effector.record();
	ofClear(0, 0, 0);
	effector.drawBlur();
	opencl.finish();


	cameraController.begin();
	particleController.drawParticles();
	cameraController.end();

	effector.process();
}
