// VRRenderThread.cpp

#include "VRRenderThread.h"
#include <vtkLight.h>
#include <vtkPlane.h>
#include <vtkClipDataSet.h>

VRRenderThread::VRRenderThread(QObject *parent) : QThread(parent)
{
    // Initialize renderer, render window, and interactor
    m_renderer = vtkSmartPointer<vtkRenderer>::New();
    m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    m_renderWindow->AddRenderer(m_renderer);
    m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_interactor->SetRenderWindow(m_renderWindow);
}

VRRenderThread::~VRRenderThread()
{
    // Stop the interactor
    m_interactor->TerminateApp();
    m_interactor->UnRegister(nullptr);
}

void VRRenderThread::addActorOffline(vtkSmartPointer<vtkActor> actor)
{
    // Add actor to the list of actors for VR rendering
    m_vrActors.append(actor);
}

void VRRenderThread::addLight()
{
    // Create a scene light
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

    // Add the light to the renderer
    m_renderer->AddLight(light);
    m_lights.append(light);
}

void VRRenderThread::applyFilters()
{
    // Apply filters to modify the rendered data (if needed)
    vtkSmartPointer<vtkPlane> clipPlane = vtkSmartPointer<vtkPlane>::New();
    clipPlane->SetOrigin(0.0, 0.0, 0.0);
    clipPlane->SetNormal(-1.0, 0.0, 0.0);

    vtkSmartPointer<vtkClipDataSet> clipFilter = vtkSmartPointer<vtkClipDataSet>::New();
    clipFilter->SetInputConnection(SOURCE->GetOutputPort());
    clipFilter->SetClipFunction(clipPlane);

    // Set up the mapper with the filtered data
    MAPPER->SetInputConnection(clipFilter->GetOutputPort());
}

void VRRenderThread::run()
{
    // Add actors to the renderer
    for (vtkSmartPointer<vtkActor> actor : m_vrActors)
    {
        m_renderer->AddActor(actor);
    }

    // Add lights to the scene
    for (vtkSmartPointer<vtkLight> light : m_lights)
    {
        m_renderer->AddLight(light);
    }

    // Start the rendering loop
    m_renderWindow->Render();
    m_interactor->Start();
}
