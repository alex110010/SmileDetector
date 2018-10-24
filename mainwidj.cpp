#include "mainwidj.h"
#include "ui_mainwidj.h"

struct Conf
{
    cv::String model_path;
    double scaleFactor;
    Conf (cv::String s, double d)
    {
        model_path = s;
        scaleFactor = d;
        face_detector.load(model_path);
    }

    cv::CascadeClassifier face_detector;
};

bool myDetector(cv::InputArray image, cv::OutputArray faces, Conf *conf)
{
    cv::Mat gray;

    if (image.channels() > 1)
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    else
        gray = image.getMat().clone();

    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> faces_;
    // !!changed face_cascade to face_detector to resolve error!!
    conf->face_detector.detectMultiScale(gray, faces_, conf->scaleFactor, 2, cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    cv::Mat(faces_).copyTo(faces);

    return true;
}

MainWidj::MainWidj(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWidj)
{
    ui->setupUi(this);

    _pictPath = QString(""); // empty, not null
    _pict = Mat();
    _pictInHsv = Mat();
    _pictHasBeenDownloaded = false;

    _darkPictPath = QString(""); // empry, not null
    _darkPict = Mat();

    _medianPict = Mat();
    _averagePict = Mat();
    _binaryPict = Mat();

    _pictAfterProcessing = Mat();

    this->setWindowTitle("FACE DETECTOR");

    ui->pB_savePict->setStyleSheet("color: black; background-color: cyan");
    ui->pB_showPict->setStyleSheet("color: black; background-color: magenta");

    ui->rB_useBlueChannel->setStyleSheet("color: blue");
    ui->rB_useGreenChannel->setStyleSheet("color: green");
    ui->rB_useRedChannel->setStyleSheet("color: red");

    ui->cB_deleteDefectPixels->setHidden(1);
    ui->cB_deleteNoiseByMed->setHidden(1);
    ui->cB_deleteStars->setHidden(1);
    ui->sB_apertureSize->setHidden(1);

    _webCameraIsOpened = false;
}

MainWidj::~MainWidj()
{
    delete ui;
}

void MainWidj::on_pB_downloadPict_clicked()
{
    QString tmpPictPath = QFileDialog::getOpenFileName(this, "Выбор изображения", QApplication::applicationDirPath(), "Изображения (*.bmp *.jpg *.jpeg *.png)");
    if (tmpPictPath.isEmpty()){
        QMessageBox::information(this, "INFO", "Fail! The image path is incorrect!");
        return;
    }

    Mat tmpPict = imread(tmpPictPath.toLocal8Bit().data(), IMREAD_COLOR);
    if (!tmpPict.data)
    {
        QMessageBox::information(this, "INFO", "Fail! The image is not loaded!");
        return;
    }

    // Save picture
    _pictPath = tmpPictPath;
    ui->le_pictPath->setText(_pictPath);
    _pict = tmpPict.clone();

    // Convert to HSV
    Mat tmpPictInHsv;
    tmpPict.convertTo(tmpPictInHsv, COLOR_BGR2HSV);
    _pictInHsv = tmpPictInHsv.clone();

    // Show picture's options
    ui->le_pictNChannels->setText(QString::number(_pict.channels()));
    ui->le_pictWidth->setText(QString::number(_pict.cols));
    ui->le_pictHeight->setText(QString::number(_pict.rows));

    _pictHasBeenDownloaded = true;

    Scalar pictInHsvMeanValue = cv::mean(_pictInHsv);
    vector<Mat> hsvChannels;
    split(_pictInHsv, hsvChannels);
    vector<Mat> hsvBinChannels;

    ui->slider_hue->setValue(pictInHsvMeanValue.val[0]);
    ui->slider_saturation->setValue(pictInHsvMeanValue.val[1]);
    ui->slider_value->setValue(pictInHsvMeanValue.val[2]);

    for (int i = 0; i < hsvChannels.size(); i++)
    {
        Mat bin;
        threshold(hsvChannels[i], bin, pictInHsvMeanValue.val[i], 255, THRESH_BINARY);
        hsvBinChannels.push_back(bin);

        // Show
        QString hsvChannelName = "THRESHOLDING_HSV[" + QString::number(i) + "]";
        namedWindow(hsvChannelName.toLocal8Bit().data(), WINDOW_NORMAL);
        imshow(hsvChannelName.toLocal8Bit().data(), bin);
    }

    Mat logAnd;
    bitwise_and(hsvChannels[0], hsvChannels[1], logAnd);
    bitwise_and(logAnd, hsvChannels[2], logAnd);
    namedWindow("HSV_THRESHOLDING", WINDOW_NORMAL);
    imshow("HSV_THRESHOLDING", logAnd);

    // Detect smile
    Mat image = _pict.clone();
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor sp;
    if (ui->cB_detectLandmarks->isChecked())
    {
        QString shapePredictorPath = QCoreApplication::applicationDirPath() + "/shape_predictor_68_face_landmarks.dat";
        qDebug() << "Downloading" << shapePredictorPath;
        dlib::deserialize(shapePredictorPath.toStdString()) >> sp;
    }
    else
    {
        return;
    }

    if (image.empty()) return;

    dlib::array2d<dlib::bgr_pixel> dlib_image;
    dlib::assign_image(dlib_image, dlib::cv_image<dlib::bgr_pixel>(image));

    std::vector<dlib::rectangle> detected_faces = detector(dlib_image);
    int number_of_detected_faces = detected_faces.size();

    std::vector<dlib::full_object_detection> shapes;
    int rect_x, rect_y, rect_w, rect_h;

    Scalar color_smile(0, 0, 255);
    Scalar color_other(255, 0, 0);

    for (int i = 0; i < number_of_detected_faces; i++)
    {
        dlib::full_object_detection shape = sp(dlib_image, detected_faces[i]);
        shapes.push_back(shape);

        rect_x = detected_faces[i].left();
        rect_y = detected_faces[i].top();
        rect_w = detected_faces[i].right() - rect_x;
        rect_h = detected_faces[i].bottom() - rect_y;

        Rect face_rectangle(rect_x, rect_y, rect_w, rect_h);

        rectangle(image, face_rectangle, Scalar(0, 255, 0), 3, 8, 0);

        draw_polyline(image, shape, 0, 16, color_other);           // Jaw line
        draw_polyline(image, shape, 17, 21, color_other);          // Left eyebrow
        draw_polyline(image, shape, 22, 26, color_other);          // Right eyebrow
        draw_polyline(image, shape, 27, 30, color_other);          // Nose bridge
        draw_polyline(image, shape, 30, 35, color_other, true);    // Lower nose
        draw_polyline(image, shape, 36, 41, color_other, true);    // Left eye
        draw_polyline(image, shape, 42, 47, color_other, true);    // Right Eye
        draw_polyline(image, shape, 48, 59, color_other, true);    // Outer lip
        draw_polyline(image, shape, 60, 67, color_other, true);    // Inner lip

        Point2f left_mouth_corner(shape.part(48).x(), shape.part(48).y());  // Левый уголок рта
        Point2f right_mouth_corner(shape.part(54).x(), shape.part(54).y()); // Правый уголок рта
        Point2f topmost_lips_landmark(shape.part(51).x(), shape.part(51).y()); // Верхний уголок рта
        vector<Point2f> outer_lip_landmarks;

        outer_lip_landmarks.push_back(left_mouth_corner);
        outer_lip_landmarks.push_back(right_mouth_corner);
        outer_lip_landmarks.push_back(topmost_lips_landmark);

        line(image, left_mouth_corner, topmost_lips_landmark, Scalar(0, 0, 255));
        line(image, topmost_lips_landmark, right_mouth_corner, Scalar(0, 0, 255));

        float magic_value = ((left_mouth_corner.x - topmost_lips_landmark.x) * (right_mouth_corner.y - topmost_lips_landmark.y) -
                             (right_mouth_corner.x - topmost_lips_landmark.x) * (left_mouth_corner.y - topmost_lips_landmark.y)) /
                             qSqrt(qPow(left_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(left_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                             qSqrt(qPow(right_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(right_mouth_corner.y - topmost_lips_landmark.y, 2));

        qDebug() << magic_value;

        // RULE FOR DETECTING SMILE
        if (magic_value > 0.1 || qAbs(magic_value) < 0.1)
        {
            draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
        }
    }

    namedWindow("Facial landmarks (Press ESC to exit)", WINDOW_NORMAL);
    imshow("Facial landmarks (Press ESC to exit)", image);
}

void MainWidj::on_pB_showPict_clicked()
{
    if (!_pictHasBeenDownloaded){
        QMessageBox::information(this, "INFO", "Fail! The image is not loaded!");
        return;
    }

    imshow("Source image in RGB", _pict);
    imshow("Source image in HSV", _pictInHsv);

    if (ui->cB_deleteDefectPixels->isChecked())
    {
        if (!_darkPict.data)
        {
            QMessageBox::information(this, "INFO", "Fail! The dark picture is not loaded!");
            return;
        }

        // Split dark image to channels
        std::vector<Mat> darkChannels = {Mat(_darkPict.rows, _darkPict.cols, CV_8UC1),
                                    Mat(_darkPict.rows, _darkPict.cols, CV_8UC1),
                                    Mat(_darkPict.rows, _darkPict.cols, CV_8UC1)};
        split(_darkPict, darkChannels);

        // Thresholding of dark image
        Mat binaryDarkPict;
        double darkThresh = 0;
        threshold(darkChannels[0], binaryDarkPict, darkThresh, 255, THRESH_BINARY);

        // Perform result
        for (int  i = 0; i < _pict.rows; i++)
        {
            for (int j = 0; j < _pict.cols; j++)
            {
                if (binaryDarkPict.at<uchar>(i, j) == 255)
                {
                    _pictAfterProcessing.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
                }
            }
        }
    }

    if (ui->cB_deleteStars->isChecked())
    {
        Mat pictWithoutStars(_pict.rows + 2, _pict.cols + 2, _pict.type(), Scalar(255, 0, 0));
        for (int r = 0; r < _pict.rows; r++){
            for (int c = 0; c < _pict.cols; c++){
                pictWithoutStars.at<Vec3b>(r + 1, c + 1) = _pictAfterProcessing.at<Vec3b>(r, c);
            }
        }

        for (int row = 1; row < pictWithoutStars.rows - 1; row++){
            for (int col = 1; col < pictWithoutStars.cols - 1; col++){

                Vec3b bgr = pictWithoutStars.at<Vec3b>(row, col);
                int b = bgr.val[0];
                int g = bgr.val[1];
                int r = bgr.val[2];

                if (b > 200 && g > 200 && r > 200){ // WHITE
                    std::stack<Point> processedPts;
                    processedPts.push(Point(col, row)); // положили первую точку объекта в стек
                    pictWithoutStars.at<Vec3b>(row, col) = Vec3b(0, 0, 255); // окрасили в красный цвет

                    vector<int> ptsX;
                    vector<int> ptsY;
                    ptsX.push_back(col);
                    ptsY.push_back(row);
                    float xCoord = col;
                    float yCoord = row;

                    while(!processedPts.empty()){

                        Point lastPt = processedPts.top();
                        processedPts.pop();
                        int c = lastPt.x;
                        int r = lastPt.y;

                        Mat neighborhood(pictWithoutStars, Rect(c - 1, r - 1, 3, 3));

                        for (int i = 0; i < neighborhood.rows; i++){
                            for (int j = 0; j < neighborhood.cols; j++){
                                Vec3b neighboringPix = neighborhood.at<Vec3b>(i, j);
                                int neighboringPix_b = neighboringPix.val[0];
                                int neighboringPix_g = neighboringPix.val[1];
                                int neighboringPix_r = neighboringPix.val[2];

                                if (neighboringPix_b > 50 && neighboringPix_g > 50 && neighboringPix_r > 50){

                                    processedPts.push(Point(c - 1 + j, r - 1 + i));
                                    pictWithoutStars.at<Vec3b>(r - 1 + i, c - 1 + j) = Vec3b(0, 0, 255); // RED

                                    ptsX.push_back(c - 1 + j);
                                    ptsY.push_back(r - 1 + i);

                                    xCoord += c - 1 + j;
                                    yCoord += r - 1 + i;
                                }
                            }
                        }
                    }

                    // конец обработки стека
                    xCoord = xCoord / ptsX.size();
                    yCoord = yCoord / ptsY.size();

                    if (ptsX.size() < 5){
                        for (int i = 0; i < ptsX.size(); i++){
                            pictWithoutStars.at<Vec3b>(ptsY[i], ptsX[i]) = _pictAfterProcessing.at<Vec3b>(ptsY[i] - 1, ptsX[i] - 1); // SOURCE
                        }
                    }
                    else
                    {
                        for (int i = 0; i < ptsX.size(); i++){
                            pictWithoutStars.at<Vec3b>(ptsY[i], ptsX[i]) = Vec3b(0, 0, 0); // BLACK
                        }
                        for (int i = 0; i < 3; i++)
                        {
                            for (int j = 0; j < 3; j++)
                            {
                                pictWithoutStars.at<Vec3b>(yCoord - 1 + i, xCoord - 1 + j) = Vec3b(255, 255, 255); // WHITE
                            }
                        }
                    }

                    // очистка векторов
                    ptsX.clear();
                    ptsY.clear();
                }
                // конец if
            }
        }

        Mat finalResult(pictWithoutStars, Rect(1, 1, pictWithoutStars.cols - 2, pictWithoutStars.rows - 2));
        _pictAfterProcessing = finalResult.clone();
    }

    if (ui->cB_deleteNoiseByMed->isChecked())
    {
        int kernelSize = ui->sB_apertureSize->value();
        if (kernelSize % 2 == 0)
        {
            QMessageBox::information(this, "INFO", "Fail! The aperture size is invalid!\nThe kernel's size must be an odd number!");
            return;
        }

        std::vector<Mat> channels = {Mat(_pictAfterProcessing.rows, _pictAfterProcessing.cols, CV_8UC1),
                                    Mat(_pictAfterProcessing.rows, _pictAfterProcessing.cols, CV_8UC1),
                                    Mat(_pictAfterProcessing.rows, _pictAfterProcessing.cols, CV_8UC1)};
        split(_pictAfterProcessing, channels);

        Mat medianPict;
        medianBlur(channels[0], medianPict, kernelSize);

        // Save
        _pictAfterProcessing = medianPict.clone();
    }

    /// Итог
    /// _picAfterProcessing = _pict || _pictWithoutDefectPixels || _pictWithoutStars || (_pictWithoutDefectPixels + _pictWithoutStars)
}

void MainWidj::bradleyThreshold(unsigned char *src, unsigned char *res, int width, int height)
{
    qDebug() << __FUNCTION__;

    const int S = width/8;
    int s2 = S/2;
    const float t = (float)0.15;
    unsigned long* integral_image = 0;
    long sum=0;
    int count=0;
    int index;
    int x1, y1, x2, y2;

    // Рассчитываем интегральное изображение
    integral_image = new unsigned long [width*height*sizeof(unsigned long*)];

    for (int i = 0; i < width; i++) {
      sum = 0;
      for (int j = 0; j < height; j++) {
        index = j * width + i;
        sum += src[index];
        if (i==0)
      integral_image[index] = sum;
        else
      integral_image[index] = integral_image[index-1] + sum;
      }
    }

    // Находим границы для локальные областей
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        index = j * width + i;

        x1=i-s2;
        x2=i+s2;
        y1=j-s2;
        y2=j+s2;

        if (x1 < 0)
      x1 = 0;
        if (x2 >= width)
      x2 = width-1;
        if (y1 < 0)
      y1 = 0;
        if (y2 >= height)
      y2 = height-1;

        count = (x2-x1)*(y2-y1);

        sum = integral_image[y2*width+x2] - integral_image[y1*width+x2] -
                    integral_image[y2*width+x1] + integral_image[y1*width+x1];
        if ((long)(src[index]*count) < (long)(sum*(1.0-t)))
      res[index] = 0;
        else
      res[index] = 255;
      }
    }

    delete[] integral_image;
}

void MainWidj::on_pB_threshold_clicked()
{
    if (!_pict.data)
    {
        QMessageBox::information(this, "INFO", "Fail! The image is not loaded!");
        return;
    }

    Mat pictCopy = _pict.clone();

    // Split source image to channels
    std::vector<Mat> channels = {Mat(pictCopy.rows, pictCopy.cols, CV_8UC1),
                                Mat(pictCopy.rows, pictCopy.cols, CV_8UC1),
                                Mat(pictCopy.rows, pictCopy.cols, CV_8UC1)};
    split(pictCopy, channels);

    Mat binaryPict;
    double thresh = ui->sB_thresh->value();

    int channelIndex = -1;
    if (ui->rB_useBlueChannel->isChecked()) channelIndex = 0;
    else if (ui->rB_useGreenChannel->isChecked()) channelIndex = 1;
    else if (ui->rB_useRedChannel->isChecked()) channelIndex = 2;

    if (channelIndex != -1)
    {
        if (ui->cB_useAdaptiveThreshold->isChecked())
        {
            unsigned char* poSrcData = channels[channelIndex].data;
            unsigned char* poResData = new unsigned char[_pict.cols * _pict.rows];

            bradleyThreshold(poSrcData, poResData, _pict.cols, _pict.rows);

            Mat binaryPict = Mat(_pict.rows, _pict.cols, CV_8UC1);
            binaryPict.data = poResData;

            imshow("BINARY_IMAGE", binaryPict);

            // Save
            _binaryPict = binaryPict.clone();

            // Free memory
            delete[] poResData;
        }
        else{
            threshold(channels[channelIndex], binaryPict, thresh, 255, THRESH_BINARY);
            imshow("BINARY_IMAGE (thresh = " + QString::number(thresh).toStdString() + ")", binaryPict);

            // Save
            _binaryPict = binaryPict.clone();
        }
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Please, choose color channel!");
    }
}

void MainWidj::on_pB_savePict_clicked()
{
    if (_pictPath.isEmpty()) return;

    QStringList pictPathProcessed = _pictPath.split("/");
    QString pictSavePath = QCoreApplication::applicationDirPath() + "/trash/" + pictPathProcessed[pictPathProcessed.size() - 1];

    if (!_pictAfterProcessing.data) return;

    imwrite(pictSavePath.toLocal8Bit().constData(), _pictAfterProcessing);
    QMessageBox::information(this, "INFO", "Success! The image is successfully saved in the folder \"trash\"");
}

void MainWidj::thresholdHsvPict(const Mat &hsvPict, int hueThresh, int saturationThresh, int valueThresh)
{
    vector<Mat> hsvChannels;
    split(hsvPict, hsvChannels);
    vector<Mat> hsvBinChannels;

    vector<int> hsvThreshs;
    hsvThreshs.push_back(hueThresh);
    hsvThreshs.push_back(saturationThresh);
    hsvThreshs.push_back(valueThresh);

    for (int i = 0; i < hsvChannels.size(); i++)
    {
        Mat bin;
        threshold(hsvChannels[i], bin, hsvThreshs[i], 255, THRESH_BINARY);
        hsvBinChannels.push_back(bin);

        // Show
        QString hsvChannelName = "THRESHOLDING_HSV[" + QString::number(i) + "]";
        namedWindow(hsvChannelName.toLocal8Bit().data(), WINDOW_NORMAL);
        imshow(hsvChannelName.toLocal8Bit().data(), bin);
    }

    Mat logAnd;
    bitwise_and(hsvBinChannels[0], hsvBinChannels[1], logAnd);
    bitwise_and(logAnd, hsvBinChannels[2], logAnd);
    namedWindow("THRESHOLDING_HSV_TOTAL", WINDOW_NORMAL);
    imshow("THRESHOLDING_HSV_TOTAL", logAnd);
}

void MainWidj::on_slider_hue_valueChanged(int value)
{
    if (_pictHasBeenDownloaded)
    {
        thresholdHsvPict(_pictInHsv, ui->slider_hue->value(), ui->slider_saturation->value(), ui->slider_value->value());
    }
}

void MainWidj::on_slider_saturation_valueChanged(int value)
{
    if (_pictHasBeenDownloaded)
    {
        thresholdHsvPict(_pictInHsv, ui->slider_hue->value(), ui->slider_saturation->value(), ui->slider_value->value());
    }
}

void MainWidj::on_slider_value_valueChanged(int value)
{
    if (_pictHasBeenDownloaded)
    {
        thresholdHsvPict(_pictInHsv, ui->slider_hue->value(), ui->slider_saturation->value(), ui->slider_value->value());
    }
}

void MainWidj::on_pB_downloadVectorOfFeatures_clicked()
{
    _featureVectors.clear();

    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Choose landmarks"), QCoreApplication::applicationDirPath(), tr("*.txt"));
    if (!filenames.empty())
    {
        QStringList::const_iterator it;
        for (it = filenames.constBegin(); it != filenames.constEnd(); ++it)
        {
            if (!it->isEmpty()){
                QFile file(*it);
                if (file.open(QIODevice::ReadOnly))
                {
                    QStringList fileStrings;
                    while(!file.atEnd()) fileStrings.push_back(file.readLine());

                    QStringList featuresStrList;
                    featuresStrList = fileStrings[1].split(tr(" ")); // Разбиваем 2-ую строку пробелами

                    QVector<float> features;
                    QStringList::const_iterator it = featuresStrList.constBegin();
                    while (it != featuresStrList.constEnd())
                    {
                        bool ok;
                        float tmpFeature = (*it).toFloat(&ok);
                        if (ok) features.push_back(tmpFeature);
                        it++;
                    }

                    _featureVectors.push_back(features);

                    file.close();
                }
            }
        }
    }

    QVector<float> maxFeatureValues;
    QVector<float> minFeatureValues;
    int nFeatures = 0;
    if (!_featureVectors.empty()) nFeatures = _featureVectors[0].size();

    for (int i = 0; i < nFeatures; i++)
    {
        maxFeatureValues.push_back(0);
        minFeatureValues.push_back(0);
    }


    // Find maximums of features
    if (!maxFeatureValues.empty() || !minFeatureValues.empty())
    {
        for (int i = 0; i < _featureVectors.size(); i++)
        {
            static int nProcessedFeatureVectors = 0;

            if (nProcessedFeatureVectors == 0) // INIT
            {
                for (int j = 0; j < _featureVectors[i].size(); j++)
                {
                    maxFeatureValues[j] = _featureVectors[i][j];
                    minFeatureValues[j] = _featureVectors[i][j];
                }
            }
            else
            {
                for (int j = 0; j < _featureVectors[i].size(); j++)
                {
                    if (_featureVectors[i][j] > maxFeatureValues[j]) maxFeatureValues[j] = _featureVectors[i][j];
                    if (_featureVectors[i][j] < minFeatureValues[j]) minFeatureValues[j] = _featureVectors[i][j];
                }
            }

            nProcessedFeatureVectors++;
        }
    }

    qDebug() << minFeatureValues;
    qDebug() << maxFeatureValues;

    // Normalize maxFeatureValues
    for (int i = 0; i < _featureVectors.size(); i++)
    {
        for (int j = 0; j < _featureVectors[i].size(); j++)
        {
            _featureVectors[i][j] += (0 - minFeatureValues[j]); // shift
            _featureVectors[i][j] /= (maxFeatureValues[j] + (0 - minFeatureValues[j])); // scale
        }
    }

    qDebug() << "AFTER NORMALIZATION";

    Mat graphic = Mat::zeros(512, 68 * 10, CV_8UC3);
    int colorStep = qCeil((float)255 / (float)_featureVectors.size());
    colorStep++;

    QVector<float> meanFeatureValues;
    for (int i = 0; i < maxFeatureValues.size(); i++)
    {
        meanFeatureValues.push_back((minFeatureValues[i] + maxFeatureValues[i]) / 2 + (0 - minFeatureValues[i]));
        meanFeatureValues[i] /= (maxFeatureValues[i] + (0 - minFeatureValues[i]));
    }

    for (int i = 0; i < _featureVectors.size(); i++)
    {
        Scalar featureLineColor;
        if (colorStep + colorStep * i > 255 * 2) featureLineColor = Scalar(128, 128, colorStep + colorStep * i); // red
        else if (colorStep + colorStep * i > 255) featureLineColor = Scalar(colorStep + colorStep * i, 128, 128); // blue
        else featureLineColor = Scalar(128, colorStep + colorStep * i, 128); // green

        for (int j = 1; j < _featureVectors[i].size(); j++)
        {
            cv::line(graphic, Point((j - 1) * 10, _featureVectors[i][j - 1] * 512), Point(j * 10, _featureVectors[i][j] * 512), featureLineColor);
        }
        qDebug() << _featureVectors[i];
    }

    for (int i = 0; i < meanFeatureValues.size(); i++)
    {
        cv::line(graphic, Point((i - 1) * 10, meanFeatureValues[i] * 512), Point(i * 10, meanFeatureValues[i] * 512), Scalar(255, 255, 255));
    }

    imshow("graphic", graphic);
}

void MainWidj::on_pB_openWebCamera_clicked()
{
    VideoCapture v_capture(0);

    if (v_capture.isOpened())
    {
        _webCameraIsOpened = true;
        Mat image;

        bool useDlibFacialLandmarksDetector = ui->cB_detectLandmarks->isChecked();
        dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor sp;
        if (useDlibFacialLandmarksDetector)
        {
            QString shapePredictorPath = QCoreApplication::applicationDirPath() + "/shape_predictor_68_face_landmarks.dat";
            qDebug() << "Downloading" << shapePredictorPath;
            dlib::deserialize(shapePredictorPath.toStdString()) >> sp;
        }
        else
        {
        }

        while (v_capture.read(image))
        {
            if (image.empty())
            break;

            // Show source image
            namedWindow("Source (Press ESC to exit)", WINDOW_AUTOSIZE);
            imshow("Source (Press ESC to exit)", image);

            if (useDlibFacialLandmarksDetector)
            {
                dlib::array2d<dlib::bgr_pixel> dlib_image;
                dlib::assign_image(dlib_image, dlib::cv_image<dlib::bgr_pixel>(image));

                std::vector<dlib::rectangle> detected_faces = detector(dlib_image);
                int number_of_detected_faces = detected_faces.size();

                std::vector<dlib::full_object_detection> shapes;
                int rect_x, rect_y, rect_w, rect_h;

                Scalar color_smile(0, 0, 255);
                Scalar color_other(255, 0, 0);

                for (int i = 0; i < number_of_detected_faces; i++)
                {
                    dlib::full_object_detection shape = sp(dlib_image, detected_faces[i]);
                    shapes.push_back(shape);

                    rect_x = detected_faces[i].left();
                    rect_y = detected_faces[i].top();
                    rect_w = detected_faces[i].right() - rect_x;
                    rect_h = detected_faces[i].bottom() - rect_y;

                    Rect face_rectangle(rect_x, rect_y, rect_w, rect_h);

                    rectangle(image, face_rectangle, Scalar(0, 255, 0), 3, 8, 0);

                    draw_polyline(image, shape, 0, 16, color_other);           // Jaw line
                    draw_polyline(image, shape, 17, 21, color_other);          // Left eyebrow
                    draw_polyline(image, shape, 22, 26, color_other);          // Right eyebrow
                    draw_polyline(image, shape, 27, 30, color_other);          // Nose bridge
                    draw_polyline(image, shape, 30, 35, color_other, true);    // Lower nose
                    draw_polyline(image, shape, 36, 41, color_other, true);    // Left eye
                    draw_polyline(image, shape, 42, 47, color_other, true);    // Right Eye
                    draw_polyline(image, shape, 48, 59, color_other, true);    // Outer lip
                    draw_polyline(image, shape, 60, 67, color_other, true);    // Inner lip

                    Point2f left_mouth_corner(shape.part(48).x(), shape.part(48).y());  // Левый уголок рта
                    Point2f right_mouth_corner(shape.part(54).x(), shape.part(54).y()); // Правый уголок рта
                    Point2f topmost_lips_landmark(shape.part(51).x(), shape.part(51).y()); // Верхний уголок рта
                    vector<Point2f> outer_lip_landmarks;

                    outer_lip_landmarks.push_back(left_mouth_corner);
                    outer_lip_landmarks.push_back(right_mouth_corner);
                    outer_lip_landmarks.push_back(topmost_lips_landmark);

                    line(image, left_mouth_corner, topmost_lips_landmark, Scalar(0, 255, 0));
                    line(image, topmost_lips_landmark, right_mouth_corner, Scalar(0, 255, 0));
                    line(image, left_mouth_corner, right_mouth_corner, Scalar(0, 255, 0));

                    float magic_value = ((left_mouth_corner.x - topmost_lips_landmark.x) * (right_mouth_corner.y - topmost_lips_landmark.y) -
                                         (right_mouth_corner.x - topmost_lips_landmark.x) * (left_mouth_corner.y - topmost_lips_landmark.y)) /
                                         qSqrt(qPow(left_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(left_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                                         qSqrt(qPow(right_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(right_mouth_corner.y - topmost_lips_landmark.y, 2));
                    float magic_value_2 = ((left_mouth_corner.x - topmost_lips_landmark.x) * (left_mouth_corner.y - right_mouth_corner.y) -
                                           (left_mouth_corner.x - right_mouth_corner.x) * (left_mouth_corner.y - topmost_lips_landmark.y)) /
                                           qSqrt(qPow(left_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(left_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                                           qSqrt(qPow(left_mouth_corner.x - right_mouth_corner.x, 2) + qPow(left_mouth_corner.y - right_mouth_corner.y, 2));

                    float magic_value_3 = ((right_mouth_corner.x - topmost_lips_landmark.x) * (right_mouth_corner.y - left_mouth_corner.y) -
                                           (right_mouth_corner.x - left_mouth_corner.x) * (right_mouth_corner.y - topmost_lips_landmark.y)) /
                                           qSqrt(qPow(right_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(right_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                                           qSqrt(qPow(right_mouth_corner.x - left_mouth_corner.x, 2) + qPow(right_mouth_corner.y - left_mouth_corner.y, 2));


                    // RULE FOR DETECTING SMILE
                    /*
                    if (magic_value > 0.1 || qAbs(magic_value) < 0.1)
                    {
                        draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
                    }
                    */

                    /*
                    if (qAbs(magic_value_2) < 0.07)
                    {
                        draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
                    }
                    */

                    if (qAbs(magic_value_3) < 0.1 && qAbs(magic_value_2) < 0.1)
                    {
                        draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
                    }
                }

                imshow("Facial landmarks (Press ESC to exit)", image);
            }
            else
            {
                // PreDefined trained XML classifiers with facial features
                CascadeClassifier cascade, nestedCascade;
                double scale = 1;

                // Load classifiers from "opencv/data/haarcascades" directory
                QString pathToHaarascadeXml = QCoreApplication::applicationDirPath() + "/haarcascades/haarcascade_frontalface_alt.xml";
                QString pathToNestedHaarcascadeXml = QCoreApplication::applicationDirPath() + "/haarcascades/haarcascade_smile.xml";
                cascade.load(pathToHaarascadeXml.toStdString());
                nestedCascade.load(pathToNestedHaarcascadeXml.toStdString());

                if(!cascade.load(pathToHaarascadeXml.toStdString()) || !nestedCascade.load(pathToNestedHaarcascadeXml.toStdString()))
                {
                  QMessageBox::warning(this, "Warning", "Error Loading XML file");
                  break;
                }

                detectAndDraw( image, cascade, nestedCascade, scale, "Source (Press ESC to exit)");
            }

            int key = waitKey(30);
            if (key == 27)
            {
                break;
            }
        }
    }

    v_capture.release();
    destroyAllWindows();
    _webCameraIsOpened = false;
}

void MainWidj::draw_polyline(cv::Mat& img, const dlib::full_object_detection& d, const int start, const int end, const cv::Scalar& color, bool isClosed)
{
    std::vector <cv::Point> points;
    for (int i = start; i <= end; ++i)
    {
        points.push_back(cv::Point(d.part(i).x(), d.part(i).y()));
    }
    cv::polylines(img, points, isClosed, color, 1, 16);
}

void MainWidj::detectAndDraw( Mat& img, CascadeClassifier& cascade,
                    CascadeClassifier& nestedCascade,
                    double scale, QString windowName)
{
    qDebug() << __FUNCTION__;

    vector<Rect> faces;
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
    double fx = 1 / scale;

    // Resize the Grayscale Image
    cv::resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );

    // Detect faces of different sizes using cascade classifier
    cascade.detectMultiScale( smallImg, faces, 1.1,
                            2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    // Draw circles around the faces
    for ( int i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = Scalar(0, 255, 0); // Color for Drawing tool
        int radius;

        double aspect_ratio = (double) r.width / r.height;
        if ( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width * 0.5) * scale);
            center.y = cvRound((r.y + r.height * 0.5) * scale);
            radius = cvRound((r.width + r.height) * 0.25 * scale);
            rectangle( img, Rect(r.x * scale, r.y * scale, r.width * scale, r.height * scale), color, 3, 8, 0);

            // Execute sobel filter
            Mat kernel(3, 3, CV_32FC1);
            kernel.at<float>(0) = -3;
            kernel.at<float>(1) = -10;
            kernel.at<float>(2) = -3;

            kernel.at<float>(3) = 0;
            kernel.at<float>(4) = 0;
            kernel.at<float>(5) = 0;

            kernel.at<float>(6) = 3;
            kernel.at<float>(7) = 10;
            kernel.at<float>(8) = 3;

            Mat laplaceRoi;
            filter2D(Mat(gray, r), laplaceRoi, CV_32FC1, kernel, Point(-1,-1)); // result в тонах серого

            Mat laplaceRoiNormalized;
            convertScaleAbs(laplaceRoi, laplaceRoiNormalized, 1, 0);
            cv::normalize(laplaceRoi, laplaceRoiNormalized, 255, 0, NORM_INF);

            Mat laplaceImageForShowing;
            laplaceRoiNormalized.convertTo(laplaceImageForShowing, CV_8UC1);

            imshow("LAPLACE", laplaceImageForShowing);
        }
        else
            rectangle( img, Rect(r.x * scale, r.y * scale, r.width * scale, r.height * scale), Scalar(0, 0, 255), 3, 8, 0 );

        if (!nestedCascade.empty())
        {
            Mat smallImgROI = smallImg( r );

            // Detection of eyes int the input image
            nestedCascade.detectMultiScale( smallImgROI, nestedObjects, 1.1, 2,
                                            0|CASCADE_SCALE_IMAGE, Size(30, 30) );

            // Draw circles around eyes
            for ( size_t j = 0; j < nestedObjects.size(); j++ )
            {
                Rect nr = nestedObjects[j];
                center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
                center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
                radius = cvRound((nr.width + nr.height) * 0.25 * scale);
                circle( img, center, radius, color, 3, 8, 0 );
            }
        }
    }

    // Show Processed Image with detected faces
    imshow( windowName.toStdString(), img );
}

void MainWidj::on_pB_detectSmilesOnImagesSet_clicked()
{
    QStringList images_paths = QFileDialog::getOpenFileNames(this, "Выберите изображения", QCoreApplication::applicationDirPath(), tr("*.bmp *.png *.jpeg *.jpg"));
    if (images_paths.empty()) return;

    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor sp;

    QString shapePredictorPath = QCoreApplication::applicationDirPath() + "/shape_predictor_68_face_landmarks.dat";
    qDebug() << "Downloading" << shapePredictorPath;
    dlib::deserialize(shapePredictorPath.toStdString()) >> sp;

    for (int i = 0; i < images_paths.size(); i++)
    {
        static int image_num = 0;

        Mat image = imread(images_paths[i].toLocal8Bit().data(), IMREAD_COLOR);

        dlib::array2d<dlib::bgr_pixel> dlib_image;
        dlib::assign_image(dlib_image, dlib::cv_image<dlib::bgr_pixel>(image));

        std::vector<dlib::rectangle> detected_faces = detector(dlib_image);
        int number_of_detected_faces = detected_faces.size();

        std::vector<dlib::full_object_detection> shapes;
        int rect_x, rect_y, rect_w, rect_h;

        Scalar color_smile(0, 0, 255);
        Scalar color_other(255, 0, 0);

        for (int i = 0; i < number_of_detected_faces; i++)
        {
            dlib::full_object_detection shape = sp(dlib_image, detected_faces[i]);
            shapes.push_back(shape);

            rect_x = detected_faces[i].left();
            rect_y = detected_faces[i].top();
            rect_w = detected_faces[i].right() - rect_x;
            rect_h = detected_faces[i].bottom() - rect_y;

            Rect face_rectangle(rect_x, rect_y, rect_w, rect_h);

            rectangle(image, face_rectangle, Scalar(0, 255, 0), 3, 8, 0);

            draw_polyline(image, shape, 0, 16, color_other);           // Jaw line
            draw_polyline(image, shape, 17, 21, color_other);          // Left eyebrow
            draw_polyline(image, shape, 22, 26, color_other);          // Right eyebrow
            draw_polyline(image, shape, 27, 30, color_other);          // Nose bridge
            draw_polyline(image, shape, 30, 35, color_other, true);    // Lower nose
            draw_polyline(image, shape, 36, 41, color_other, true);    // Left eye
            draw_polyline(image, shape, 42, 47, color_other, true);    // Right Eye
            draw_polyline(image, shape, 48, 59, color_other, true);    // Outer lip
            draw_polyline(image, shape, 60, 67, color_other, true);    // Inner lip

            Point2f left_mouth_corner(shape.part(48).x(), shape.part(48).y());  // Левый уголок рта
            Point2f right_mouth_corner(shape.part(54).x(), shape.part(54).y()); // Правый уголок рта
            Point2f topmost_lips_landmark(shape.part(51).x(), shape.part(51).y()); // Верхний уголок рта
            vector<Point2f> outer_lip_landmarks;

            outer_lip_landmarks.push_back(left_mouth_corner);
            outer_lip_landmarks.push_back(right_mouth_corner);
            outer_lip_landmarks.push_back(topmost_lips_landmark);

            line(image, left_mouth_corner, topmost_lips_landmark, Scalar(0, 255, 0));
            line(image, topmost_lips_landmark, right_mouth_corner, Scalar(0, 255, 0));
            line(image, left_mouth_corner, right_mouth_corner, Scalar(0, 255, 0));

            float magic_value = ((left_mouth_corner.x - topmost_lips_landmark.x) * (right_mouth_corner.y - topmost_lips_landmark.y) -
                                 (right_mouth_corner.x - topmost_lips_landmark.x) * (left_mouth_corner.y - topmost_lips_landmark.y)) /
                                 qSqrt(qPow(left_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(left_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                                 qSqrt(qPow(right_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(right_mouth_corner.y - topmost_lips_landmark.y, 2));
            float magic_value_2 = ((left_mouth_corner.x - topmost_lips_landmark.x) * (left_mouth_corner.y - right_mouth_corner.y) -
                                   (left_mouth_corner.x - right_mouth_corner.x) * (left_mouth_corner.y - topmost_lips_landmark.y)) /
                                   qSqrt(qPow(left_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(left_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                                   qSqrt(qPow(left_mouth_corner.x - right_mouth_corner.x, 2) + qPow(left_mouth_corner.y - right_mouth_corner.y, 2));

            float magic_value_3 = ((right_mouth_corner.x - topmost_lips_landmark.x) * (right_mouth_corner.y - left_mouth_corner.y) -
                                   (right_mouth_corner.x - left_mouth_corner.x) * (right_mouth_corner.y - topmost_lips_landmark.y)) /
                                   qSqrt(qPow(right_mouth_corner.x - topmost_lips_landmark.x, 2) + qPow(right_mouth_corner.y - topmost_lips_landmark.y, 2)) /
                                   qSqrt(qPow(right_mouth_corner.x - left_mouth_corner.x, 2) + qPow(right_mouth_corner.y - left_mouth_corner.y, 2));


            // RULE FOR DETECTING SMILE
            /*
            if (magic_value > 0.1 || qAbs(magic_value) < 0.1)
            {
                draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
            }
            */

            /*
            if (qAbs(magic_value_2) < 0.07)
            {
                draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
            }
            */

            if (qAbs(magic_value_3) < 0.1 && qAbs(magic_value_2) < 0.1)
            {
                draw_polyline(image, shape, 48, 59, color_smile, true);    // Outer lip
            }
        }

        QString result_images_path = QCoreApplication::applicationDirPath() + "/result_images";
        if (!QDir(result_images_path).exists())
        {
            QDir(result_images_path).mkdir(result_images_path);
        }

        QString image_path = result_images_path + "/" + QString::number(image_num) + ".png";
        imwrite(image_path.toLocal8Bit().data(), image);

        image_num++;

        qDebug() << "image" << image_num << "have been processed";
    }
}
