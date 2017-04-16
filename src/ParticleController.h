#pragma once
#include <memory>
#include "constant.h"
#include "MSAOpenCL.h"
#include "Particle.h"
#include "Emitter.h"
#include "WorldPhysics.h"

class ParticleController {
public:
	ParticleController():
		prev(0),
		count(0),
		iLocation(0, 0, 0, 1.0){}

	void setup(Emitter*, WorldPhysics*, std::shared_ptr<msa::OpenCLKernel>);

	void setNumberOfSpawn(const int);
	void setInitialHole(const float);
	void setInitialDiffuse(const float);
	void setInitialLocation(Direction dir, const float &);

	void update();
	void drawParticles();
private:

	float4 randomizedPosition() const;
	void setParticlePosition(int index);
	void vboSetup();
	void clKernelInit(std::shared_ptr<msa::OpenCLKernel> kernelUpdate);

	Emitter* emitter;
	WorldPhysics* worldPhysics;

	msa::OpenCLBufferManagedT<Particle> particlesMemory;
	msa::OpenCLBufferManagedT<float4> randomTableMemory; 
	msa::OpenCLBufferManagedT<float4> positionMemory;
	msa::OpenCLBufferManagedT<float4> colorMemory;

	int numSpawn;
	float iHole;
	float iDiffuse;
	float4 iLocation;
	int prev;
	int count;
	GLuint vboIDs[2];
};


inline void ParticleController::setup(Emitter* em, WorldPhysics *wp, std::shared_ptr<msa::OpenCLKernel> kernelUpdate) {
	emitter = em;
	worldPhysics = wp;
	vboSetup();
	clKernelInit(kernelUpdate);
}


inline void ParticleController::vboSetup() {

	glGenBuffersARB(2, vboIDs);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDs[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float4) * NUM_LINES, 0, GL_DYNAMIC_COPY_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDs[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float4) * NUM_LINES, 0, GL_DYNAMIC_COPY_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

inline void ParticleController::clKernelInit(std::shared_ptr<msa::OpenCLKernel> kernelUpdate) {
	
	std::vector<Particle> particlesInitData;
	std::vector<float4> positionInitData;
	std::vector<float4> colorInitData;
	std::vector<float4> randomTableInitData;

	for (int i = 0; i<NUM_PARTICLES; i++) {
		Particle p;
		p.vel.set(0, 0, 0, 0);
		p.accel.set(ofRandom(-0.001, 0.001), 0.0, 0, 0);
		p.mass = ofRandom(0.5, 1);
		particlesInitData.push_back(p);
		positionInitData.emplace_back(0, 0, 0, 1.0);
		positionInitData.emplace_back(0, 0, 0, 1.0);
		colorInitData.emplace_back(1.0, 1.0, 1.0, 1.0);
		colorInitData.emplace_back(1.0, 1.0, 1.0, 1.0);
		randomTableInitData.emplace_back(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), 0.0);
	}

	particlesMemory.initBuffer(NUM_PARTICLES, &particlesInitData[0], CL_MEM_READ_WRITE);
	positionMemory.initFromGLObject(vboIDs[0], NUM_LINES, &positionInitData[0], CL_MEM_READ_WRITE);
	colorMemory.initFromGLObject(vboIDs[1], NUM_LINES, &colorInitData[0], CL_MEM_READ_WRITE);
	randomTableMemory.initBuffer(NUM_PARTICLES, &randomTableInitData[0], CL_MEM_READ_ONLY);
	
	kernelUpdate->setArg(0, particlesMemory);
	kernelUpdate->setArg(1, positionMemory);
	kernelUpdate->setArg(2, colorMemory);
	kernelUpdate->setArg(3, randomTableMemory);
}

inline void ParticleController::setNumberOfSpawn(const int spawn) {
	numSpawn = spawn;
}

inline void ParticleController::setInitialHole(const float hole) {
	iHole = hole;
}

inline void ParticleController::setInitialDiffuse(const float diffuse) {
	iDiffuse = diffuse;
}

inline void ParticleController::setInitialLocation(Direction dir, const float &location) {
	switch (dir) {
	case Direction::x:
		iLocation.x = location; break;
	case Direction::y:
		iLocation.y = location; break;
	case Direction::z:
		iLocation.z = location; break;
	}
}

inline void ParticleController::update() {
	if (numSpawn == 0)return;

	count += numSpawn;  // count possibly greater as NUM_PARTICLE
	for (int i = prev; i < count; i++) {
		int wrapped = i % NUM_PARTICLES;
		particlesMemory[wrapped] = emitter->spawnParticle();
		setParticlePosition(wrapped);
	}

	if (count < NUM_PARTICLES) {
		particlesMemory.writeToDevice(prev, numSpawn);
		positionMemory.writeToDevice(prev*2, numSpawn*2);

	} else {

		int excess = count - NUM_PARTICLES; // 20030 - 20000 = 30
		int remain = numSpawn - excess; // 50 - 30 = 20
		particlesMemory.writeToDevice(prev, remain); //
		positionMemory.writeToDevice(prev*2, remain*2);

		if (excess > 0) {
			particlesMemory.writeToDevice(0, excess);
			positionMemory.writeToDevice(0, excess*2);
		}
		count %= NUM_PARTICLES; 
	}

	prev = count;
}

inline float4 ParticleController::randomizedPosition() const{
	ofVec3f rp = ofVec3f(ofRandom(iDiffuse) + iHole, 0, 0);
	rp.rotate(ofRandom(360), ofRandom(360), ofRandom(360));
	return std::move(float4(rp.x, rp.y, rp.z, 0.0));
}

inline void ParticleController::setParticlePosition(int index) {
	float4 randomPos = randomizedPosition();
	positionMemory[index * 2] = iLocation + randomPos;
	positionMemory[index * 2+1] = iLocation + randomPos;
}

inline void ParticleController::drawParticles() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDs[0]);
	glVertexPointer(4, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDs[1]);
	glColorPointer(4, GL_FLOAT, 0, 0);

	glDrawArrays(GL_LINES, 0, NUM_LINES);
	glPushMatrix();
	glScalef(-1, -1, -1);
	glDrawArrays(GL_LINES, 0, NUM_LINES);
	glPopMatrix();

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}
