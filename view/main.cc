#include <QApplication>
#include <QSettings>

#include "../controller/facade.h"
#include "mainwindow.h"
#include "qtscenedrawer.h"
using namespace viewer;

int main(int argc, char *argv[]) {
  Scene scene;
  Facade facade(&scene);
  QTSceneDrawer sceneDrawer;

  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("PetProject");
  QCoreApplication::setApplicationName("3DViewer");
  MainWindow w;
  w.setFacade(&facade);
  QObject::connect(&w, &MainWindow::loadSceneRequested, &facade,
                   &Facade::onLoadSceneRequested);
  QObject::connect(&facade, &Facade::sceneLoaded, &w,
                   &MainWindow::onSceneLoaded);
  w.show();
  return a.exec();
}