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

void main()
{
    float blue = float(u_objIndex) / 255.0f;
    color = vec4(0.0f, 0.0f, blue, 1.0f);
}

