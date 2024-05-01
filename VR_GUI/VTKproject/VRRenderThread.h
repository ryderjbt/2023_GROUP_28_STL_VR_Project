// VRRenderThread.h

#ifndef VR_RENDER_THREAD_H
#define VR_RENDER_THREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <chrono>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRCamera.h>
#include <vtkActorCollection.h>
#include <vtkCommand.h>

class VRRenderThread : public QThread {
    Q_OBJECT

public:

    enum Command {
        END_RENDER,
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z
    };

    VRRenderThread(QObject* parent = nullptr);

    ~VRRenderThread() override;

    void addActorOffline(vtkActor* actor);
    void issueCommand(Command cmd, double value);

protected:

    void run() override;

private:

    vtkSmartPointer<vtkOpenVRRenderWindow> window;
    vtkSmartPointer<vtkOpenVRRenderWindowInteractor> interactor;
    vtkSmartPointer<vtkOpenVRRenderer> renderer;
    vtkSmartPointer<vtkOpenVRCamera> camera;

    QMutex mutex;
    QWaitCondition condition;

    vtkSmartPointer<vtkActorCollection> actors;

    std::chrono::time_point<std::chrono::steady_clock> t_last;

    bool endRender;

    double rotateX;         // Degrees to rotate around X axis (per time-step)
    double rotateY;         // Degrees to rotate around Y axis (per time-step)
    double rotateZ;         // Degrees to rotate around Z axis (per time-step)
};

#endif
