/**     @file ModelPart.cpp
  * This file creates the modelPart class and all functions required to render/alter 3D objects in the main window
  */

#include "ModelPart.h"
#include <vtkSTLReader.h>


/* Commented out for now, will be uncommented later when you have
 * installed the VTK library
 */
#include <vtkSmartPointer.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSTLReader.h>
#include <vtkPlane.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>



ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {
    ColourR = 50;
    ColourG = 100;
    ColourB = 10;
    isVisible = 100;
    isShrinked = false;
    isClipped = false;
    
    mapper_copy = vtkDataSetMapper::New();


    /* You probably want to give the item a default colour */
}


ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}


void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}


ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}


int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}


void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}


ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}


int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    ColourR = R;
    ColourG = G;
    ColourB = B;

    vtkColor3<unsigned char> color(getColourR(), getColourG(), getColourB());
    double r = color.GetRed() / 255.0;
    double g = color.GetGreen() / 255.0;
    double b = color.GetBlue() / 255.0;
    //Sets colour of treeitem or entire level by looping through children
    if (actor != nullptr) {
        actor->GetProperty()->SetColor(r, g, b);
    } else {
        for (i = 0; i < childCount(); i++) {
            child(i)->setColour(R, G, B);
        }
    }
}

unsigned char ModelPart::getColourR() {
    return ColourR;
}

unsigned char ModelPart::getColourG() {
    return ColourG;
}


unsigned char ModelPart::getColourB() {
    return ColourB;
}


void ModelPart::setVisible(double visibleParam) {
    isVisible = visibleParam;
    double visibility = isVisible / 100;
    set(1, visibility);
    //Sets visibility of treeitem or entire level by looping through children
    if (actor != nullptr) {
        actor->GetProperty()->SetOpacity(visibility);
    }
    else {
        for (i = 0; i < childCount(); i++) {
            child(i)->setVisible(isVisible);
        }
    }
}

double ModelPart::visible() {
    return isVisible;
}

void ModelPart::loadSTL( QString fileName ) {
    /* 1. Use the vtkSTLReader class to load the STL file 
     *     https://vtk.org/doc/nightly/html/classvtkSTLReader.html
     */
    file = vtkSmartPointer<vtkSTLReader>::New();
    std::string stdfileName = fileName.toStdString();
    file->SetFileName(stdfileName.c_str());
    file->Update();

    /* 2. Initialise the part's vtkMapper */
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(file->GetOutputPort());

    /* 3. Initialise the part's vtkActor and link to the mapper */
    actor = vtkSmartPointer<vtkActor>::New();
    vtkColor3<unsigned char> color(getColourR(), getColourG(), getColourB());
    double r = color.GetRed() / 255.0;
    double g = color.GetGreen() / 255.0;
    double b = color.GetBlue() / 255.0;
    actor->SetMapper(mapper);
    actor->GetProperty()->SetDiffuse(0.8);
    actor->GetProperty()->SetColor(r, g, b);
    actor->GetProperty()->SetSpecular(0.3);
    actor->GetProperty()->SetSpecularPower(60.0);
}

vtkSmartPointer<vtkActor> ModelPart::getActor() {
    return actor;
}

vtkActor* ModelPart::getNewActor()
{
    vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
    pd->DeepCopy(mapper->GetInputDataObject(0, 0));

    /* 1. Create new mapper */
    vrMapper = vtkDataSetMapper::New();
    if (file == nullptr) {

        qDebug() << "ERROR: nothing in file reader";

        return nullptr;

    }

    vrMapper->SetInputDataObject(pd);
    vrActor = vtkSmartPointer<vtkActor>::New();
    vrActor->SetMapper(vrMapper);
    vrActor->SetProperty(actor->GetProperty());

    /* The new vtkActor pointer must be returned here */

    return vrActor;
}

bool ModelPart::shrinked()
{
    return isShrinked;
}

void ModelPart::setShrinked(bool boolstate)
{
    isShrinked = boolstate;
}

bool ModelPart::clipped()
{
    return isClipped;
}

void ModelPart::setClipped(bool boolstate)
{
    isClipped = boolstate;
}

void ModelPart::addFilters()
{
    /* Checks are made to understand which filters are to be applied to the model part
    If multiple filters must be applied to a model at once, the second filter must take the first as its input connection, and thus a separate
    check must be made to determine if both filters are to be applied to the model */
    if (isClipped == true && isShrinked == true)
    {
        /* A plane originating at the origin and intersecting the y axis at y = 0 is created */
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(0.0, 1.0, 0.0);

        /* A clip filter object is declared with an input connection to the source file
        The clip filter is linked to the plane object */
        vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
        clipFilter->SetInputConnection(file->GetOutputPort());
        clipFilter->SetClipFunction(planeLeft.Get());
        clipFilter->GenerateClippedOutputOn();
        clipFilter->Update();

        /* A shrink filter object is created with an input connection to the clip filter */
        vtkSmartPointer<vtkShrinkFilter> shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        shrinkFilter->SetInputConnection(clipFilter->GetOutputPort());
        shrinkFilter->SetShrinkFactor(.8);
        shrinkFilter->Update();

        /* The mapper is connected to the shrink filter object */
        mapper_copy->SetInputConnection(shrinkFilter->GetOutputPort());
        if (vrMapper != nullptr) vrMapper->SetInputConnection(shrinkFilter->GetOutputPort());
    }
    else if (isClipped == true)
    {
        vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
        planeLeft->SetOrigin(0.0, 0.0, 0.0);
        planeLeft->SetNormal(0.0, 1.0, 0.0);

        vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
        clipFilter->SetInputConnection(file->GetOutputPort());
        clipFilter->SetClipFunction(planeLeft.Get());
        clipFilter->GenerateClippedOutputOn();
        clipFilter->Update();

        mapper_copy->SetInputConnection(clipFilter->GetOutputPort());
        if (vrMapper != nullptr) vrMapper->SetInputConnection(clipFilter->GetOutputPort());
    }
    else if (isShrinked == true)
    {
        vtkSmartPointer<vtkShrinkFilter> shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        shrinkFilter->SetInputConnection(file->GetOutputPort());
        shrinkFilter->SetShrinkFactor(.8);
        shrinkFilter->Update();

        mapper_copy->SetInputConnection(shrinkFilter->GetOutputPort());
        if (vrMapper != nullptr) vrMapper->SetInputConnection(shrinkFilter->GetOutputPort());
    }
    else
    {
        // If neither checkbox is checked, the mapper is simply connected to the original source file
        mapper_copy->SetInputConnection(file->GetOutputPort());
        if (vrMapper != nullptr) vrMapper->SetInputConnection(file->GetOutputPort());
    }

    /* Initialise a new vtkActor for the part and link to the original, unmodified mapper */
    actor = vtkSmartPointer<vtkActor>::New();
    vtkColor3<unsigned char> color(getColourR(), getColourG(), getColourB());
    double r = color.GetRed() / 255.0;
    double g = color.GetGreen() / 255.0;
    double b = color.GetBlue() / 255.0;
    actor->SetMapper(mapper_copy);
    actor->GetProperty()->SetDiffuse(0.8);
    actor->GetProperty()->SetColor(r, g, b);
    actor->GetProperty()->SetSpecular(0.3);
    actor->GetProperty()->SetSpecularPower(60.0);
    if (vrMapper != nullptr)
    {
        vrActor = vtkSmartPointer<vtkActor>::New();
        vrActor->SetMapper(vrMapper);
        vrActor->GetProperty()->SetDiffuse(0.8);
        vrActor->GetProperty()->SetColor(r, g, b);
        vrActor->GetProperty()->SetSpecular(0.3);
        vrActor->GetProperty()->SetSpecularPower(60.0);
    }
}

