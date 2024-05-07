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

    //Sets up sliders for colours and visibility 
    
    ui->horizontalSlider->setRange(0, 255);
    ui->horizontalSlider_2->setRange(0, 255);
    ui->horizontalSlider_3->setRange(0, 255);
    ui->horizontalSlider_4->setRange(0, 100);

    //Sets up colour changing sqaure to represent current colour selection

    ui->label_5->setStyleSheet("QLabel{background-color:rgb(255,0,0);border:2px solid red;}");

    //Links all sliders to signals so that interaction properly updates values

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeR()));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(changeG()));
    connect(ui->horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(changeB()));
    connect(ui->horizontalSlider_4, SIGNAL(valueChanged(double)), this, SLOT(setVisibleDialog(double)));
}

//Destructor deletes ui object

OptionDialog::~OptionDialog()
{
    delete ui;
}

//Sets name of current selected part to current input

void OptionDialog::setName(QString name)
{
    ui->lineEdit->setText(name);
}

//Returns final input name to store in selected part properties

QString OptionDialog::getName()
{
    return ui->lineEdit->text();
}

//Sets the sliders to represent current colour of the selected part and calls functions to update based on user changes

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

//Updates red colour to current slider amount and represents it in the Qlabel colour square

void OptionDialog::changeR()
{
    r = QString::number(ui->horizontalSlider->value());
    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

//Updates green colour to current slider amount and represents it in the Qlabel colour square

void OptionDialog::changeG()
{
    g = QString::number(ui->horizontalSlider_2->value());
    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

//Updates blue colour to current slider amount and represents it in the Qlabel colour square

void OptionDialog::changeB()
{
    b = QString::number(ui->horizontalSlider_3->value());
    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

//Returns current slider values for red colour

unsigned int OptionDialog::getR()
{
    return ui->horizontalSlider->value();
}

//Returns current slider values for green colour

unsigned int OptionDialog::getG()
{
    return ui->horizontalSlider_2->value();
}

//Returns current slider values for blue colour

unsigned int OptionDialog::getB()
{
    return ui->horizontalSlider_3->value();
}

//Sets visibility slider to current amount held by the selected part

void OptionDialog::setVisibleDialog(double visible)
{
    ui->horizontalSlider_4->setValue(static_cast<int>(visible));
}

//Returns current slider value of visibility, so selected parts can be updated

double OptionDialog::getVisible()
{
    return ui->horizontalSlider_4->value();
}
