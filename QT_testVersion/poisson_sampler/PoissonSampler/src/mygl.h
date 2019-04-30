#ifndef MYGL_H
#define MYGL_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QRubberBand>
#include <QMouseEvent>

#include <QTime>
#include <QSound>
#include <QStringRef>
#include <QFile>

#include "openGL/glwidget277.h"
#include "openGL/shaderprogram.h"
#include "scene/camera.h"
#include "scene/scene.h"
#include "scene/mesh.h"
#include "scene/grid.h"
#include "la.h"

class Scene;
class Mesh;
class COM;

class MyGL
    : public GLWidget277
{
    Q_OBJECT
private:
    ShaderProgram prog_lambert;
    ShaderProgram prog_flat;

    // vertex attribute obj for our shaders
    QOpenGLVertexArrayObject vao;

    Camera gl_camera;

    // Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;
    int timeCount;

    Scene* scene;

    QString output_filepath;

    Mesh* mesh_source;
    Mesh* mesh_target;
    Grid* grid;

    bool view_VOXELS;
    bool view_OBJ;

    bool using_com;

    COM* com_obj;

public:
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void GLDrawScene();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();

public slots:

    void slot_loadSourceObj();
    void slot_loadTargetObj();

    void slot_viewVOXELS(bool b);
    void slot_viewOBJ(bool b);

    void slot_calculateVolumeOfPoissonObj();
    void slot_calculateCenterOfMass();

signals:
    void sig_ResizeToCamera(int,int);
    void sig_DisableGUI(bool);
};

#endif // MYGL_H
