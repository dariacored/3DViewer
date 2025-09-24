#ifndef SRC_3DVIEWER_CONTROLLER_FACADE_H_
#define SRC_3DVIEWER_CONTROLLER_FACADE_H_

#include <QObject>

#include "../model/model.h"

namespace viewer {

class Facade : public QObject {
  Q_OBJECT
 public:
  explicit Facade(Scene *scene, BaseFileReader *fileReader = nullptr)
      : fileReader_(fileReader), scene_(scene) {
    if (!fileReader_) {
      fileReader_ = new FileReader();
    }
  }

  ~Facade() {
    if (fileReader_) {
      delete fileReader_;
    }
  }
  Scene *getScene();

  // паттерн фасад
  void LoadScene(string path, NormalizationParameters params);
  void MoveScene(double x, double y, double z);
  void RotateScene(double x, double y, double z);
  void ScaleScene(double x);
 signals:
  void sceneLoaded(const SceneInfo &info);

 public slots:
  void onLoadSceneRequested(const QString &path,
                            NormalizationParameters params);

 private:
  BaseFileReader *fileReader_;
  Scene *scene_;
};

}  // namespace viewer

#endif  // SRC_3DVIEWER_CONTROLLER_FACADE_H_