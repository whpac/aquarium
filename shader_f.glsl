#version 330


uniform sampler2D texture;

uniform vec4 obsPos = vec4(0, 0, 0, 1);
uniform vec4 tintColor = vec4(0, 0, 0, 1);

out vec4 pixelColor; //Output variable of the fragment shader. (Almost) final pixel color.

//Varying variables
in vec2 i_tc;
in vec4 n;
in vec4 vertex_eye;
in vec4 sunlight;
in vec4 torchlight;

void main(void) {
	vec4 color = texture(texture, i_tc);
	if(color.a < 0.1)
		discard;

	vec4 mn = normalize(n);
	if(dot(mn, obsPos - vertex_eye) < 0){
		mn = -mn;
	}

	vec4 l_sun = normalize(sunlight);
	float I_sun = clamp(dot(mn, l_sun), 0, 1);

	vec4 l_torch = normalize(torchlight);
	float I_torch = clamp(dot(mn, l_torch), 0, 1);

	float I = I_sun * 0.7 + I_torch * 0.3;
	color = vec4(color.rgb * I, color.a);

	float tintIntensity = 1 - pow(1.4, -distance(obsPos, vertex_eye) / 10);

	pixelColor = mix(color, tintColor, tintIntensity);
}
