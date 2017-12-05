#ifndef SORTERFORM_H
#define SORTERFORM_H

#include <QWidget>
#include <QMap>
#include <QFileDialog>
#include <QTextStream>


namespace Ui {
class SorterForm;
}

class SorterForm : public QWidget
{
    Q_OBJECT

public:
    explicit SorterForm(QWidget *parent = 0);
    ~SorterForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::SorterForm *ui;
    QMap<QString, QList<QPair<double, double> > > weatherDataMap;
    QString fileName;

    void loadData(QString fileName);
    void selectData(QMap<QString, QList<QPair<double, double> > > dataMap); //first: air temp, second: wind
    void plotWithGnuPlot(QString plotFile);
};

#endif // SORTERFORM_H
