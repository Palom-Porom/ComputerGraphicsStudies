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
};

ObjData obj_data;
string obj_path = "bunny.obj";

GLuint Program;
GLint Attrib_vertex;
GLint Attrib_normal;
GLint Attrib_color;

//GLuint VBO;
enum VAO_IDs { RotatedCube, NumVAOs };
enum Buffer_IDs { VertexBuffer, NormalBuffer, ColorBuffer, IndexBuffer, NumBuffers };
GLuint Buffers[NumBuffers];
const GLuint VERTICIES_NUM = 8;

GLuint VAOs[NumVAOs];

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::vec3 lightDir;

GLint modelLoc;
GLint viewLoc;
GLint projLoc;
GLint lightDirLoc;

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
        objData.colors[3 * i] = 0.2f;
        objData.colors[3 * i + 1] = 0.8f;
        objData.colors[3 * i + 2] = 0.2f;
    }

    return true;
}

const char* VertexShaderSource = R"(
    #version 330 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec3 color;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec3 lightDir;
     
    out vec3 Color; 
    out vec3 LightDiff;     

    void main() {
        gl_Position = projection * view * model * vec4(position, 1.0);
    
        vec3 Normal = mat3(transpose(inverse(model))) * normal;
        Normal = normalize(Normal);
    
        LightDiff = max(dot(Normal, lightDir), 0.0) * vec3(1.0f, 1.0f, 1.0f);

        Color = color;
}
)";

const char* FragShaderSource = R"(
 #version 330 core

 in vec3 LightDiff;
 in vec3 Color;

 out vec4 color;

 void main() 
{
    vec3 ambient = 0.1f * vec3(1.0f, 1.0f, 1.0f);

    vec3 resultingColor = (ambient + LightDiff) * Color;
    color = vec4(resultingColor, 1.0f);
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

void init_VBO(const ObjData& objData)
{
    glCreateBuffers(NumBuffers, Buffers);

    glNamedBufferStorage(Buffers[VertexBuffer], objData.positions.size() * sizeof(float), objData.positions.data(), 0);
    glNamedBufferStorage(Buffers[NormalBuffer], objData.normals.size() * sizeof(float), objData.normals.data(), 0);
    glNamedBufferStorage(Buffers[ColorBuffer], objData.colors.size() * sizeof(float), objData.colors.data(), 0);
    glNamedBufferStorage(Buffers[IndexBuffer], objData.indices.size() * sizeof(unsigned int), objData.indices.data(), 0);

    glCreateVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[RotatedCube]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexBuffer]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[IndexBuffer]);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    check_openGL_error();
}

bool init()
{
    init_shader();
    if (!loadOBJ(obj_path, obj_data))
    {
        cerr << "Could not load OBJ\n";
        return false;
    }
    cout << "Loaded " << obj_data.positions.size() / 3 << " vertices\n";
    cout << "Loaded " << obj_data.indices.size() / 3 << " triangles\n";
    if (obj_data.normals.empty()) {
        cerr << "OBJ has no normals, generating them...\n";
        computeNormals(obj_data);
    }
    init_VBO(obj_data);
    glEnable(GL_DEPTH_TEST);

    model = glm::mat4(1.0f);
    //lookAt(cameraPos, Vector to target, up vector)
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.5f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    //perspective(FOV, Aspect Ratio, Near plane, Far plane)
    projection = glm::perspective(glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f);

    lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f));
    lightDir = glm::normalize(-lightDir); //Rotate TO the sun

    return true;
}

void Draw()
{
    glUseProgram(Program);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    glBindVertexArray(VAOs[RotatedCube]);

    glDrawElements(GL_TRIANGLES, obj_data.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    check_openGL_error();
}

void release_VBO()
{
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(NumBuffers, Buffers);
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

    float angleX = 0.0f;
    float angleY = 0.0f;
    float angleZ = 0.0f;

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
            else if (event.type == sf::Event::KeyPressed) {
                // Y-axis rotation
                if (event.key.code == sf::Keyboard::A) {
                    angleY -= 5.0f;
                }
                else if (event.key.code == sf::Keyboard::D) {
                    angleY += 5.0f;
                }
                // X-axis rotation
                if (event.key.code == sf::Keyboard::W) {
                    angleX -= 5.0f;
                }
                else if (event.key.code == sf::Keyboard::S) {
                    angleX += 5.0f;
                }
                // Z-axis rotation
                if (event.key.code == sf::Keyboard::Q) {
                    angleZ -= 5.0f;
                }
                else if (event.key.code == sf::Keyboard::E) {
                    angleZ += 5.0f;
                }
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(glm::mat4(1.0f), glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));

        Draw();

        // Перерисовка окна
        window.display();
    }

    release_shader();
    release_VBO();

    return 0;
}