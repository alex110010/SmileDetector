#ifndef MAINWIDJ_H
#define MAINWIDJ_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QStack>
#include <QDir>
#include <stack>
#include <qmath.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/face.hpp>

#include <opencv2/face/facemark.hpp>

#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>

using namespace cv;
using namespace cv::face;
using namespace std;

namespace Ui {
class MainWidj;
}

class MainWidj : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWidj(QWidget *parent = 0);
    ~MainWidj();

    Mat applyAverageFilter(const Mat& src, int kernel);
    void bradleyThreshold(unsigned char* src, unsigned char* res, int width, int height);

private slots:
    void on_pB_downloadPict_clicked();

    void on_pB_showPict_clicked();

    void on_pB_threshold_clicked();

    void on_pB_savePict_clicked();

    void on_slider_hue_valueChanged(int value);

    void on_slider_saturation_valueChanged(int value);

    void on_slider_value_valueChanged(int value);

    void on_pB_downloadVectorOfFeatures_clicked();

    void on_pB_openWebCamera_clicked();

    void on_pB_detectSmilesOnImagesSet_clicked();

private:
    Ui::MainWidj *ui;

    QString _pictPath;
    Mat _pict;
    Mat _pictInHsv;
    Mat _medianPict;
    Mat _averagePict;
    Mat _binaryPict;
    Mat _pictAfterProcessing;
    const int _apertureSize = 3;
    bool _pictHasBeenDownloaded;

    void thresholdHsvPict(const Mat& hsvPict, int hueThresh, int saturationThresh, int valueThresh);
    bool _webCameraIsOpened;

    void detectAndDraw(Mat& img, CascadeClassifier& cascade,
                        CascadeClassifier& nestedCascade,
                        double scale, QString windowName);

    // Темновой кадр
    Mat _darkPict;
    QString _darkPictPath;

    QVector<QVector<float>> _featureVectors;
    void draw_polyline(cv::Mat& img, const dlib::full_object_detection& d, const int start, const int end, const cv::Scalar& color, bool isClosed = false);
};

#endif // MAINWIDJ_H
