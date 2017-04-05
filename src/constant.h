#pragma once

//#define WIDTH 1024
//#define HEIGHT 768

#include "MSAOpenCL.h"
#include "Particle.h"

const int WIDTH = 1280;
const int HEIGHT = 720;
const int NUM_PARTICLES = 20000;
const int NUM_LINES = NUM_PARTICLES * 2;
const float RADIAN = 0.01744f;
const size_t LINE_SIZE = sizeof(float4) * 2;
const size_t PARTICLE_SIZE = sizeof(Particle);

enum class Direction {
	x,
	y,
	z
};