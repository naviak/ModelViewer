#ifndef MODEL_H
#define MODEL_H

#include <QDir>
#include <QVector>
#include <QString>
#include <QFileInfo>
#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "mesh.h"

struct Node {
    ~Node();
    QString name;
    QVector<Node*> children;
    QMatrix4x4 transformation;
    QVector<unsigned int> meshes;
};

class Model {
public:
    Model(QString filename, QOpenGLFunctions_3_3_Core* context,QOpenGLShaderProgram* program);
    ~Model();
    void draw(QOpenGLFunctions_3_3_Core* context);

private:
    void processNode(QOpenGLFunctions_3_3_Core* context, aiNode* source, Node* node, const aiScene* scene, QOpenGLShaderProgram* program);
    void processMesh(aiMesh* source, Mesh* mesh, const aiScene* scene);
    void loadTextures(Mesh* mesh, aiMaterial* material, aiTextureType type);


    Node* m_rootNode;
    QFileInfo m_fileInfo;
    QVector<Mesh*> m_meshes;
};

#endif // MODEL_H
