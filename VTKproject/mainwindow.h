// mainwindow.h
/** @file
* Declarations of all includes and functions used by mainwindow.cpp
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QDirIterator>

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

    /** Handles treeview interactions so items can be viewed and managed
    */

    void handleTreeClicked();

    /** Handles the function of the startVR button
    * Creates a VR renderer thread when the "Start VR" button is pressed, and sends all model part actors to the thread
    */

    void handleVRbuttonPressed();

    /** Updates the renderer with all actors and properties and resets the camera
    */

    void updateRenderer();

    /** Retrieves VTK actors and adds them to the renderer
    * Can loop through entire tree levels or act on specific items
    * @param index passes in the selected part's index in relation to the ModelPart class
    */

    void updateRenderFromTree(const QModelIndex&);

    /** Resets the camera settings so that the clipping range captures all actors rendered correctly
    */

    void updateCamera();

    /** Calls destructor of the VRRenderThread
    */

    void stopVR();

    /** Changes the lighting intensity to match the user input value
    * @param value is the current slider value in the gui set by user
    */

    void changeLighting(int value);

    /** Called when either the "Clip Filter" or "Shrink Filter" checkbox is clicked. Applies/removes filters from the selected modelPart depending on the checkState of either checkbox
    */

    void applyFilters();

signals:

    /**
    *
    */

    void statusUpdateMessage(const QString &message, int timeout);

public:

    /** Constructor
     *  This will construct a MainWindow object, and set up all nessecary interactable features
     * @param *parent is a pointer to a QWidget, if no object is provided this will be set to nullptr
     */

    MainWindow(QWidget *parent = nullptr);

    /** Destructor
    * Deletes the MainWindow ui object
    */

    ~MainWindow();

private slots:

    /** Opens and loads stl files into main window rendering window
    */

    void on_actionOpen_File_triggered();

    /** Opens and loads all stl files in a directory into main window rendering window at the same time
    */

    void on_actionOpen_Folder_triggered();

    /** Opens options menu for selected part in tree view
    */

    void on_actionItem_Options_triggered();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;

    VRRenderThread* vrThread;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    OptionDialog dialog; // Declare an instance of OptionDialog
};


#endif // MAINWINDOW_H
