uniform vec3 light_pos;
uniform vec4 light_color;
varying vec3 pos;

void main(void)
{
	vec3 normal = normalize(cross(dFdy(pos), dFdx(pos)));
	vec3 light_normal = normalize(light_pos);

	vec4 light = clamp(light_color * dot(light_normal, normal), 0.0, 1.0);

    gl_FragColor = gl_Color * light;
}
