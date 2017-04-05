#pragma once

#include "constant.h"
#include "ofMain.h"

class CameraController {
public:
	void setup();
	void setCameraSwitch(int);
	void setCameraAzimuth(float);
	void setCameraElevation(float);
	void setCameraDistance(float);
	void setSubcamAzimuth(float);
	void setSubcamElevation(float);
	void setSubcamDistance(float);
	void begin();
	void end();

private:

	void updateCameraPosition();
	void updateSubcamPosition();
	ofCamera camera;
	ofCamera subcam;

	float cameraAzimuth;
	float cameraElevation;
	float cameraDistance;

	float subcamAzimuth;
	float subcamElevation;
	float subcamDistance;

	int cswitch;

};

inline void CameraController::begin() {
	if (cswitch == 0) {
		camera.lookAt(ofVec3f(0, 0, 0));
		camera.begin();
	} else {
		subcam.lookAt(ofVec3f(0, 0, 0));
		subcam.begin();
	}
}

inline void CameraController::end() {
    if(cswitch == 0){
        camera.end();
    }else{
        subcam.end();
    }
}

inline void CameraController::setCameraSwitch(int cs) {
	cswitch = cs;
}

inline void  CameraController::setCameraAzimuth(float az) {
	cameraAzimuth = az;
	updateCameraPosition();
}

inline void  CameraController::setCameraElevation(float el) {
	cameraElevation = el;
	updateCameraPosition();
}

inline void  CameraController::setCameraDistance(float dist) {
	cameraDistance = dist;
	updateCameraPosition();
}

inline void  CameraController::setSubcamAzimuth(float az) {
	subcamAzimuth = az;
	updateSubcamPosition();
}

inline void  CameraController::setSubcamElevation(float el) {
	subcamElevation = el;
	updateSubcamPosition();
}

inline void  CameraController::setSubcamDistance(float dist) {
	subcamDistance = dist;
	updateSubcamPosition();

}

inline void CameraController::updateCameraPosition() {
	camera.setPosition(sin(cameraAzimuth * RADIAN) * cameraDistance,
		sin(cameraElevation * RADIAN) * cameraDistance,
		cos(cameraAzimuth * RADIAN) * cos(cameraElevation * RADIAN) * cameraDistance);
}

inline void CameraController::updateSubcamPosition() {
	subcam.setPosition(sin(subcamAzimuth * RADIAN) * subcamDistance,
		sin(subcamElevation * RADIAN) * subcamDistance,
		cos(subcamAzimuth * RADIAN) * cos(subcamElevation * RADIAN) * subcamDistance);
}

inline void CameraController::setup() {
	cswitch = 0;

	camera.setNearClip(10);
	camera.setFarClip(10000);
	camera.lookAt(ofVec3f(0, 0, 0));
	camera.setPosition(0, 0, 300);

	subcam.setNearClip(10);
	subcam.setFarClip(10000);
	subcam.lookAt(ofVec3f(0, 0, 0));
	subcam.setPosition(0, 0, 300);
}