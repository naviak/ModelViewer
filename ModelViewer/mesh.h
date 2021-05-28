#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_3_3_Core>

struct Vertex {
    float position[3];
    float normal[3];
    float texCoords[2];
};

struct VertexWeight {
    unsigned int vertexIndex;
    float weight;
};

struct Bone {
    QString name;
    QMatrix4x4 offsetMatrix;
    QVector<VertexWeight> weights;
};

class Mesh  {
public:
    Mesh();
    ~Mesh();
    void setup(QOpenGLFunctions_3_3_Core* gl, QOpenGLShaderProgram* program);
    void draw(QOpenGLFunctions_3_3_Core* gl);

    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<Bone*> bones;
    QVector<QOpenGLTexture*> textures;

private:
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int VIO = 0;
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QOpenGLVertexArrayObject m_vertexArray;
};

#endif // MESH_H
