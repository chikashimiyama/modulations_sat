#define NUM_PARTICLES (20000)

typedef struct{
    float4 vel;
	float4 accel;
    float4 aRand;
    float4 rotFact;
    float2 distLimit;
    float2 dummyParam;
    
	float mass;
	float lifetime;
	float life;
	bool alive;
    float limit;
    float momentum;
    float rot;
    float dummy;
} Particle;

__kernel void test(){
	

}

__kernel void updateParticle(
            __global Particle* particles,
            __global float4* posBuffer,
            __global float4* colorBuffer,
            __global float4* randomTable,
            const float gravity,
            const int seed,
            const float attraction,
            const float4 attLocation,
            const float attDetach,
            const float rev,
            const float4 revFact
                             )
{
/*
	int id = get_global_id(0);
    int posID = id * 2;
	__global Particle *p = &particles[id];
    
    //if(p->alive == true ){
        posBuffer[posID + 1] = posBuffer[posID];//store previous

        //momentum
        p->accel *= p->momentum;

        //random
        p->accel += (randomTable[(id+seed)%NUM_PARTICLES] * p->aRand);
        
        //rotate
        
        float4 ax = p->rotFact;
        float sina = sin( p->rot );
        float cosa = cos( p->rot );
        float cosb = 1.0f - cosa;

        p->accel.x = p->accel.x*(ax.x*ax.x*cosb + cosa) +
                     p->accel.y*(ax.x*ax.y*cosb - ax.z*sina)+
                     p->accel.z*(ax.x*ax.z*cosb + ax.y*sina);
        
        p->accel.y = p->accel.x*(ax.y*ax.x*cosb + ax.z*sina) +
                     p->accel.y*(ax.y*ax.y*cosb + cosa)+
                     p->accel.z*(ax.y*ax.z*cosb - ax.x*sina);
        
        p->accel.z = p->accel.x*(ax.z*ax.x*cosb - ax.y*sina)+
                     p->accel.y*(ax.z*ax.y*cosb + ax.x*sina)+
                     p->accel.z*(ax.z*ax.z*cosb + cosa);
        
        //attraction .... len no 2 bai jya ikan ... attractor no ichi kioku
        float4 detachedLocation = attLocation * attDetach;
        detachedLocation.w = 1.0;
        float4 diff = detachedLocation - posBuffer[posID]; // diffrence between two points
        
        float len = length(diff); // distance
        len = clamp(len, p->distLimit.x, p->distLimit.y);
        float4 direction = normalize(diff);

        float strength = (attraction * p->mass) / (len * len);
        direction *= strength;
        p->accel += (float4)(direction.x, direction.y, direction.z, 0.0f);
        //revolution
        
        ax = revFact;
        sina = sin( rev );
        cosa = cos( rev );
        cosb = 1.0f - cosa;
        posBuffer[id*2].x = posBuffer[id*2].x*(ax.x*ax.x*cosb + cosa) +
                            posBuffer[id*2].y*(ax.x*ax.y*cosb - ax.z*sina)+
                            posBuffer[id*2].z*(ax.x*ax.z*cosb + ax.y*sina);
        
        posBuffer[id*2].y = posBuffer[id*2].x*(ax.y*ax.x*cosb + ax.z*sina) +
                            posBuffer[id*2].y*(ax.y*ax.y*cosb + cosa)+
                            posBuffer[id*2].z*(ax.y*ax.z*cosb - ax.x*sina);
        
        posBuffer[id*2].z = posBuffer[id*2].x*(ax.z*ax.x*cosb - ax.y*sina)+
                            posBuffer[id*2].y*(ax.z*ax.y*cosb + ax.x*sina)+
                            posBuffer[id*2].z*(ax.z*ax.z*cosb + cosa);
        
        // gravity
        p->accel.y += gravity;
        p->vel += p->accel;
        
        //speed limit
        p->vel = clamp(p->vel,-p->limit, p->limit);
        posBuffer[id*2] +=  p->vel;
        
        float brightness = p->life / p->lifetime;
        colorBuffer[id*2] = (float4)(brightness,brightness,brightness,1.0);
        brightness = p->life / p->lifetime;
        
        p->life -= 1.0;

        //colorBuffer
        colorBuffer[id*2+1] = (float4)(brightness,brightness,brightness,1.0);*/
}
