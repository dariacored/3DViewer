#include "myglwidget.h"

using namespace viewer;
MyGLWidget::MyGLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      isRotating_(false),
      isMoving_(false),
      xRot_(0),
      yRot_(0),
      xMove_(0),
      yMove_(0),
      currentScale_(1.0f),
      facade_(nullptr) {
  sceneDrawer_ = new QTSceneDrawer();
  setMouseTracking(true);
  setFocusPolicy(Qt::StrongFocus);
}

MyGLWidget::~MyGLWidget() {
  makeCurrent();
  delete sceneDrawer_;
  doneCurrent();
}

void MyGLWidget::setScene(const viewer::Scene& scene) {
  currentScene_ = scene;
  update();
}

void MyGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void MyGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  updateProjection();
}

void MyGLWidget::updateProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float aspect = float(width()) / float(height());

  if (projection_style_ == PERSPECTIVE) {
    float fov = 45.0f;
    float near = 0.1f;
    float far = 100.0f;
    gluPerspective(fov, aspect, near, far);
  } else {
    float scale = 2.0f;
    glOrtho(-scale * aspect, scale * aspect, -scale, scale, -100.0f, 100.0f);
  }

  glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (projection_style_ == PERSPECTIVE) {
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
  } else {
    glTranslatef(0.0f, 0.0f, -5.0f);
  }

  glTranslatef(xMove_, yMove_, 0.0f);

  glRotatef(xRot_, 1.0f, 0.0f, 0.0f);
  glRotatef(yRot_, 0.0f, 1.0f, 0.0f);

  glScalef(currentScale_, currentScale_, currentScale_);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect = float(width()) / float(height());

  if (projection_style_ == PERSPECTIVE) {
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
  } else {
    float scale = 2.0f;
    glOrtho(-scale * aspect, scale * aspect, -scale, scale, -100.0f, 100.0f);
  }

  glMatrixMode(GL_MODELVIEW);

  if (edge_style_ == DOTTED) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  glLineWidth(edge_size_);

  if (sceneDrawer_) {
    sceneDrawer_->DrawScene(currentScene_, edge_color_);
  }
  if (vertex_style_ != INVISIBLE) {
    glPointSize(vertex_size_);

    if (vertex_style_ == CIRCLE) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }

    glBegin(GL_POINTS);
    glColor3f(vertex_color_.redF(), vertex_color_.greenF(),
              vertex_color_.blueF());
    for (auto& figure : currentScene_.GetFigures()) {
      for (auto& vertex : figure->GetVertices()) {
        ThreeDPoint p = vertex->GetPosition();
        glVertex3f(p.x, p.y, p.z);
      }
    }
    glEnd();
  }
}

QByteArray MyGLWidget::getWidgetScreenshot(const char* format, int quality) {
  QByteArray screenshot_data;
  QPixmap px = this->grab();

  QBuffer buffer(&screenshot_data);
  buffer.open(QIODevice::WriteOnly);
  px.save(&buffer, format, quality);

  return screenshot_data;
}

void MyGLWidget::setBackgroundColor(const QColor& color) {
  if (!isValid()) return;

  background_color_ = color;

  if (isVisible()) {
    makeCurrent();
    glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    doneCurrent();
  }
}

void MyGLWidget::setEdgeColor(const QColor& color) {
  edge_color_ = color;
  update();
}

QColor MyGLWidget::getEdgeColor() const { return edge_color_; }

QColor MyGLWidget::getBackgroundColor() const { return background_color_; }

void MyGLWidget::setVertexColor(const QColor& color) {
  vertex_color_ = color;
  update();
}

QColor MyGLWidget::getVertexColor() const { return vertex_color_; }
double MyGLWidget::getVertexSize() const { return vertex_size_; }
double MyGLWidget::getEdgeSize() const { return edge_size_; }
void MyGLWidget::setVertexStyle(VertexStyle style) {
  vertex_style_ = style;
  update();
}
void MyGLWidget::setEdgeStyle(EdgeStyle style) {
  edge_style_ = style;
  update();
}
MyGLWidget::VertexStyle MyGLWidget::getVertexStyle() { return vertex_style_; }
MyGLWidget::EdgeStyle MyGLWidget::getEdgeStyle() { return edge_style_; }

void MyGLWidget::setVertexSize(double size) {
  vertex_size_ = size;
  update();
}

void MyGLWidget::setEdgeSize(double size) {
  edge_size_ = size;
  update();
}

MyGLWidget::ProjectionStyle MyGLWidget::getProjectionStyle() {
  return projection_style_;
}
void MyGLWidget::setProjectionStyle(ProjectionStyle style) {
  if (projection_style_ != style) {
    projection_style_ = style;
    updateProjection();
    update();
  }
}

void MyGLWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    lastMousePos_ = event->pos();
    isRotating_ = true;
  } else if (event->button() == Qt::RightButton) {
    lastRightMousePos_ = event->pos();
    isMoving_ = true;
  }
  QOpenGLWidget::mousePressEvent(event);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if (isRotating_ && (event->buttons() & Qt::LeftButton)) {
    QPoint delta = event->pos() - lastMousePos_;

    float sensitivity = 0.5f;

    yRot_ += delta.x() * sensitivity;
    xRot_ -= delta.y() *
             sensitivity;

    xRot_ = qBound(-90.0f, xRot_, 90.0f);

    lastMousePos_ = event->pos();

    if (facade_) {
      facade_->RotateScene(xRot_, yRot_, 0);
    }
    update();
  } else if (isMoving_ && (event->buttons() & Qt::RightButton)) {
    QPoint delta = event->pos() - lastRightMousePos_;

    float sensitivity = 0.01f;

    xMove_ += delta.x() * sensitivity;
    yMove_ -= delta.y() * sensitivity;

    lastRightMousePos_ = event->pos();

    if (facade_) {
      facade_->MoveScene(xMove_, yMove_, 0);
    }
    update();
  }
  QOpenGLWidget::mousePressEvent(event);
}

void MyGLWidget::wheelEvent(QWheelEvent* event) {
  QPoint numDegrees = event->angleDelta() / 8;

  if (!numDegrees.isNull()) {
    float zoomFactor = 1.1f;
    if (numDegrees.y() > 0) {
      currentScale_ *= zoomFactor;
    } else {
      currentScale_ /= zoomFactor;
    }

    currentScale_ = qBound(0.1f, currentScale_, 10.0f);

    if (facade_) {
      facade_->ScaleScene(currentScale_);
    }
    update();
  }
  event->accept();
}