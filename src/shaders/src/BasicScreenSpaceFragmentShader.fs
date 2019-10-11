#version 330 core

in vec2 uv;

uniform sampler2D colorBuffer;

out vec4 FragColor;

void main()
{
    FragColor = texture(colorBuffer, uv);
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    //FragColor = vec4(average, average, average, 1.0);
}
