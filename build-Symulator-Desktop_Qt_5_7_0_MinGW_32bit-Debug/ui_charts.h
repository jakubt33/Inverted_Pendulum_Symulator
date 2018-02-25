/********************************************************************************
** Form generated from reading UI file 'charts.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHARTS_H
#define UI_CHARTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Charts
{
public:
    QCustomPlot *customPlot;
    QLCDNumber *angleNumber;
    QLCDNumber *forceNumber;

    void setupUi(QDialog *Charts)
    {
        if (Charts->objectName().isEmpty())
            Charts->setObjectName(QStringLiteral("Charts"));
        Charts->resize(733, 264);
        customPlot = new QCustomPlot(Charts);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(19, 29, 701, 171));
        angleNumber = new QLCDNumber(Charts);
        angleNumber->setObjectName(QStringLiteral("angleNumber"));
        angleNumber->setGeometry(QRect(40, 220, 64, 23));
        forceNumber = new QLCDNumber(Charts);
        forceNumber->setObjectName(QStringLiteral("forceNumber"));
        forceNumber->setGeometry(QRect(620, 220, 64, 23));

        retranslateUi(Charts);

        QMetaObject::connectSlotsByName(Charts);
    } // setupUi

    void retranslateUi(QDialog *Charts)
    {
        Charts->setWindowTitle(QApplication::translate("Charts", "Dialog", 0));
    } // retranslateUi

};

namespace Ui {
    class Charts: public Ui_Charts {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTS_H
