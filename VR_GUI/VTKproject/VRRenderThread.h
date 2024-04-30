// VRRenderThread.h

#ifndef VRRENDERTHREAD_H
#define VRRENDERTHREAD_H

#include <QThread>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

// Forward declaration of ModelPart class
class ModelPart;

class VRRenderThread : public QThread
{
    Q_OBJECT

public:
    explicit VRRenderThread(QObject *parent = nullptr);
    ~VRRenderThread() override;

    // Function to add actors for VR rendering
    void addActorOffline(vtkSmartPointer<vtkActor> actor);

    // Function to set up lighting in the scene
    void addLight();

    // Function to apply filters to modify the rendered data
    void applyFilters();

protected:
    // Overridden run function for the thread
    void run() override;

private:
    vtkSmartPointer<vtkRenderer> m_renderer; // Renderer for VR rendering
    vtkSmartPointer<vtkRenderWindow> m_renderWindow; // Render window for VR rendering
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor; // Interactor for VR rendering

    // List to store actors for VR rendering
    QList<vtkSmartPointer<vtkActor>> m_vrActors;

    // List to store lights in the scene
    QList<vtkSmartPointer<vtkLight>> m_lights;
};

#endif // VRRENDERTHREAD_H
