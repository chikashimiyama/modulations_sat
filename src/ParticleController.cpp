#include "ParticleController.h"
#include "ofApp.h"

 void ParticleController::setup() {

	for (int i = 0; i<NUM_PARTICLES; i++) {
		Particle &p = particles.ram[i];
		p.vel.set(0, 0, 0, 0);
		p.accel.set(ofRandom(-0.001, 0.001), 0.0, 0, 0);
		p.mass = ofRandom(0.5, 1);

		randomTable.ram[i] = float4(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), 0.0);

		position.ram[i * 2] = float4(0, 0, 0, 1.0);
		position.ram[i * 2 + 1] = float4(0, 0, 0, 1.0);
		color.ram[i * 2] = float4(1.0, 1.0, 1.0, 1.0);
		color.ram[i * 2 + 1] = float4(1.0, 1.0, 1.0, 1.0);
		normal.ram[i * 2] = float4(0.0, 1.0, 0.0, 1.0);
		normal.ram[i * 2 + 1] = float4(0.0, 1.0, 0.0, 1.0);
	}

	particles.vram.initBuffer(sizeof(Particle) * NUM_PARTICLES, CL_MEM_READ_WRITE, particles.ram);
	randomTable.vram.initBuffer(sizeof(float4) * NUM_PARTICLES, CL_MEM_READ_ONLY, randomTable.ram);
}

 void ParticleController::setNumberOfSpawn(const int &spawn) {
	numSpawn = spawn;
}

 void ParticleController::update() {

	count += numSpawn;  // count possibly greater as NUM_PARTICLE
	for (int i = prev; i < count; i++) {
		int wrapped = i % NUM_PARTICLES;
		app->emitter.initializeParticle(particles.ram[wrapped]);

		ofVec3f rp = ofVec3f(ofRandom(iDiffuse) + iHole, 0, 0);
		rp.rotate(ofRandom(360), ofRandom(360), ofRandom(360));
		float4 randomPos = float4(rp.x, rp.y, rp.z, 0.0);

		particlesPos[wrapped * 2] = iLocation + randomPos;
		particlesPos[wrapped * 2 + 1] = iLocation + randomPos;
	}

	int offset = prev * sizeParticle;
	if (count < NUM_PARTICLES) {
		int dataSize = numSpawn * sizeParticle;
		clMemParticles.write(&particles[prev], offset, dataSize);
		offset = prev * sizeLine;
		dataSize = numSpawn * sizeLine;
		clMemPosVBO.write(&particlesPos[prev * 2], offset, dataSize);

	} else {
		int excess = count - NUM_PARTICLES; // e.g. 20021 - 20000 = 21

		int dataSize = (numSpawn - excess) * sizeParticle; // (100 - 50) * sizeParticle
		clMemParticles.write(&particles[prev], offset, dataSize); // 9050 - 9999

		dataSize = excess * sizeParticle; // 0 - 49
		clMemParticles.write(&particles[0], 0, dataSize);

		// vertex
		offset = prev * sizeLine;
		dataSize = (numSpawn - excess) * sizeLine;
		clMemPosVBO.write(&particlesPos[prev * 2], offset, dataSize);
		dataSize = excess * sizeLine;
		clMemPosVBO.write(&particlesPos[0], 0, dataSize);
	}

	count %= NUM_PARTICLES; // e.g. 9950
	prev = count; // pre = 9950
}
