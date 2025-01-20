#version 410 core

// Uniforms
uniform vec3 iResolution;  // Viewport resolution (pixels)
uniform float iTime;       // Shader playback time (seconds)
uniform vec4 iMouse;       // Mouse coordinates

// Inputs from vertex shader
in vec3 fragColor;
in vec3 fragPos;
in vec2 TexCoords;

// Output color
out vec4 FragColor;

#define time iTime

// Creates a 2x2 rotation matrix for rotating vectors 
// in 2D space by angle a.
mat2 mm2(in float a) {
    float c = cos(a), s = sin(a);
    return mat2(c, s, -s, c);
}

// A fixed rotation matrix for an angle of approximately 17 degrees
mat2 m2 = mat2(0.95534, 0.29552, -0.29552, 0.95534);

// Generates a triangle wave pattern between 0.01 and 0.49.
// The fract function returns the fractional part of x,
// and abs(fract(x) - .5) creates a symmetric triangle wave.
float tri(in float x) {
    return clamp(abs(fract(x) - .5), 0.01, 0.49);
}

// Combines triangle waves in
// both x and y directions to produce a 2D pattern.
vec2 tri2(in vec2 p) {
    return vec2(tri(p.x) + tri(p.y), tri(p.y + tri(p.x)));
}

// Generates a 2D noise pattern using the triangle wave functions
// and rotations to simulate the aurora's texture.
float triNoise2d(in vec2 p, float spd) {
    float z = 1.8; // amplitude
    float z2 = 2.5; // frequency
    float rz = 0.; // accumulatedNoise
    p *= mm2(p.x * 0.06);
    vec2 bp = p;
    for (float i = 0.; i < 5.; i++) {
        vec2 dg = tri2(bp * 1.85) * .75; // displacement
        dg *= mm2(time * spd);
        p -= dg / z2; // displacement / frequency

        bp *= 1.3; // basePoint
        z2 *= .45; // frequency
        z *= .42; // amplitude
        p *= 1.21 + (rz - 1.0) * .02;

        rz += tri(p.x + tri(p.y)) * z;
        p *= -m2;
    }
    return clamp(1. / pow(rz * 29., 1.3), 0., .55);
}

// Generates a pseudo-random value based on a 2D vector n
// used for adding randomness to the effect.
float hash21(in vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

// ray origin, ray direction, frag coordinate
vec4 aurora(vec3 ro, vec3 rd, vec2 fragCoord) {
    vec4 col = vec4(0); // accumulatedColor
    vec4 avgCol = vec4(0); // averageColor

    // Loop to simulate integration along the ray
    for (float i = 0.; i < 50.; i++) {
        // Offset for randomness
        float of = 0.006 * hash21(fragCoord.xy) * smoothstep(0., 15., i);
        // Parameter along the ray where sampling occurs
        float pt = ((.8 + pow(i, 1.4) * .002) - ro.y) / (rd.y * 2. + 0.4);
        pt -= of;
        // Position along the ray
        vec3 bpos = ro + pt * rd; // beamPosition
        // 2D position for noise function
        vec2 p = bpos.zx;
        // Compute noise value
        float rzt = triNoise2d(p, 0.06); // noiseValue
        // Color with alpha channel set to noise value
        vec4 col2 = vec4(0, 0, 0, rzt);
        // Color modulation to simulate aurora's color variation
        col2.rgb = (sin(1. - vec3(2.15, -.5, 1.2) + i * 0.043) * 0.5 + 0.5) * rzt;
        // Averaging colors
        avgCol = mix(avgCol, col2, .5);
        // Accumulate color with exponential decay
        col += avgCol * exp2(-i * 0.065 - 2.5) * smoothstep(0., 5., i);
    }

    // Adjust brightness based on ray direction
    col *= (clamp(rd.y * 15. + .4, 0., 1.));

    // Return the final aurora color
    return col * 1.8;
}

// Background and Stars
// Generates a pseudo-random 3D vector based on input q,
// used for star placement and brightness.
vec3 nmzHash33(vec3 q) {
    uvec3 p = uvec3(ivec3(q));
    p = p * uvec3(374761393U, 1103515245U, 668265263U) + p.zxy + p.yzx;
    p = p.yzx * (p.zxy ^ (p >> 3U));
    return vec3(p ^ (p >> 16U)) * (1.0 / vec3(0xffffffffU));
}

// Simulates stars in the background by placing bright points in the sky
// with slight variations in color and brightness.
vec3 stars(in vec3 p) {
    vec3 c = vec3(0.);
    float res = iResolution.x * 1.;

    for (float i = 0.; i < 4.; i++) {
        vec3 q = fract(p * (.15 * res)) - 0.5;
        vec3 id = floor(p * (.15 * res));
        vec2 rn = nmzHash33(id).xy;
        float c2 = 1. - smoothstep(0., .6, length(q));
        c2 *= step(rn.x, .0005 + i * i * 0.001);
        c += c2 * (mix(vec3(1.0, 0.49, 0.1), vec3(0.75, 0.9, 1.), rn.y) * 0.1 + 0.9);
        p *= 1.3;
    }
    return c * c * .8;
}

// Generates the background gradient of the sky by blending
// between two colors based on the view direction.
vec3 bg(in vec3 rd) {
    float sd = dot(normalize(vec3(-0.5, -0.6, 0.9)), rd) * 0.5 + 0.5;
    sd = pow(sd, 5.);
    vec3 col = mix(vec3(0.05, 0.1, 0.2), vec3(0.1, 0.05, 0.2), sd);
    return col * .63;
}


void main() {
    // Map texture coordinates to fragment coordinates
    vec2 fragCoord = TexCoords * iResolution.xy;

    // Compute normalized coordinates
    vec2 q = fragCoord.xy / iResolution.xy; // normalized coord
    vec2 p = q - 0.5; // screen position
    p.x *= iResolution.x / iResolution.y; // Correct for aspect ratio

    // Camera setup
    vec3 ro = vec3(0, 0, -6.7); // ray origin
    vec3 rd = normalize(vec3(p, 1.3)); // Initial ray direction

    // Mouse interaction
    vec2 mo = iMouse.xy / iResolution.xy - 0.5; // mouse offset
    mo = (mo == vec2(-0.5)) ? vec2(-0.1, 0.1) : mo;
    mo.x *= iResolution.x / iResolution.y;

    // Apply rotations based on mouse movement and time
    rd.yz *= mm2(mo.y);
    rd.xz *= mm2(mo.x + sin(time * 0.05) * 0.2);

    // Initialize color
    vec3 col = vec3(0.0);

    // Fade effect based on ray direction
    vec3 brd = rd; // initialRayDirection = rayDirection
    float fade = smoothstep(0.0, 0.01, abs(brd.y)) * 0.1 + 0.9;

    // Background color
    col = bg(rd) * fade;

    if (rd.y > 0.0) {
        // If the ray is pointing upwards, render the aurora and stars
        vec4 aur = smoothstep(0.0, 1.5, aurora(ro, rd, fragCoord)) * fade;
        col += stars(rd);
        col = col * (1.0 - aur.a) + aur.rgb;
    }
    // Output the final color
    FragColor = vec4(col, 1.0);
}
