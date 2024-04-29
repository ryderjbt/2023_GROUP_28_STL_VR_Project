#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect( ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
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
        QString visible("true");

        /* Create child item */
        ModelPart *childItem = new ModelPart({ name, visible });

        /* Append to tree top-level */
        rootItem->appendChild(childItem);
    }

    //Link a render window with the Qt widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    //Add a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
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

void MainWindow::handleButton1() {
    emit statusUpdateMessage( QString("Button 1 was clicked"), 0 );
}

void MainWindow::handleButton2() {
    emit statusUpdateMessage( QString("Button 2 was clicked"), 0 );
}

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
    QString text = selectedPart->data(0).toString();
    QString name = QString("Item %1,%2").arg(text).arg(fileName);
    QString visible("true");

    ModelPart *childItem = new ModelPart({ name, visible });

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
    dialog.setVisibleDialog( selectedPart->data(1).toBool() );

    if(dialog.exec() == QDialog::Accepted) {
        emit statusUpdateMessage(QString("Dialog accepted "),0);
        selectedPart->set( 0, dialog.getName() );
        selectedPart->setColour( dialog.getR(), dialog.getG(), dialog.getB() );
        selectedPart->setVisible(1, dialog.getVisible());
        updateRenderer();
    } else {
        emit statusUpdateMessage(QString("Dialog Rejected "),0);
    }

}

void MainWindow::updateRenderer() {
    renderer->RemoveAllViewProps();
    for(int i = 0; i < 3; i++){
        updateRenderFromTree(partList->index(i, 0, QModelIndex() ) );
    }
    renderer->Render();
    updateCamera();
}

void MainWindow::updateRenderFromTree( const QModelIndex& index ){
    if( index.isValid() ) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        // Retrieve the VTK actor from the selected part and add it to the renderer
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor) { // Ensure the actor is not null
            renderer->AddActor(actor);
        }
        //Retrieve actor from selected part and add to renderer
    }

    //Check to see if this part has any children
    if( !partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren) ){
        return;
    }

    //Loop through childrn and add their actors
    int rows = partList->rowCount( index );
    for(int i = 0; i < rows; i++){
        updateRenderFromTree(partList->index(i, 0, index));
    }
}

void MainWindow::updateCamera(){
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}
