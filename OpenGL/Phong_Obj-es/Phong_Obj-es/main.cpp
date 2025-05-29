#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include<iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "tiny_obj_loader.h"
#include <vector>

#define BUFFER_OFFSET(offset) ((void *) (offset))

using namespace std;

struct ObjData {
    vector<float> positions;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> colors;
    vector<float> texCoords;
    GLuint textureID;
};

vector<ObjData> loadedObjects;
vector<string> obj_paths = {
        //"tiger.obj",
        "pineapple2.obj",
        //"bunny.obj",
        //"shkaf.obj",
        //"Skull.obj",
        //"cheburashka.obj"
};
vector<const char*> obj_textures_paths = {
        //"tiger.jpg",
        "pineapple2.jpg",
        //"bunny.png",
        //"shkaf.jpg",
        //"Skull.jpg",
        //"cheburashka.jpeg"
};
vector<GLuint> obj_textures;

GLuint Program;
GLint Attrib_vertex;
GLint Attrib_normal;
GLint Attrib_color;

//GLuint VBO;
enum Buffer_IDs { VertexBuffer, NormalBuffer, ColorBuffer, IndexBuffer, NumBuffers };
GLuint Buffers[NumBuffers];
GLuint texCoordBuffer;
const GLuint VERTICIES_NUM = 8;

vector<GLuint> VAOs, IndexCounts;

//glm::mat4 model;
vector<glm::mat4> modelMatrices;
glm::mat4 view;
glm::mat4 projection;
glm::vec3 lightDir;

GLint modelLoc;
GLint viewLoc;
GLint projLoc;
GLint lightDirLoc;

glm::vec3 cameraPos(1.0f, 0.8f, 1.8f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f); 
glm::vec3 cameraRight; 
float cameraHorAngle = -90.0f;   
float cameraVerAngle = 0.0f;   
float cameraSpeed = 0.01f; 
float cameraTurnSpeed = 3.0f; 

float lightHorAngle = glm::radians(0.0f);
float lightVerAngle = glm::radians(-45.0f);
float lightAngleSpeed = glm::radians(0.1f);

void check_openGL_error()
{
    GLenum code;
    code = glGetError();
    if (code != GL_NO_ERROR)
    {
        cout << "ERROR! " << gluErrorString(code) << "\n";
    }
}

void ShaderLog(unsigned int shader)
{
    int infologLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
    if (infologLen > 1)
    {
        int charsWritten = 0;
        vector<char> infoLog(infologLen);
        glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
        cout << "InfoLog: " << infoLog.data() << "\n";
    }
}

void computeNormals(ObjData& objData) {
    objData.normals.assign(objData.positions.size(), 0.0f);

    for (size_t i = 0; i < objData.indices.size(); i += 3) {
        unsigned int ia = objData.indices[i];
        unsigned int ib = objData.indices[i + 1];
        unsigned int ic = objData.indices[i + 2];

        glm::vec3 v0(
            objData.positions[3 * ia + 0],
            objData.positions[3 * ia + 1],
            objData.positions[3 * ia + 2]
        );
        glm::vec3 v1(
            objData.positions[3 * ib + 0],
            objData.positions[3 * ib + 1],
            objData.positions[3 * ib + 2]
        );
        glm::vec3 v2(
            objData.positions[3 * ic + 0],
            objData.positions[3 * ic + 1],
            objData.positions[3 * ic + 2]
        );

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        for (int j = 0; j < 3; ++j) {
            unsigned idx = objData.indices[i + j];
            objData.normals[3 * idx + 0] += normal.x;
            objData.normals[3 * idx + 1] += normal.y;
            objData.normals[3 * idx + 2] += normal.z;
        }
    }

    size_t vertCount = objData.positions.size() / 3;
    for (size_t i = 0; i < vertCount; ++i) {
        glm::vec3 n(
            objData.normals[3 * i + 0],
            objData.normals[3 * i + 1],
            objData.normals[3 * i + 2]
        );
        n = glm::normalize(n);
        objData.normals[3 * i + 0] = n.x;
        objData.normals[3 * i + 1] = n.y;
        objData.normals[3 * i + 2] = n.z;
    }
}

bool loadOBJ(const string& filename, ObjData& objData)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = ""; // no mtl path
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename, reader_config)) {
        if (!reader.Error().empty()) {
            cerr << "TinyObjReader: " << reader.Error();
        }
        return false;
    }
    if (!reader.Warning().empty()) {
        cout << "TinyObjReader: " << reader.Warning();
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    objData.positions.clear();
    objData.normals.clear();
    objData.indices.clear();
    objData.colors.clear();

    objData.positions = attrib.vertices;

    if (!attrib.texcoords.empty()) {
        objData.texCoords = attrib.texcoords;
    }

    if (!attrib.normals.empty()) {
        objData.normals = attrib.normals;
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            objData.indices.push_back(index.vertex_index);
        }
    }

    objData.colors.resize(objData.positions.size());
    for (size_t i = 0; i < objData.colors.size() / 3; ++i) {
        objData.colors[3 * i] = 1.0f;
        objData.colors[3 * i + 1] = 1.0f;
        objData.colors[3 * i + 2] = 1.0f;
    }

    return true;
}

GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    sf::Image image;
    if (!image.loadFromFile(path)) {
        cerr << "Failed to load texture: " << path << endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }

    //image.flipVertically();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y,
        0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

const char* VertexShaderSource = R"(
    #version 330 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec3 color;
    layout(location = 3) in vec2 texCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 lightDir;
     
    out vec3 Color; 
    out vec3 Normal;
    out vec3 LightDir;
    out vec3 FragPos;     
    out vec2 TexCoord; 

    void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    Normal = normalize(Normal);
    LightDir = normalize(-lightDir);
    Color = color;
    TexCoord = texCoord;
    }
)";

const char* FragShaderSource = R"(
 #version 330 core

 in vec3 LightDir;
 in vec3 Color;
 in vec3 Normal;
 in vec3 FragPos;
 in vec2 TexCoord;

 uniform sampler2D textureSampler;

 out vec4 color;

 void main() 
{
    vec4 texColor = texture(textureSampler, TexCoord);
    vec3 finalColor = Color * texColor.rgb;

    vec3 ambient = 0.1f * finalColor;

    float diff = max(dot(Normal, LightDir), 0.0);
    vec3 diffuse = diff * finalColor;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-LightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.8) * spec;

    vec3 resultingColor = (ambient + diffuse + specular);
    color = vec4(resultingColor, texColor.a);
 }
)";

void init_shader()
{
    //Vertex shader
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    //Set the source code
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    //Compile
    glCompileShader(vShader);
    cout << "Vertex Shader is compiled!\n";
    ShaderLog(vShader);

    //Fragment shader
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Set the source code
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    //Compile
    glCompileShader(fShader);
    cout << "Fragment Shader is compiled!\n";
    ShaderLog(fShader);

    Program = glCreateProgram();

    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);

    glLinkProgram(Program);

    int link_ok;
    glGetProgramiv(Program, GL_LINK_STATUS, &link_ok);
    if (!link_ok)
    {
        cout << "Error of attaching shaders \n";
        return;
    }

    modelLoc = glGetUniformLocation(Program, "model");
    viewLoc = glGetUniformLocation(Program, "view");
    projLoc = glGetUniformLocation(Program, "projection");
    lightDirLoc = glGetUniformLocation(Program, "lightDir");

    check_openGL_error();
}

void init_VBO(const ObjData& objData, GLuint& VAO, GLuint& indexCount)
{
    glCreateBuffers(NumBuffers, Buffers);

    glNamedBufferStorage(Buffers[VertexBuffer], objData.positions.size() * sizeof(float), objData.positions.data(), 0);
    glNamedBufferStorage(Buffers[NormalBuffer], objData.normals.size() * sizeof(float), objData.normals.data(), 0);
    glNamedBufferStorage(Buffers[ColorBuffer], objData.colors.size() * sizeof(float), objData.colors.data(), 0);
    glNamedBufferStorage(Buffers[IndexBuffer], objData.indices.size() * sizeof(unsigned int), objData.indices.data(), 0);

    if (!objData.texCoords.empty()) {
        glCreateBuffers(1, &texCoordBuffer);
        glNamedBufferStorage(texCoordBuffer, objData.texCoords.size() * sizeof(float), objData.texCoords.data(), 0);
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexBuffer]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(2);

    if (!objData.texCoords.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(3);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[IndexBuffer]);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    indexCount = objData.indices.size();

    check_openGL_error();
}

bool init()
{
    init_shader();

    for (int i = 0; i < obj_paths.size(); i++) {
        ObjData new_data;
        if (!loadOBJ(obj_paths[i], new_data)) {
            cerr << "Could not load OBJ: " << obj_paths[i] << endl;
            return false;
        }
        cout << "Loaded " << obj_paths[i] << " with "
            << new_data.positions.size() / 3 << " vertices and "
            << new_data.indices.size() / 3 << " triangles." << endl;

        if (new_data.normals.empty()) {
            cerr << "OBJ " << obj_paths[i] << " has no normals, generating them..." << endl;
            computeNormals(new_data);
        }

        if (obj_textures_paths.size() <= i)
        {
            cout << obj_paths[i] << " has no texture!\n";
        }
        else
        {
            new_data.textureID = loadTexture(obj_textures_paths[i]);
            if (new_data.textureID == 0) {
                cerr << "Failed to load texture: " << obj_textures_paths[i] << endl;
                return false;
            }
            //obj_textures.push_back(new_data.textureID);
        }
        loadedObjects.push_back(new_data);
    }

    VAOs.resize(loadedObjects.size());
    glCreateVertexArrays(loadedObjects.size(), VAOs.data());
    IndexCounts.resize(loadedObjects.size());

    for (size_t i = 0; i < loadedObjects.size(); ++i) {
        init_VBO(loadedObjects[i], VAOs[i], IndexCounts[i]);
        if (glGetError() != GL_NO_ERROR) {
            cerr << "Failed to initialize VBO for object " << i << endl;
            return false;
        }
    }

    glEnable(GL_DEPTH_TEST);

    //model = glm::mat4(1.0f);
    for (size_t i = 0; i < loadedObjects.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(i * 2.0f, 0.0f, 0.0f));
        //model = glm::mat4(1.0f);
        modelMatrices.push_back(model);
    }
    //lookAt(cameraPos, Vector to target, up vector)
    //view = glm::lookAt(glm::vec3(1.0f, 0.8f, 1.8f),
    //    glm::vec3(0.0f, 0.0f, 0.0f),
    //    glm::vec3(0.0f, 1.0f, 0.0f));
    //perspective(FOV, Aspect Ratio, Near plane, Far plane)
    projection = glm::perspective(glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f);

    //lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f));
    //lightDir = glm::normalize(-lightDir); //Rotate TO the sun

    //cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

    return true;
}

void Draw()
{
    glUseProgram(Program);

    //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    for (size_t i = 0; i < loadedObjects.size(); ++i) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, loadedObjects[i].textureID);
        glUniform1i(glGetUniformLocation(Program, "textureSampler"), 0);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrices[i]));

        glBindVertexArray(VAOs[i]);
        glDrawElements(GL_TRIANGLES, IndexCounts[i], GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glUseProgram(0);

    check_openGL_error();
}

void release_VBO()
{
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(NumBuffers, Buffers);
    glDeleteVertexArrays(loadedObjects.size(), VAOs.data());
}

void release_shader()
{
    glUseProgram(0);
    glDeleteProgram(Program);
}

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24; // Request a 24-bit depth buffer

    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML window", sf::Style::Default, contextSettings);

    glewInit();
    if (!init())
        return -1;

    //float angleX = 0.0f;
    //float angleY = 0.0f;
    //float angleZ = 0.0f;

    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    sf::Mouse::setPosition(windowCenter, window);
    float mouseSensitivity = 0.1f;

    sf::Clock clock;

    // Главный цикл
    while (window.isOpen())
    {
        sf::Event event;
        // Цикл обработки событий
        while (window.pollEvent(event))
        {
            // Событие закрытия окна, 
            if (event.type == sf::Event::Closed)
                window.close();


            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int dx = mousePos.x - windowCenter.x;
            int dy = mousePos.y - windowCenter.y;
            sf::Mouse::setPosition(windowCenter, window);

            if (dx != 0 || dy != 0)
            {
                cameraHorAngle += dx * mouseSensitivity;
                cameraVerAngle -= dy * mouseSensitivity;
                if (cameraVerAngle > 89.0f) cameraVerAngle = 89.0f;
                if (cameraVerAngle < -89.0f) cameraVerAngle = -89.0f;

                float verRad = glm::radians(cameraVerAngle);
                float horRad = glm::radians(cameraHorAngle);
                cameraFront = glm::normalize(glm::vec3(
                    cos(horRad) * cos(verRad),
                    sin(verRad),
                    sin(horRad) * cos(verRad)
                ));

                //view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            }
        }

        float deltaTime = (clock.restart().asMicroseconds()) / 1000.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            cameraPos += cameraSpeed * cameraFront * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            cameraPos -= cameraSpeed * cameraFront * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * deltaTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            cameraPos += cameraSpeed * cameraUp * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            cameraPos -= cameraSpeed * cameraUp * deltaTime;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            lightHorAngle -= lightAngleSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            lightHorAngle += lightAngleSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            lightVerAngle -= lightAngleSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            lightVerAngle += lightAngleSpeed * deltaTime;

        lightDir = glm::normalize(glm::vec3(cos(lightHorAngle) * cos(lightVerAngle), sin(lightVerAngle), sin(lightHorAngle) * cos(lightVerAngle)));

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //for (size_t i = 0; i < loadedObjects.size(); ++i) {
        //    modelMatrices[i] = glm::rotate(glm::mat4(1.0f), glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
        //    modelMatrices[i] = glm::rotate(modelMatrices[i], glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        //    modelMatrices[i] = glm::rotate(modelMatrices[i], glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        //}

        Draw();

        // Перерисовка окна
        window.display();
    }

    release_shader();
    release_VBO();
    for (auto& obj : loadedObjects) {
        glDeleteTextures(1, &obj.textureID);
    }

    return 0;
}