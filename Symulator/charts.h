#ifndef CHARTS_H
#define CHARTS_H

#include <QDialog>

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
    void setHeight(int height);
    void setRange(float range);

private slots:
    void makePlot();

private:
    Ui::Charts *ui;
};

#endif // CHARTS_H
