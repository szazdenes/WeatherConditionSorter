#include "sorterform.h"
#include "ui_sorterform.h"

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

void SorterForm::on_pushButton_3_clicked()
{

}
