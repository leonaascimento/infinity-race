#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent) {
  fade = 0;
}

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

  sky = std::make_shared<Skybox>(this);
  sky->shape(skybox.get());
  sky->viewedBy(&camera);
  sky->litBy(&light);

  player = std::make_shared<Aircraft>(this);
  player->shape(aircraft.get());
  player->viewedBy(&camera);
  player->litBy(&light);

  collectable = std::make_shared<Collectable>(this);
  collectable->shape(sandclock.get());
  collectable->viewedBy(&camera);
  collectable->litBy(&light);

  connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
  timer.start(0);
}

void OpenGLWidget::paintGL() {
  glViewport(0, 0, width(), height());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // skybox
  sky->draw();

  // aircraft
  player->draw(fade);

  // sandclock
  collectable->draw();
}

void OpenGLWidget::resizeGL(int width, int height) {
  camera.resizeViewport(width, height);

  update();
}

void OpenGLWidget::animate() {
  if (sandclock->translationVector.z() > 2)
    sandclock->translationVector.setZ(-5);
  else
    sandclock->translationVector += QVector3D(0, 0, 0.01f * aircraftBoost);

  if (aircraft->collide(sandclock.get())) {
    sandclock->translationVector.setZ(-5);
    fade -= 0.1f;

    if (fade < 0)
      fade = 0;
  } else {
    fade += 0.0005f;

    if (fade > 1)
      fade = 1;
  }

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
    camera.eye.setY(camera.eye.y() + 0.5f);
    camera.computeViewMatrix();

    qDebug("up");
  }
  if (event->key() == Qt::Key_Down) {
    aircraft->translationVector += QVector3D(0, -0.5f, 0);
    camera.eye.setY(camera.eye.y() - 0.5f);
    camera.computeViewMatrix();

    qDebug("down");
  }
  if (event->key() == Qt::Key_Space) {
    aircraftBoost = 3;
  }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Space) {
    aircraftBoost = 1;
  }
}
