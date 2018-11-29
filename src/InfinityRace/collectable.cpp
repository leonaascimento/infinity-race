#include "collectable.h"

Collectable::Collectable(QOpenGLWidget* glWidget) {
  initializeOpenGLFunctions();
  m_glWidget = glWidget;
}

void Collectable::shape(Model* model) {
  Collectable::model = model;
}

void Collectable::viewedBy(Camera* camera) {
  Collectable::camera = camera;
}

void Collectable::litBy(Light* light) {
  Collectable::light = light;
}

void Collectable::draw() {
  m_glWidget->makeCurrent();

  GLuint shaderProgramID = model->shaderProgram[model->shaderIndex];

  QVector4D ambientProduct = light->ambient * model->material.ambient;
  QVector4D diffuseProduct = light->diffuse * model->material.diffuse;
  QVector4D specularProduct = light->specular * model->material.specular;

  GLint locProjection = glGetUniformLocation(shaderProgramID, "projection");
  GLint locView = glGetUniformLocation(shaderProgramID, "view");
  GLint locLightPosition =
      glGetUniformLocation(shaderProgramID, "lightPosition");
  GLint locAmbientProduct =
      glGetUniformLocation(shaderProgramID, "ambientProduct");
  GLint locDiffuseProduct =
      glGetUniformLocation(shaderProgramID, "diffuseProduct");
  GLint locSpecularProduct =
      glGetUniformLocation(shaderProgramID, "specularProduct");
  GLint locShininess = glGetUniformLocation(shaderProgramID, "shininess");

  glUseProgram(shaderProgramID);

  glUniformMatrix4fv(locProjection, 1, GL_FALSE,
                     camera->projectionMatrix.data());
  glUniformMatrix4fv(locView, 1, GL_FALSE, camera->viewMatrix.data());
  glUniform4fv(locLightPosition, 1, &(light->position[0]));
  glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
  glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
  glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));
  glUniform1f(locShininess, model->material.shininess);

  model->drawModel(false);
}
