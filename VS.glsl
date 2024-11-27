#version 330

layout(location = 0) in vec4 vertex_position; // Input position
layout(location = 1) in vec4 vertex_color;    // Input color

uniform float theta; // Uniform for animation

out vec4 frag_color; // Pass color to fragment shader

void main()
{
    // Modify the x-coordinate for horizontal movement using theta as a phase shift
    float y = 0.5 * sin(3 * (vertex_position.x + theta)) * sin(3 * (vertex_position.x + theta) * 10);
    gl_Position = vec4(vertex_position.x, y, vertex_position.z, 1.0);

    // Dynamically change color based on theta
    frag_color = vec4(abs(sin(theta + vertex_color.r)), abs(cos(theta + vertex_color.g)), abs(sin(theta + vertex_color.b)), 1.0);
}
