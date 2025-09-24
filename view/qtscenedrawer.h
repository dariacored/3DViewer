#ifndef SRC_3DVIEWER_VIEW_QTSCENEDRAWE_H_
#define SRC_3DVIEWER_VIEW_QTSCENEDRAWE_H_

#include <QApplication>
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QOpenGLFunctions>
#include <QPixmap>
#include <QScreen>
#include <QWidget>

#include "scenedrawerbase.h"
namespace viewer {
class QTSceneDrawer : public SceneDrawerBase, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  explicit QTSceneDrawer() {};
  void DrawScene(Scene scene, const QColor& edgeColor = Qt::white) override;

  QByteArray getScreenshot(QWidget* widget, const char* format,
                           int quality = -1);
};
}  // namespace viewer

#endif  // SRC_3DVIEWER_VIEW_QTSCENEDRAWE_H_