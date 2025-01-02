#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aOffset;

flat out vec3 flatColor;    // 传递平面颜色（如果是平面渲染）
out vec3 FragPos;           // 传递碎片位置到片段着色器
out vec3 Normal;            // 传递法线到片段着色器
out vec3 Color;             // 传递颜色到片段着色器
out float Height;  

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 u_viewPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

vec3 calculateLighting(vec3 Normal, vec3 FragPos) {
    // Ambient lighting
    vec3 ambient = light.ambient;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular);
}

void main() {
    // 计算位置
    FragPos = vec3(u_model * vec4(aPos + aOffset, 1.0));
    // 传递法线
    Normal = normalize(mat3(transpose(inverse(u_model))) * aNormal);
    // 传递颜色
    Color = aColor;
    Height = FragPos.y;
    // 计算平面颜色（默认情况下计算光照）
    vec3 lighting = calculateLighting(Normal, FragPos);
    flatColor = aColor * lighting;

    // 计算最终位置
    gl_Position = u_projection * u_view * u_model * vec4(aPos + aOffset, 1.0);
}
