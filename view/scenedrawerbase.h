#ifndef SRC_3DVIEWER_VIEW_SCENEDRAWERBASE_H_
#define SRC_3DVIEWER_VIEW_SCENEDRAWERBASE_H_

#include <QObject>

#include "../controller/facade.h"

namespace viewer {
class SceneDrawerBase : public QObject {
  Q_OBJECT
 public:
  virtual void DrawScene(Scene scene, const QColor& edgeColor) = 0;
  virtual ~SceneDrawerBase() = default;
};
}  // namespace viewer

#endif  // SRC_3DVIEWER_VIEW_SCENEDRAWERBASE_H_