#version 330

//Uniform variables
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


uniform vec4 sunDir = vec4(0, 1, 0, 0);     // In world space
uniform vec4 torchPos = vec4(0, 0, 0, 1);   // In eye space

//Attributes
layout (location=0) in vec4 vertex; //vertex coordinates in model space
layout (location=1) in vec4 normal; //vertex normal vector in model space
layout (location=2) in vec2 texCoord; //texturing coordinates


//varying variables
out vec2 i_tc;
out vec4 n;
out vec4 vertex_eye;
out vec4 sunlight;
out vec4 torchlight;

void main(void) {
    vertex_eye = V * M * vertex;
    gl_Position = P * vertex_eye;

    mat4 G = mat4(inverse(transpose(mat3(M))));
    n = normalize(V * G * normal);
    sunlight = V * sunDir;
    torchlight = torchPos - vertex_eye;

    i_tc = texCoord;
}
