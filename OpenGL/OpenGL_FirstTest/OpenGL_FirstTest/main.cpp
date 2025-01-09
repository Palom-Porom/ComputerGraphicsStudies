#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include<iostream>

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

//GLuint VBO;
enum Buffer_IDs {ArrayBuffer, NumBuffers};
GLuint Buffers[NumBuffers];

//Used for 4.5 version of draw
GLuint VAO;

struct Vertex {
    GLfloat x;
    GLfloat y;
};

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

const char* VertexShaderSource = R"(
    #version 330 core
    in vec2 coord;
    void main()
    {
        gl_Position = vec4(coord, 0.0, 1.0);
    }
)";

const char* FragShaderSource = R"(
 #version 330 core
 out vec4 color;
 void main() {
 color = vec4(0, 1, 0, 1);
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
    glCompileShader(vShader);
    cout << "Fragment Shader is compiled!\n";
    ShaderLog(vShader);
    
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

    const char* attr_name = "coord";
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1)
    {
        cout << "Couldn't bind attrib " << attr_name << "\n";
        return;
    }

    check_openGL_error();
}

void init_VBO()
{
    Vertex triangle[3] =
    {
        {-1.0f, -1.0f},
        {0.0f, 1.0f},
        {1.0f, -1.0f}
    };

    glCreateBuffers(NumBuffers, Buffers);
    glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(triangle), triangle, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, NULL);

    //Used for 4.5 version of draw
    glCreateVertexArrays(1, &VAO);
    glEnableVertexArrayAttrib(VAO, Attrib_vertex);
    glVertexArrayAttribFormat(VAO, Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, Attrib_vertex, 0);
    glVertexArrayVertexBuffer(VAO, 0, Buffers[ArrayBuffer], 0, sizeof(float) * 2);

    check_openGL_error();
}

void init()
{
    //static const GLfloat vertices[NumVertices][2] =
    //{
    //    { -0.90, -0.90 }, // Triangle 1
    //    { 0.85, -0.90 },
    //    { -0.90, 0.85 },
    //    { 0.90, -0.85 }, // Triangle 2
    //    { 0.90, 0.90 },
    //    { -0.85, 0.90 }
    //};

    //glCreateBuffers(NumBuffers, Buffers);
    //glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);

    init_shader();
    init_VBO();
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
    glUseProgram(Program);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

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
    // Создаём окно
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML window");

    glewInit();
    init();

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
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw();

        // Перерисовка окна
        window.display();
    }

    release_shader();
    release_VBO();

    return 0;
}