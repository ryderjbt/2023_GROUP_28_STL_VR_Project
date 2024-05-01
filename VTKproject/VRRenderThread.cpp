// VRRenderThread.cpp

#include "VRRenderThread.h"
#include <vtkNamedColors.h>
#include <vtkPlane.h>
#include <vtkClipDataSet.h>
#include <vtkShrinkFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkLight.h>
#include <vtkSmartPointer.h>

VRRenderThread::VRRenderThread(QObject* parent) : QThread(parent), endRender(false), rotateX(0.0), rotateY(0.0), rotateZ(0.0) {
    actors = vtkActorCollection::New();
}

VRRenderThread::~VRRenderThread() {
    actors->Delete();
}

void VRRenderThread::addActorOffline(vtkActor* actor) {
    if (!isRunning()) {
        double* ac = actor->GetOrigin();
        actor->RotateX(-90);
        actor->AddPosition(-ac[0] + 0, -ac[1] - 100, -ac[2] - 200);

        vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
        plane->SetOrigin(0.0, 0.0, 0.0);
        plane->SetNormal(-1.0, 0.0, 0.0);

        vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
        clipFilter->SetInputConnection(actor->GetMapper()->GetInputConnection(0, 0));
        clipFilter->SetClipFunction(plane);
        clipFilter->Update();

        vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
        mapper->SetInputConnection(clipFilter->GetOutputPort());
        actor->SetMapper(mapper);

        actors->AddItem(actor);
    }
}

void VRRenderThread::issueCommand(Command cmd, double value) {
    switch (cmd) {
        case END_RENDER:
            endRender = true;
            break;
        case ROTATE_X:
            rotateX = value;
            break;
        case ROTATE_Y:
            rotateY = value;
            break;
        case ROTATE_Z:
            rotateZ = value;
            break;
    }
}

void VRRenderThread::run() {
    vtkNew<vtkNamedColors> colors;
    std::array<unsigned char, 4> bkg{ {26, 51, 102, 255} };
    colors->SetColor("BkgColor", bkg.data());
    renderer = vtkSmartPointer<vtkOpenVRRenderer>::New();
    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
    window = vtkSmartPointer<vtkOpenVRRenderWindow>::New();
    window->Initialize();
    window->AddRenderer(renderer);
    camera = vtkSmartPointer<vtkOpenVRCamera>::New();
    renderer->SetActiveCamera(camera);
    interactor = vtkSmartPointer<vtkOpenVRRenderWindowInteractor>::New();
    interactor->SetRenderWindow(window);
    interactor->Initialize();
    window->Render();

    setupLights();
    setupFilters();

    vtkActor* a;
    actors->InitTraversal();
    while ((a = (vtkActor*)actors->GetNextActor())) {
        renderer->AddActor(a);
    }

    t_last = std::chrono::steady_clock::now();
    while (!interactor->GetDone() && !endRender) {
        interactor->DoOneEvent(window, renderer);

        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t_last).count() > 20) {
            vtkActorCollection* actorList = renderer->GetActors();
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateX(rotateX);
                a->RotateY(rotateY);
                a->RotateZ(rotateZ);
            }
            t_last = std::chrono::steady_clock::now();
        }
    }
}

void VRRenderThread::setupLights() {
    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition(5, 5, 15);
    light->SetPositional(true);
    light->SetConeAngle(10);
    light->SetFocalPoint(0, 0, 0);
    light->SetDiffuseColor(1, 1, 1);
    light->SetAmbientColor(1, 1, 1);
    light->SetSpecularColor(1, 1, 1);
    light->SetIntensity(0.5);
    renderer->AddLight(light);
}

void VRRenderThread::setupFilters() {
    vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New();
    planeLeft->SetOrigin(0.0, 0.0, 0.0);
    planeLeft->SetNormal(-1.0, 0.0, 0.0);

    vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    // Assuming there is a source of data named "dataSource", connect its output port to the input connection of the clip filter
    clipFilter->SetInputConnection(/* dataSource->GetOutputPort() */); // Replace dataSource with your actual data source
    clipFilter->SetClipFunction(planeLeft);
    clipFilter->Update();

    vtkSmartPointer<vtkShrinkFilter> shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
    shrinkFilter->SetInputData(clipFilter->GetOutput()); // Set the output of the clip filter as input data for the shrink filter
    shrinkFilter->Update(); // Update the shrink filter after setting the input data

    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(shrinkFilter->GetOutputPort());

    actors->InitTraversal();
    vtkActor* a;
    while ((a = (vtkActor*)actors->GetNextActor())) {
        vtkSmartPointer<vtkActor> clippedActor = vtkSmartPointer<vtkActor>::New(); // Create a new actor for each clipped actor
        vtkSmartPointer<vtkDataSetMapper> actorMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        actorMapper->SetInputConnection(mapper->GetOutputPort());
        clippedActor->SetMapper(actorMapper);
        renderer->AddActor(clippedActor);
    }
}





