#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D noiseMap;  // 添加噪声纹理

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
     // 获取法线贴图的法线，并将其转换到[-1,1]范围
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);  // 这时法线在切线空间


    vec3 noise = texture(noiseMap, fs_in.TexCoords * 5.0).rgb;  // 增加缩放因子来控制噪声的细节
    normal += (noise - 0.5) * 0.2;  // 轻微扰动法线

    // 获取漫反射颜色
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;

    // 环境光
    vec3 ambient = 0.3 * color;

    // 漫反射
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // 高光
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;

    
    float noiseIntensity = texture(noiseMap, fs_in.TexCoords * 2.0).r;  // 以噪声纹理的一个通道影响光照强度
    vec3 lighting = ambient + diffuse + specular * noiseIntensity;

    FragColor = vec4(lighting, 1.0);
}
