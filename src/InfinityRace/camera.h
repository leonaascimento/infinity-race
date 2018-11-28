#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
 public:
  Camera();

  QVector3D eye = QVector3D(0, 1, 1.5f);
  QVector3D center = QVector3D(0, 0, -0.8f);
  QVector3D up = QVector3D(0, 1, 0);

  QMatrix4x4 projectionMatrix;
  QMatrix4x4 viewMatrix;

  void computeViewMatrix();
  void resizeViewport(int width, int height);
};

#endif  // CAMERA_H
