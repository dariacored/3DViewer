#include "gifrecorder.h"

#include "myglwidget.h"

namespace viewer {

GifRecorder::GifRecorder(QObject *parent)
    : QObject(parent), targetWidget_(nullptr), timer_(nullptr), frameCount_(0) {
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &GifRecorder::captureFrame);
}

GifRecorder::~GifRecorder() { stopRecording(); }

void GifRecorder::startRecording(QWidget *widget, const QString &fileName,
                                 int width, int height, int fps,
                                 int durationMs) {
  targetWidget_ = widget;
  outputFileName_ = fileName;
  frameWidth_ = width;
  frameHeight_ = height;
  frameIntervalMs_ = 1000 / fps;
  totalDurationMs_ = durationMs;
  frameCount_ = 0;
  frames_.clear();

  QFileInfo fileInfo(fileName);
  QDir dir(fileInfo.absolutePath());
  if (!dir.exists() && !dir.mkpath(".")) {
    return;
  }
  if (!QFileInfo(fileInfo.absolutePath()).isWritable()) {
    return;
  }

  timer_->start(frameIntervalMs_);
}

void GifRecorder::stopRecording() {
  if (timer_->isActive()) {
    timer_->stop();
    writeGif();
  }
}

void GifRecorder::captureFrame() {
  if (!targetWidget_) {
    return;
  }

  MyGLWidget *glWidget = qobject_cast<MyGLWidget *>(targetWidget_);
  if (!glWidget) {
    return;
  }

  glWidget->update();
  QByteArray screenshotData = glWidget->getWidgetScreenshot("PNG", -1);
  QImage frame;
  if (!frame.loadFromData(screenshotData)) {
    return;
  }

  frame = frame.scaled(frameWidth_, frameHeight_, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);
  if (frame.isNull()) {
    return;
  }

  frames_.append(frame);
  frameCount_++;

  if (frameCount_ * frameIntervalMs_ >= totalDurationMs_) {
    stopRecording();
  }
}

void GifRecorder::writeGif() {
  if (frames_.isEmpty()) {
    QMessageBox::warning(nullptr, "Error", "No frames captured for GIF");
    return;
  }

  MyGLWidget *glWidget = qobject_cast<MyGLWidget *>(targetWidget_);
  if (!glWidget) {
    return;
  }

  QColor bgColor = glWidget->getBackgroundColor();
  QColor edgeColor = glWidget->getEdgeColor();
  QColor vertexColor = glWidget->getVertexColor();

  int error;
  GifFileType *gif =
      EGifOpenFileName(outputFileName_.toUtf8().constData(), false, &error);
  if (!gif) {
    QMessageBox::warning(nullptr, "Error",
                         "Failed to initialize GIF: " + QString::number(error));
    return;
  }
  auto gifGuard = qScopeGuard([&] { EGifCloseFile(gif, &error); });

  ColorMapObject *globalColorMap = GifMakeMapObject(256, nullptr);
  auto globalColorMapGuard =
      qScopeGuard([&] { GifFreeMapObject(globalColorMap); });
  if (EGifPutScreenDesc(gif, frameWidth_, frameHeight_, 256, 0,
                        globalColorMap) == GIF_ERROR) {
    return;
  }

  unsigned char loopParams[3] = {0x01, 0x00, 0x00};
  if (EGifPutExtension(gif, APPLICATION_EXT_FUNC_CODE, 11, "NETSCAPE2.0") ==
          GIF_ERROR ||
      EGifPutExtension(gif, APPLICATION_EXT_FUNC_CODE, 3, loopParams) ==
          GIF_ERROR) {
    return;
  }

  for (int i = 0; i < frames_.size(); ++i) {
    QImage frame = frames_[i].convertToFormat(
        QImage::Format_Indexed8, Qt::DiffuseDither | Qt::AutoColor);

    ColorMapObject *localColorMap = GifMakeMapObject(256, nullptr);
    auto localColorMapGuard =
        qScopeGuard([&] { GifFreeMapObject(localColorMap); });

    QVector<QRgb> colorTable = frame.colorTable();
    if (colorTable.size() >= 3) {
      colorTable[0] = bgColor.rgb();
      colorTable[1] = edgeColor.rgb();
      colorTable[2] = vertexColor.rgb();
    } else {
      colorTable.resize(256);
      colorTable[0] = bgColor.rgb();
      colorTable[1] = edgeColor.rgb();
      colorTable[2] = vertexColor.rgb();
    }
    frame.setColorTable(colorTable);

    for (int j = 0; j < colorTable.size() && j < 256; ++j) {
      QRgb rgb = colorTable[j];
      localColorMap->Colors[j].Red = qRed(rgb);
      localColorMap->Colors[j].Green = qGreen(rgb);
      localColorMap->Colors[j].Blue = qBlue(rgb);
    }

    int delay = frameIntervalMs_ / 10;
    unsigned char gfxExt[4] = {0x04, static_cast<unsigned char>(delay & 0xFF),
                               static_cast<unsigned char>((delay >> 8) & 0xFF),
                               0x00};
    if (EGifPutExtension(gif, GRAPHICS_EXT_FUNC_CODE, 4, gfxExt) == GIF_ERROR) {
      continue;
    }

    if (EGifPutImageDesc(gif, 0, 0, frameWidth_, frameHeight_, false,
                         localColorMap) == GIF_ERROR) {
      continue;
    }

    for (int y = 0; y < frameHeight_; ++y) {
      const uchar *scanLine = frame.scanLine(y);
      if (EGifPutLine(gif, const_cast<GifPixelType *>(scanLine), frameWidth_) ==
          GIF_ERROR) {
        break;
      }
    }
  }

  QMessageBox::information(nullptr, "Success",
                           "GIF successfully saved to " + outputFileName_);
}

}  // namespace viewer