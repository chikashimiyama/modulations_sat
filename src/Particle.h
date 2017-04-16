#pragma once
#include "MSAOpenCL.h"

typedef struct {
	float4 vel;         // velocity								16
	float4 accel;       // acceleration							32
	float4 aRand;       // acceleration random					48
	float4 rotFact;     // rotate factor						64
	float2 distLimit;   // distance limit agains attractor		72
	float2 dummyParam;  // dummy								80

	float mass;			//	84
	float lifetime;		//	88
	float life;			//	92
	bool alive;			//	96
	float limit;		//	100
	float momentum;		//	104
	float rot;			//	108
	float dummy;		//	112
} Particle;

// 112 = 16 * 7 

