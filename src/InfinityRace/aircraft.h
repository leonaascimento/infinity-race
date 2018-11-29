#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <QtOpenGL>

#include "camera.h"
#include "light.h"
#include "model.h"

class Aircraft : protected QOpenGLExtraFunctions {
 public:
  explicit Aircraft(QOpenGLWidget* glWidget);

  void shape(Model* model);
  void viewedBy(Camera* camera);
  void litBy(Light* light);
  void draw(GLfloat fade);

 private:
  QOpenGLWidget* m_glWidget;
  Model* m_model;
  Camera* m_camera;
  Light* m_light;
};

#endif  // AIRCRAFT_H
