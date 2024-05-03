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

    connect( ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(changeR()));
    connect( ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(changeG()));
    connect( ui->horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(changeB()));
    connect(ui->horizontalSlider_4, SIGNAL(valueChanged(double)), this, SLOT(setVisibleDialog()));
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &OptionDialog::changeLevel1Visibility);
    connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &OptionDialog::changeLevel2Visibility);
    connect(ui->checkBox_3, &QCheckBox::stateChanged, this, &OptionDialog::changeLevel3Visibility);

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

void OptionDialog::changeLevel1Visibility(int state) {
    // Check if the checkbox is checked (state == Qt::Checked) or unchecked (state == Qt::Unchecked)
    if (state == Qt::Checked) {
        // Set the visibility of level 1 to true
        emit level1VisibilityChanged(true);
    } else {
        // Set the visibility of level 1 to false
        emit level1VisibilityChanged(false);
    }
}

void OptionDialog::changeLevel2Visibility(int state) {
    // Check if the checkbox is checked (state == Qt::Checked) or unchecked (state == Qt::Unchecked)
    if (state == Qt::Checked) {
        // Set the visibility of level 2 to true
        emit level2VisibilityChanged(true);
    } else {
        // Set the visibility of level 2 to false
        emit level2VisibilityChanged(false);
    }
}

void OptionDialog::changeLevel3Visibility(int state) {
    // Check if the checkbox is checked (state == Qt::Checked) or unchecked (state == Qt::Unchecked)
    if (state == Qt::Checked) {
        // Set the visibility of level 3 to true
        emit level3VisibilityChanged(true);
    } else {
        // Set the visibility of level 3 to false
        emit level3VisibilityChanged(false);
    }
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

void OptionDialog::setVisibleDialog(double visible ){

    ui->horizontalSlider_4->setValue( visible );
    changeVisibility();
}

void OptionDialog::changeVisibility() {
    qDebug() << ui->horizontalSlider_4->value();
    ui->horizontalSlider_4->setValue(ui->horizontalSlider_4->value());
}

double OptionDialog::getVisible(){
    return ui->horizontalSlider_4->value();
}
