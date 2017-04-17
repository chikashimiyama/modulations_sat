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
		std::string firstElement = address[0];
		if (firstElement == "camera") {
			processMessageToCamera(mes, address);
		} else if (firstElement == "subcam") {
			processMessageToSubcam(mes, address);
		} else if (firstElement == "emitter") {
			processMessageToEmitter(mes, address);
		} else if (firstElement == "world") {
			processMessageToWorld(mes, address);
		} else if (firstElement == "effect") {
			processMessageToEffect(mes, address);
		} else if (firstElement == "fullscreen") {
			bool fs =  static_cast<bool>(mes.getArgAsInt32(0));
			ofSetFullscreen(fs);
		}else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + address[0];
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
	} else {
		ofLog(OF_LOG_WARNING) << "undefined message:" + secondElement;
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
	} else {
		ofLog(OF_LOG_WARNING) << "undefined message:" + secondElement;
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
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	} else if (secondElement == "iVel") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialVelocity(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialVelocity(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialVelocity(Direction::z, mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	} else if (secondElement == "iAccel") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialAcceleration(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialAcceleration(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialAcceleration(Direction::z, mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	} else if (secondElement == "iAccelRand") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialAccelerationRandom(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialAccelerationRandom(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialAccelerationRandom(Direction::z, mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	} else if (secondElement == "iRotFact") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			emitter->setInitialRotationFactor(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			emitter->setInitialRotationFactor(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			emitter->setInitialRotationFactor(Direction::z, mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
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
	} else {
		ofLog(OF_LOG_WARNING) << "undefined message:" + secondElement;
	}
}


inline void OSCInterpreter::processMessageToWorld(ofxOscMessage &mes, const std::vector<std::string> &address) {
	std::string secondElement = address[1];

	if (secondElement == "rev") {
		worldPhysics->setReverse(mes.getArgAsFloat(0));
	}else if (secondElement == "revFact") {
		std::string thirdElement = address[2];
		if (thirdElement == "x") {
			worldPhysics->setReverseFactor(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			worldPhysics->setReverseFactor(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "z") {
			worldPhysics->setReverseFactor(Direction::z, mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	}else if (secondElement == "attraction") {
		worldPhysics->setAttraction(mes.getArgAsFloat(0));
	}else if (secondElement == "attDetach") {
		worldPhysics->setAttractionDetach(mes.getArgAsFloat(0));
	}else if (secondElement == "gravity") {
		worldPhysics->setGravity(mes.getArgAsFloat(0));
	} else {
		ofLog(OF_LOG_WARNING) << "undefined message:" + secondElement;
	}
}

inline void OSCInterpreter::processMessageToEffect(ofxOscMessage &mes , const std::vector<std::string> &address) {
	std::string secondElement = address[1];
	std::string thirdElement = address[2];

	if (secondElement == "bloom") {
		if (thirdElement == "x") {
			effector->setBloomSpread(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			effector->setBloomSpread(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "gain") {
			effector->setBloomGain(mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	} else if (secondElement == "blur") {
		if (thirdElement == "x") {
			effector->setBlur(Direction::x, mes.getArgAsFloat(0));
		} else if (thirdElement == "y") {
			effector->setBlur(Direction::y, mes.getArgAsFloat(0));
		} else if (thirdElement == "rotate") {
			effector->setBlurRotate(mes.getArgAsFloat(0));
		} else if (thirdElement == "brightness") {
			effector->setBlurBrightness(mes.getArgAsFloat(0));
		} else {
			ofLog(OF_LOG_WARNING) << "undefined message:" + thirdElement;
		}
	} else {
		ofLog(OF_LOG_WARNING) << "undefined message:" + secondElement;
	}
}

inline std::vector<std::string> OSCInterpreter::separateAddress(const std::string& str) const{
	std::istringstream iss(str);
	std::string token;
	std::vector<std::string> separated;
	while (std::getline(iss, token, '/')) {
		separated.push_back(token);
	}
	separated.erase(separated.begin());
	return separated;
}
