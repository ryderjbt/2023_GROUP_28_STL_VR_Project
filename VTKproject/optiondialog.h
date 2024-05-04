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

signals:
    /*void level1VisibilityChanged(int state);
    void level2VisibilityChanged(int state);
    void level3VisibilityChanged(int state);*/

private slots:
    /*void changeLevel1Visibility(int state);
    void changeLevel2Visibility(int state);
    void changeLevel3Visibility(int state);*/
    void changeR();
    void changeG();
    void changeB();
    void changeVisibility();

private:
    Ui::OptionDialog *ui;
    QString r;
    QString g;
    QString b;
};

#endif // OPTIONDIALOG_H
