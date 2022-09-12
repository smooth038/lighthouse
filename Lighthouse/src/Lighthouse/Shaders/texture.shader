// Vertex shader

# version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 o_texCoord;

void main()
{
    o_texCoord = texCoord;
    gl_Position = u_proj * u_view * u_model * vec4(position, 1.0);
}


// Fragment shader

# version 330 core

in vec2 o_texCoord;

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;

void main()
{
    color = texture(u_texture, o_texCoord);
}

