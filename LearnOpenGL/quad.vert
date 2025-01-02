#version 330 core
layout (location = 0) in vec3 aPos;    // ����λ��
layout (location = 1) in vec2 aTexCoord; // ��������

out vec2 TexCoord; // ���ݸ�Ƭ����ɫ��

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord; // �����������굽Ƭ����ɫ��
}
