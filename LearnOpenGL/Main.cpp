// ͷ�ļ���������
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<Windows.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "modelcamera.h"
#include"MineModel.h"
#include "ShapeRenderer.h"
#include <iostream>
#include <vector>
#include "filesystem.h"
#include "model.h"
#include "PerlinNoise.h"
#pragma comment(lib,"winmm.lib")
// ������������
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void configureLighting(Shader& shader, const glm::vec3* pointLightPositions);
unsigned int loadTexture(const char* path);

// ȫ�ֱ�����������

// ��Ļ����
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ���������
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // �������
ModelViewerCamera modelViewCamera(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f); // ģ�Ͳ鿴���
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool useModelViewerCamera = false; // true: ģ�Ͳ鿴���, false: �������

// ʱ�����
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ��������
glm::vec3 lightPos(1.2f, 1.0f, 2.0f); // ��Դλ��
int selectedLight = 1; // Ĭ��ѡ����Դ (1: ���Դ, 0: �����, 2: �۹��)

// ģ��ѡ����ƶ�
int selectedModel = 0; // ��ǰѡ�е�ģ�� (0: δѡ��)
glm::vec3 spherePosition = glm::vec3(0.0f, 0.4f, 0.0f);
glm::vec3 spherePosition_2 = glm::vec3(1.4f, 0.7f, 0.0f);
glm::vec3 conePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 torusPosition = glm::vec3(0.0f, 0.0f, 0.0f);
float movementSpeed = 0.03f; // ģ���ƶ��ٶ�

// �Զ���ת����
bool rotationEnabled = true;
float autoRotateAngleX = 0.0f, autoRotateAngleY = 0.0f, autoRotateAngleZ = 0.0f;



// positions all containers
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
};



float Screenvertices[] = {
    // Positions         // Texture Coords
      // ��һ��������
      -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top Left
      -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,  // Bottom Left
       1.0f, -1.0f, 0.0f,   1.0f, 0.0f,  // Bottom Right

       // �ڶ���������
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f,  // Top Right
       -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // Top Left
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f   // Bottom Rights
};

std::string SphereDiffsue_1 = "scratched.jpg";
std::string SphereSpecular_1 = "container2_specular.png";
std::string SphereDiffuse_2 = "container2.png";
std::string SphereSpecular_2 = "normal.png";
std::string TorusImage1 = "silk.png";
std::string TorusImage2 = "silk.png";
std::string ConeImage1 = "11096.jpg";
std::string ConeImage2 = "silk2.jpg";
std::string BackgroundImg = "8296224.jpg";

std::string HouseTexture = "../resources/objects/MyHouseModel/texture.png";

std::string NoiseTexture = "noise-perlin1.jpg";


//change light color
bool changeLightColor = true;

//isvisble
bool isShowModel1 = true;
bool isShowModel2 = true;
bool isShowModel3 = true;
bool isShowModel4 = true;

//��������
void playMusic(const char* filename) {
    PlaySound("bg_noman.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); // �첽ѭ������
}



//����������غ���
std::vector<int> generate_indices();
std::vector<float> generate_noise_map(int xOffset, int yOffset);
std::vector<float> generate_vertices(const std::vector<float>& noise_map);
std::vector<float> generate_normals(const std::vector<int>& indices, const std::vector<float>& vertices);
std::vector<float> generate_biome(const std::vector<float>& vertices, std::vector<plant>& plants, int xOffset, int yOffset);

void generate_map_chunk(GLuint& VAO, int xOffset, int yOffset, std::vector<plant>& plants);
//����⣬���Դ
glm::vec3 lightDirection = glm::vec3(0.1f, -.81f, -.61f);



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3DGameScene", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("minelights.vert", "minelights.frag");
    Shader screenShader("quad.vert", "quad.frag");
    Shader terrainShader("terrain.vs", "terrain.fs");
    Shader normalshader("normal.vs", "normal.fs");
     unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);



    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------

    unsigned int diffuseMap = loadTexture(SphereDiffsue_1.c_str());
    unsigned int specularMap = loadTexture(SphereSpecular_1.c_str());
    unsigned int diffuseMap_2 = loadTexture(TorusImage1.c_str());
    unsigned int specularMap_2 = loadTexture(TorusImage2.c_str());
    unsigned int diffuseMap_3 = loadTexture(ConeImage1.c_str());
    unsigned int specularMap_3 = loadTexture(ConeImage2.c_str());
    unsigned int diffuseMap_4 = loadTexture(SphereDiffuse_2.c_str());
    unsigned int specularMap_4 = loadTexture(SphereSpecular_2.c_str());
    unsigned int diffuseMap_5 = loadTexture("bricks2.jpg");
    unsigned int normalMap_5 = loadTexture("bricks2_normal.jpg");


    unsigned int textureMap = loadTexture(HouseTexture.c_str());

    unsigned int noiseMap = loadTexture(NoiseTexture.c_str());
       
    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);



    unsigned int textureID = loadTexture(BackgroundImg.c_str());
    // ����������˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // ���� VAO �� VBO
    GLuint  BGVAO;
    glGenVertexArrays(1, &BGVAO);
    glBindVertexArray(BGVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Screenvertices), Screenvertices, GL_STATIC_DRAW);

    // λ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ������������
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);;

    glBindVertexArray(0);


    //����ģ��
    Model ourModel("../resources/objects/backpack/backpack.obj");
    Model AirBallonModel("../resources/objects/AirBallon/11809_Hot_air_balloon_l2.obj");
    Model HouseModel("../resources/objects/planet/planet.stl");
    
    Shader ourShader("model.vs", "model.fs");




    //PCG START
    std::vector<plant> plants;
    std::vector<GLuint> map_chunks(xMapChunks* yMapChunks);

    for (int y = 0; y < yMapChunks; y++)
    {
        for (int x = 0; x < xMapChunks; x++)
        {
            generate_map_chunk(map_chunks[x + y * xMapChunks], x, y, plants);
        }
    }
    
    int nIndices = chunkWidth * chunkHeight * 6;
    //PCG END


    //��ʼ��ShapeRenderer��
    ShapeRenderer * shapeRenderer = new ShapeRenderer();

    // render loop
    // -----------
    playMusic("");
    while (!glfwWindowShouldClose(window))
    {
        //������ֱͬ��
        glfwMakeContextCurrent(window); // �������ĵ�����
        glfwSwapInterval(1);            // ���ô�ֱͬ��

        // ����Ⱦѭ����ÿ֡���µĲ��֣�
        float currentTime = glfwGetTime(); // ��ȡ��ǰʱ�䣨�룩
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
  
        autoRotateAngleX = rotationEnabled ? sin(currentTime) * 20.0f : autoRotateAngleX; // X����Զ���ת�Ƕ�
        autoRotateAngleY = rotationEnabled ? currentTime * 30.0f : autoRotateAngleY;      // Y����Զ���ת�Ƕ�
        autoRotateAngleZ = rotationEnabled ? cos(currentTime) * 15.0f : autoRotateAngleZ; // Z����Զ���ת�Ƕ�

        // input
        // -----
        processInput(window);


        // make light position oscillation (�����˶�)
        float timeValue = glfwGetTime();
        for (int i = 0; i < 4; i++) {
            // ����ÿ����Դ��λ������ʱ�������˶�
            pointLightPositions[i].x = 3.0f * sin(timeValue + i);  // ��x�������˶�
            pointLightPositions[i].y = 3.0f * sin(timeValue + i);  // ��y�������˶�
            pointLightPositions[i].z = 3.0f * sin(timeValue + i);  // ��z�������˶�
        }



        // render
        // ------
        glClearColor(0.811f, 0.753f, 0.773f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // ������Ȳ��ԣ�ȷ���������β������������ڵ���
        glDisable(GL_DEPTH_TEST);
        screenShader.use();
        glActiveTexture(GL_TEXTURE0);
        // ������
        glBindTexture(GL_TEXTURE_2D, textureID);

        // ���ƾ���
        glBindVertexArray(BGVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);  // ���ƾ��ε�����������

        // ������Ȳ��ԣ�������Ⱦ�������壩
        glEnable(GL_DEPTH_TEST);


        //������ͼ
        normalshader.use();
        normalshader.setInt("diffuseMap", 0);
        normalshader.setInt("norMap", 1);
        normalshader.setInt("noiseMap", 2);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        normalshader.use();
        normalshader.setMat4("projection", projection);
        normalshader.setMat4("view", view);
        // render normal-mapped quad
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
        model = glm::translate(model, glm::vec3(0.0f,-4.0f,0.0f));
        normalshader.setMat4("model", model);
        normalshader.setVec3("viewPos", camera.Position);
        normalshader.setVec3("lightPos", lightPos);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap_5);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, normalMap_5);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, noiseMap);
        //renderSphere();
        shapeRenderer->renderSphere(0.5,64,64);


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setInt("selectedLight", selectedLight);//choose light

        configureLighting(lightingShader, pointLightPositions);



        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = useModelViewerCamera
            ? modelViewCamera.GetViewMatrix()
            : camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);


        // ���������
        glEnable(GL_MULTISAMPLE);


        // Render the sphere
            // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap_4);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap_4);
        // Start with the identity matrix for the sphere
        model = glm::mat4(1.0f);
        model = glm::translate(model, spherePosition);
        // �Զ���ת - ׶��
        
        model = glm::rotate(model, glm::radians(autoRotateAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
        
        lightingShader.setMat4("model", model);

        // Render the sphere
        if(isShowModel1)
        //renderSphere(0.15f);
        shapeRenderer->renderSphere(0.15,64,64);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.0f));
        model = glm::translate(model, spherePosition_2);

        model = glm::rotate(model, glm::radians(autoRotateAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

        lightingShader.setMat4("model", model);
        // Render the sphere 2
        if(isShowModel4)
        //renderSphere(0.75f);
        shapeRenderer->renderSphere(0.75,64,64);





        // calculate the model matrix for each object and pass it to shader before drawing

        model = glm::mat4(1.0f);
        model = glm::translate(model, conePosition);
        // �Զ���ת - ׶��
        model = glm::rotate(model, glm::radians(autoRotateAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
        
        // Rotate the cone to tilt its top upwards
        // Example: Tilt the cone by 30 degrees along the X-axis and 15 degrees along the Y-axis
        float tiltAngleX = -17.3f; // Tilt angle along X-axis
        float tiltAngleY = -22.5f; // Tilt angle along Y-axis
        float tiltAngleZ = 16.8f; // Tilt angle along Y-axis
        model = glm::rotate(model, glm::radians(tiltAngleX), glm::vec3(1.0f, 0.0f, 0.0f)); // Tilt on X-axis
        model = glm::rotate(model, glm::radians(tiltAngleY), glm::vec3(0.0f, 1.0f, 0.0f)); // Tilt on Y-axis
        model = glm::rotate(model, glm::radians(tiltAngleZ), glm::vec3(0.0f, 0.0f, 1.0f)); // Tilt on Z-axis

        // Assign the model matrix to the shader
        lightingShader.setMat4("model", model);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap_3);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap_3);
        // Render the cone
        if(isShowModel3)
        //renderCone();
        shapeRenderer->renderCone(1.5f,0.75f,64,32);

        model = glm::mat4(1.0f);
        model = glm::translate(model, torusPosition);
        // �Զ���ת - ׶��
        model = glm::rotate(model, glm::radians(autoRotateAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(autoRotateAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

        model = glm::rotate(model, glm::radians(tiltAngleX), glm::vec3(1.0f, 0.0f, 0.0f)); // Tilt on X-axis
        model = glm::rotate(model, glm::radians(tiltAngleY), glm::vec3(0.0f, 1.0f, 0.0f)); // Tilt on Y-axis
        model = glm::rotate(model, glm::radians(tiltAngleZ), glm::vec3(0.0f, 0.0f, 1.0f)); // Tilt on Z-axis



        // Assign the model matrix to the shader
        lightingShader.setMat4("model", model);
        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap_2);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap_2);
        if(isShowModel2)
        //renderTorus(0.5f, 0.1f, 64, 32);
        shapeRenderer->renderCone(0.5f, 0.1f, 64, 32);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        //render AirBallon Model
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));	// it's a bit too big for our scene, so scale it down
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);
        AirBallonModel.Draw(ourShader);



        //���ƺ���ģ��
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.5f, 1.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));	
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ourShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        // ������
        glBindTexture(GL_TEXTURE_2D, textureMap);
        HouseModel.Draw(ourShader);

     
        terrainShader.use();
        terrainShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        terrainShader.setVec3("light.diffuse", glm::vec3(0.3f, 0.3f, 0.3f));
        terrainShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        terrainShader.setVec3("light.direction", lightDirection);
        terrainShader.setBool("isFlat", true);
        terrainShader.setMat4("u_projection", projection);
        terrainShader.setMat4("u_view", view);
        terrainShader.setVec3("u_viewPos", camera.Position);
        // Render map chunks
        float offsetx = 10.0, yoffset = -25.0, zoffset = 10.0;
        for (int y = 0; y < yMapChunks; y++)
            for (int x = 0; x < xMapChunks; x++) {
                //�����ݾ�����вü���һ��
        
                model = glm::mat4(1.0f);
                float scaleFactor = 0.3f;
                //��������һ��
                model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
                model = glm::translate(model, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * x + offsetx, yoffset, -chunkHeight / 2.0 + (chunkHeight - 1) * y) + zoffset);
                //model = glm::translate(model, glm::vec3(-chunkWidth / 2.0 + (chunkWidth - 1) * x, 0.0, -chunkHeight / 2.0 + (chunkHeight - 1) * y));
                terrainShader.setMat4("u_model", model);
                // Terrain chunk
                glBindVertexArray(map_chunks[x + y * xMapChunks]);
                glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
            }




        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}



void configureLighting(Shader& shader, const glm::vec3* pointLightPositions) {
    // Directional Lights
    if (!changeLightColor)
    {
        // ��ɫ�����
        shader.setVec3("dirLight[0].direction", 1.0f, -1.0f, 0.3f);
        shader.setVec3("dirLight[0].ambient", 1.0f, 0.0f, 0.0f); // ��ɫ������
        shader.setVec3("dirLight[0].diffuse", 1.0f, 0.0f, 0.0f); // ǿ�ҵĺ�ɫ��ɢ��
        shader.setVec3("dirLight[0].specular", 1.0f, 0.0f, 0.0f); // ��ɫ�߹�

        // ��ɫ�����
        shader.setVec3("dirLight[1].direction", -1.0f, -1.0f, 0.3f);
        shader.setVec3("dirLight[1].ambient", 0.0f, 0.0f, 1.0f); // ��ɫ������
        shader.setVec3("dirLight[1].diffuse", 0.0f, 0.0f, 1.0f); // ��ɫ��ɢ��
        shader.setVec3("dirLight[1].specular", 0.0f, 0.0f, 1.0f); // ��ɫ�߹�

        // ��ɫ�����
        shader.setVec3("dirLight[2].direction", 0.0f, -1.0f, 1.0f);
        shader.setVec3("dirLight[2].ambient", 0.0f, 1.0f, 0.0f); // ��ɫ������
        shader.setVec3("dirLight[2].diffuse", 0.0f, 1.0f, 0.0f); // ��ɫ��ɢ��
        shader.setVec3("dirLight[2].specular", 0.0f, 1.0f, 0.0f); // ��ɫ�߹�

        // ��ɫ�����
        shader.setVec3("dirLight[3].direction", 0.0f, -1.0f, -1.0f);
        shader.setVec3("dirLight[3].ambient", 1.0f, 1.0f, 0.0f); // ��ɫ������
        shader.setVec3("dirLight[3].diffuse", 1.0f, 1.0f, 0.0f); // ��ɫ��ɢ��
        shader.setVec3("dirLight[3].specular", 1.0f, 1.0f, 0.0f); // ��ɫ�߹�



        // Point Lights
        for (int i = 0; i < 4; ++i) {
            shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f); // ���û�����

            // ���ò�ͬ����ɫ
            switch (i) {
            case 0: // Red
                shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 1.0f, 0.0f, 0.0f); // ��ɫ��ɢ��
                shader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 0.0f, 0.0f); // ��ɫ�߹�
                break;
            case 1: // Blue
                shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.0f, 0.0f, 1.0f); // ��ɫ��ɢ��
                shader.setVec3("pointLights[" + std::to_string(i) + "].specular", 0.0f, 0.0f, 1.0f); // ��ɫ�߹�
                break;
            case 2: // Green
                shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.0f, 1.0f, 0.0f); // ��ɫ��ɢ��
                shader.setVec3("pointLights[" + std::to_string(i) + "].specular", 0.0f, 1.0f, 0.0f); // ��ɫ�߹�
                break;
            case 3: // Yellow
                shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 1.0f, 1.0f, 0.0f); // ��ɫ��ɢ��
                shader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 0.0f); // ��ɫ�߹�
                break;
            }

            shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }



        // Spotlights
        for (int i = 0; i < 4; ++i) {
            shader.setVec3("spotLight[" + std::to_string(i) + "].position", pointLightPositions[i]);
            shader.setVec3("spotLight[" + std::to_string(i) + "].direction", glm::normalize(glm::vec3(0.0f) - pointLightPositions[i]));
            shader.setVec3("spotLight[" + std::to_string(i) + "].ambient", 0.0f, 0.0f, 0.0f);

            switch (i) {
            case 0: // Red
                shader.setVec3("spotLight[0].diffuse", 1.0f, 0.3f, 0.3f); // ��ǿ��ɫ
                shader.setVec3("spotLight[0].specular", 1.0f, 0.2f, 0.2f); // ���Ӻ�ɫ�ĸ߹�
                break;
            case 1: // Light Red/Orange (still in red family)
                shader.setVec3("spotLight[1].diffuse", 0.8f, 0.4f, 0.3f); // ��΢��ǿ��ɫ
                shader.setVec3("spotLight[1].specular", 0.7f, 0.3f, 0.2f); // ��ɫ�ĸ߹�
                break;
            case 2: // Dark Red/Orange
                shader.setVec3("spotLight[2].diffuse", 0.6f, 0.3f, 0.2f); // ���ɫ
                shader.setVec3("spotLight[2].specular", 0.5f, 0.2f, 0.1f); // ���ɫ�߹�
                break;
            case 3: // Light Yellow-Red
                shader.setVec3("spotLight[3].diffuse", 1.0f, 0.7f, 0.3f); // ��ɫƫ��
                shader.setVec3("spotLight[3].specular", 1.0f, 0.6f, 0.3f); // ��ɫƫ��ĸ߹�
                break;
            }

            shader.setFloat("spotLight[" + std::to_string(i) + "].constant", 1.0f);
            shader.setFloat("spotLight[" + std::to_string(i) + "].linear", 0.09f);
            shader.setFloat("spotLight[" + std::to_string(i) + "].quadratic", 0.032f);
            shader.setFloat("spotLight[" + std::to_string(i) + "].cutOff", glm::cos(glm::radians(12.5f)));
            shader.setFloat("spotLight[" + std::to_string(i) + "].outerCutOff", glm::cos(glm::radians(15.0f)));
        }

    }
    else
    {
        shader.setVec3("dirLight[0].direction", 1.0f, -1.0f, 0.3f);
        shader.setVec3("dirLight[0].ambient", 0.1f, 0.2f, 0.5f); // Sky blue-like ambient
        shader.setVec3("dirLight[0].diffuse", 0.4f, 0.7f, 1.0f);  // Sky blue-like diffuse
        shader.setVec3("dirLight[0].specular", 0.6f, 0.9f, 1.0f); // Sky blue-like specular

        shader.setVec3("dirLight[1].direction", -1.0f, -1.0f, 0.3f);
        shader.setVec3("dirLight[1].ambient", 0.1f, 0.2f, 0.6f); // Sky blue-like ambient
        shader.setVec3("dirLight[1].diffuse", 0.4f, 0.8f, 1.0f);  // Sky blue-like diffuse
        shader.setVec3("dirLight[1].specular", 0.6f, 1.0f, 1.0f); // Sky blue-like specular

        shader.setVec3("dirLight[2].direction", 0.0f, -1.0f, 1.0f);
        shader.setVec3("dirLight[2].ambient", 0.1f, 0.2f, 0.7f); // Sky blue-like ambient
        shader.setVec3("dirLight[2].diffuse", 0.4f, 0.8f, 1.0f);  // Sky blue-like diffuse
        shader.setVec3("dirLight[2].specular", 0.6f, 1.0f, 1.0f); // Sky blue-like specular

        shader.setVec3("dirLight[3].direction", 0.0f, -1.0f, -1.0f);
        shader.setVec3("dirLight[3].ambient", 0.1f, 0.2f, 0.6f); // Sky blue-like ambient
        shader.setVec3("dirLight[3].diffuse", 0.4f, 0.7f, 1.0f);  // Sky blue-like diffuse
        shader.setVec3("dirLight[3].specular", 0.6f, 1.0f, 1.0f); // Sky blue-like specular



        // Point Lights
        for (int i = 0; i < 4; ++i) {
            shader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
            shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);

            switch (i) {
            case 0: // Blue-ish Red point light
                shader.setVec3("pointLights[0].ambient", 0.1f, 0.05f, 0.15f);  // Blue-ish Red ambient
                shader.setVec3("pointLights[0].diffuse", 0.4f, 0.2f, 0.6f);   // Blue-ish Red diffuse
                shader.setVec3("pointLights[0].specular", 0.5f, 0.3f, 0.8f);  // Blue-ish Red specular
                break;

            case 1: // Blue-ish Green point light
                shader.setVec3("pointLights[1].ambient", 0.05f, 0.1f, 0.2f);  // Blue-ish Green ambient
                shader.setVec3("pointLights[1].diffuse", 0.2f, 0.5f, 0.7f);   // Blue-ish Green diffuse
                shader.setVec3("pointLights[1].specular", 0.3f, 0.7f, 1.0f);  // Blue-ish Green specular
                break;

            case 2: // Pure Blue point light
                shader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.2f);  // Pure Blue ambient
                shader.setVec3("pointLights[2].diffuse", 0.2f, 0.2f, 1.0f);   // Pure Blue diffuse
                shader.setVec3("pointLights[2].specular", 0.3f, 0.3f, 1.0f);  // Pure Blue specular
                break;

            case 3: // Light Blue point light
                shader.setVec3("pointLights[3].ambient", 0.1f, 0.1f, 0.2f);  // Light Blue ambient
                shader.setVec3("pointLights[3].diffuse", 0.4f, 0.6f, 1.0f);   // Light Blue diffuse
                shader.setVec3("pointLights[3].specular", 0.5f, 0.8f, 1.0f);  // Light Blue specular
                break;
            }
        }




        // Spotlights
        for (int i = 0; i < 4; ++i) {
            shader.setVec3("spotLight[" + std::to_string(i) + "].position", pointLightPositions[i]);
            shader.setVec3("spotLight[" + std::to_string(i) + "].direction", glm::normalize(glm::vec3(0.0f) - pointLightPositions[i]));

            switch (i) {
            case 0: // Blue-ish Red spotlight
                shader.setVec3("spotLight[0].ambient", 0.1f, 0.05f, 0.15f);   // Dark blue-ish red ambient
                shader.setVec3("spotLight[0].diffuse", 0.4f, 0.2f, 0.6f);    // Bright blue-ish red diffuse
                shader.setVec3("spotLight[0].specular", 0.5f, 0.3f, 0.8f);   // Shiny blue-ish red specular
                break;
            case 1: // Blue-green spotlight
                shader.setVec3("spotLight[1].ambient", 0.05f, 0.1f, 0.2f);   // Dark blue-green ambient
                shader.setVec3("spotLight[1].diffuse", 0.2f, 0.5f, 0.7f);    // Bright blue-green diffuse
                shader.setVec3("spotLight[1].specular", 0.3f, 0.7f, 1.0f);   // Shiny blue-green specular
                break;
            case 2: // Pure Blue spotlight
                shader.setVec3("spotLight[2].ambient", 0.05f, 0.05f, 0.2f);   // Dark blue ambient
                shader.setVec3("spotLight[2].diffuse", 0.2f, 0.2f, 1.0f);    // Bright blue diffuse
                shader.setVec3("spotLight[2].specular", 0.3f, 0.3f, 1.0f);   // Shiny blue specular
                break;
            case 3: // Light Blue spotlight
                shader.setVec3("spotLight[3].ambient", 0.1f, 0.1f, 0.2f);    // Light blue ambient
                shader.setVec3("spotLight[3].diffuse", 0.4f, 0.6f, 1.0f);    // Light blue diffuse
                shader.setVec3("spotLight[3].specular", 0.5f, 0.8f, 1.0f);   // Shiny light blue specular
                break;
            }

            shader.setFloat("spotLight[" + std::to_string(i) + "].constant", 1.0f);
            shader.setFloat("spotLight[" + std::to_string(i) + "].linear", 0.09f);
            shader.setFloat("spotLight[" + std::to_string(i) + "].quadratic", 0.032f);
            shader.setFloat("spotLight[" + std::to_string(i) + "].cutOff", glm::cos(glm::radians(12.5f)));
            shader.setFloat("spotLight[" + std::to_string(i) + "].outerCutOff", glm::cos(glm::radians(15.0f)));
        }


    }


}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    // ����ƹ�ѡ��
    for (int i = 1; i <= 3; ++i) {
        if (glfwGetKey(window, GLFW_KEY_1 + i - 1) == GLFW_PRESS) {
            selectedLight = i-1; // ����Ϊ 1, 2 �� 3
        }
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        selectedModel = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        selectedModel = 2;
    }

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        selectedModel = 3;
    }

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        selectedModel = 4;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        isShowModel1 = !isShowModel1;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        isShowModel2 = !isShowModel2;
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        isShowModel3 = !isShowModel3;
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        isShowModel4 = !isShowModel4;
    }

    // ����ģ���ƶ�����
    glm::vec3* positions[] = { &spherePosition, &spherePosition_2, &conePosition, &torusPosition };
    if (selectedModel >= 1 && selectedModel <= 4) {
        glm::vec3& position = *positions[selectedModel - 1];
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) position.y += movementSpeed;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) position.y -= movementSpeed;
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) position.z -= movementSpeed;
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) position.z += movementSpeed;
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) position.x += movementSpeed;
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) position.x -= movementSpeed;
    }



    static bool cameraSwitchPressed = false;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !cameraSwitchPressed) {
        useModelViewerCamera = !useModelViewerCamera;
        cameraSwitchPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
        cameraSwitchPressed = false;
    }

    //change visibility of four model
    

    // �л���ת״̬
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        rotationEnabled = !rotationEnabled;
    }

    // �رմ���
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // ����ƶ�
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);


    //change light color
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        changeLightColor = !changeLightColor;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // ��תy�᷽��

    lastX = xpos;
    lastY = ypos;

    if (useModelViewerCamera) {
        modelViewCamera.ProcessMouseMovement(xoffset, yoffset);
    }
    else {
        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

    if (useModelViewerCamera) {
        modelViewCamera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
    else {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


//
glm::vec3 get_color(int r, int g, int b) {
    return glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}


void generate_map_chunk(GLuint& VAO, int xOffset, int yOffset, std::vector<plant>& plants) {
    std::vector<int> indices;
    std::vector<float> noise_map;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> colors;

    // Generate map
    indices = generate_indices();
    noise_map = generate_noise_map(xOffset, yOffset);
    vertices = generate_vertices(noise_map);
    normals = generate_normals(indices, vertices);
    colors = generate_biome(vertices, plants, xOffset, yOffset);

    GLuint VBO[3], EBO;

    // Create buffers and arrays
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // Bind vertices to VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Create element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

    // Configure vertex position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    // Configure vertex normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // Bind vertices to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);

    // Configure vertex colors attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
}




//ʹ�ð���������������ͼ
std::vector<float> generate_noise_map(int offsetX, int offsetY) {
    std::vector<float> noiseValues;
    std::vector<float> normalizedNoiseValues;
    std::vector<int> p = get_permutation_vector();

    float amp = 1;
    float freq = 1;
    float maxPossibleHeight = 0;

    for (int i = 0; i < octaves; i++) {
        maxPossibleHeight += amp;
        amp *= persistence;
    }

    for (int y = 0; y < chunkHeight; y++) {
        for (int x = 0; x < chunkWidth; x++) {
            amp = 1;
            freq = 1;
            float noiseHeight = 0;
            for (int i = 0; i < octaves; i++) {
                float xSample = (x + offsetX * (chunkWidth - 1)) / noiseScale * freq;
                float ySample = (y + offsetY * (chunkHeight - 1)) / noiseScale * freq;

                float perlinValue = perlin_noise(xSample, ySample, p);
                noiseHeight += perlinValue * amp;

                // Lacunarity  --> Increase in frequency of octaves
                // Persistence --> Decrease in amplitude of octaves
                amp *= persistence;
                freq *= lacunarity;
            }

            noiseValues.push_back(noiseHeight);
        }
    }

    for (int y = 0; y < chunkHeight; y++) {
        for (int x = 0; x < chunkWidth; x++) {
            // Inverse lerp and scale values to range from 0 to 1
            normalizedNoiseValues.push_back((noiseValues[x + y * chunkWidth] + 1) / maxPossibleHeight);
        }
    }

    return normalizedNoiseValues;
}


//���εĸ߶Ⱥ���ɫ
struct terrainColor {
    terrainColor(float _height, glm::vec3 _color) {
        height = _height;
        color = _color;
    };
    float height;
    glm::vec3 color;
};

std::vector<float> generate_biome(const std::vector<float>& vertices, std::vector<plant>& plants, int xOffset, int yOffset) {
    std::vector<float> colors;
    std::vector<terrainColor> biomeColors;
    glm::vec3 color = get_color(255, 255, 255);

    // NOTE: Terrain color height is a value between 0 and 1
    biomeColors.push_back(terrainColor(WATER_HEIGHT * 0.5, get_color(60, 95, 190)));   // Deep water
    biomeColors.push_back(terrainColor(WATER_HEIGHT, get_color(60, 100, 190)));  // Shallow water
    biomeColors.push_back(terrainColor(0.15, get_color(210, 215, 130)));                // Sand
    biomeColors.push_back(terrainColor(0.30, get_color(95, 165, 30)));                // Grass 1
    biomeColors.push_back(terrainColor(0.40, get_color(65, 115, 20)));                // Grass 2
    biomeColors.push_back(terrainColor(0.50, get_color(90, 65, 60)));                // Rock 1
    biomeColors.push_back(terrainColor(0.80, get_color(75, 60, 55)));                // Rock 2
    biomeColors.push_back(terrainColor(1.00, get_color(255, 255, 255)));                // Snow

    std::string plantType;

    // Determine which color to assign each vertex by its y-coord
    // Iterate through vertex y values
    for (int i = 1; i < vertices.size(); i += 3) {
        for (int j = 0; j < biomeColors.size(); j++) {
            // NOTE: The max height of a vertex is "meshHeight"
            if (vertices[i] <= biomeColors[j].height * meshHeight) {
                color = biomeColors[j].color;
                if (j == 3) {
                    if (rand() % 1000 < 5) {
                        if (rand() % 100 < 70) {
                            plantType = "flower";
                        }
                        else {
                            plantType = "tree";
                        }
                        plants.push_back(plant{ plantType, vertices[i - 1], vertices[i], vertices[i + 1], xOffset, yOffset });
                    }
                }
                break;
            }
        }
        colors.push_back(color.r);
        colors.push_back(color.g);
        colors.push_back(color.b);
    }
    return colors;
}

std::vector<float> generate_normals(const std::vector<int>& indices, const std::vector<float>& vertices) {
    int pos;
    glm::vec3 normal;
    std::vector<float> normals;
    std::vector<glm::vec3> verts;

    // Get the vertices of each triangle in mesh
    // For each group of indices
    for (int i = 0; i < indices.size(); i += 3) {

        // Get the vertices (point) for each index
        for (int j = 0; j < 3; j++) {
            pos = indices[i + j] * 3;
            verts.push_back(glm::vec3(vertices[pos], vertices[pos + 1], vertices[pos + 2]));
        }

        // Get vectors of two edges of triangle
        glm::vec3 U = verts[i + 1] - verts[i];
        glm::vec3 V = verts[i + 2] - verts[i];

        // Calculate normal
        normal = glm::normalize(-glm::cross(U, V));
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }

    return normals;
}

std::vector<float> generate_vertices(const std::vector<float>& noise_map) {
    std::vector<float> v;
    //
    for (int y = 0; y < chunkHeight; y++)
        for (int x = 0; x < chunkWidth; x++) {
            v.push_back(x);
            // Apply cubic easing to the noise
            float easedNoise = std::pow(noise_map[x + y * chunkWidth] * 1.1, 3);
            // Scale noise to match meshHeight
            // Pervent vertex height from being below WATER_HEIGHT
            v.push_back(std::fmax(easedNoise * meshHeight, WATER_HEIGHT * 0.5 * meshHeight));
            v.push_back(y);
        }

    return v;
}

std::vector<int> generate_indices() {
    std::vector<int> indices;

    for (int y = 0; y < chunkHeight; y++)
        for (int x = 0; x < chunkWidth; x++) {
            int pos = x + y * chunkWidth;

            if (x == chunkWidth - 1 || y == chunkHeight - 1) {
                // Don't create indices for right or top edge
                continue;
            }
            else {
                // Top left triangle of square
                indices.push_back(pos + chunkWidth);
                indices.push_back(pos);
                indices.push_back(pos + chunkWidth + 1);
                // Bottom right triangle of square
                indices.push_back(pos + 1);
                indices.push_back(pos + 1 + chunkWidth);
                indices.push_back(pos);
            }
        }

    return indices;
}

