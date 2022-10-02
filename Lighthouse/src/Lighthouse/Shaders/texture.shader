// Vertex shader

# version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 o_texCoord;
out vec3 o_normal;
out vec3 o_position;

void main()
{
    o_position = vec3(u_view * u_model * vec4(position, 1.0));
    gl_Position = u_proj * vec4(o_position, 1.0);
    o_texCoord = texCoord;
    o_normal = normal;
}


// Fragment shader

# version 330 core

in vec2 o_texCoord;
in vec3 o_normal;
in vec3 o_position;

layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_lightColor;
uniform vec3 u_lightPosition;

void main()
{
    float ambientLightIntensity = 0.2f;
    vec3 ambientLightDirection = normalize(vec3(1.0f, -0.8f, 0.0f));

    vec3 normal = o_normal;
    vec3 lightSourceDirection = normalize(u_lightPosition - o_position);

    float lightSourceValue = max(dot(normal, lightSourceDirection), 0.0f);
    float ambientLightValue = ambientLightIntensity * max(dot(normal, ambientLightDirection), 0.0f);

    color = texture(u_texture, o_texCoord) * u_lightColor * (1.0f * lightSourceValue + ambientLightValue);
}
