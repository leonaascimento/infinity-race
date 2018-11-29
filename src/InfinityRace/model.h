#ifndef MODEL_H
#define MODEL_H

#include <QFile>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QTextStream>
#include <QtOpenGL>

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>

#include "light.h"
#include "material.h"

class Model : public QOpenGLExtraFunctions {
 public:
  Model(QOpenGLWidget* _glWidget);
  ~Model();

  std::unique_ptr<QVector4D[]> vertices;
  std::unique_ptr<QVector3D[]> normals;
  std::unique_ptr<unsigned int[]> indices;
  std::unique_ptr<QVector2D[]> texCoords;
  std::unique_ptr<QVector4D[]> tangents;

  QOpenGLWidget* glWidget;

  unsigned int numVertices;
  unsigned int numFaces;

  void createNormals();
  void createTexCoords();
  void createTangents();
  bool collide(Model* model);

  GLuint vao = 0;

  GLuint vboVertices = 0;
  GLuint vboIndices = 0;
  GLuint vboNormals = 0;
  GLuint vboTexCoords = 0;
  GLuint vboTangents = 0;
  GLuint textureID = 0;
  GLuint textureLayerID = 0;
  GLuint textureCubeMapID = 0;

  void createVBOs();
  void destroyVBOs();

  void createShaders();
  void destroyShaders();

  void readOFFFile(const QString& fileName);
  void loadTexture(const QString& fileName);
  void loadTextureLayer(const QString& fileName);
  void loadCubeMapTexture();

  void drawModel(bool skybox, GLuint shaderProgramId);

  QMatrix4x4 modelMatrix;
  QVector3D midPoint;
  double invDiag;
  double zoom = 0;
  QMatrix4x4 rotationMatrix;
  QVector3D translationVector;

  Material material;

  unsigned int shaderIndex;
  int numShaders;

  std::vector<GLuint> shaderProgram;
};

#endif  // MODEL_H
