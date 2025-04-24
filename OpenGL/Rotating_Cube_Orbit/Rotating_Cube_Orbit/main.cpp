#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include<iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

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
GLint Attrib_color;

//GLuint VBO;
enum VAO_IDs { RotatedCube, NumVAOs };
enum Buffer_IDs { VertexBuffer, NumBuffers };
GLuint Buffers[NumBuffers];
const GLuint VERTICIES_NUM = 8;

//Used for 4.5 version of draw
GLuint VAO;

//glm::mat4 test;

GLuint VAOs[NumVAOs];

GLint Uniform_rotation;
glm::mat4 current_mat;

struct Vertex {
    GLfloat pos[3];
    GLubyte col[3];

    inline void apply_matrix(glm::mat4 matrix)
    {
        glm::vec4 res = matrix * glm::vec4(pos[0], pos[1], pos[2], 1.0f);
        pos[0] = res.x;
        pos[1] = res.y;
        pos[2] = res.z;
    }
};

glm::mat4 get_rotation_matrix(float angle_degrees, int axis = 0)
{
    float angle_radians = angle_degrees * glm::pi<float>() / 180.0f;

    float sinTheta = sin(angle_radians);
    float cosTheta = cos(angle_radians);

    glm::mat4 res = glm::mat4(1.0f);
    res[(axis + 1) % 3][(axis + 1) % 3] = cosTheta;
    res[(axis + 1) % 3][(axis + 2) % 3] = sinTheta;
    res[(axis + 2) % 3][(axis + 1) % 3] = -sinTheta;
    res[(axis + 2) % 3][(axis + 2) % 3] = cosTheta;

    return res;
}

glm::mat4 get_transporation_matrix(float x, float y, float z)
{
    return glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     x, y, z, 1.0f);
}

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

    in vec3 position;
    in vec3 clr;

    uniform mat4 rotation;

    out vec4 vertexColor;

    void main()
    {
        gl_Position = rotation * vec4(position, 1.0);
        vertexColor = vec4(clr, 1.0);
    }
)";

const char* FragShaderSource = R"(
 #version 330 core
 in vec4 vertexColor;
 out vec4 color;
 void main() 
{
    color = vertexColor;
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

    const char* attr_name = "position";
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1)
    {
        cout << "Couldn't bind attrib " << attr_name << "\n";
        return;
    }

    const char* attr_name2 = "clr";
    Attrib_color = glGetAttribLocation(Program, attr_name2);
    if (Attrib_color == -1)
    {
        cout << "Couldn't bind attrib " << attr_name2 << "\n";
        return;
    }

    const char* uniform_name = "rotation";
    Uniform_rotation = glGetUniformLocation(Program, uniform_name);
    if (Uniform_rotation == -1)
    {
        cout << "Couldn't bind uniform " << uniform_name << "\n";
        return;
    }

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

    Vertex vertices[] = {
        {{ -0.5, -0.5, -0.5 }, {255, 0, 0}},    // 0: Bottom-back-left
        {{  0.5, -0.5, -0.5 }, {0, 255, 0}},    // 1: Bottom-back-right
        {{  0.5,  0.5, -0.5 }, {0, 0, 255}},    // 2: Top-back-right
        {{ -0.5,  0.5, -0.5 }, {255, 255, 0}},  // 3: Top-back-left
        {{ -0.5, -0.5,  0.5 }, {0, 255, 255}},  // 4: Bottom-front-left
        {{  0.5, -0.5,  0.5 }, {255, 0, 255}},  // 5: Bottom-front-right
        {{  0.5,  0.5,  0.5 }, {255, 255, 255}},// 6: Top-front-right
        {{ -0.5,  0.5,  0.5 }, {122, 122, 0}},      // 7: Top-front-left
    };

    GLuint indices[] = {
        // Front face
        4, 5, 6,  6, 7, 4,
        // Back face
        0, 1, 2,  2, 3, 0,
        // Top face
        3, 2, 6,  6, 7, 3,
        // Bottom face
        0, 1, 5,  5, 4, 0,
        // Right face
        1, 5, 6,  6, 2, 1,
        // Left face
        0, 4, 7,  7, 3, 0
    };



    glm::mat4 rot_mat1 = get_rotation_matrix(30, 1);
    glm::mat4 rot_mat2 = get_rotation_matrix(30, 0);
    glm::mat4 trans_mat = get_transporation_matrix(0, 0, -1.5f);
    current_mat = rot_mat2 * rot_mat1;
    for (int i = 0; i < VERTICIES_NUM; i++)
    {
        vertices[i].apply_matrix(trans_mat * current_mat);
        //vertices[i].rotate(rot_mat2);
    }

#pragma region OldCode
    /*
    glCreateBuffers(NumBuffers, Buffers);
    glNamedBufferStorage(Buffers[VertexBuffer], sizeof(verticies), verticies, 0);
    //glNamedBufferStorage(Buffers[ColorBuffer], sizeof(colors), colors, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, NULL);

    //Used for 4.5 version of draw
    glCreateVertexArrays(1, &VAO);

    glEnableVertexArrayAttrib(VAO, Attrib_vertex);
    glVertexArrayAttribFormat(VAO, Attrib_vertex, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, Attrib_vertex, 0);
    glVertexArrayVertexBuffer(VAO, 0, Buffers[VertexBuffer], 0, sizeof(GLfloat) * 3);

    //glEnableVertexArrayAttrib(VAO, Attrib_color);
    //glVertexArrayAttribFormat(VAO, Attrib_color, 4, GL_FLOAT, GL_FALSE, 0);
    //glVertexArrayAttribBinding(VAO, Attrib_color, 1);
    //glVertexArrayVertexBuffer(VAO, 1, Buffers[ColorBuffer], 0, sizeof(GLfloat) * 4);
    */
#pragma endregion
    cout << vertices[6].pos[0] << " " << vertices[6].pos[1] << " " << vertices[6].pos[2] << "\n";
    cout << vertices[5].pos[0] << " " << vertices[5].pos[1] << " " << vertices[5].pos[2] << "\n";
#pragma region Check_Rotation_Matrix_Generator
    glm::mat4 test = trans_mat;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            cout << test[j][i] << " ";
        cout << "\n";
    }
#pragma endregion

#pragma region After RedBook Code

    glCreateBuffers(NumBuffers, Buffers);
    glNamedBufferStorage(Buffers[VertexBuffer], sizeof(vertices), vertices, 0);

    GLuint indexBuffer;
    glCreateBuffers(1, &indexBuffer);
    glNamedBufferStorage(indexBuffer, sizeof(indices), indices, 0);

    glCreateVertexArrays(NumVAOs, VAOs);

    glBindVertexArray(VAOs[RotatedCube]);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VertexBuffer]);
    glVertexAttribPointer(Attrib_vertex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(0));
    glVertexAttribPointer(Attrib_color, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), BUFFER_OFFSET(sizeof(vertices[0].pos)));
    glEnableVertexAttribArray(Attrib_vertex);
    glEnableVertexAttribArray(Attrib_color);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

#pragma endregion

    check_openGL_error();
}

void init()
{
    init_shader();
    init_VBO();
    glEnable(GL_DEPTH_TEST);
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
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML window", 7U, contextSettings);

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
                    current_mat = get_rotation_matrix(-5, 1) * current_mat;
                }
                else if (event.key.code == sf::Keyboard::D) {
                    current_mat = get_rotation_matrix(5, 1) * current_mat;
                }
                // X-axis rotation
                if (event.key.code == sf::Keyboard::W) {
                    current_mat = get_rotation_matrix(-5, 0) * current_mat;
                }
                else if (event.key.code == sf::Keyboard::S) {
                    current_mat = get_rotation_matrix(5, 0) * current_mat;
                }
                // Z-axis rotation
                if (event.key.code == sf::Keyboard::Q) {
                    current_mat = get_rotation_matrix(-5, 2) * current_mat;
                }
                else if (event.key.code == sf::Keyboard::E) {
                    current_mat = get_rotation_matrix(5, 2) * current_mat;
                }
            }
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(Program);
        glUniformMatrix4fv(Uniform_rotation, 1, GL_FALSE, &current_mat[0][0]);
        Draw();

        // Перерисовка окна
        window.display();
    }

    release_shader();
    release_VBO();

    return 0;
}