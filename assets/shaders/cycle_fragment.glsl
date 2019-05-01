#version 330

uniform sampler2D sky;
uniform sampler2D glow;

uniform vec3 planetPosition;
uniform bool daytime;

//unused; keeping for future testing
//uniform float lightIntensity;
//uniform float planetProgress;

in vec3 out_vertex;

out vec4 fragColor;

void main() {
   	vec3 V = (out_vertex);
    vec3 L = (planetPosition.xyz);
    //Since un-normalized coordinates in openGL start at the
    //top left, a lower y translates to a higher position.
    //Therefore, we'll invert the y values to accurately
    //reflect that the sun's y position actually increases
    //as it goes up.
    //Keep in mind that it never actually gets to the very top, more like
    //80% or 90%. Maybe fix this later (in DayNightCycle.cpp)?
	V.y = 1 - V.y;
	L.y = 1 - L.y;
	//Read from the lefthand side of the sky file if it's nighttime
	if(!daytime) {
		L.y -= 1;
	}

    //Compute the proximity of this fragment to the sun.
    float vl = dot(V, L);

    //Look up the sky color and glow colors.

	//Note: (L.y + 1) / 2 is to accomodate for sky.png having
	//its right half dedicated to the daytime gradient and
	//left half to the nighttime gradient
	vec4 Kc = texture2D(sky, 	vec2((L.y + 1) / 2, V.y));
    //vec4 Kg = texture2D(glow,  	vec2((L.y + 1) / 2, vl));
	fragColor = Kc;
	
    //Combine the sky and glow giving the pixel value.
    //fragColor = vec4(Kc.rgb + Kg.rgb * Kg.a / 2.0, Kc.a);
}
