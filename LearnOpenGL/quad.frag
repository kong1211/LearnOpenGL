#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // 来自顶点着色器的纹理坐标

uniform sampler2D texture1; // 纹理

void main()
{
    FragColor = texture(texture1, TexCoord); // 使用纹理颜色
}
