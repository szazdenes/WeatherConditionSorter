#include "sorterform.h"
#include "ui_sorterform.h"
#include <gnuplot-cpp/gnuplot_i.hpp>

SorterForm::SorterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SorterForm)
{
    ui->setupUi(this);
    ui->windMinDoubleSpinBox->setValue(1);
    ui->windMaxDoubleSpinBox->setValue(10);
    ui->windRangeDoubleSpinBox->setValue(1);
    ui->tempMinDoubleSpinBox->setValue(25);
    ui->tempMaxDoubleSpinBox->setValue(37);
    ui->tempRangeDoubleSpinBox->setValue(1);
}

SorterForm::~SorterForm()
{
    delete ui;
}

void SorterForm::on_pushButton_clicked()
{
    weatherDataMap.clear();
    fileName = QFileDialog::getOpenFileName(this, "Open file", "../../", "*.dat");
    loadData(fileName);
    selectData(weatherDataMap);
}

void SorterForm::loadData(QString fileName)
{
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug("File cannot be opened.");
            return;
        }

        QPair<double, double> dataPair;
        QTextStream stream(&file);
        QString date;
        double temp, wind;
        stream.readLine();
        while(!stream.atEnd()){
            QString line = stream.readLine();
            QTextStream lineStream(&line);
            lineStream >> date >> temp >> wind;
            dataPair.first = temp;
            dataPair.second = wind;
            weatherDataMap[date].append(dataPair);
        }
        file.close();
    }
}

void SorterForm::selectData(QMap<QString, QList<QPair<double, double> > > dataMap)
{
    if(!dataMap.isEmpty()){
        for(double i = ui->tempMinDoubleSpinBox->value(); i <= ui->tempMaxDoubleSpinBox->value(); i += ui->tempRangeDoubleSpinBox->value()){

            QString month = fileName.split("/").last().remove(".dat");
            QFile outFile("../results/" + month + "_T_" + QString::number(i) + "_.csv");
            if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug("File cannot be opened.");
                return;
            }
            QTextStream outStream(&outFile);
            outStream << "#time/wind(" + ui->windMinDoubleSpinBox->text() + "-" + ui->windMaxDoubleSpinBox->text() + "," + ui->windRangeDoubleSpinBox->text() + ")\n";
            QMap<QString, QList<int> > resultMap;
            for(double j = ui->windMinDoubleSpinBox->value(); j <= ui->windMaxDoubleSpinBox->value(); j += ui->windRangeDoubleSpinBox->value()){
                foreach(QString currentKey, dataMap.keys()){
                    int num = 0;
                    for(int k = 0; k < dataMap[currentKey].size(); k++){
                        if(dataMap[currentKey].at(k).first > i && dataMap[currentKey].at(k).second < j)
                            num++;
                    }
                    resultMap[currentKey].append(num);
                }
            }
            QStringList keyList = resultMap.keys();
            qSort(keyList.begin(), keyList.end());
            foreach(QString currKey, keyList){
                outStream << currKey + "\t";
                for(int q = 0; q < resultMap[currKey].size(); q++){
                    if(q < resultMap[currKey].size()-1)
                        outStream << QString::number(resultMap[currKey].at(q)) + "\t";
                    else
                        outStream << QString::number(resultMap[currKey].at(q)) + "\n";
                }
            }
            outFile.close();
        }
    }
}

void SorterForm::plotWithGnuPlot(QString plotFile)
{
    if(!plotFile.isEmpty()){
        QString outName = plotFile;
        outName.remove(".csv").append(".ps");
        QString com = QString("reset;set terminal postscript landscape enhanced color font 'Times-Roman,14';set output '%1';filename = '%2';"
                              "set style line 1 lc rgb 'black' pt 7 ps 1.0 lt 1 lw 1.0;set style line 2 lc rgb 'black' pt 7 ps 1.0 lt 2 lw 1.0;"
                              "set style line 3 lc rgb 'grey' pt 7 ps 1.0 lt 1 lw 1.0;set style line 4 lc rgb 'grey' pt 7 ps 1.0 lt 2 lw 1.0;"
                              "set style line 5 lc rgb 'red' pt 7 ps 1.0 lt 1 lw 1.0;set style line 6 lc rgb 'red' pt 7 ps 1.0 lt 2 lw 1.0;"
                              "set style line 7 lc rgb 'green' pt 7 ps 1.0 lt 1 lw 1.0;set style line 8 lc rgb 'green' pt 7 ps 1.0 lt 2 lw 1.0;"
                              "set style line 9 lc rgb 'blue' pt 7 ps 1.0 lt 1 lw 1.0;set style line 10 lc rgb 'blue' pt 7 ps 1.0 lt 2 lw 1.0;"
                              "set xdata time;set timefmt '%H:%M:%S';set xrange ['00:00:00':'23:55:00'];set yrange [0:23];set xlabel 'time';"
                              "set ylabel 'number of occurrence';set key left top;plot filename u 1:2 w l ls 1 title 'wind 1 km/h', '' u 1:3 w l ls 2 title 'wind 2 km/h',"
                              " '' u 1:4 w l ls 3 title 'wind 3 km/h', '' u 1:5 w l ls 4 title 'wind 4 km/h', '' u 1:6 w l ls 5 title 'wind 5 km/h', '' u 1:7 w l ls 6 title 'wind 6 km/h',"
                              " '' u 1:8 w l ls 7 title 'wind 7 km/h', '' u 1:9 w l ls 8 title 'wind 8 km/h', '' u 1:10 w l ls 9 title 'wind 9 km/h', '' u 1:11 w l ls 10 title 'wind 10 km/h';").arg(outName).arg(plotFile);
        Gnuplot gp;
        gp.cmd(com.toStdString());
        QApplication::processEvents();
    }
}

void SorterForm::on_pushButton_3_clicked()
{
    QStringList fileList = QFileDialog::getOpenFileNames(this, "Open files", "../results", "*.csv");
    foreach(QString currentFile, fileList)
        plotWithGnuPlot(currentFile);
}
