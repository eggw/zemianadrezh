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
    vec3 L = (planetPosition);

    //Since un-normalized coordinates in openGL start at the
    //top left, a lower y translates to a higher position.
    //
    //Therefore, we'll invert the  sun/moon's y value to
    //accurately reflect that the sun's y position actually
    //increases as it goes up.
    //
    //Keep in mind that it never actually gets to the very top, more
    //like 80% or 90%. Maybe fix this later [DayNightCycle.cpp]
	L.y = 1 - L.y;

	//Additionally, since 0 is the center in OpenGL, we're going to
	//convert our planet's X position from the 0, 1 range (where 0.5
	//is the center to the -1, 1 range (where 0 is the center)
	L.x = (L.x * 2) - 1;

	//Read from the lefthand side of the sky file if it's nighttime
	if(!daytime) {
		L.y -= 1;
	}

    //Compute the proximity of this fragment to the sun.
    float vl = distance(V, L);

    //Look up the sky color and glow colors.

	//Note: (L.y + 1) / 2 is to accomodate for sky.png having
	//its right half dedicated to the daytime gradient and
	//left half to the nighttdime gradient
	vec4 Kc = texture2D(sky, 	vec2((L.y + 1) / 2, V.y));
    vec4 Kg = texture2D	(glow,  vec2(L.y, vl));
	//fragColor = vec4(vl,vl,vl,255);

	vec4 dither = vec4(texture2D(glow, V.xy / 8.0).r / 32.0 - (1.0 / 128.0));
	Kg += dither;

    //Combine the sky and glow giving the pixel value.
    fragColor = vec4(Kc.rgb + Kg.rgb * Kg.a / 2.0, Kc.a);
}
