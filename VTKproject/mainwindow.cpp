/** @file mainwindow.cpp
* This file contains all run processes relating to the GUI of the program
*/
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkLight.h>
#include <vtkNamedColors.h>
#include <vtkPlane.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( ui->pushButton, &QPushButton::released, this, &MainWindow::handleVRbuttonPressed);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::stopVR);
    /*connect(&dialog, &OptionDialog::level1VisibilityChanged, this, &MainWindow::updateLevel1Visibility);
    connect(&dialog, &OptionDialog::level2VisibilityChanged, this, &MainWindow::updateLevel2Visibility);
    connect(&dialog, &OptionDialog::level3VisibilityChanged, this, &MainWindow::updateLevel3Visibility);*/
    connect( ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    connect( this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage );
    ui->treeView->addAction(ui->actionItem_Options);

    /* Create / allocate the ModelList */
    this -> partList = new ModelPartList("PartsList");

    /* Link it to the treeview in the GUI */
    ui->treeView->setModel(this->partList);

    /*Manually create a model tree - there a much better and more flexible ways of doing
      this,
      e.g. with nested functions. This is just a quick example of a starting point. */
    ModelPart *rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for(int i = 0; i<3; i++){
        /* Create strings for both data columns */
        QString name = QString("TopLevel %1").arg(i);
        QString visible("1.0");

        /* Create child item */
        ModelPart *childItem = new ModelPart({ name, visible });

        /* Append to tree top-level */
        rootItem->appendChild(childItem);
    }

    //Link a render window with the Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    vtkNew<vtkNamedColors> colors;

    //Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(colors->GetColor3d("Silver").GetData());
    renderWindow->AddRenderer(renderer);

    /*Create an object and add to renderer (this will change later to display a CAD
     * model)
     * Will just copy and paster cylinder example from before
     * This creates a polygonal cylinder model wirh eight circumfrential facets
     * (i.e, in practice an octagonal prism).*/
    vtkNew<vtkCylinderSource>cylinder;
    cylinder->SetResolution(100);

    /*The mapper is responsible for pushing the geometry into the graphics
     * library. It may also do colour mapping, if scalars or other attributes are
     * defined*/
    vtkNew<vtkPolyDataMapper>cylinderMapper;
    cylinderMapper->SetInputConnection( cylinder->GetOutputPort());

    /*The actor is a grouping mechanism: besides the geometry (mapper), it
     * also has a property, transformation matrix, and/or texture map.
     * Here we set its colour and rotate it around the X and Y axes.*/
    vtkNew<vtkActor>cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    //Setup light parameters
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition(-15, 200, 200);
    light->SetPositional(true);
    light->SetConeAngle(180);
    light->SetFocalPoint(0, 0, 0);
    light->SetColor(1, 1, 1);
    light->SetIntensity(1);
    renderer->AddLight(light);

    // Add the actor to the renderer
    renderer->AddActor(cylinderActor);

    //Reset Camera (Propably needs ro go in its own function that is called whenever model is changed)
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::handleButton2() {
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);

    OptionDialog dialog(this);
    dialog.setName(selectedPart->data(0).toString());
    dialog.setRGB(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB());
    dialog.setVisibleDialog(selectedPart->visible());

    if (dialog.exec() == QDialog::Accepted) {
        emit statusUpdateMessage(QString("Dialog accepted "), 0);
        selectedPart->set(0, dialog.getName());
        selectedPart->setColour(dialog.getR(), dialog.getG(), dialog.getB());
        selectedPart->setVisible(dialog.getVisible());
        updateRenderer();
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected "), 0);
    }
}*/


void MainWindow::handleTreeClicked() {
    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* In this case, we will retrieve the name string from the internal QVariant data array */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ")+text, 0);
}

void MainWindow::on_actionOpen_File_triggered(){
    // Checking action works
    emit statusUpdateMessage( QString( "Open file action triggered" ),0 );
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)") );
    emit statusUpdateMessage( QString(fileName),0 );

    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* In this case, we will retrieve the name string from the internal QVariant data array */
    int text = 1 + selectedPart->childCount();
    QString name = QString("Item %1 - %2").arg(text).arg(fileName);
    QString visible("1.0");

    ModelPart *childItem = new ModelPart({ name, visible });
    childItem->setSource(fileName);

    /* Append to parent */
    selectedPart->appendChild(childItem);

    childItem->loadSTL(fileName);
    updateRenderer();
}

void MainWindow::on_actionItem_Options_triggered() {
    /* Get the index of the selected item */
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* In this case, we will retrieve the name string from the internal QVariant data array */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ")+text, 0);

    OptionDialog dialog(this);

    dialog.setName( selectedPart->data(0).toString() );
    dialog.setRGB( selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB() );
    dialog.setVisibleDialog( selectedPart->visible() );

    if(dialog.exec() == QDialog::Accepted) {
        emit statusUpdateMessage(QString("Dialog accepted "),0);
        selectedPart->set( 0, dialog.getName() );
        selectedPart->setColour( dialog.getR(), dialog.getG(), dialog.getB() );
        selectedPart->setVisible(dialog.getVisible());
        updateRenderer();
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected "),0);
    }

}

/* Creates a VR renderer thread when the "Start VR" button is pressed, and sends all model part actors
to the thread */
void MainWindow::handleVRbuttonPressed()
{
    /* a separate vr thread is created and run */
    VRRenderThread* vrThread = new VRRenderThread();
    emit statusUpdateMessage(QString("Start VR button pressed, VR thread created"), 0);

    /* All render objects in the tree are found and new mappers/actors are created for them */
    ModelPart* rootItem = this->partList->getRootItem();
    for (int i = 0; i < 3; i++)
    {
        ModelPart* topLevel = rootItem->child(i);
        unsigned int childCount = topLevel->childCount();
        for (int j = 0; j < childCount; j++)
        {
            ModelPart* childItem = topLevel->child(j);
            vtkActor *childActor = childItem->getNewActor();
            vrThread->addActorOffline(childActor);
        }
    }
    emit statusUpdateMessage(QString("Actors added to VR render thread"), 0);
    vrThread->start();
}

void MainWindow::updateRenderer() {
    renderer->RemoveAllViewProps();
    for(int i = 0; i < 3; i++){
        updateRenderFromTree(partList->index(i, 0, QModelIndex() ) );
    }
    renderer->Render();
    updateCamera();
    renderWindow->Render(); //Updates rendering window to represent actors automatically
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        // Retrieve the VTK actor from the selected part and add it to the renderer
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor != nullptr && selectedPart->visible()) { // Ensure the actor is not null
            renderer->AddActor(actor);
        }
        // Update the comment to reflect that this function handles both parent and child items
        // Retrieve actor from selected part and add to renderer
    }

    // Check to see if this part has any children
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    // Loop through children and add their actors
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

void MainWindow::updateCamera(){
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

/* Function to stop the separate VR thread from running. Currently doesn't work :) */
void MainWindow::stopVR()
{
    if (vrThread->isRunning())
    {
        vrThread->issueCommand(0, 0);
    }
    else
    {
        emit statusUpdateMessage(QString("No VR Render Thread are currently running"), 0);
    }
}

//void MainWindow::clipFilter(bool checked)
//{
//    if (checked == true)
//    {
//        /* Get the index of the selected item */
//        QModelIndex index = ui->treeView->currentIndex();
//
//        /* Get a pointer to the item from the index */
//        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
//
//        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
//        planeLeft->SetOrigin(0.0, 0.0, 0.0);
//        planeLeft->SetNormal(-1.0, 0, 0);
//        vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
//        clipFilter->SetInputConnection(file->GetOutputPort());
//        clipFilter->SetClipFunction(planeLeft.Get());
//
//        mapper->SetInputConnection(clipFilter->GetOutputPort());
//    }
//}

//void MainWindow::updateLevel1Visibility(bool visible) {
//    QModelIndex index = ui->treeView->currentIndex();
//    if (!index.isValid()) {
//        return; // No item selected in the tree view
//    }
//
//    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
//    // Check if the selected item is a parent item
//    if (selectedPart->childCount() > 0) {
//        // Assuming level 1 corresponds to the first child of the root item
//        ModelPart *level1 = selectedPart->child(0);
//        if (level1) {
//            level1->setVisible(visible);
//            updateRenderer(); // Update the renderer to reflect the visibility change
//        }
//    }
//}
//
//void MainWindow::updateLevel2Visibility(bool visible) {
//    QModelIndex index = ui->treeView->currentIndex();
//    if (!index.isValid()) {
//        return; // No item selected in the tree view
//    }
//
//    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
//    // Check if the selected item is a parent item and has at least two children
//    if (selectedPart->childCount() > 1) {
//        // Assuming level 2 corresponds to the second child of the root item
//        ModelPart *level2 = selectedPart->child(1);
//        if (level2) {
//            level2->setVisible(visible);
//            updateRenderer(); // Update the renderer to reflect the visibility change
//        }
//    }
//}
//
//void MainWindow::updateLevel3Visibility(bool visible) {
//    QModelIndex index = ui->treeView->currentIndex();
//    if (!index.isValid()) {
//        return; // No item selected in the tree view
//    }
//
//    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
//    // Check if the selected item is a parent item and has at least three children
//    if (selectedPart->childCount() > 2) {
//        // Assuming level 3 corresponds to the third child of the root item
//        ModelPart *level3 = selectedPart->child(2);
//        if (level3) {
//            level3->setVisible(visible);
//            updateRenderer(); // Update the renderer to reflect the visibility change
//        }
//    }
//}


