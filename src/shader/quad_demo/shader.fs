#version 330 core
out vec4 FragColor;
in vec3 OurColor;
void main()
{
FragColor = vec4(OurColor, 1.0f);
}