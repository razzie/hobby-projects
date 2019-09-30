uniform mat4 world;
uniform mat4 world_view_proj;
varying vec3 pos;

void main(void)
{
	gl_Position = world_view_proj * gl_Vertex;
	gl_FrontColor = gl_Color;
	pos = (world * gl_Vertex).xyz;
}
