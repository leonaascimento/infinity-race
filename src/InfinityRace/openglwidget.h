#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QWidget>

#include "aircraft.h"
#include "camera.h"
#include "collectable.h"
#include "light.h"
#include "model.h"
#include "skybox.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  OpenGLWidget(QWidget* parent = nullptr);

  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

  QTimer timer;

 signals:

 public slots:
  void animate();

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  std::shared_ptr<Skybox> sky;
  std::shared_ptr<Aircraft> player;
  std::shared_ptr<Collectable> collectable;

  std::shared_ptr<Model> skybox;
  std::shared_ptr<Model> aircraft;
  std::shared_ptr<Model> sandclock;
  Light light;
  Camera camera;

  GLfloat fade;
  GLfloat aircraftBoost = 1;

  void updateScene(int);
};

#endif  // OPENGLWIDGET_H
