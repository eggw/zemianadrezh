#version 330

uniform sampler2D sky;
uniform sampler2D glow;
uniform float lightIntensity;
uniform vec3 sunPosition;
uniform float sunProgress;

in vec3 out_vertex;

out vec4 fragColor;

void main() {
   	//vec3 V = normalize(out_vertex);
    //vec3 L = normalize(sunPosition.xyz);

    //Compute the proximity of this fragment to the sun.

    //float vl = dot(V, L);

    //Look up the sky color and glow colors.

	//vec4 Kc = texture2D(sky, vec2((L.y + 1.0) / 2.0, V.y));
    //vec4 Kg = texture2D(glow,  vec2((L.y + 1.0) / 2.0, vl));

    // Combine the sky and glow giving the pixel value.

	//fragColor = vec4(0,0,vl,1);
	//fragColor = vec4(0,0,lightIntensity,1);
	//fragColor = vec4(0.352, 0.439, 1.0, 1.0);
    //fragColor = vec4(Kc.rgb + Kg.rgb * Kg.a / 2.0, Kc.a);

    //NOTE: make sure sunPosition/sunProgress affect the final result,
    //or they will be automatically optimized out
}
