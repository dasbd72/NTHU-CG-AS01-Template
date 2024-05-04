#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <math.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Matrices.h"
#include "Vectors.h"
#include "textfile.h"
#include "utils.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

using namespace std;

// Default window size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

bool mouse_pressed = false;
int starting_press_x = -1;
int starting_press_y = -1;

enum TransMode {
    GeoTranslation = 0,
    GeoRotation = 1,
    GeoScaling = 2,
    LightEdit = 3,
    ShininessEdit = 4,
};

struct Uniform {
    GLint iLocMVP;
};
Uniform uniform;

vector<string> filenames;  // .obj filename list

struct PhongMaterial {
    Vector3 Ka;
    Vector3 Kd;
    Vector3 Ks;
};

typedef struct
{
    GLuint vao;
    GLuint vbo;
    GLuint vboTex;
    GLuint ebo;
    GLuint p_color;
    int vertex_count;
    GLuint p_normal;
    PhongMaterial material;
    int indexCount;
    GLuint m_texture;
} Shape;

struct model {
    Vector3 position = Vector3(0, 0, 0);
    Vector3 scale = Vector3(1, 1, 1);
    Vector3 rotation = Vector3(0, 0, 0);  // Euler form

    vector<Shape> shapes;
};
vector<model> models;

struct camera {
    Vector3 position;
    Vector3 center;
    Vector3 up_vector;
};
camera main_camera;

struct project_setting {
    GLfloat nearClip, farClip;
    GLfloat fovy;
    GLfloat aspect;
    GLfloat left, right, top, bottom;
};
project_setting proj;

TransMode cur_trans_mode = GeoTranslation;

Matrix4 view_matrix;
Matrix4 project_matrix;

int cur_idx = 0;  // represent which model should be rendered now

Matrix4 translate(Vector3 vec);
Matrix4 scaling(Vector3 vec);
Matrix4 rotateX(GLfloat val);
Matrix4 rotateY(GLfloat val);
Matrix4 rotateZ(GLfloat val);
Matrix4 rotate(Vector3 vec);
void setViewingMatrix();
void setPerspective();
void setGLMatrix(GLfloat* glm, Matrix4& m);
void changeSize(GLFWwindow* window, int width, int height);
void renderScene(void);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void setShaders();
void loadModels(string model_path);
void initParameter();
void setupRC();

// [TODO] given a translation vector then output a Matrix4 (Translation Matrix)
Matrix4 translate(Vector3 vec) {
    Matrix4 mat;

    /*
    mat = Matrix4(
            ...
    );
    */

    return mat;
}

// [TODO] given a scaling vector then output a Matrix4 (Scaling Matrix)
Matrix4 scaling(Vector3 vec) {
    Matrix4 mat;

    /*
    mat = Matrix4(
            ...
    );
    */

    return mat;
}

// [TODO] given a float value then ouput a rotation matrix alone axis-X (rotate alone axis-X)
Matrix4 rotateX(GLfloat val) {
    Matrix4 mat;

    /*
    mat = Matrix4(
            ...
    );
    */

    return mat;
}

// [TODO] given a float value then ouput a rotation matrix alone axis-Y (rotate alone axis-Y)
Matrix4 rotateY(GLfloat val) {
    Matrix4 mat;

    /*
    mat = Matrix4(
            ...
    );
    */

    return mat;
}

// [TODO] given a float value then ouput a rotation matrix alone axis-Z (rotate alone axis-Z)
Matrix4 rotateZ(GLfloat val) {
    Matrix4 mat;

    /*
    mat = Matrix4(
            ...
    );
    */

    return mat;
}

Matrix4 rotate(Vector3 vec) {
    return rotateX(vec.x) * rotateY(vec.y) * rotateZ(vec.z);
}

// [TODO] compute viewing matrix accroding to the setting of main_camera
void setViewingMatrix() {
    // view_matrix[...] = ...
}

// [TODO] compute persepective projection matrix
void setPerspective() {
    // GLfloat f = ...
    // project_matrix [...] = ...
}

void setGLMatrix(GLfloat* glm, Matrix4& m) {
    glm[0] = m[0];
    glm[4] = m[1];
    glm[8] = m[2];
    glm[12] = m[3];
    glm[1] = m[4];
    glm[5] = m[5];
    glm[9] = m[6];
    glm[13] = m[7];
    glm[2] = m[8];
    glm[6] = m[9];
    glm[10] = m[10];
    glm[14] = m[11];
    glm[3] = m[12];
    glm[7] = m[13];
    glm[11] = m[14];
    glm[15] = m[15];
}

// Vertex buffers
GLuint VAO, VBO;

// Call back function for window reshape
void changeSize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // [TODO] change your aspect ratio
}

// Render function for display rendering
void renderScene(void) {
    // clear canvas
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    Matrix4 T, R, S;
    // [TODO] update translation, rotation and scaling

    Matrix4 MVP;
    GLfloat mvp[16];

    // [TODO] multiply all the matrix
    // row-major ---> column-major
    setGLMatrix(mvp, MVP);

    // use uniform to send mvp to vertex shader
    glUniformMatrix4fv(uniform.iLocMVP, 1, GL_FALSE, mvp);
    for (int i = 0; i < models[cur_idx].shapes.size(); i++) {
        // set glViewport and draw twice ...
        glBindVertexArray(models[cur_idx].shapes[i].vao);
        glDrawArrays(GL_TRIANGLES, 0, models[cur_idx].shapes[i].vertex_count);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // [TODO] Call back function for keyboard
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // [TODO] scroll up positive, otherwise it would be negtive
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // [TODO] mouse press callback function
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    // [TODO] cursor position callback function
}

void setShaders() {
    GLuint v, f, p;
    char* vs = NULL;
    char* fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead("shader.vs");
    fs = textFileRead("shader.fs");

    glShaderSource(v, 1, (const GLchar**)&vs, NULL);
    glShaderSource(f, 1, (const GLchar**)&fs, NULL);

    free(vs);
    free(fs);

    GLint success;
    char infoLog[1000];
    // compile vertex shader
    glCompileShader(v);
    // check for shader compile errors
    glGetShaderiv(v, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(v, 1000, NULL, infoLog);
        std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n"
                  << infoLog << std::endl;
    }

    // compile fragment shader
    glCompileShader(f);
    // check for shader compile errors
    glGetShaderiv(f, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(f, 1000, NULL, infoLog);
        std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n"
                  << infoLog << std::endl;
    }

    // create program object
    p = glCreateProgram();

    // attach shaders to program object
    glAttachShader(p, f);
    glAttachShader(p, v);

    // link program
    glLinkProgram(p);
    // check for linking errors
    glGetProgramiv(p, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(p, 1000, NULL, infoLog);
        std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(v);
    glDeleteShader(f);

    uniform.iLocMVP = glGetUniformLocation(p, "mvp");

    if (success)
        glUseProgram(p);
    else {
        system("pause");
        exit(123);
    }
}

string getBaseDir(const string& filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

void loadModels(string model_path) {
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;
    tinyobj::attrib_t attrib;
    vector<GLfloat> vertices;
    vector<GLfloat> colors;
    vector<GLfloat> normals;

    string err;
    string warn;

    string base_dir = getBaseDir(model_path);  // handle .mtl with relative path

#ifdef _WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str(), base_dir.c_str());

    if (!warn.empty()) {
        cout << warn << std::endl;
    }

    if (!err.empty()) {
        cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    cout << "Load Models Success ! Shapes size " << shapes.size() << " Material size " << materials.size() << endl;
    model tmp_model;

    vector<PhongMaterial> allMaterial;
    for (int i = 0; i < materials.size(); i++) {
        PhongMaterial material;
        material.Ka = Vector3(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
        material.Kd = Vector3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
        material.Ks = Vector3(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
        allMaterial.push_back(material);
    }

    for (int i = 0; i < shapes.size(); i++) {
        vertices.clear();
        colors.clear();
        normals.clear();
        normalization(&attrib, vertices, colors, normals, &shapes[i]);

        Shape tmp_shape;
        glGenVertexArrays(1, &tmp_shape.vao);
        glBindVertexArray(tmp_shape.vao);

        glGenBuffers(1, &tmp_shape.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, tmp_shape.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GL_FLOAT), &vertices.at(0), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        tmp_shape.vertex_count = vertices.size() / 3;

        glGenBuffers(1, &tmp_shape.p_color);
        glBindBuffer(GL_ARRAY_BUFFER, tmp_shape.p_color);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GL_FLOAT), &colors.at(0), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &tmp_shape.p_normal);
        glBindBuffer(GL_ARRAY_BUFFER, tmp_shape.p_normal);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GL_FLOAT), &normals.at(0), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // not support per face material, use material of first face
        if (allMaterial.size() > 0)
            tmp_shape.material = allMaterial[shapes[i].mesh.material_ids[0]];
        tmp_model.shapes.push_back(tmp_shape);
    }
    shapes.clear();
    materials.clear();
    models.push_back(tmp_model);
}

void initParameter() {
    // [TODO] Setup some parameters if you need
    proj.left = -1;
    proj.right = 1;
    proj.top = 1;
    proj.bottom = -1;
    proj.nearClip = 0.001;
    proj.farClip = 100.0;
    proj.fovy = 80;
    proj.aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    main_camera.position = Vector3(0.0f, 0.0f, 2.0f);
    main_camera.center = Vector3(0.0f, 0.0f, 0.0f);
    main_camera.up_vector = Vector3(0.0f, 1.0f, 0.0f);

    setViewingMatrix();
    setPerspective();  // set default projection matrix as perspective matrix
}

void setupRC() {
    // setup shaders
    setShaders();
    initParameter();

    // OpenGL States and Values
    glClearColor(0.2, 0.2, 0.2, 1.0);
    vector<string> model_list{"../NormalModels/bunny5KN.obj", "../NormalModels/dragon10KN.obj", "../NormalModels/lucy25KN.obj", "../NormalModels/teapot4KN.obj", "../NormalModels/dolphinN.obj"};
    // [TODO] Load five model at here
    loadModels(model_list[cur_idx]);
}

int main(int argc, char** argv) {
    // initial glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // fix compilation on OS X
#endif

    // create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Student ID HW2", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // load OpenGL function pointer
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // register glfw callback functions
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    glfwSetFramebufferSizeCallback(window, changeSize);
    glEnable(GL_DEPTH_TEST);
    // Setup render context
    setupRC();

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // render
        renderScene();

        // swap buffer from back to front
        glfwSwapBuffers(window);

        // Poll input event
        glfwPollEvents();
    }

    // just for compatibiliy purposes
    return 0;
}
