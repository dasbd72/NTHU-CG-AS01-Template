#ifndef UTILS_H_DEF
#define UTILS_H_DEF

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <vector>

#include "tiny_obj_loader.h"

static GLvoid Normalize(GLfloat v[3]);
static GLvoid Cross(GLfloat u[3], GLfloat v[3], GLfloat n[3]);
void normalization(tinyobj::attrib_t* attrib, std::vector<GLfloat>& vertices, std::vector<GLfloat>& colors, std::vector<GLfloat>& normals, tinyobj::shape_t* shape);
void glPrintContextInfo(bool printExtension);

#endif  // UTILS_H_DEF