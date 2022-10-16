// Vertex shader

# version 330 core

layout(location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_camera;

out vec3 o_position;

void main()
{
    o_position = vec3(u_model * vec4(position, 1.0));
    gl_Position = u_camera * vec4(o_position, 1.0);
}


// Fragment shader

# version 330 core

in vec3 o_position;

layout(location = 0) out vec4 color;

uniform uint u_objIndex;

out uvec3 o_color;

void main()
{
    o_color = uvec3(0, 255, u_objIndex);
}

