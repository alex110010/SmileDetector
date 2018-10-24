/********************************************************************************
** Form generated from reading UI file 'mainwidj.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDJ_H
#define UI_MAINWIDJ_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidj
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *gB_thresholding;
    QGridLayout *gridLayout_4;
    QPushButton *pB_threshold;
    QGridLayout *gridLayout_5;
    QLabel *lbl_thresh;
    QSpinBox *sB_thresh;
    QGroupBox *gB_channels;
    QVBoxLayout *verticalLayout;
    QRadioButton *rB_useRedChannel;
    QRadioButton *rB_useGreenChannel;
    QRadioButton *rB_useBlueChannel;
    QCheckBox *cB_useAdaptiveThreshold;
    QGroupBox *gB_hsvValues;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QLabel *lbl_hue;
    QSlider *slider_hue;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lbl_saturation;
    QSlider *slider_saturation;
    QHBoxLayout *horizontalLayout_3;
    QLabel *lbl_value;
    QSlider *slider_value;
    QGroupBox *gB_pictProperties;
    QGridLayout *gridLayout;
    QCheckBox *cB_deleteStars;
    QLineEdit *le_pictHeight;
    QLabel *lbl_pictNChannels;
    QLabel *lbl_pictHeight;
    QLineEdit *le_pictWidth;
    QPushButton *pB_showPict;
    QCheckBox *cB_deleteDefectPixels;
    QLabel *lbl_pictWidth;
    QLineEdit *le_pictNChannels;
    QPushButton *pB_savePict;
    QCheckBox *cB_deleteNoiseByMed;
    QSpinBox *sB_apertureSize;
    QPushButton *pB_downloadPict;
    QPushButton *pB_downloadVectorOfFeatures;
    QLineEdit *le_pictPath;
    QFrame *line;
    QPushButton *pB_openWebCamera;
    QCheckBox *cB_detectLandmarks;
    QPushButton *pB_detectSmilesOnImagesSet;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWidj)
    {
        if (MainWidj->objectName().isEmpty())
            MainWidj->setObjectName(QStringLiteral("MainWidj"));
        MainWidj->resize(740, 530);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWidj->sizePolicy().hasHeightForWidth());
        MainWidj->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(MainWidj);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gB_thresholding = new QGroupBox(centralWidget);
        gB_thresholding->setObjectName(QStringLiteral("gB_thresholding"));
        gridLayout_4 = new QGridLayout(gB_thresholding);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        pB_threshold = new QPushButton(gB_thresholding);
        pB_threshold->setObjectName(QStringLiteral("pB_threshold"));

        gridLayout_4->addWidget(pB_threshold, 2, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        lbl_thresh = new QLabel(gB_thresholding);
        lbl_thresh->setObjectName(QStringLiteral("lbl_thresh"));

        gridLayout_5->addWidget(lbl_thresh, 0, 0, 1, 1);

        sB_thresh = new QSpinBox(gB_thresholding);
        sB_thresh->setObjectName(QStringLiteral("sB_thresh"));
        sB_thresh->setMinimum(0);
        sB_thresh->setMaximum(255);
        sB_thresh->setSingleStep(1);
        sB_thresh->setValue(50);

        gridLayout_5->addWidget(sB_thresh, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_5, 1, 0, 1, 1);

        gB_channels = new QGroupBox(gB_thresholding);
        gB_channels->setObjectName(QStringLiteral("gB_channels"));
        verticalLayout = new QVBoxLayout(gB_channels);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        rB_useRedChannel = new QRadioButton(gB_channels);
        rB_useRedChannel->setObjectName(QStringLiteral("rB_useRedChannel"));

        verticalLayout->addWidget(rB_useRedChannel);

        rB_useGreenChannel = new QRadioButton(gB_channels);
        rB_useGreenChannel->setObjectName(QStringLiteral("rB_useGreenChannel"));

        verticalLayout->addWidget(rB_useGreenChannel);

        rB_useBlueChannel = new QRadioButton(gB_channels);
        rB_useBlueChannel->setObjectName(QStringLiteral("rB_useBlueChannel"));

        verticalLayout->addWidget(rB_useBlueChannel);

        cB_useAdaptiveThreshold = new QCheckBox(gB_channels);
        cB_useAdaptiveThreshold->setObjectName(QStringLiteral("cB_useAdaptiveThreshold"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        cB_useAdaptiveThreshold->setFont(font);

        verticalLayout->addWidget(cB_useAdaptiveThreshold);


        gridLayout_4->addWidget(gB_channels, 0, 0, 1, 1);


        gridLayout_3->addWidget(gB_thresholding, 5, 2, 1, 1);

        gB_hsvValues = new QGroupBox(centralWidget);
        gB_hsvValues->setObjectName(QStringLiteral("gB_hsvValues"));
        gridLayout_2 = new QGridLayout(gB_hsvValues);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lbl_hue = new QLabel(gB_hsvValues);
        lbl_hue->setObjectName(QStringLiteral("lbl_hue"));

        horizontalLayout->addWidget(lbl_hue);

        slider_hue = new QSlider(gB_hsvValues);
        slider_hue->setObjectName(QStringLiteral("slider_hue"));
        slider_hue->setMaximum(255);
        slider_hue->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(slider_hue);


        gridLayout_2->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        lbl_saturation = new QLabel(gB_hsvValues);
        lbl_saturation->setObjectName(QStringLiteral("lbl_saturation"));

        horizontalLayout_2->addWidget(lbl_saturation);

        slider_saturation = new QSlider(gB_hsvValues);
        slider_saturation->setObjectName(QStringLiteral("slider_saturation"));
        slider_saturation->setMaximum(255);
        slider_saturation->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(slider_saturation);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lbl_value = new QLabel(gB_hsvValues);
        lbl_value->setObjectName(QStringLiteral("lbl_value"));

        horizontalLayout_3->addWidget(lbl_value);

        slider_value = new QSlider(gB_hsvValues);
        slider_value->setObjectName(QStringLiteral("slider_value"));
        slider_value->setMaximum(255);
        slider_value->setOrientation(Qt::Horizontal);

        horizontalLayout_3->addWidget(slider_value);


        gridLayout_2->addLayout(horizontalLayout_3, 2, 0, 1, 1);


        gridLayout_3->addWidget(gB_hsvValues, 6, 0, 1, 3);

        gB_pictProperties = new QGroupBox(centralWidget);
        gB_pictProperties->setObjectName(QStringLiteral("gB_pictProperties"));
        gridLayout = new QGridLayout(gB_pictProperties);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        cB_deleteStars = new QCheckBox(gB_pictProperties);
        cB_deleteStars->setObjectName(QStringLiteral("cB_deleteStars"));

        gridLayout->addWidget(cB_deleteStars, 7, 0, 1, 1);

        le_pictHeight = new QLineEdit(gB_pictProperties);
        le_pictHeight->setObjectName(QStringLiteral("le_pictHeight"));
        le_pictHeight->setReadOnly(true);

        gridLayout->addWidget(le_pictHeight, 2, 1, 1, 1);

        lbl_pictNChannels = new QLabel(gB_pictProperties);
        lbl_pictNChannels->setObjectName(QStringLiteral("lbl_pictNChannels"));

        gridLayout->addWidget(lbl_pictNChannels, 0, 0, 1, 1);

        lbl_pictHeight = new QLabel(gB_pictProperties);
        lbl_pictHeight->setObjectName(QStringLiteral("lbl_pictHeight"));

        gridLayout->addWidget(lbl_pictHeight, 2, 0, 1, 1);

        le_pictWidth = new QLineEdit(gB_pictProperties);
        le_pictWidth->setObjectName(QStringLiteral("le_pictWidth"));
        le_pictWidth->setReadOnly(true);

        gridLayout->addWidget(le_pictWidth, 1, 1, 1, 1);

        pB_showPict = new QPushButton(gB_pictProperties);
        pB_showPict->setObjectName(QStringLiteral("pB_showPict"));
        pB_showPict->setFont(font);

        gridLayout->addWidget(pB_showPict, 3, 0, 1, 2);

        cB_deleteDefectPixels = new QCheckBox(gB_pictProperties);
        cB_deleteDefectPixels->setObjectName(QStringLiteral("cB_deleteDefectPixels"));

        gridLayout->addWidget(cB_deleteDefectPixels, 6, 0, 1, 1);

        lbl_pictWidth = new QLabel(gB_pictProperties);
        lbl_pictWidth->setObjectName(QStringLiteral("lbl_pictWidth"));

        gridLayout->addWidget(lbl_pictWidth, 1, 0, 1, 1);

        le_pictNChannels = new QLineEdit(gB_pictProperties);
        le_pictNChannels->setObjectName(QStringLiteral("le_pictNChannels"));
        le_pictNChannels->setReadOnly(true);

        gridLayout->addWidget(le_pictNChannels, 0, 1, 1, 1);

        pB_savePict = new QPushButton(gB_pictProperties);
        pB_savePict->setObjectName(QStringLiteral("pB_savePict"));
        pB_savePict->setEnabled(false);
        pB_savePict->setFont(font);

        gridLayout->addWidget(pB_savePict, 4, 0, 1, 2);

        cB_deleteNoiseByMed = new QCheckBox(gB_pictProperties);
        cB_deleteNoiseByMed->setObjectName(QStringLiteral("cB_deleteNoiseByMed"));

        gridLayout->addWidget(cB_deleteNoiseByMed, 8, 0, 1, 1);

        sB_apertureSize = new QSpinBox(gB_pictProperties);
        sB_apertureSize->setObjectName(QStringLiteral("sB_apertureSize"));
        sB_apertureSize->setMinimum(1);
        sB_apertureSize->setMaximum(15);
        sB_apertureSize->setSingleStep(2);
        sB_apertureSize->setValue(3);

        gridLayout->addWidget(sB_apertureSize, 8, 1, 1, 1);


        gridLayout_3->addWidget(gB_pictProperties, 5, 0, 1, 2);

        pB_downloadPict = new QPushButton(centralWidget);
        pB_downloadPict->setObjectName(QStringLiteral("pB_downloadPict"));
        pB_downloadPict->setFont(font);

        gridLayout_3->addWidget(pB_downloadPict, 0, 0, 1, 1);

        pB_downloadVectorOfFeatures = new QPushButton(centralWidget);
        pB_downloadVectorOfFeatures->setObjectName(QStringLiteral("pB_downloadVectorOfFeatures"));
        pB_downloadVectorOfFeatures->setEnabled(false);

        gridLayout_3->addWidget(pB_downloadVectorOfFeatures, 1, 0, 1, 1);

        le_pictPath = new QLineEdit(centralWidget);
        le_pictPath->setObjectName(QStringLiteral("le_pictPath"));
        le_pictPath->setReadOnly(true);

        gridLayout_3->addWidget(le_pictPath, 0, 1, 1, 2);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 4, 0, 1, 3);

        pB_openWebCamera = new QPushButton(centralWidget);
        pB_openWebCamera->setObjectName(QStringLiteral("pB_openWebCamera"));
        QFont font1;
        font1.setBold(true);
        font1.setUnderline(false);
        font1.setWeight(75);
        pB_openWebCamera->setFont(font1);

        gridLayout_3->addWidget(pB_openWebCamera, 2, 0, 1, 1);

        cB_detectLandmarks = new QCheckBox(centralWidget);
        cB_detectLandmarks->setObjectName(QStringLiteral("cB_detectLandmarks"));

        gridLayout_3->addWidget(cB_detectLandmarks, 2, 1, 1, 1);

        pB_detectSmilesOnImagesSet = new QPushButton(centralWidget);
        pB_detectSmilesOnImagesSet->setObjectName(QStringLiteral("pB_detectSmilesOnImagesSet"));

        gridLayout_3->addWidget(pB_detectSmilesOnImagesSet, 3, 0, 1, 1);

        MainWidj->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWidj);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 740, 20));
        MainWidj->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWidj);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWidj->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWidj);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWidj->setStatusBar(statusBar);

        retranslateUi(MainWidj);

        QMetaObject::connectSlotsByName(MainWidj);
    } // setupUi

    void retranslateUi(QMainWindow *MainWidj)
    {
        MainWidj->setWindowTitle(QApplication::translate("MainWidj", "MainWidj", Q_NULLPTR));
        gB_thresholding->setTitle(QApplication::translate("MainWidj", "\320\221\320\270\320\275\320\260\321\200\320\270\320\267\320\260\321\206\320\270\321\217", Q_NULLPTR));
        pB_threshold->setText(QApplication::translate("MainWidj", "\320\221\320\270\320\275\320\260\321\200\320\270\320\267\320\260\321\206\320\270\321\217", Q_NULLPTR));
        lbl_thresh->setText(QApplication::translate("MainWidj", "\320\237\320\276\321\200\320\276\320\263\320\276\320\262\320\276\320\265 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265: ", Q_NULLPTR));
        gB_channels->setTitle(QString());
        rB_useRedChannel->setText(QApplication::translate("MainWidj", "\320\232\321\200\320\260\321\201\320\275\321\213\320\271 \320\272\320\260\320\275\320\260\320\273", Q_NULLPTR));
        rB_useGreenChannel->setText(QApplication::translate("MainWidj", "\320\227\320\265\320\273\320\265\320\275\321\213\320\271 \320\272\320\260\320\275\320\260\320\273", Q_NULLPTR));
        rB_useBlueChannel->setText(QApplication::translate("MainWidj", "\320\241\320\270\320\275\320\270\320\271 \320\272\320\260\320\275\320\260\320\273", Q_NULLPTR));
        cB_useAdaptiveThreshold->setText(QApplication::translate("MainWidj", "\320\220\320\273\320\263\320\276\321\200\320\270\321\202\320\274 \320\221\321\200\321\215\320\264\320\273\320\270", Q_NULLPTR));
        gB_hsvValues->setTitle(QApplication::translate("MainWidj", "HSV", Q_NULLPTR));
        lbl_hue->setText(QApplication::translate("MainWidj", "Hue: ", Q_NULLPTR));
        lbl_saturation->setText(QApplication::translate("MainWidj", "Saturation: ", Q_NULLPTR));
        lbl_value->setText(QApplication::translate("MainWidj", "Value: ", Q_NULLPTR));
        gB_pictProperties->setTitle(QApplication::translate("MainWidj", "\320\241\320\262\320\276\320\271\321\201\321\202\320\262\320\260 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\321\217", Q_NULLPTR));
        cB_deleteStars->setText(QApplication::translate("MainWidj", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\267\320\262\320\265\320\267\320\264\321\213", Q_NULLPTR));
        lbl_pictNChannels->setText(QApplication::translate("MainWidj", "\320\232\320\276\320\273-\320\262\320\276 \320\272\320\260\320\275\320\260\320\273\320\276\320\262: ", Q_NULLPTR));
        lbl_pictHeight->setText(QApplication::translate("MainWidj", "\320\222\321\213\321\201\320\276\321\202\320\260: ", Q_NULLPTR));
        pB_showPict->setText(QApplication::translate("MainWidj", "\320\237\320\276\320\272\320\260\320\267\320\260\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", Q_NULLPTR));
        cB_deleteDefectPixels->setText(QApplication::translate("MainWidj", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\264\320\265\321\204\320\265\320\272\321\202\320\275\321\213\320\265 \320\277\320\270\320\272\321\201\320\265\320\273\320\270", Q_NULLPTR));
        lbl_pictWidth->setText(QApplication::translate("MainWidj", "\320\250\320\270\321\200\320\270\320\275\320\260: ", Q_NULLPTR));
        pB_savePict->setText(QApplication::translate("MainWidj", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", Q_NULLPTR));
        cB_deleteNoiseByMed->setText(QApplication::translate("MainWidj", "\320\241\320\275\320\265\321\201\321\202\320\270 \320\274\320\265\320\273\320\272\320\270\320\265 \320\267\320\262\320\265\320\267\320\264\321\213 (\321\210\321\203\320\274?) \320\274\320\265\320\264\320\270\320\260\320\275\320\276\320\271", Q_NULLPTR));
        pB_downloadPict->setText(QApplication::translate("MainWidj", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\265", Q_NULLPTR));
        pB_downloadVectorOfFeatures->setText(QApplication::translate("MainWidj", "\320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214 *.txt \320\262\320\265\320\272\321\202\320\276\321\200\321\213 \320\277\321\200\320\270\320\267\320\275\320\260\320\272\320\276\320\262", Q_NULLPTR));
        pB_openWebCamera->setText(QApplication::translate("MainWidj", " \320\236\321\202\320\272\321\200\321\213\321\202\321\214 WEB \320\272\320\260\320\274\320\265\321\200\321\203", Q_NULLPTR));
        cB_detectLandmarks->setText(QApplication::translate("MainWidj", "detect face's langmarks", Q_NULLPTR));
        pB_detectSmilesOnImagesSet->setText(QApplication::translate("MainWidj", "\320\236\320\261\321\200\320\260\320\261\320\276\321\202\320\260\321\202\321\214 \320\275\320\260\320\261\320\276\321\200 \320\270\320\267\320\276\320\261\321\200\320\260\320\266\320\265\320\275\320\270\320\271", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWidj: public Ui_MainWidj {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDJ_H
