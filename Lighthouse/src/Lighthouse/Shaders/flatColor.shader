// Vertex shader

# version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 o_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
	gl_Position = u_proj * u_view * u_model * vec4(position, 1.0);
	o_color = color;
}


// Fragment shader

# version 330 core

in vec4 o_color;
layout(location = 0) out vec4 color;

void main()
{
	color = o_color;
}

