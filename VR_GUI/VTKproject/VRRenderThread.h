#ifndef VRRENDERTHREAD_H
#define VRRENDERTHREAD_H

#include <QThread>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

class VRRenderThread : public QThread {
    Q_OBJECT

public:
    VRRenderThread(QObject *parent = nullptr);
    ~VRRenderThread();

    void addActorOffline(vtkSmartPointer<vtkActor> actor);
    void startVR();

signals:
    void vrRenderingFinished();

protected:
    void run() override;

private:
    vtkSmartPointer<vtkRenderer> vrRenderer;
    vtkSmartPointer<vtkRenderWindow> vrRenderWindow;
    QList<vtkSmartPointer<vtkActor>> vrActors;

    void setupVRRenderer();
    void renderVRActors();
};

#endif // VRRENDERTHREAD_H
