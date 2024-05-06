/** @file 
* defines the OptionDialog class and its associated functions
*/
#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(0, 255);
    ui->horizontalSlider_2->setRange(0, 255);
    ui->horizontalSlider_3->setRange(0, 255);
    ui->horizontalSlider_4->setRange(0, 100);
    ui->label_5->setStyleSheet("QLabel{background-color:rgb(255,0,0);border:2px solid red;}");

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeR()));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(changeG()));
    connect(ui->horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(changeB()));
    connect(ui->horizontalSlider_4, SIGNAL(valueChanged(double)), this, SLOT(setVisibleDialog(double)));
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setName(QString name)
{
    ui->lineEdit->setText(name);
}

QString OptionDialog::getName()
{
    return ui->lineEdit->text();
}

void OptionDialog::setRGB(unsigned int R, unsigned int G, unsigned int B)
{
    ui->horizontalSlider->setValue(R);
    ui->horizontalSlider_2->setValue(G);
    ui->horizontalSlider_3->setValue(B);

    r = QString::number(R);
    g = QString::number(G);
    b = QString::number(B);

    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");

    changeR();
    changeG();
    changeB();
}

void OptionDialog::changeR()
{
    r = QString::number(ui->horizontalSlider->value());
    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

void OptionDialog::changeG()
{
    g = QString::number(ui->horizontalSlider_2->value());
    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

void OptionDialog::changeB()
{
    b = QString::number(ui->horizontalSlider_3->value());
    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

unsigned int OptionDialog::getR()
{
    return ui->horizontalSlider->value();
}

unsigned int OptionDialog::getG()
{
    return ui->horizontalSlider_2->value();
}

unsigned int OptionDialog::getB()
{
    return ui->horizontalSlider_3->value();
}

void OptionDialog::setVisibleDialog(double visible)
{
    ui->horizontalSlider_4->setValue(static_cast<int>(visible));
}

double OptionDialog::getVisible()
{
    return ui->horizontalSlider_4->value();
}
