#include "ofApp.h"
#define USE_OPENGL_CONTEXT

void ofApp::setup() {
	cameraController.setup();
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofEnableSmoothing();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	oscInterpreter.setup(15000, &emitter, &particleController, &cameraController, &worldPhysics, &effector);

	opencl.setupFromOpenGL();
	opencl.loadProgramFromFile("MSAOpenCL/Particle.cl");
	kernelUpdate = opencl.loadKernel("updateParticle");

	particleController.setup(&emitter, &worldPhysics);
	particleController.initCL(kernelUpdate);

}




void ofApp::update() {
	oscInterpreter.processMessage(); // renew numspawn
	worldPhysics.updateCLKernel(kernelUpdate);
	kernelUpdate->run1D(NUM_PARTICLES);
}

void ofApp::draw() {

	//    effect2DFbo.begin(); // begin 2D recording
	ofClear(0, 0, 0);
	//    recursiveBlur(); // draw recursive blur

	cameraController.begin();
	glLineWidth(2);
	
	//opencl.finish();

	//render particles
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[0]);
	//    glVertexPointer(4, GL_FLOAT, 0, 0);

	//    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[1]);
	//    glColorPointer(4, GL_FLOAT, 0, 0);

	//    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo[2]);
	//    glNormalPointer(GL_FLOAT, 0, 0);

	//	glDrawArrays(GL_LINES, 0, NUM_LINES);
	//    glPushMatrix();
	//    glScalef(-1, -1, -1);
	//	glDrawArrays(GL_LINES, 0, NUM_LINES);
	//    glPopMatrix();

	//	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	//	glDisableClientState(GL_VERTEX_ARRAY);
	//	glDisableClientState(GL_COLOR_ARRAY);
	//	glDisableClientState(GL_NORMAL_ARRAY);

	cameraController.end();
	effector.process();
}
