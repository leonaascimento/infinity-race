#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
  if (!skybox)
    return;

  skybox->zoom += 0.001 * event->delta();
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();

  qDebug("OpenGL version: %s", glGetString(GL_VERSION));
  qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);

  skybox = std::make_shared<Model>(this);
  skybox->shaderIndex = 9;
  skybox->readOFFFile(":/models/models/cube.off");
  skybox->trackBall.resizeViewport(width(), height());
  skybox->invDiag *= 5;

  player = std::make_shared<Model>(this);
  player->shaderIndex = 6;
  player->readOFFFile(":/models/models/aircraft.off");
  player->loadTexture(":/textures/textures/camo-army.png");
  player->rotationMatrix.rotate(180, 0, 1, 0);
  player->trackBall.resizeViewport(width(), height());

  connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer.start(0);
}

void OpenGLWidget::paintGL() {
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!skybox)
    return;

  GLuint shaderProgramID = skybox->shaderProgram[skybox->shaderIndex];

  QVector4D ambientProduct = light.ambient * skybox->material.ambient;
  QVector4D diffuseProduct = light.diffuse * skybox->material.diffuse;
  QVector4D specularProduct = light.specular * skybox->material.specular;

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
                     camera.projectionMatrix.data());
  glUniformMatrix4fv(locView, 1, GL_FALSE, camera.viewMatrix.data());
  glUniform4fv(locLightPosition, 1, &(light.position[0]));
  glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
  glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
  glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));
  glUniform1f(locShininess, skybox->material.shininess);

  skybox->drawModel(true);

  if (!player)
    return;

  shaderProgramID = player->shaderProgram[player->shaderIndex];

  ambientProduct = light.ambient * player->material.ambient;
  diffuseProduct = light.diffuse * player->material.diffuse;
  specularProduct = light.specular * player->material.specular;

  locProjection = glGetUniformLocation(shaderProgramID, "projection");
  locView = glGetUniformLocation(shaderProgramID, "view");
  locLightPosition = glGetUniformLocation(shaderProgramID, "lightPosition");
  locAmbientProduct = glGetUniformLocation(shaderProgramID, "ambientProduct");
  locDiffuseProduct = glGetUniformLocation(shaderProgramID, "diffuseProduct");
  locSpecularProduct = glGetUniformLocation(shaderProgramID, "specularProduct");
  locShininess = glGetUniformLocation(shaderProgramID, "shininess");

  glUseProgram(shaderProgramID);

  glUniformMatrix4fv(locProjection, 1, GL_FALSE,
                     camera.projectionMatrix.data());
  glUniformMatrix4fv(locView, 1, GL_FALSE, camera.viewMatrix.data());
  glUniform4fv(locLightPosition, 1, &(light.position[0]));
  glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
  glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
  glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));
  glUniform1f(locShininess, player->material.shininess);

  player->drawModel(false);
}

void OpenGLWidget::resizeGL(int width, int height) {
  camera.resizeViewport(width, height);

  if (skybox)
    skybox->trackBall.resizeViewport(width, height);

  update();
}

void OpenGLWidget::animate() {
  update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if (!skybox)
    return;

  skybox->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {
  if (!skybox)
    return;

  if (event->button() & Qt::LeftButton)
    skybox->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (!skybox)
    return;

  if (event->button() & Qt::LeftButton)
    skybox->trackBall.mouseRelease(event->localPos());
}

// Strong focus is required
void OpenGLWidget::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    QApplication::quit();
  }
}
