#ifndef SRC_3DVIEWER_VIEW_MAINWINDOW_H_
#define SRC_3DVIEWER_VIEW_MAINWINDOW_H_
#include <QCloseEvent>
#include <QColorDialog>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QMainWindow>
#include <QMessageBox>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimer>

#include "../controller/facade.h"
#include "../model/model.h"
#include "qtscenedrawer.h"
#include "ui_mainwindow.h"
using namespace viewer;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace viewer {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void setFacade(Facade *facade);
 signals:
  void loadSceneRequested(const QString &path, NormalizationParameters params);
 public slots:
  void onSceneLoaded(const SceneInfo &info);
 private slots:
  void on_openFileButton_clicked();
  void on_chooseFileButton_clicked();
  void on_recordImageButton_clicked();
  void on_recordScreencastButton_clicked();
  void on_chooseColorBackgroundButton_clicked();
  void on_chooseColorEdgeButton_clicked();
  void on_chooseColorVertexButton_clicked();
  void on_sizeVertexSlider_valueChanged(int value);
  void on_sizeVertexSpinbox_valueChanged(double value);
  void on_sizeEdgeSlider_valueChanged(int value);
  void on_sizeEdgeSpinbox_valueChanged(double value);
  void on_lineComboBox(int index);
  void on_verticesComboBox(int index);
  void on_projectionComboBox(int index);

  void on_sliderX_valueChanged(int value);
  void on_spinBoxX_valueChanged(double arg1);
  void on_sliderY_valueChanged(int value);
  void on_spinBoxY_valueChanged(double arg1);
  void on_sliderZ_valueChanged(int value);
  void on_spinBoxZ_valueChanged(double arg1);
  void on_sliderMoveY_valueChanged(int value);
  void on_sliderMoveX_valueChanged(int value);
  void on_spinBoxMoveX_valueChanged(double value);
  void on_spinBoxMoveY_valueChanged(double value);
  void on_sliderMoveZ_valueChanged(int value);
  void on_spinBoxMoveZ_valueChanged(double value);

  void on_sliderIncrease_valueChanged(int value);

  void on_spinBoxIncrease_valueChanged(double value);

 private:
  Ui::MainWindow *ui;
  QString fileName_;
  Facade *facade_;
  double turnX_;
  double turnY_;
  double turnZ_;
  double moveX_;
  double moveY_;
  double moveZ_;
  double scale_;

 protected:
  void closeEvent(QCloseEvent *event) override;
};
}  // namespace viewer

#endif  // SRC_3DVIEWER_VIEW_MAINWINDOW_H_