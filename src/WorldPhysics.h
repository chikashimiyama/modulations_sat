#pragma once

class WorldPhysics {

public:
	WorldPhysics() :
		gravity(-0.0003),
		attDetach(1.0),
		attraction(0),
		attLocation(0, 0, 0, 0),
		revFact(0, 1, 0, 1.0),
		rev(0) {}

	void updateCLKernel(std::shared_ptr<msa::OpenCLKernel>);
	void setInitialAttractorLocation(Direction dir, const float &atLoc);
private:
	float gravity;
	float attDetach;
	float attraction;
	float4 attLocation;
	float4 revFact;
	float rev;
};

inline void WorldPhysics::setInitialAttractorLocation(Direction dir, const float &attLoc) {
	switch (dir) {
	case Direction::x:
		attLocation.x = attLoc; break;
	case Direction::y:
		attLocation.y = attLoc; break;
	case Direction::z:
		attLocation.z = attLoc; break;
	}
}

inline void WorldPhysics::updateCLKernel(std::shared_ptr<msa::OpenCLKernel> kernelUpdate) {
	int seed = (int)ofRandom(NUM_PARTICLES);
	kernelUpdate->setArg(4, gravity);
	kernelUpdate->setArg(5, seed);
	kernelUpdate->setArg(6, attraction);
	kernelUpdate->setArg(7, attLocation);
	kernelUpdate->setArg(8, attDetach);
	kernelUpdate->setArg(9, rev);
	kernelUpdate->setArg(10, revFact);
}

