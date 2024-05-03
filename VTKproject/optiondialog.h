#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    void setName( QString );
    QString getName();
    void setRGB( unsigned int, unsigned int, unsigned int);
    void changeR();
    unsigned int getR();
    void changeG();
    unsigned int getG();
    void changeB();
    unsigned int getB();
    void setVisibleDialog(double);
    void changeVisibility();
    double getVisible();


private:
    Ui::OptionDialog *ui;
    QString r;
    QString g;
    QString b;
};

#endif // OPTIONDIALOG_H
