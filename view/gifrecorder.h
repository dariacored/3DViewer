#ifndef SRC_3DVIEWER_VIEW_GIFRECORDER_H_
#define SRC_3DVIEWER_VIEW_GIFRECORDER_H_

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QScopeGuard>
#include <QTimer>
#include <QVector>

extern "C" {
#include <gif_lib.h>
}
namespace viewer {
class GifRecorder : public QObject {
  Q_OBJECT
 public:
  explicit GifRecorder(QObject *parent = nullptr);
  ~GifRecorder();

  void startRecording(QWidget *widget, const QString &fileName, int width,
                      int height, int fps, int durationMs);
  void stopRecording();

 private slots:
  void captureFrame();

 private:
  void writeGif();

  QWidget *targetWidget_;
  QString outputFileName_;
  QTimer *timer_;
  QVector<QImage> frames_;
  int frameWidth_;
  int frameHeight_;
  int frameIntervalMs_;
  int totalDurationMs_;
  int frameCount_;
};
}  // namespace viewer

#endif  // SRC_3DVIEWER_VIEW_GIFRECORDER_H_