# vertex shader
# version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fragmentNormal;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    fragmentNormal = mat3(transpose(inverse(model))) * vertexNormal;
};

# fragment shader
# version 330 core

in vec3 fragmentNormal;

out vec4 fragmentColor;

void main()
{
    vec3 norm = normalize(fragmentNormal);
    vec3 l0 = normalize(vec3(0.4, 0.5, 0.6));
    vec3 l1 = normalize(vec3(-0.4, 0.5, -0.6));

    float ndotl0 = clamp(dot(norm, l0), 0, 1);
    float ndotl1 = clamp(dot(norm, l1), 0, 1);

    float ambientStrength = 0.25;
    vec3 ambient = ambientStrength * vec3(1.0);
    vec3 diffuse = ndotl0 + ndotl1 + ambient;

    fragmentColor = vec4(diffuse, 1.0);
};