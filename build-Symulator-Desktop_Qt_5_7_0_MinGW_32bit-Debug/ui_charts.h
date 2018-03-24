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
#include <QtWidgets/QLabel>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Charts
{
public:
    QCustomPlot *customPlot;
    QLCDNumber *customValue1;
    QLCDNumber *customValue2;
    QLabel *label1;
    QLabel *label2;

    void setupUi(QDialog *Charts)
    {
        if (Charts->objectName().isEmpty())
            Charts->setObjectName(QStringLiteral("Charts"));
        Charts->resize(647, 338);
        customPlot = new QCustomPlot(Charts);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(9, 9, 631, 321));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy);
        customValue1 = new QLCDNumber(customPlot);
        customValue1->setObjectName(QStringLiteral("customValue1"));
        customValue1->setGeometry(QRect(40, 20, 64, 20));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        customValue1->setFont(font);
        customValue1->setAutoFillBackground(true);
        customValue2 = new QLCDNumber(customPlot);
        customValue2->setObjectName(QStringLiteral("customValue2"));
        customValue2->setGeometry(QRect(40, 40, 64, 23));
        customValue2->setFont(font);
        customValue2->setAutoFillBackground(true);
        label1 = new QLabel(customPlot);
        label1->setObjectName(QStringLiteral("label1"));
        label1->setGeometry(QRect(110, 20, 91, 21));
        label2 = new QLabel(customPlot);
        label2->setObjectName(QStringLiteral("label2"));
        label2->setGeometry(QRect(110, 40, 91, 20));

        retranslateUi(Charts);

        QMetaObject::connectSlotsByName(Charts);
    } // setupUi

    void retranslateUi(QDialog *Charts)
    {
        Charts->setWindowTitle(QApplication::translate("Charts", "Dialog", 0));
        label1->setText(QApplication::translate("Charts", "TextLabel", 0));
        label2->setText(QApplication::translate("Charts", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class Charts: public Ui_Charts {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHARTS_H
