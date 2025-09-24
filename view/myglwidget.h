#ifndef SRC_3DVIEWER_VIEW_MYGLWIDGET_H_
#define SRC_3DVIEWER_VIEW_MYGLWIDGET_H_

#include <GL/glu.h>

#include <QApplication>
#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QPixmap>
#include <QPoint>
#include <QScreen>
#include <QVector3D>
#include <QVector>
#include <QWheelEvent>

#include "qtscenedrawer.h"
using namespace viewer;
namespace viewer {
class QTSceneDrawer;
class Facade;
}  // namespace viewer
class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 public:
  MyGLWidget(QWidget* parent = nullptr);
  ~MyGLWidget();

  enum VertexStyle { SQUARE, CIRCLE, INVISIBLE };

  enum EdgeStyle { SOLID, DOTTED };

  enum ProjectionStyle { PERSPECTIVE, ORTHOGRAPHIC };

  void setScene(const Scene& scene);
  QByteArray getWidgetScreenshot(const char* format, int quality = -1);

  void setBackgroundColor(const QColor& color);
  void setEdgeColor(const QColor& color);
  QColor getBackgroundColor() const;
  QColor getEdgeColor() const;
  void setVertexColor(const QColor& color);
  QColor getVertexColor() const;

  void setVertexStyle(VertexStyle style);

  void setEdgeStyle(EdgeStyle style);

  void setVertexSize(double size);
  double getVertexSize() const;
  void setEdgeSize(double size);
  double getEdgeSize() const;
  VertexStyle getVertexStyle();
  EdgeStyle getEdgeStyle();

  ProjectionStyle getProjectionStyle();
  void setProjectionStyle(ProjectionStyle style);

  void updateProjection();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  QTSceneDrawer* sceneDrawer_;
  Scene currentScene_;
  QColor background_color_ = Qt::black;
  QColor edge_color_ = Qt::white;
  QColor vertex_color_ = Qt::red;
  VertexStyle vertex_style_ = CIRCLE;
  float vertex_size_ = 1.0f;
  float edge_size_ = 1.0f;
  EdgeStyle edge_style_ = SOLID;
  ProjectionStyle projection_style_ = PERSPECTIVE;

  QPoint lastMousePos_;
  float currentScale_;
  bool isRotating_;
  float xRot_, yRot_;
  bool isMoving_;
  float xMove_, yMove_;
  QPoint lastRightMousePos_;
  Facade* facade_;
};

#endif  // SRC_3DVIEWER_VIEW_MYGLWIDGET_H_