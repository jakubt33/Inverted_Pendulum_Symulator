#ifndef CHARTS_H
#define CHARTS_H

#include <QDialog>
#include <QString>

namespace Ui {
class Charts;
}

class Charts : public QDialog
{
    Q_OBJECT

public:
    explicit Charts(QWidget *parent = 0);
    ~Charts();

    void addData( float newData );
    void addData( float newData1, float newData2 );
    void setHeight(int height);
    void setRange(float range);
    void setLabelName(QString label1, QString label2);
    void setLabelName(QString label1);

private slots:
    void makePlot();

private:
    Ui::Charts *ui;
};

#endif // CHARTS_H
