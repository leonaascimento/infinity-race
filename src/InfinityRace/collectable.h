#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include <QtOpenGL>

#include "camera.h"
#include "light.h"
#include "model.h"

class Collectable : protected QOpenGLExtraFunctions {
 public:
  explicit Collectable(QOpenGLWidget* glWidget);

  void shape(Model* m_model);
  void viewedBy(Camera* m_camera);
  void litBy(Light* m_light);
  void draw();

 private:
  QOpenGLWidget* m_glWidget;
  Model* m_model;
  Camera* m_camera;
  Light* m_light;
};

#endif  // COLLECTABLE_H
