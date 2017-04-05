#pragma once
#include "MSAOpenCL.h"

typedef struct {
	float4 vel;         // velocity
	float4 accel;       // acceleration
	float4 aRand;       // acceleration random
	float4 rotFact;     // rotate factor
	float2 distLimit;   // distance limit agains attractor
	float2 dummyParam;  // dummy

	float mass;
	float lifetime;
	float life;
	bool alive;
	float limit;
	float momentum;
	float rot;
	float dummy;
} Particle;
