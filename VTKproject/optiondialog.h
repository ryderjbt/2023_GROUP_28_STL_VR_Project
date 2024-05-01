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
    QString r;
    void changeG();
    unsigned int getG();
    QString g;
    void changeB();
    unsigned int getB();
    QString b;
    void setVisibleDialog( bool );
    bool getVisible();


private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
