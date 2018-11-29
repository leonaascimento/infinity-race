#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <QtOpenGL>

#include "camera.h"
#include "light.h"
#include "model.h"

class Collectable : protected QOpenGLExtraFunctions {
 public:
  explicit Collectable(QOpenGLWidget* glWidget);

  void shape(Model* model);
  void viewedBy(Camera* camera);
  void litBy(Light* light);
  void draw();

  Model* model;
  Camera* camera;
  Light* light;

 private:
  QOpenGLWidget* m_glWidget;
};

#endif  // COLLECTABLE_H
