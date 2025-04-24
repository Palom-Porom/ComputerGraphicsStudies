#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include<iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(offset) ((void *) (offset))

using namespace std;

//enum VAO_IDs { Triangles, NumVAOs };
//enum Buffer_IDs { ArrayBuffer, NumBuffers };
//enum Attrib_IDs { vPosition = 0 };
//
//GLuint VAOs[NumVAOs];
//GLuint Buffers[NumBuffers]; 
//
//const GLuint NumVertices = 6;


GLuint Program;
GLint Attrib_vertex;
GLint Attrib_normal;
GLint Attrib_color;

//GLuint VBO;
enum VAO_IDs { RotatedCube, NumVAOs };
enum Buffer_IDs { VertexBuffer, NormalBuffer, ColorBuffer, IndexBuffer, NumBuffers };
GLuint Buffers[NumBuffers];
const GLuint VERTICIES_NUM = 8;

//Used for 4.5 version of draw
//GLuint VAO;

//glm::mat4 test;

GLuint VAOs[NumVAOs];

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::vec3 lightDir;

GLint modelLoc;
GLint viewLoc;
GLint projLoc;
GLint lightDirLoc;

struct Vertex {
    GLfloat pos[3];
    glm::vec3 normal;
    GLubyte color[3];

    //inline void rotate(glm::mat4 rotation_matrix)
    //{
    //    glm::vec4 res = rotation_matrix * glm::vec4(pos[0], pos[1], pos[2], 1.0f);
    //    pos[0] = res.x;
    //    pos[1] = res.y;
    //    pos[2] = res.z;
    //}
};

//glm::mat4 get_rotation_matrix(float angle_degrees, int axis = 0)
//{
//    float angle_radians = angle_degrees * glm::pi<float>() / 180.0f;
//
//    float sinTheta = sin(angle_radians);
//    float cosTheta = cos(angle_radians);
//
//    glm::mat4 res = glm::mat4(1.0f);
//    res[(axis + 1) % 3][(axis + 1) % 3] = cosTheta;
//    res[(axis + 1) % 3][(axis + 2) % 3] = sinTheta;
//    res[(axis + 2) % 3][(axis + 1) % 3] = -sinTheta;
//    res[(axis + 2) % 3][(axis + 2) % 3] = cosTheta;
//
//    return res;
//}

//glm::mat4 get_view_matrix(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
//    glm::vec3 f = glm::normalize(target - pos); // Forward
//    glm::vec3 r = glm::normalize(glm::cross(f, up)); // Right
//    glm::vec3 u = glm::cross(r, f); // Up 
//    glm::vec3 t = glm::vec3(-r * pos, -u * pos, f * pos);
//
//    glm::mat4 view = glm::mat4(glm::vec4(r, t.x), glm::vec4(u, t.y), glm::vec4(-f, t.z), glm::vec4(0, 0, 0, 1.0f));
//    view = glm::transpose(view);
//
//    return view;
//}
//
//glm::mat4 get_projection_matrix(float fovRadians, float aspect, float near, float far) {
//    float tanHalfFov = tan(fovRadians / 2.0f);
//
//    glm::mat4 proj = glm::mat4(glm::vec4(1.0f / (aspect * tanHalfFov), 0, 0, 0),
//                               glm::vec4(0, 1.0f / tanHalfFov, 0, 0),
//                               glm::vec4(0, 0, -(far + near) / (far - near), -1.0f),
//                               glm::vec4(0, 0, -2.0f * far * near / (far - near), 0));
//
//    return proj;
//}


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

//abs(vec4(1.0, 1.0, 1.0, 1.0) - gl_Position);
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

void init_VBO()
{
    //Vertex verticies[] =
    //{
    //    {{ -0.5, -0.5, +0.5 }, {255, 0, 0}}, {{ -0.5, +0.5, +0.5 }, {0, 255, 0}}, {{ +0.5, +0.5, +0.5} , {0, 0, 255}},
    //    {{ +0.5, +0.5, +0.5 }, {255, 0, 0}}, {{ +0.5, -0.5, +0.5 }, {0, 255, 0}}, {{ -0.5, -0.5, +0.5 }, {0, 0, 255}},
    //    {{ -0.5, -0.5, -0.5 }, {255, 0, 0}}, {{ +0.5, +0.5, -0.5 }, {0, 255, 0}}, {{ -0.5, +0.5, -0.5 }, {0, 0, 255}},
    //    {{ +0.5, +0.5, -0.5 }, {255, 0, 0}}, {{ -0.5, -0.5, -0.5 }, {0, 255, 0}}, {{ +0.5, -0.5, -0.5 }, {0, 0, 255}},

    //    {{ -0.5, +0.5, -0.5 }, {255, 0, 0}}, {{ -0.5, +0.5, +0.5 }, {0, 255, 0}}, {{ +0.5, +0.5, +0.5 }, {0, 0, 255}},
    //    {{ +0.5, +0.5, +0.5 }, {255, 0, 0}}, {{ +0.5, +0.5, -0.5 }, {0, 255, 0}}, {{ -0.5, +0.5, -0.5 }, {0, 0, 255}},
    //    {{ -0.5, -0.5, -0.5 }, {255, 0, 0}}, {{ +0.5, -0.5, +0.5 }, {0, 255, 0}}, {{ -0.5, -0.5, +0.5 }, {0, 0, 255}},
    //    {{ +0.5, -0.5, +0.5 }, {255, 0, 0}}, {{ -0.5, -0.5, -0.5 }, {0, 255, 0}}, {{ +0.5, -0.5, -0.5 }, {0, 0, 255}},

    //    {{ +0.5, -0.5, -0.5 }, {255, 0, 0}}, {{ +0.5, -0.5, +0.5 }, {0, 255, 0}}, {{ +0.5, +0.5, +0.5 }, {0, 0, 255}},
    //    {{ +0.5, +0.5, +0.5 }, {255, 0, 0}}, {{ +0.5, +0.5, -0.5 }, {0, 255, 0}}, {{ +0.5, -0.5, -0.5 }, {0, 0, 255}},
    //    {{ -0.5, -0.5, -0.5 }, {255, 0, 0}}, {{ -0.5, +0.5, +0.5 }, {0, 255, 0}}, {{ -0.5, -0.5, +0.5 }, {0, 0, 255}},
    //    {{ -0.5, +0.5, +0.5 }, {255, 0, 0}}, {{ -0.5, -0.5, -0.5 }, {0, 255, 0}}, {{ -0.5, +0.5, -0.5 }, {0, 0, 255}}
    //};

    GLfloat vertices[] = {
        // Front face (Z+)
        -0.5f, -0.5f,  0.5f, // 0
        0.5f, -0.5f,  0.5f, // 1
        0.5f,  0.5f,  0.5f, // 2
        -0.5f,  0.5f,  0.5f, // 3

        // Back face (Z-)
        -0.5f, -0.5f, -0.5f, // 4
        -0.5f,  0.5f, -0.5f, // 5
        0.5f,  0.5f, -0.5f, // 6
        0.5f, -0.5f, -0.5f, // 7

        // Top face (Y+)
        -0.5f,  0.5f, -0.5f, // 8
        -0.5f,  0.5f,  0.5f, // 9
        0.5f,  0.5f,  0.5f, //10
        0.5f,  0.5f, -0.5f, //11

        // Bottom face (Y-)
        -0.5f, -0.5f, -0.5f, //12
        0.5f, -0.5f, -0.5f, //13
        0.5f, -0.5f,  0.5f, //14
        -0.5f, -0.5f,  0.5f, //15

        // Right face (X+)
        0.5f, -0.5f, -0.5f, //16
        0.5f,  0.5f, -0.5f, //17
        0.5f,  0.5f,  0.5f, //18
        0.5f, -0.5f,  0.5f, //19

        // Left face (X-)
        -0.5f, -0.5f, -0.5f, //20
        -0.5f, -0.5f,  0.5f, //21
        -0.5f,  0.5f,  0.5f, //22
        -0.5f,  0.5f, -0.5f, //23
    };

    GLfloat normals[] = {
        // Front face (Z+)
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,
         0.0f,  0.0f,  1.0f,

        // Back face (Z-)
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,
         0.0f,  0.0f, -1.0f,

        // Top face (Y+)
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,
         0.0f,  1.0f,  0.0f,

        // Bottom face (Y-)
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,
         0.0f, -1.0f,  0.0f,

        // Right face (X+)
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,
         1.0f,  0.0f,  0.0f,

        // Left face (X-)
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
        -1.0f,  0.0f,  0.0f,
    };

    GLubyte baseColors[8][3] = {
        255,   0,   0,  // Red
          0, 255,   0,  // Green
          0,   0, 255,  // Blue
        255, 255,   0,  // Yellow
          0, 255, 255,  // Biruza
        255,   0, 255,  // Violet
        255, 255, 255,  // White
        122, 122,   0   // Dark yellow, right?
    };

    GLubyte colors[24 * 3];
    for (int face = 0; face < 6; ++face)
        for (int v = 0; v < 4; ++v)
            for (int i = 0; i < 3; ++i)
                colors[(face * 4 + i) * 3 + i] = baseColors[face][i];

    GLuint indices[] = {
        // Front face
        0,1,2,    0,2,3,
        // Back face
        4,5,6,    4,6,7,
        // Top face
        8,9,10,    8,10,11,
        // Bottom face
        12,13,14,  12,14,15,
        // Right face
        16,17,18,  16,18,19,
        // Left face
        20,21,22,  20,22,23
    };

#pragma region All Old Code
//    glm::mat4 rot_mat1 = get_rotation_matrix(30, 1);
//    glm::mat4 rot_mat2 = get_rotation_matrix(30, 0);
//    glm::mat4 model = rot_mat2 * rot_mat1;
//    //for (int i = 0; i < VERTICIES_NUM; i++)
//    //{
//    //    vertices[i].rotate(rot_mat2 * rot_mat1);
//    //    //vertices[i].rotate(rot_mat2);
//    //}
//
//    glm::mat4 view = get_view_matrix(glm::vec3(0, 0, 3), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
//    glm::mat4 projection = get_projection_matrix(glm::radians(45), 800.0f / 600.0f, 0.1f, 100);
//
//    glm::vec3 lightDir = glm::vec3(-0.5f, -1.0f, 0.5f);
//    lightDir = glm::normalize(lightDir);
//
//#pragma region OldCode
//    /*
//    glCreateBuffers(NumBuffers, Buffers);
//    glNamedBufferStorage(Buffers[VertexBuffer], sizeof(verticies), verticies, 0);
//    //glNamedBufferStorage(Buffers[ColorBuffer], sizeof(colors), colors, 0);
//
//    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    //glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
//    //glBindBuffer(GL_ARRAY_BUFFER, NULL);
//
//    //Used for 4.5 version of draw
//    glCreateVertexArrays(1, &VAO);
//
//    glEnableVertexArrayAttrib(VAO, Attrib_vertex);
//    glVertexArrayAttribFormat(VAO, Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 0);
//    glVertexArrayAttribBinding(VAO, Attrib_vertex, 0);
//    glVertexArrayVertexBuffer(VAO, 0, Buffers[VertexBuffer], 0, sizeof(GLfloat) * 3);
//
//    //glEnableVertexArrayAttrib(VAO, Attrib_color);
//    //glVertexArrayAttribBinding(VAO, Attrib_color, 1);
//    //glVertexArrayVertexBuffer(VAO, 1, Buffers[ColorBuffer], 0, sizeof(GLfloat) * 4);
//    //glVertexArrayAttribFormat(VAO, Attrib_color, 4, GL_FLOAT, GL_FALSE, 0);
//    */
//#pragma endregion
//    cout << vertices[6].pos[0] << " " << vertices[6].pos[1] << " " << vertices[6].pos[2] << "\n";
//    cout << vertices[5].pos[0] << " " << vertices[5].pos[1] << " " << vertices[5].pos[2] << "\n";
//#pragma region Check_Rotation_Matrix_Generator
//    glm::mat4 test = get_rotation_matrix(30, 1);
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = 0; j < 4; j++)
//            cout << test[j][i] << " ";
//        cout << "\n";
//    }
//#pragma endregion
#pragma endregion

    

#pragma region After RedBook Code

    glCreateBuffers(NumBuffers, Buffers);

    glNamedBufferStorage(Buffers[VertexBuffer], sizeof(vertices), vertices, 0);
    glNamedBufferStorage(Buffers[NormalBuffer], sizeof(normals), normals, 0);
    glNamedBufferStorage(Buffers[ColorBuffer], sizeof(colors), colors, 0);
    glNamedBufferStorage(Buffers[IndexBuffer], sizeof(indices), indices, 0);

    glCreateVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[RotatedCube]);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexBuffer]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NormalBuffer]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ColorBuffer]);
    glVertexAttribPointer(2, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[IndexBuffer]);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#pragma endregion

    check_openGL_error();
}

void init()
{
    init_shader();
    init_VBO();
    //glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glDisable(GL_CULL_FACE);

    model = glm::mat4(1.0f);
    //lookAt(cameraPos, Vector to target, up vector)
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    //perspective(FOV, Aspect Ratio, Near plane, Far plane)
    projection = glm::perspective(glm::radians(45.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f);

    lightDir = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f));
    lightDir = glm::normalize(-lightDir); //Rotate TO the sun
}

void Draw()
{
#pragma region Lecture Variant
    //glUseProgram(Program);

    //glEnableVertexAttribArray(Attrib_vertex);

    //glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    //glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, NULL);

    //glDrawArrays(GL_TRIANGLES, 0, 3);

    //glDisableVertexAttribArray(Attrib_vertex);
    //glUseProgram(0);
#pragma endregion

#pragma region OpenGL 4.5 (with Direct State Access) variant
    /*
    glUseProgram(Program);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glUseProgram(0);
    */
#pragma endregion

#pragma region After RedBook
    glUseProgram(Program);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    glBindVertexArray(VAOs[RotatedCube]);

    //glDrawArrays(GL_TRIANGLES, 0, VERTICIES_NUM);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);

#pragma endregion


    //OpenGL 4.5 (with using Direct State Access) variant

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
    init();

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
        //glm::mat4 rotationX = get_rotation_matrix(angleX, 0);
        //glm::mat4 rotationY = get_rotation_matrix(angleY, 1);
        //glm::mat4 rotationZ = get_rotation_matrix(angleZ, 2);
        //glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        Draw();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Перерисовка окна
        window.display();
    }

    release_shader();
    release_VBO();

    return 0;
}