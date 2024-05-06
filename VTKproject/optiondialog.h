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
    void setName(QString name);
    QString getName();
    void setRGB(unsigned int red, unsigned int green, unsigned int blue);
    unsigned int getR();
    unsigned int getG();
    unsigned int getB();
    void setVisibleDialog(double visibility);
    double getVisible();

private slots:
    void changeR();
    void changeG();
    void changeB();

private:
    Ui::OptionDialog *ui;
    QString r;
    QString g;
    QString b;
};

#endif // OPTIONDIALOG_H
