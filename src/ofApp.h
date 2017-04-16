#pragma once

#include "constant.h"
#include "ofMain.h"
#include "OSCInterpreter.h"
#include "CameraController.h"
#include "ParticleController.h"

#include "Emitter.h"
#include "Effector.h"
#include "WorldPhysics.h"

class ofApp : public ofBaseApp {

public:	
	void setup();
	void update();
	void draw();

private:

	msa::OpenCL opencl;
	std::shared_ptr<msa::OpenCLKernel> kernelUpdate;
	std::shared_ptr<msa::OpenCLKernel> kernelTest;

	ParticleController particleController;
	Emitter emitter;
	CameraController cameraController;
	Effector effector;
	WorldPhysics worldPhysics;
	OSCInterpreter oscInterpreter;
};



