#version 330

in vec4 frag_color; // Input color from vertex shader
out vec4 out_color; // Output color

void main()
{
    out_color = frag_color; // Pass the color
}
