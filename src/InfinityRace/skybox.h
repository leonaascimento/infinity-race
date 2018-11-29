#ifndef SKYBOX_H
#define SKYBOX_H

#include <QtOpenGL>

#include "camera.h"
#include "light.h"
#include "model.h"

class Skybox : protected QOpenGLExtraFunctions {
 public:
  explicit Skybox(QOpenGLWidget* glWidget);

  void shape(Model* model);
  void viewedBy(Camera* camera);
  void litBy(Light* light);
  void draw();

 private:
  QOpenGLWidget* m_glWidget;
  Model* m_model;
  Camera* m_camera;
  Light* m_light;
};

#endif  // SKYBOX_H
