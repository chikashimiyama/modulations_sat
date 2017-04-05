#pragma once

#include "constant.h"
#include "MSAOpenCL.h"
#include "Particle.h"
#include "Emitter.h"
#include "WorldPhysics.h"

class ParticleController {
public:
	ParticleController():
		count(0),
		iLocation(0, 0, 0, 1.0){}

	void setup(Emitter*, WorldPhysics*);
	void setNumberOfSpawn(const int&);
	void setInitialHole(const float&);
	void setInitialDiffuse(const float&);
	void setInitialLocation(Direction dir, const float &);

	void initCL(std::shared_ptr<msa::OpenCLKernel> kernelUpdate);
	void update();

private:

	float4 randomizedPosition() const;
	void setParticlePosition(int index, float4 randomPos);
	void vboSetup();

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

inline void ParticleController::vboSetup() {
	glGenBuffersARB(2, vboIDs);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDs[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float4) * NUM_LINES, 0, GL_DYNAMIC_COPY_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboIDs[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float4) * NUM_LINES, 0, GL_DYNAMIC_COPY_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

inline void ParticleController::setup(Emitter* em, WorldPhysics *wp) {
	emitter = em;
	worldPhysics = wp;

	vboSetup();

	std::array<Particle, NUM_PARTICLES> particlesInitData;
	std::array<float4, NUM_LINES> positionInitData;
	std::array<float4, NUM_LINES> colorInitData;
	std::array<float4, NUM_PARTICLES> randomTableInitData;

	for (int i = 0; i<NUM_PARTICLES; i++) {
	
		particlesInitData[i].vel.set(0, 0, 0, 0);
		particlesInitData[i].accel.set(ofRandom(-0.001, 0.001), 0.0, 0, 0);
		particlesInitData[i].mass = ofRandom(0.5, 1);
		positionInitData[i * 2] = float4(0, 0, 0, 1.0);
		positionInitData[i * 2 + 1] = float4(0, 0, 0, 1.0);
		colorInitData[i * 2] = float4(1.0, 1.0, 1.0, 1.0);
		colorInitData[i * 2 + 1] = float4(1.0, 1.0, 1.0, 1.0);
		randomTableInitData[i] = float4(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), 0.0);
	}

	particlesMemory.initBuffer(NUM_PARTICLES, &particlesInitData[0], CL_MEM_READ_WRITE);
	positionMemory.initFromGLObject(vboIDs[0], NUM_LINES, &positionInitData[0], CL_MEM_READ_WRITE);
	colorMemory.initFromGLObject(vboIDs[1], NUM_LINES, &colorInitData[0], CL_MEM_READ_WRITE);
	randomTableMemory.initBuffer(NUM_PARTICLES, &randomTableInitData[0], CL_MEM_READ_ONLY);
}

inline void ParticleController::initCL(std::shared_ptr<msa::OpenCLKernel> kernelUpdate) {
	kernelUpdate->setArg(0, particlesMemory.getCLMem());
	kernelUpdate->setArg(1, positionMemory.getCLMem());
	kernelUpdate->setArg(2, colorMemory.getCLMem());
	kernelUpdate->setArg(3, randomTableMemory.getCLMem());
}

inline void ParticleController::setNumberOfSpawn(const int &spawn) {
	numSpawn = spawn;
}

inline void ParticleController::setInitialHole(const float &hole) {
	iHole = hole;
}

inline void ParticleController::setInitialDiffuse(const float &diffuse) {
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

	count += numSpawn;  // count possibly greater as NUM_PARTICLE
	for (int i = prev; i < count; i++) {
		int wrapped = i % NUM_PARTICLES;
		Particle p = emitter->spawnParticle();
		particlesMemory[i] = p;
		float4 randomPos = randomizedPosition();
		setParticlePosition(wrapped, randomPos);
	}

	int offset = prev * PARTICLE_SIZE;
	if (count < NUM_PARTICLES) {
		int dataSize = numSpawn * PARTICLE_SIZE;
		particlesMemory.writeToDevice(offset, dataSize);
		offset = prev * LINE_SIZE;
		dataSize = numSpawn * LINE_SIZE;
		positionMemory.writeToDevice(offset, dataSize);

	} else {
		int excess = count - NUM_PARTICLES; // e.g. 20021 - 20000 = 21

		int dataSize = (numSpawn - excess) * PARTICLE_SIZE; // (100 - 50) * sizeParticle
		particlesMemory.writeToDevice(offset, dataSize); // 9050 - 9999

		dataSize = excess * PARTICLE_SIZE; // 0 - 49
		particlesMemory.writeToDevice(0, dataSize);

		// vertex
		offset = prev * LINE_SIZE;
		dataSize = (numSpawn - excess) * LINE_SIZE;
		positionMemory.writeToDevice(offset, dataSize);
		dataSize = excess * LINE_SIZE;
		positionMemory.writeToDevice(0, dataSize);
	}

	count %= NUM_PARTICLES; // e.g. 9950
	prev = count; // pre = 9950
}


inline float4 ParticleController::randomizedPosition() const{
	ofVec3f rp = ofVec3f(ofRandom(iDiffuse) + iHole, 0, 0);
	rp.rotate(ofRandom(360), ofRandom(360), ofRandom(360));
	return std::move(float4(rp.x, rp.y, rp.z, 0.0));
}

inline void ParticleController::setParticlePosition(int index, float4 randomPos) {
	positionMemory[index * 2] = iLocation + randomPos;
	positionMemory[index * 2+1] = iLocation + randomPos;
}
