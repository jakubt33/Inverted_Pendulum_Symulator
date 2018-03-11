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
    QLCDNumber *customValue;

    void setupUi(QDialog *Charts)
    {
        if (Charts->objectName().isEmpty())
            Charts->setObjectName(QStringLiteral("Charts"));
        Charts->resize(647, 241);
        customPlot = new QCustomPlot(Charts);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(9, 9, 631, 221));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy);
        customValue = new QLCDNumber(customPlot);
        customValue->setObjectName(QStringLiteral("customValue"));
        customValue->setGeometry(QRect(540, 20, 64, 23));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        customValue->setFont(font);
        customValue->setAutoFillBackground(true);

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
