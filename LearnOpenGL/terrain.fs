#version 330 core
flat in vec3 flatColor;     // 从顶点着色器传递的平面颜色
in vec3 Color;              // 从顶点着色器传递的光照计算颜色
in float Height;            // 从顶点着色器传递的高度值
out vec4 FragColor;

uniform bool isFlat;        // 控制是否使用平面颜色

void main() {
    vec3 gradientColor;

    // 根据高度设置更细致的颜色渐变
    if (Height < 0.05) {
        // 低海拔：深蓝色（模拟深水）
        gradientColor = mix(vec3(0.1, 0.2, 0.6), vec3(0.2, 0.5, 0.8), Height / 3.0);
    } else if (Height < 0.15) {
        // 浅海：从浅蓝色过渡到绿色
        gradientColor = mix(vec3(0.2, 0.5, 0.8), vec3(0.3, 0.7, 0.4), (Height - 3.0) / 3.0);
    } else if (Height < 0.5) {
        // 草地：从绿色过渡到黄绿色（模拟草地和低矮植被）
        gradientColor = mix(vec3(0.3, 0.7, 0.4), vec3(0.5, 0.8, 0.2), (Height - 6.0) / 4.0);
    } else if (Height < 1.0) {
        // 高原：从黄绿色过渡到棕色（模拟岩石地形）
        gradientColor = mix(vec3(0.5, 0.8, 0.2), vec3(0.6, 0.4, 0.2), (Height - 10.0) / 5.0);
    } else if (Height < 2.0) {
        // 高山：从棕色过渡到灰色（模拟裸露岩石）
        gradientColor = mix(vec3(0.6, 0.4, 0.2), vec3(0.5, 0.5, 0.5), (Height - 15.0) / 5.0);
    } else if (Height < 5.0) {
        // 雪线附近：从灰色过渡到白灰色（模拟冰雪覆盖的岩石）
        gradientColor = mix(vec3(0.5, 0.5, 0.5), vec3(0.8, 0.8, 0.8), (Height - 20.0) / 5.0);
    } else {
        // 高海拔：从白灰色过渡到纯白色（模拟雪顶）
        gradientColor = mix(vec3(0.8, 0.8, 0.8), vec3(1.0, 1.0, 1.0), (Height - 25.0) / 5.0);
    }


    // 使用 isFlat 决定最终颜色
    if (true) {
        // 如果 isFlat 为 true，使用平面颜色
        FragColor = vec4(flatColor, 1.0);
    } else {
        // 否则，结合渐变色与光照计算结果
        FragColor = vec4(gradientColor, 1.0);
    }
}
