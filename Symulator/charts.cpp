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
    ui->customPlot->graph(0)->setPen(QPen(QColor(0, 0, 255)));
    ui->label1->setStyleSheet("QLabel { color : blue; }");
    ui->customPlot->addGraph(); // green line
    ui->customPlot->graph(1)->setPen(QPen(QColor(0, 255, 0)));
    ui->label2->setStyleSheet("QLabel { color : green; }");


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-180.0f, 180.0f);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void Charts::addData( float newData )
{
    static double iterator = 0.0;
    iterator += 0.005;

    ui->customPlot->graph(0)->addData(iterator, newData);
    ui->customValue1->display(newData);

    ui->customPlot->xAxis->setRange(iterator, 8, Qt::AlignRight);
    ui->customPlot->replot();
}

void Charts::addData( float newData1, float newData2 )
{
    static double iterator = 0.0;
    iterator += 0.005;

    ui->customPlot->graph(0)->addData(iterator, newData1);
    ui->customPlot->graph(1)->addData(iterator, newData2);
    ui->customValue1->display(newData1);
    ui->customValue2->display(newData2);

    ui->customPlot->xAxis->setRange(iterator, 8, Qt::AlignRight);
    ui->customPlot->replot();
}

void Charts::setLabelName(QString label1, QString label2)
{
    ui->label1->setText(label1);
    ui->label2->setText(label2);
}
void Charts::setLabelName(QString label1)
{
    ui->label1->setText(label1);
    ui->label2->hide();
    ui->customValue2->hide();
}

void Charts::setHeight(int height){
    this->resize(ui->customPlot->width(), height);
    ui->customPlot->resize(ui->customPlot->width(), height-30);
}

void Charts::setRange(float range){
    ui->customPlot->yAxis->setRange(-range, range);
}
