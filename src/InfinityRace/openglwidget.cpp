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
  skybox->invDiag *= 5;

  aircraft = std::make_shared<Model>(this);
  aircraft->shaderIndex = 6;
  aircraft->readOFFFile(":/models/models/aircraft.off");
  aircraft->loadTexture(":/textures/textures/camo-army.png");
  aircraft->rotationMatrix.rotate(180, 0, 1, 0);
  aircraft->invDiag *= 0.5f;

  sandclock = std::make_shared<Model>(this);
  sandclock->shaderIndex = 3;
  sandclock->readOFFFile(":/models/models/sandclock.off");
  sandclock->zoom = 0.1;
  sandclock->invDiag *= 0.05;
  sandclock->translationVector = QVector3D(0, 0, -1.f);

  connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer.start(0);
}

void OpenGLWidget::paintGL() {
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // skybox
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

  // aircraft

  if (!aircraft)
    return;

  shaderProgramID = aircraft->shaderProgram[aircraft->shaderIndex];

  ambientProduct = light.ambient * aircraft->material.ambient;
  diffuseProduct = light.diffuse * aircraft->material.diffuse;
  specularProduct = light.specular * aircraft->material.specular;

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
  glUniform1f(locShininess, aircraft->material.shininess);

  aircraft->drawModel(false);

  // sandclock

  if (!sandclock)
    return;

  shaderProgramID = sandclock->shaderProgram[sandclock->shaderIndex];

  ambientProduct = light.ambient * sandclock->material.ambient;
  diffuseProduct = light.diffuse * sandclock->material.diffuse;
  specularProduct = light.specular * sandclock->material.specular;

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
  glUniform1f(locShininess, sandclock->material.shininess);

  sandclock->drawModel(false);
}

void OpenGLWidget::resizeGL(int width, int height) {
  camera.resizeViewport(width, height);

  update();
}

void OpenGLWidget::animate() {
  if (sandclock->translationVector.z() > 2)
    sandclock->translationVector.setZ(-5);
  else
    sandclock->translationVector += QVector3D(0, 0, 0.01f);

  update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event) {}

void OpenGLWidget::mousePressEvent(QMouseEvent* event) {}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {}

// Strong focus is required
void OpenGLWidget::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Escape) {
    QApplication::quit();
  }
  if (event->key() == Qt::Key_Left) {
    aircraft->translationVector += QVector3D(-0.5f, 0, 0);
    aircraft->rotationMatrix.rotate(10, 0, 0, 1);
    camera.eye.setX(camera.eye.x() - 0.5f);
    // camera.center.setX(camera.center.x() - 0.5f);
    camera.computeViewMatrix();

    qDebug("left");
  }
  if (event->key() == Qt::Key_Right) {
    aircraft->translationVector += QVector3D(0.5f, 0, 0);
    aircraft->rotationMatrix.rotate(10, 0, 0, -1);
    camera.eye.setX(camera.eye.x() + 0.5f);
    // camera.center.setX(camera.center.x() + 0.5f);
    camera.computeViewMatrix();

    qDebug("right");
  }
  if (event->key() == Qt::Key_Up) {
    aircraft->translationVector += QVector3D(0, 0.5f, 0);
    // aircraft->rotationMatrix.rotate(10, 0, 0, -1);
    camera.eye.setY(camera.eye.y() + 0.5f);
    // camera.center.setX(camera.center.x() + 0.5f);
    camera.computeViewMatrix();

    qDebug("right");
  }
  if (event->key() == Qt::Key_Down) {
    aircraft->translationVector += QVector3D(0, -0.5f, 0);
    // aircraft->rotationMatrix.rotate(10, 0, 0, -1);
    camera.eye.setY(camera.eye.y() - 0.5f);
    // camera.center.setX(camera.center.x() + 0.5f);
    camera.computeViewMatrix();

    qDebug("right");
  }
}
