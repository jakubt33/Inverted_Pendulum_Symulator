#include "charts.h"
#include "ui_charts.h"

Charts::Charts(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Charts)
{
    ui->setupUi(this);
    Charts::makePlot();
}

Charts::~Charts()
{
    delete ui;
}

void Charts::makePlot()
{
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 0, 0)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-180.0f, 180.0f);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void Charts::addData( float newAngle, float newForce )
{
    static double iterator = 0.0;
    iterator += 0.005;


    ui->customPlot->graph(0)->addData(iterator, newAngle);
    ui->angleNumber->display(newAngle);

    ui->customPlot->graph(1)->addData(iterator, newForce);
    ui->forceNumber->display(newForce);


    ui->customPlot->xAxis->setRange(iterator, 8, Qt::AlignRight);
    ui->customPlot->replot();
}
