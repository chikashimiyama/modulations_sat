#pragma once

#include "ofxOscReceiver.h"
#include "CameraController.h"
#include "ParticleController.h"
#include "Effector.h"
#include "WorldPhysics.h"
#include "Emitter.h"


class OSCInterpreter {
public:
	OSCInterpreter(){}
	void setup(int, Emitter*, ParticleController*, CameraController*, WorldPhysics*, Effector*);
	void processMessage();
private:
	void processMessageToEmitter(ofxOscMessage &, const std::vector<std::string> &address);
	void processMessageToCamera(ofxOscMessage &, const std::vector<std::string> &address);
	void processMessageToSubcam(ofxOscMessage &, const std::vector<std::string> &address);

	void processMessageToWorld(ofxOscMessage &, const std::vector<std::string> &address);
	void processMessageToEffect(ofxOscMessage &, const std::vector<std::string> &address);
	std::vector<std::string> separateAddress(const std::string&) const;
	ofxOscReceiver oscReceiver;

	Emitter *emitter;
	ParticleController *particleController;
	CameraController *cameraController;
	WorldPhysics *worldPhysics;
	Effector *effector;
};

inline void OSCInterpreter::setup(int port, Emitter* em, ParticleController* pt, CameraController* cc, WorldPhysics* wp, Effector* ef) {
	oscReceiver.setup(port);
	emitter = em;
	particleController = pt;
	cameraController = cc;
	worldPhysics = wp;
	effector = ef;	
}

inline void OSCInterpreter::processMessage() {
	while (oscReceiver.hasWaitingMessages()) {
		ofxOscMessage mes;
		oscReceiver.getNextMessage(&mes);
		std::vector<std::string> address = separateAddress(mes.getAddress());
		if (address[0] == "camera") {
			processMessageToCamera(mes, address);
		} else if (address[0] == "subcam") {
			processMessageToSubcam(mes, address);
		} else if (address[0] == "emitter") {
			processMessageToEmitter(mes, address);
		} else if (address[0] == "world") {
			processMessageToWorld(mes, address);
		} else if (address[0] == "effect") {
			processMessageToEffect(mes, address);
		}
	}
}

inline void OSCInterpreter::processMessageToCamera(ofxOscMessage &mes, const std::vector<std::string> &address) {
	
	std::string secondElement = address[1];
	if (secondElement == "switch") {
		cameraController->setCameraSwitch(mes.getArgAsInt32(0));
	} else if (secondElement == "azimuth") {
		cameraController->setCameraAzimuth(mes.getArgAsFloat(0));
	} else if (secondElement == "elevation") {
		cameraController->setCameraElevation(mes.getArgAsFloat(0));
	} else if (secondElement == "distance") {
		cameraController->setCameraDistance(mes.getArgAsFloat(0));
	}
}

inline void OSCInterpreter::processMessageToSubcam(ofxOscMessage &mes, const std::vector<std::string> &address) {

	std::string secondElement = address[1];
	if (secondElement == "azimuth") {
		cameraController->setSubcamAzimuth(mes.getArgAsFloat(0));
	} else if (secondElement == "elevation") {
		cameraController->setSubcamElevation(mes.getArgAsFloat(0));
	} else if (secondElement == "distance") {
		cameraController->setSubcamDistance(mes.getArgAsFloat(0));
	}
}


inline void OSCInterpreter::processMessageToEmitter(ofxOscMessage &mes, const std::vector<std::string> &address) {
	
	std::string secondElement = address[1];
	if(secondElement == "location") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			particleController->setInitialLocation(Direction::x, mes.getArgAsFloat(0));
			worldPhysics->setInitialAttractorLocation(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			particleController->setInitialLocation(Direction::y, mes.getArgAsFloat(0));
			worldPhysics->setInitialAttractorLocation(Direction::y,mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			particleController->setInitialLocation(Direction::z, mes.getArgAsFloat(0));
			worldPhysics->setInitialAttractorLocation(Direction::z, mes.getArgAsFloat(0));
		}
	} else if (secondElement == "iVel") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialVelocity(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialVelocity(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialVelocity(Direction::z, mes.getArgAsFloat(0));
		}
	} else if (secondElement == "iAccel") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialAcceleration(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialAcceleration(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialAcceleration(Direction::z, mes.getArgAsFloat(0));
		}
	} else if (secondElement == "iAccelRand") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialAccelerationRandom(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialAccelerationRandom(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialAccelerationRandom(Direction::z, mes.getArgAsFloat(0));
		}
	} else if (secondElement == "iRotFact") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialRotationFactor(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialRotationFactor(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialRotationFactor(Direction::z, mes.getArgAsFloat(0));
		}
	} else if (secondElement == "iLifetime") {
		emitter->setInitialLifetime(mes.getArgAsInt32(0));
	} else if (secondElement == "iLimit") {
		emitter->setInitialLimit(mes.getArgAsFloat(0));
	} else if (secondElement == "iBrightness") {
		emitter->setInitialBrightness(mes.getArgAsFloat(0));
	} else if (secondElement == "iMomentum") {
		emitter->setInitialMomentum(mes.getArgAsFloat(0));
	} else if (secondElement == "iRot") {
		emitter->setInitialRotation(mes.getArgAsFloat(0));
	} else if (secondElement == "distLimit") {
		std::string thirdElement = address[2];
		if (thirdElement == "near") {
			emitter->setNearDistanceLimit(mes.getArgAsFloat(0));
		} else if (thirdElement == "far") {
			emitter->setFarDistanceLimit(mes.getArgAsFloat(0));
		}
	} else if (secondElement == "iMassMin") {
		float massMin = mes.getArgAsFloat(0);
		if (massMin < 0.1) {
			massMin = 0.1;
		}
		emitter->setInitialMassMin(massMin);
	} else if (secondElement == "iMassMax") {
		float massMax = mes.getArgAsFloat(0);
		if (massMax < 0.1) {
			massMax = 0.1;
		}
		emitter->setInitialMassMax(massMax);
	} else if (secondElement == "numSpawn") {
		particleController->setNumberOfSpawn(mes.getArgAsInt32(0));
	} else if (secondElement == "iHole") {
		particleController->setInitialHole(mes.getArgAsFloat(0));
	} else if (secondElement == "iDiffuse") {
		particleController->setInitialDiffuse(mes.getArgAsFloat(0));
	}
}


inline void OSCInterpreter::processMessageToWorld(ofxOscMessage &mes, const std::vector<std::string> &address) {
	std::string secondElement = address[1];
/*
	if (secondElement == "rev") {
		rev = mes.getArgAsFloat(0);
	}else if (secondElement == "revFact") {
		std::string thirdElement = address[2];
		revFact.x = mes.getArgAsFloat(0);
	}else if (secondElement == "revFact/y") {
		revFact.y = mes.getArgAsFloat(0);
	}else if (secondElement == "revFact/z") {
		revFact.z = mes.getArgAsFloat(0);
	}else if (secondElement == "attraction") {
		attraction = mes.getArgAsFloat(0);
	}else if (secondElement == "attDetach") {
		attDetach = mes.getArgAsFloat(0);
	}else if (secondElement == "gravity") {
		gravity = mes.getArgAsFloat(0);
	}
	*/
}

inline void OSCInterpreter::processMessageToEffect(ofxOscMessage &mes , const std::vector<std::string> &address) {
	/*
	if (secondElement == "/effect/bloom/x") {
		bloomSpread[0] = m.getArgAsFloat(0);
	}else if (secondElement == "/effect/bloom/y") {
		bloomSpread[1] = m.getArgAsFloat(0);
	}else if (secondElement == "/effect/bloom/gain") {
		bloomGain = m.getArgAsFloat(0);
	}else if (secondElement == "/effect/blur/x") {
		blur[0] = m.getArgAsFloat(0);
	}else if (secondElement == "/effect/blur/y") {
		blur[1] = m.getArgAsFloat(0);
	}else if (secondElement == "/effect/blur/rotate") {
		blurRotate = m.getArgAsFloat(0);
	}else if (secondElement == "/effect/blur/brightness") {
		blurBrightness = m.getArgAsFloat(0);
	}else if (secondElement == "/fullscreen") {
		int fs = m.getArgAsInt32(0);
		if (fs > 0) {
			ofSetFullscreen(true);
		}
		else {
			ofSetFullscreen(false);
		}
	}
	*/
}

inline std::vector<std::string> OSCInterpreter::separateAddress(const std::string& str) const{
	std::istringstream iss(str);
	std::string token;
	std::vector<std::string> separated;
	while (std::getline(iss, token, '/')) {
		separated.push_back(token);
	}
	return separated;
}
