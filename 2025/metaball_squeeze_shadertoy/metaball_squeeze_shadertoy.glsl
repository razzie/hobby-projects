struct Circle
{
    vec2 pos;
    float radius;
    int group;
};

float smin(float a, float b, float k)
{
    float h = clamp(0.5 + 0.5*(b - a)/k, 0.0, 1.0);
    return mix(b, a, h) - k*h*(1.0 - h);
}

float edge( float dist, vec2 normal )
{
    const vec2 light = vec2(1.0,0.0);
    const float dMax = -0.02;
    dist = clamp(dist / dMax, 0.0, 1.0);
    return mix(0.8, 1.0, (1.0 - dist) * dot(normal, light));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord/iResolution.xy;
    vec2 mouse = iMouse.xy/iResolution.xy;

    const float BIG = 1e20;
    const float K = 0.1;
    const int NUM_CIRCLES = 5;
    const int NUM_GROUPS = 4;

    vec3 groupColors[NUM_GROUPS] = vec3[NUM_GROUPS](
        vec3(0.0,0.0,0.0), // background
        vec3(1.0,0.0,0.0), // red
        vec3(0.0,0.0,1.0), // blue
        vec3(0.0,1.0,0.0)  // green
    );

    Circle circles[NUM_CIRCLES] = Circle[NUM_CIRCLES](
        // red
        Circle(vec2(0.5,0.55)+vec2(sin(iTime*3.0)*0.01,sin(iTime*3.0)*0.01), 0.1, 1),
        Circle(vec2(0.5,0.40)-vec2(cos(iTime*2.0)*-0.01,cos(iTime*2.0)*0.01), 0.075, 1),
        // blue
        Circle(mouse, 0.075, 2),
        Circle(mouse+vec2(sin(iTime*4.0)*0.05,sin(iTime*4.0)*0.05), 0.05, 2),
        // green
        Circle(vec2(0.5,0.5)+vec2(sin(iTime)*0.1,cos(iTime)*0.15), 0.05, 3)
    );

    float distances[NUM_GROUPS];
    vec2  normals[NUM_GROUPS];
    for (int i = 0; i < NUM_GROUPS; ++i)
    {
        distances[i] = BIG;
        normals[i] = vec2(0.0,0.0);
    }

    // calculate distances and normals
    for (int i = 0; i < NUM_CIRCLES; ++i)
    {
        vec2 v = uv - circles[i].pos;
        float len = length(v) + 1e-5; // non-zero
        float d = len - circles[i].radius;
        int g = circles[i].group;
        distances[g] = smin(distances[g], d, K);
        normals[g] += v / (len * len);
    }

    // do the squeezing
    float squeezedDistances[NUM_GROUPS];
    for (int i = 0; i < NUM_GROUPS; ++i)
    {
        float otherDist = BIG;
        for (int j = 0; j < NUM_GROUPS; ++j)
        {
            float isSelfGroup = float(i == j);
            float notSelfGroup = 1.0 - isSelfGroup;
            float d = mix(distances[j], BIG, isSelfGroup);
            otherDist = min(otherDist, d);
        }
        squeezedDistances[i] = distances[i] - smin(otherDist, 0.0, K/2.0);
    }

    // select group at pixel
    int selectedGroup = 0;
    for (int i = 0; i < NUM_GROUPS; ++i)
    {
        int shouldSelect = int(squeezedDistances[i] < 0.0);
        selectedGroup = selectedGroup * (1 - shouldSelect) + i * shouldSelect;
    }

    vec3 color = groupColors[selectedGroup];
    float dist = distances[selectedGroup];
    vec2 normal = normalize(normals[selectedGroup]);
    fragColor = vec4(color * edge(dist, normal), 1.0);
}