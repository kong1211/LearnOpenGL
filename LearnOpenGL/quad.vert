#version 330 core
layout (location = 0) in vec3 aPos;    // 顶点位置
layout (location = 1) in vec2 aTexCoord; // 纹理坐标

out vec2 TexCoord; // 传递给片段着色器

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord; // 传递纹理坐标到片段着色器
}
