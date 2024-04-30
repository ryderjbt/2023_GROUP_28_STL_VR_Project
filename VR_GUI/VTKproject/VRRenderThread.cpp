#include "VRRenderThread.h"

VRRenderThread::VRRenderThread(QObject *parent) : QThread(parent) {
    setupVRRenderer();
}

VRRenderThread::~VRRenderThread() {
    // Clean up resources
    vrRenderWindow->Finalize();
}

void VRRenderThread::addActorOffline(vtkSmartPointer<vtkActor> actor) {
    vrActors.append(actor);
}

void VRRenderThread::startVR() {
    start();
}

void VRRenderThread::run() {
    renderVRActors();
    emit vrRenderingFinished();
}

void VRRenderThread::setupVRRenderer() {
    vrRenderer = vtkSmartPointer<vtkRenderer>::New();
    vrRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    vrRenderWindow->AddRenderer(vrRenderer);
    vrRenderWindow->SetStereoRender(1); // Enable stereo rendering
}

void VRRenderThread::renderVRActors() {
    for (int i = 0; i < vrActors.size(); ++i) {
        vrRenderer->AddActor(vrActors.at(i));
    }
    vrRenderWindow->Render();
}
