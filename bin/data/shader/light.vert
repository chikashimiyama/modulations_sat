uniform vec4 lightPosition;
uniform float lightRange;

void main(){
    vec4 distance3D;
    float dist, intensity;
    vec3 center;
    
    center = vec3(0.0, 0.0, 0.0);
    gl_Position = ftransform();
    dist = distance(gl_Position , lightPosition);
    intensity = 1.0 - (dist / (lightRange * lightRange)); // direct distance
    intensity = clamp(intensity, 0.0, 1.0);
    gl_FrontColor = gl_Color * intensity;
}