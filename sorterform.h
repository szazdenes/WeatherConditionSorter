#ifndef SORTERFORM_H
#define SORTERFORM_H

#include <QWidget>

namespace Ui {
class SorterForm;
}

class SorterForm : public QWidget
{
    Q_OBJECT

public:
    explicit SorterForm(QWidget *parent = 0);
    ~SorterForm();

private:
    Ui::SorterForm *ui;
};

#endif // SORTERFORM_H
