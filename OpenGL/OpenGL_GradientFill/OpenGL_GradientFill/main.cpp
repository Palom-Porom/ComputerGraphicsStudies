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
GLint Attrib_color;

//GLuint VBO;
enum Buffer_IDs { VertexBuffer, ColorBuffer, NumBuffers };
GLuint Buffers[NumBuffers];

//Used for 4.5 version of draw
GLuint VAO;

struct Vertex {
    GLfloat x;
    GLfloat y;
    GLfloat z;
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
    in vec3 position;
    out vec4 vertexColor;
    void main()
    {
        gl_Position = vec4(position, 1.0);
        vertexColor = gl_Position;
    }
)";

const char* FragShaderSource = R"(
 #version 330 core
 in vec4 vertexColor;
 out vec4 color;
 void main() {
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

    const char* attr_name = "position";
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1)
    {
        cout << "Couldn't bind attrib " << attr_name << "\n";
        return;
    }

    //const char* attr_name2 = "clr";
    //Attrib_color = glGetAttribLocation(Program, attr_name);
    //if (Attrib_vertex == -1)
    //{
    //    cout << "Couldn't bind attrib " << attr_name << "\n";
    //    return;
    //}

    check_openGL_error();
}

void init_VBO()
{
    Vertex verticies[] =
    {
        //{ -0.7, -0.7, 0.0 }, { -0.7, 0.0, 0.0 }, { 0.0, 0.0, 0.7 },
        //{ 0.0, 0.0, 0.7 }, { 0.0, -0.7, 0.7 }, { -0.7, -0.7, 0.0 },
        //{ 0.0, -0.7, -0.7 }, { 0.7, 0.0, -0.7 }, { 0.0, 0.7, -0.7 },
        //{ 0.7, 0.0, -0.7 }, { 0.0, -0.7, -0.7 }, { 0.7, -0.7, 0.0 },

        //{ 0.0, 0.7, -0.7 }, { -0.7, 0.0, 0.0 }, { 0.0, 0.0, 0.7 },
        //{ 0.0, 0.0, 0.7 }, { 0.7, 0.0, -0.7 }, { 0.0, 0.7, -0.7 },
        //{ 0.0, -0.7, -0.7 }, { 0.0, -0.7, 0.7 }, { -0.7, -0.7, 0.0 },
        //{ 0.0, -0.7, 0.7 }, { 0.0, -0.7, -0.7 }, { 0.7, -0.7, 0.0 },

        //{ 0.7, -0.7, 0.0 }, { 0.0, -0.7, 0.7 }, { 0.0, 0.0, 0.7 },
        //{ 0.0, 0.0, 0.7 }, { 0.7, 0.0, -0.7 }, { 0.7, -0.7, 0.0 },
        //{ 0.0, -0.7, -0.7 }, { -0.7, 0.0, 0.0 }, { -0.7, -0.7, 0.0 },
        //{ -0.7, 0.0, 0.0 }, { 0.0, -0.7, -0.7 }, { 0.0, 0.7, -0.7 }
        { -0.35, -0.7, -0.35 }, { -0.84, 0, 0.14 }, { 0.14, 0, 0.84 },
        { -0.14, 0, 0.84 }, { 0.35, -0.7, 0.35 }, { -0.35, -0.7, -0.35 },
        { 0.14, 0, -0.84 }, { 0.35, 0.7, 0.35 }, { -0.35, 0.7, -0.35 },
        { 0.35, 0.7, 0.35 }, { 0.14, 0, -0.84 }, { 0.84, 0, -0.14 },

        { -0.35, 0.7, -0.35 }, { -0.84, 0, 0.14 }, { -0.14, 0, 0.84 },
        { -0.14, 0, 0.84 }, { 0.35, 0.7, 0.35 }, { -0.35, 0.7, -0.35 },
        { 0.14, 0, -0.84 }, { 0.35, -0.7, 0 }, { -0.35, -0.7, -0.35 },
        { 0.35, -0.7, 0.35 }, { 0.14, 0, -0.84 }, { 0.84, 0, -0.14 },

        { 0.84, 0, -0.14 }, { 0.35, -0.7, 0.35 }, { -0.14, 0, 0.84 },
        { -0.14, 0, 0.84 }, { 0.35, 0.7, 0.35 }, { 0.84, 0, -0.14 },
        { 0.14, 0, -0.84 }, { -0.84, 0, 0.14 }, { -0.35, -0.7, -0.35 },
        { -0.84, 0, 0.14 }, { 0.14, 0, -0.84 }, { -0.35, 0.7, -0.35 },
    };
    //GLfloat colors[36][4] = 
    //{ 
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},

    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},

    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
    //    {1.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 0.0, 1.0, 1.0}
    //};

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
    glUseProgram(Program);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 36);

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