uniform mat4 world;
uniform mat4 world_view_proj;
varying vec3 pos;
varying vec3 norm;

void main(void)
{
	gl_Position = world_view_proj * gl_Vertex;
	gl_FrontColor = gl_Color;
	pos = (world * gl_Vertex).xyz;
	norm = gl_Normal;
}
