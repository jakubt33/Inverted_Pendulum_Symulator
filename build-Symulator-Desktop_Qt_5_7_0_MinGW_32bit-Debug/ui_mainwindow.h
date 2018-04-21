/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGraphicsView *ViewSpace;
    QPushButton *buttonAddForce;
    QPushButton *buttonReset;
    QPushButton *buttonPauseResume;
    QPushButton *setAngle;
    QLineEdit *editAngle;
    QLineEdit *editForce;
    QCheckBox *checkBoxTrainingMode;
    QLabel *labelTriesCounter;
    QLCDNumber *numberTries;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(699, 252);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ViewSpace = new QGraphicsView(centralWidget);
        ViewSpace->setObjectName(QStringLiteral("ViewSpace"));
        ViewSpace->setGeometry(QRect(10, 10, 681, 151));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ViewSpace->sizePolicy().hasHeightForWidth());
        ViewSpace->setSizePolicy(sizePolicy);
        ViewSpace->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        ViewSpace->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
        buttonAddForce = new QPushButton(centralWidget);
        buttonAddForce->setObjectName(QStringLiteral("buttonAddForce"));
        buttonAddForce->setGeometry(QRect(460, 170, 75, 23));
        buttonReset = new QPushButton(centralWidget);
        buttonReset->setObjectName(QStringLiteral("buttonReset"));
        buttonReset->setGeometry(QRect(10, 170, 51, 23));
        buttonPauseResume = new QPushButton(centralWidget);
        buttonPauseResume->setObjectName(QStringLiteral("buttonPauseResume"));
        buttonPauseResume->setGeometry(QRect(70, 170, 75, 23));
        setAngle = new QPushButton(centralWidget);
        setAngle->setObjectName(QStringLiteral("setAngle"));
        setAngle->setGeometry(QRect(580, 170, 75, 23));
        editAngle = new QLineEdit(centralWidget);
        editAngle->setObjectName(QStringLiteral("editAngle"));
        editAngle->setGeometry(QRect(660, 170, 31, 20));
        editForce = new QLineEdit(centralWidget);
        editForce->setObjectName(QStringLiteral("editForce"));
        editForce->setGeometry(QRect(540, 170, 31, 20));
        checkBoxTrainingMode = new QCheckBox(centralWidget);
        checkBoxTrainingMode->setObjectName(QStringLiteral("checkBoxTrainingMode"));
        checkBoxTrainingMode->setGeometry(QRect(360, 172, 91, 20));
        labelTriesCounter = new QLabel(centralWidget);
        labelTriesCounter->setObjectName(QStringLiteral("labelTriesCounter"));
        labelTriesCounter->setGeometry(QRect(60, 20, 31, 20));
        numberTries = new QLCDNumber(centralWidget);
        numberTries->setObjectName(QStringLiteral("numberTries"));
        numberTries->setGeometry(QRect(20, 20, 31, 23));
        numberTries->setDigitCount(3);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 699, 20));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        buttonAddForce->setText(QApplication::translate("MainWindow", "Add force", 0));
        buttonReset->setText(QApplication::translate("MainWindow", "Reset", 0));
        buttonPauseResume->setText(QApplication::translate("MainWindow", "PauseResume", 0));
        setAngle->setText(QApplication::translate("MainWindow", "Set Angle", 0));
        checkBoxTrainingMode->setText(QApplication::translate("MainWindow", "Training Mode", 0));
        labelTriesCounter->setText(QApplication::translate("MainWindow", "Tries", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
