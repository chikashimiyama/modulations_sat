#pragma once

#include "constant.h"
#include "MSAOpenCL.h"
#include "Particle.h"

/* a factory class that spawns new particle based on the given parameters */

class Emitter {

public:
	Emitter():
		attLocation(0, 0, 0, 1.0),
		iVel(0.05, 0.05, 0.05, 0.0),
		iAccel(0.05, 0.05, 0.05, 0),
		iAccelRand(0, 0, 0, 0),
		iLifetime(0),
		iLimit(20),
		iMassMin(1.0),
		iMassMax(20.0),
		iMomentum(1.0),
		iRotFact(0, 0, 0, 1.0),
		iRot(0),
		distLimit(5, 1000),
		iBrightness(1.0){}

	void setInitialVelocity(Direction dir, const float &);
	void setInitialAcceleration(Direction dir, const float &);
	void setInitialAccelerationRandom(Direction dir, const float &);
	void setInitialRotationFactor(Direction dir, const float &);

	void setInitialLifetime(const float &);
	void setInitialLimit(const float &);
	void setInitialBrightness(const float &);
	void setInitialMassMin(const float &);
	void setInitialMassMax(const float &);
	void setInitialMomentum(const float &);
	void setInitialRotation(const float &);
	void setNearDistanceLimit(const float &);
	void setFarDistanceLimit(const float &);

	Particle spawnParticle() const;

private:
	float4 iVel;
	float4 iAccel;
	float4 iAccelRand;
	float4 iRotFact;
	float4 attLocation;
	float2 distLimit;

	float iDiffuse;
	float iLifetime;
	float iLimit;
	float iBrightness;
	float iMassMin;
	float iMassMax;
	float iMomentum;
	float iRot;
};

inline void Emitter::setInitialVelocity(Direction dir, const float &velocity) {
	switch (dir) {
		case Direction::x:
			iVel.x = velocity; break;
		case Direction::y:
			iVel.y = velocity; break;
		case Direction::z:
			iVel.z = velocity; break;
	}
}

inline void Emitter::setInitialAcceleration(Direction dir, const float &accel) {
	switch (dir) {
		case Direction::x:
			iAccel.x = accel; break;
		case Direction::y:
			iAccel.y = accel; break;
		case Direction::z:
			iAccel.z = accel; break;
	}
}

inline void Emitter::setInitialAccelerationRandom(Direction dir, const float &accelRand) {
	switch (dir) {
		case Direction::x:
			iAccelRand.x = accelRand; break;
		case Direction::y:
			iAccelRand.y = accelRand; break;
		case Direction::z:
			iAccelRand.z = accelRand; break;
	}
}

inline void Emitter::setInitialRotationFactor(Direction dir, const float &rotFact) {
	switch (dir) {
	case Direction::x:
		iRotFact.x = rotFact; break;
	case Direction::y:
		iRotFact.y = rotFact; break;
	case Direction::z:
		iRotFact.z = rotFact; break;
	}
}

inline void Emitter::setNearDistanceLimit(const float& dlim) {
	distLimit.x = dlim;
}

inline void Emitter::setFarDistanceLimit(const float &dlim) {
	distLimit.y = dlim;
}


inline void Emitter::setInitialLifetime(const float &lifetime) {
	iLifetime = lifetime;
}

inline void Emitter::setInitialLimit(const float &limit) {
	iLimit = limit;
}

inline void Emitter::setInitialBrightness(const float &brightness) {
	iBrightness = brightness;
}

inline void Emitter::setInitialMassMin(const float &massMin) {
	iMassMin = massMin;
}

inline void Emitter::setInitialMassMax(const float &massMax) {
	iMassMax = massMax;
}

inline void Emitter::setInitialMomentum(const float &momentum) {
	iMomentum = momentum;
}

inline void Emitter::setInitialRotation(const float &rot) {
	iRot = rot;
}

inline Particle Emitter::spawnParticle() const {
	Particle p;
	p.vel = float4(ofRandom(iVel.x, -iVel.x),
		ofRandom(iVel.y, -iVel.y),
		ofRandom(iVel.z, -iVel.z), 0.0);

	p.accel = float4(ofRandom(iAccel.x, -iAccel.x),
		ofRandom(iAccel.y, -iAccel.y),
		ofRandom(iAccel.z, -iAccel.z), 0.0);

	p.aRand = float4(iAccelRand.x, iAccelRand.y, iAccelRand.z, 0.0);
	p.mass = ofRandom(iMassMin, iMassMax);
	p.alive = true;
	p.lifetime = iLifetime;
	p.life = iLifetime * iBrightness;
	p.limit = iLimit;
	p.momentum = iMomentum;
	p.rot = iRot;
	p.rotFact = iRotFact;
	p.distLimit = distLimit;
	return std::move(p);

}
