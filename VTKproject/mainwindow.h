// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/** @file
* Declarations of all includes and functions used by mainwindow.cpp
*/

#include <QMainWindow>
#include <QFileDialog>

#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include "ModelPartList.h"
#include "ModelPart.h"
#include "optiondialog.h"
#include "VRRenderThread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void handleButton1();
    void handleButton2();
    void handleTreeClicked();
    void handleVRbuttonPressed();
    void updateLevel1Visibility(bool visible);
    void updateLevel2Visibility(bool visible);
    void updateLevel3Visibility(bool visible);
    void updateRenderer();
    void updateRenderFromTree(const QModelIndex&);
    void updateCamera();

signals:
    void statusUpdateMessage(const QString &message, int timeout);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;

   vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    OptionDialog dialog; // Declare an instance of OptionDialog
};


#endif // MAINWINDOW_H
