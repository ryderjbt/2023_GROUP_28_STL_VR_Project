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
    ui->label_5->setStyleSheet("QLabel{background-color:rgb(255,0,0);border:2px solid red;}");

    connect( ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeR()));
    connect( ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(changeG()));
    connect( ui->horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(changeB()));
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setName( QString name ){
    ui->lineEdit->setText( name );
}

QString OptionDialog::getName(){
    return ui->lineEdit->text();
}

void OptionDialog::setRGB( unsigned int R, unsigned int G, unsigned int B ){
    ui->horizontalSlider->setValue( R );
    ui->horizontalSlider_2->setValue( G );
    ui->horizontalSlider_3->setValue( B );

    r = QString::number(ui->horizontalSlider->value());
    g = QString::number(ui->horizontalSlider_2->value());
    b = QString::number(ui->horizontalSlider_3->value());

    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");

    changeR();
    changeG();
    changeB();
}

void OptionDialog::changeR(){
    qDebug() << ui->horizontalSlider->value();
    ui->horizontalSlider->setValue(ui->horizontalSlider->value());
    r = QString::number(ui->horizontalSlider->value());

    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

void OptionDialog::changeG(){
    qDebug() << ui->horizontalSlider_2->value();
    ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value());
    g = QString::number(ui->horizontalSlider_2->value());

    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

void OptionDialog::changeB(){
    qDebug() << ui->horizontalSlider_3->value();
    ui->horizontalSlider_3->setValue(ui->horizontalSlider_3->value());
    b = QString::number(ui->horizontalSlider_3->value());

    ui->label_5->setStyleSheet("QLabel{background-color:rgb("+r+","+g+","+b+");}");
}

unsigned int OptionDialog::getR(){
    return ui->horizontalSlider->value();
}

unsigned int OptionDialog::getG(){
    return ui->horizontalSlider_2->value();
}

unsigned int OptionDialog::getB(){
    return ui->horizontalSlider_3->value();
}

void OptionDialog::setVisibleDialog( bool visible ){
    ui->checkBox->QAbstractButton::setChecked( visible );
}

bool OptionDialog::getVisible(){
    return ui->checkBox->isChecked();
}
