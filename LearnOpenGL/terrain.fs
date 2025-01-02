#version 330 core
flat in vec3 flatColor;     // �Ӷ�����ɫ�����ݵ�ƽ����ɫ
in vec3 Color;              // �Ӷ�����ɫ�����ݵĹ��ռ�����ɫ
in float Height;            // �Ӷ�����ɫ�����ݵĸ߶�ֵ
out vec4 FragColor;

uniform bool isFlat;        // �����Ƿ�ʹ��ƽ����ɫ

void main() {
    vec3 gradientColor;

    // ���ݸ߶����ø�ϸ�µ���ɫ����
    if (Height < 0.05) {
        // �ͺ��Σ�����ɫ��ģ����ˮ��
        gradientColor = mix(vec3(0.1, 0.2, 0.6), vec3(0.2, 0.5, 0.8), Height / 3.0);
    } else if (Height < 0.15) {
        // ǳ������ǳ��ɫ���ɵ���ɫ
        gradientColor = mix(vec3(0.2, 0.5, 0.8), vec3(0.3, 0.7, 0.4), (Height - 3.0) / 3.0);
    } else if (Height < 0.5) {
        // �ݵأ�����ɫ���ɵ�����ɫ��ģ��ݵغ͵Ͱ�ֲ����
        gradientColor = mix(vec3(0.3, 0.7, 0.4), vec3(0.5, 0.8, 0.2), (Height - 6.0) / 4.0);
    } else if (Height < 1.0) {
        // ��ԭ���ӻ���ɫ���ɵ���ɫ��ģ����ʯ���Σ�
        gradientColor = mix(vec3(0.5, 0.8, 0.2), vec3(0.6, 0.4, 0.2), (Height - 10.0) / 5.0);
    } else if (Height < 2.0) {
        // ��ɽ������ɫ���ɵ���ɫ��ģ����¶��ʯ��
        gradientColor = mix(vec3(0.6, 0.4, 0.2), vec3(0.5, 0.5, 0.5), (Height - 15.0) / 5.0);
    } else if (Height < 5.0) {
        // ѩ�߸������ӻ�ɫ���ɵ��׻�ɫ��ģ���ѩ���ǵ���ʯ��
        gradientColor = mix(vec3(0.5, 0.5, 0.5), vec3(0.8, 0.8, 0.8), (Height - 20.0) / 5.0);
    } else {
        // �ߺ��Σ��Ӱ׻�ɫ���ɵ�����ɫ��ģ��ѩ����
        gradientColor = mix(vec3(0.8, 0.8, 0.8), vec3(1.0, 1.0, 1.0), (Height - 25.0) / 5.0);
    }


    // ʹ�� isFlat ����������ɫ
    if (true) {
        // ��� isFlat Ϊ true��ʹ��ƽ����ɫ
        FragColor = vec4(flatColor, 1.0);
    } else {
        // ���򣬽�Ͻ���ɫ����ռ�����
        FragColor = vec4(gradientColor, 1.0);
    }
}
