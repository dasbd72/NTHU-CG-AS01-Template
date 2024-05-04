#include "utils.h"

#include <glad/glad.h>
#include <math.h>

#include <iostream>
#include <vector>

#include "tiny_obj_loader.h"

static GLvoid Normalize(GLfloat v[3]) {
    GLfloat l;

    l = (GLfloat)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= l;
    v[1] /= l;
    v[2] /= l;
}

static GLvoid Cross(GLfloat u[3], GLfloat v[3], GLfloat n[3]) {
    n[0] = u[1] * v[2] - u[2] * v[1];
    n[1] = u[2] * v[0] - u[0] * v[2];
    n[2] = u[0] * v[1] - u[1] * v[0];
}

void normalization(tinyobj::attrib_t* attrib, std::vector<GLfloat>& vertices, std::vector<GLfloat>& colors, std::vector<GLfloat>& normals, tinyobj::shape_t* shape) {
    std::vector<float> xVector, yVector, zVector;
    float minX = 10000, maxX = -10000, minY = 10000, maxY = -10000, minZ = 10000, maxZ = -10000;

    // find out min and max value of X, Y and Z axis
    for (int i = 0; i < attrib->vertices.size(); i++) {
        // maxs = max(maxs, attrib->vertices.at(i));
        if (i % 3 == 0) {
            xVector.push_back(attrib->vertices.at(i));

            if (attrib->vertices.at(i) < minX) {
                minX = attrib->vertices.at(i);
            }

            if (attrib->vertices.at(i) > maxX) {
                maxX = attrib->vertices.at(i);
            }
        } else if (i % 3 == 1) {
            yVector.push_back(attrib->vertices.at(i));

            if (attrib->vertices.at(i) < minY) {
                minY = attrib->vertices.at(i);
            }

            if (attrib->vertices.at(i) > maxY) {
                maxY = attrib->vertices.at(i);
            }
        } else if (i % 3 == 2) {
            zVector.push_back(attrib->vertices.at(i));

            if (attrib->vertices.at(i) < minZ) {
                minZ = attrib->vertices.at(i);
            }

            if (attrib->vertices.at(i) > maxZ) {
                maxZ = attrib->vertices.at(i);
            }
        }
    }

    float offsetX = (maxX + minX) / 2;
    float offsetY = (maxY + minY) / 2;
    float offsetZ = (maxZ + minZ) / 2;

    for (int i = 0; i < attrib->vertices.size(); i++) {
        if (offsetX != 0 && i % 3 == 0) {
            attrib->vertices.at(i) = attrib->vertices.at(i) - offsetX;
        } else if (offsetY != 0 && i % 3 == 1) {
            attrib->vertices.at(i) = attrib->vertices.at(i) - offsetY;
        } else if (offsetZ != 0 && i % 3 == 2) {
            attrib->vertices.at(i) = attrib->vertices.at(i) - offsetZ;
        }
    }

    float greatestAxis = maxX - minX;
    float distanceOfYAxis = maxY - minY;
    float distanceOfZAxis = maxZ - minZ;

    if (distanceOfYAxis > greatestAxis) {
        greatestAxis = distanceOfYAxis;
    }

    if (distanceOfZAxis > greatestAxis) {
        greatestAxis = distanceOfZAxis;
    }

    float scale = greatestAxis / 2;

    for (int i = 0; i < attrib->vertices.size(); i++) {
        // std::cout << i << " = " << (double)(attrib.vertices.at(i) / greatestAxis) << std::endl;
        attrib->vertices.at(i) = attrib->vertices.at(i) / scale;
    }
    size_t index_offset = 0;
    for (size_t f = 0; f < shape->mesh.num_face_vertices.size(); f++) {
        int fv = shape->mesh.num_face_vertices[f];

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
            // access to vertex
            tinyobj::index_t idx = shape->mesh.indices[index_offset + v];
            vertices.push_back(attrib->vertices[3 * idx.vertex_index + 0]);
            vertices.push_back(attrib->vertices[3 * idx.vertex_index + 1]);
            vertices.push_back(attrib->vertices[3 * idx.vertex_index + 2]);
            // Optional: vertex colors
            colors.push_back(attrib->colors[3 * idx.vertex_index + 0]);
            colors.push_back(attrib->colors[3 * idx.vertex_index + 1]);
            colors.push_back(attrib->colors[3 * idx.vertex_index + 2]);
            // Optional: vertex normals
            if (idx.normal_index >= 0) {
                normals.push_back(attrib->normals[3 * idx.normal_index + 0]);
                normals.push_back(attrib->normals[3 * idx.normal_index + 1]);
                normals.push_back(attrib->normals[3 * idx.normal_index + 2]);
            }
        }
        index_offset += fv;
    }
}

void glPrintContextInfo(bool printExtension) {
    std::cout << "GL_VENDOR = " << (const char*)glGetString(GL_VENDOR) << std::endl;
    std::cout << "GL_RENDERER = " << (const char*)glGetString(GL_RENDERER) << std::endl;
    std::cout << "GL_VERSION = " << (const char*)glGetString(GL_VERSION) << std::endl;
    std::cout << "GL_SHADING_LANGUAGE_VERSION = " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    if (printExtension) {
        GLint numExt;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
        std::cout << "GL_EXTENSIONS =" << std::endl;
        for (GLint i = 0; i < numExt; i++) {
            std::cout << "\t" << (const char*)glGetStringi(GL_EXTENSIONS, i) << std::endl;
        }
    }
}