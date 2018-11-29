#include "collectable.h"

Collectable::Collectable(QOpenGLWidget* glWidget) {
  initializeOpenGLFunctions();
  m_glWidget = glWidget;
}

void Collectable::shape(Model* model) {
  Collectable::m_model = model;
}

void Collectable::viewedBy(Camera* camera) {
  Collectable::m_camera = camera;
}

void Collectable::litBy(Light* light) {
  Collectable::m_light = light;
}

void Collectable::draw() {
  m_glWidget->makeCurrent();

  GLuint shaderProgramID = m_model->shaderProgram[m_model->shaderIndex];

  QVector4D ambientProduct = m_light->ambient * m_model->material.ambient;
  QVector4D diffuseProduct = m_light->diffuse * m_model->material.diffuse;
  QVector4D specularProduct = m_light->specular * m_model->material.specular;

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
                     m_camera->projectionMatrix.data());
  glUniformMatrix4fv(locView, 1, GL_FALSE, m_camera->viewMatrix.data());
  glUniform4fv(locLightPosition, 1, &(m_light->position[0]));
  glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
  glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
  glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));
  glUniform1f(locShininess, m_model->material.shininess);

  m_model->drawModel(false, shaderProgramID);
}
