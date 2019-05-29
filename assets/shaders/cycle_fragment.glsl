#version 330

uniform sampler2D sky;
uniform sampler2D glow;

uniform vec3  planetPosition;
uniform float planetYProgress;
uniform bool  daytime;

in  vec3 out_vertex;
out vec4 fragColor;

void main() {
   	vec3 V = out_vertex;
	vec3 L = planetPosition;

	float progress = planetYProgress;

	//Read from the lefthand side of the sky file if it's nighttime
	if(!daytime) {
		progress -= 1;
	}

	//Compute the proximity of this fragment to the sun.
    float vl = distance(V, L);

	//The position given by L is the center (vertically and horizontally)
	//of our planet. However, in order to calculate the texture coordinates,
	//we're going to use planetYProgress. This is because the center of
	//the sun will never hit the very top of the screen.

    //Look up the sky color and glow colors.
	//Note: (L.y + 1) / 2 is to accomodate for sky.png having
	//its right half dedicated to the daytime gradient and
	//left half to the nighttime gradient
	vec4 Kc = texture2D(sky,  vec2((progress + 1) / 2, V.y));
    vec4 Kg = texture2D(glow, vec2(progress, vl));

	vec4 dither = vec4(texture2D(glow, V.xy / 8.0).r / 32.0 - (1.0 / 128.0));
	Kg += dither;

    //Combine the sky and glow giving the pixel value.
    fragColor = vec4(Kc.rgb + Kg.rgb * (Kg.a * 0.9), Kc.a);
}
