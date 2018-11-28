#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QWidget>

#include "camera.h"
#include "light.h"
#include "model.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  OpenGLWidget(QWidget* parent = nullptr);

  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void keyPressEvent(QKeyEvent* event);
  void wheelEvent(QWheelEvent* event);

  QTimer timer;

 signals:

 public slots:
  void animate();

 protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();

  std::shared_ptr<Model> skybox;
  std::shared_ptr<Model> player;
  Light light;
  Camera camera;

  void updateScene(int);
};

#endif  // OPENGLWIDGET_H
