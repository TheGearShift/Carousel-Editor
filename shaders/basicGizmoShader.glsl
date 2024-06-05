# vertex shader
# version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 u_color;

out vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    color = u_color;
};

# fragment shader
# version 330 core

in vec3 color;

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(color, 1.0);
};