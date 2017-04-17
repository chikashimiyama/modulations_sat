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
		void setReverse(const float &r);
		void setReverseFactor(Direction dir, const float &rf);
		void setAttraction(const float &att);
		void setAttractionDetach(const float &att);
		void setGravity(const float &att);

	private:
		float gravity;
		float attDetach;
		float attraction;
		float4 attLocation;
		float4 revFact;
		float rev;
};


inline void WorldPhysics::setReverse(const float &r) {
	rev = r;
}

inline void WorldPhysics::setReverseFactor(Direction dir, const float &rf){
	switch (dir) {
		case Direction::x:
			revFact.x = rf; break;
		case Direction::y:
			revFact.y = rf; break;
		case Direction::z:
			revFact.z = rf; break;
	}
}


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

inline void WorldPhysics::setAttraction(const float &att) {
	attraction = att;
}

inline void WorldPhysics::setAttractionDetach(const float &ad) {
	attDetach = ad;
}

inline void WorldPhysics::setGravity(const float &gr) {
	gravity = gr;
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
