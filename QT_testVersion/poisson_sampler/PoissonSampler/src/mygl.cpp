#include "mygl.h"
#include <la.h>

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>

#include <QApplication>
#include <QKeyEvent>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <QXmlStreamReader>
#include <QFileDialog>
#include <QOpenGLTexture>

#include <QDateTime>

#include <openGL/drawable.h>
#include <vector>


MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent),
      gl_camera(),
      scene(new Scene()),
      mesh_source(nullptr), mesh_target(nullptr), grid(nullptr),
      view_VOXELS(true), view_OBJ(true),
      using_com(false), com_obj(nullptr),
      grid_show_target(false)
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);

    this->setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL() {
    makeCurrent();
    vao.destroy();
}

void MyGL::initializeGL() {
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(6);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.2, 0.2, 0.2, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    vao.create();

    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    vao.bind();

    //  WENLI TESTING RANDOM THINGS HERE ////////////////////////////
    Triangle tri = Triangle(Vector3f(0,0,0), Vector3f(0,1,0), Vector3f(0,0,1));
    std::cout<< "HELLO WENLI" <<std::endl;
    Point3f source(0,24,0);
    Point3f closestPoint = tri.GetClosestPointToAPoint(source);
    std::cout << closestPoint.x << " ," << closestPoint.y << " ," << closestPoint.z << std::endl;

    std::vector<Triangle*> tris;
    KDNode node = KDNode(tris, Vector3f(0,0,0), Vector3f(1,1,1) );
    float dist = node.GetClosestDistanceToPoint(Vector3f(-1.0f,12.0f,-1.0f));
    std::cout << "Distance to box is : " << dist << std::endl;
}

void MyGL::resizeGL(int w, int h) {
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    gl_camera = Camera(w, h);
    glm::mat4 viewproj = gl_camera.GetViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)
    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);
    prog_lambert.setModelMatrix(glm::mat4(1.0f));
    prog_flat.setModelMatrix(glm::mat4(1.0f));

    printGLErrorLog();
}

void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the viewproj matrix
    prog_lambert.setViewProjMatrix(gl_camera.GetViewProj());
    prog_flat.setViewProjMatrix(gl_camera.GetViewProj());
    GLDrawScene();
}

void MyGL::GLDrawScene()
{
    prog_lambert.setModelMatrix(glm::mat4(1.0f));


    if (view_OBJ) {
        if (mesh_source) {
            prog_lambert.draw(*this, *mesh_source);
        }
        if (mesh_target) {
            prog_lambert.draw(*this, *mesh_target);
        }
    }
    if (view_VOXELS) {
        if (grid) {
            prog_flat.draw(*this, *grid);
        }
    }

    glDisable(GL_DEPTH_TEST);
    if (using_com) {
        prog_flat.draw(*this, *com_obj);
    }
    glEnable(GL_DEPTH_TEST);

}

void MyGL::timerUpdate() {
    // This function is called roughly 60 times per second.
    // Use it to perform any repeated actions you want to do,

    timeCount++;
}

void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }

    bool invalid_key = false;

    // http://doc.qt.io/qt-5/qt.html#Key-enum
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_F) {
        gl_camera.CopyAttributes(scene->camera);
    } else if (e->key() == Qt::Key_G) {
        scene->camera = Camera(gl_camera);
    } else
    {
        invalid_key = true;
    }

    if (!invalid_key)
    {
        gl_camera.RecomputeAttributes();

        update();  // Calls paintGL, among other things
    }
}

void MyGL::slot_loadSourceObj() {
    QString filepath = QFileDialog::getOpenFileName(0, QString("Load Obj"), QString("../scene_files"), tr("*.obj"));
    std::cout<<"filepath:"<<filepath.toStdString()<<std::endl;

    if(filepath.length() == 0) {
        return;
    }

    int i = filepath.length() - 1;
    while(QString::compare(filepath.at(i), QChar('/')) != 0) {
        --i;
    }
    QStringRef local_path = filepath.leftRef(i+1);
    QStringRef file_name = filepath.rightRef(filepath.length() - 1 - (i));

    Transform t = Transform();
    mesh_source = new Mesh();
    mesh_source->LoadOBJ(file_name, local_path, t);
    mesh_source->create();

    scene->all_mesh.push_back(mesh_source);
    this->update();

    using_com = false;
}

void MyGL::slot_loadTargetObj() {
    QString filepath = QFileDialog::getOpenFileName(0, QString("Load Obj"), QString("../scene_files"), tr("*.obj"));
    std::cout<<"filepath:"<<filepath.toStdString()<<std::endl;

    if(filepath.length() == 0) {
        return;
    }

    int i = filepath.length() - 1;
    while(QString::compare(filepath.at(i), QChar('/')) != 0) {
        --i;
    }
    QStringRef local_path = filepath.leftRef(i+1);
    QStringRef file_name = filepath.rightRef(filepath.length() - 1 - (i));

    Transform t = Transform();
    mesh_target = new Mesh();
    mesh_target->LoadOBJ(file_name, local_path, t);
    mesh_target->create();
}

void MyGL::slot_calculateVolumeOfPoissonObj() {
    if (!mesh_source) {
        return;
    }
    float volume = 0;
    for (Triangle* f : mesh_source->faces) {
        volume += glm::dot(glm::cross(f->points[0], f->points[1]), f->points[2]);
    }
    std::cout << "VOLUME : " << volume << std::endl;
}

void MyGL::slot_calculateCenterOfMass() {
    // barycenter of all faces
    // average of these based on surface area weighting

    if (!mesh_source) {
        return;
    }
    glm::vec3 barycenter(0.f);
    float totalSA = 0.f;
    for (Triangle* f : mesh_source->faces) {
        glm::vec3 centroid_onTriangle = (f->points[0] + f->points[1] + f->points[2]) / 3.f;
        float surfaceArea_onTriangle = 0.5f * glm::length(glm::cross(f->points[0] - f->points[1], f->points[2] - f->points[1]));

        barycenter += centroid_onTriangle * surfaceArea_onTriangle;
        totalSA += surfaceArea_onTriangle;
    }
    barycenter /= totalSA;
    using_com = true;
    com_obj = new COM();
    com_obj->loc = barycenter;
    com_obj->create();
    std::cout << "BARYCENTER : " << barycenter[0] << ", " << barycenter[1] << ", " << barycenter[2] << std::endl;
}

void MyGL::slot_viewVOXELS(bool b) {
    view_VOXELS = b;
    this->update();
}

void MyGL::slot_viewOBJ(bool b) {
    view_OBJ = b;
    this->update();
}

void MyGL::slot_viewTARGETVOXELS(bool b) {
    grid_show_target = b;
    grid->create(grid_show_target);
    update();
}

void MyGL::slot_createGrid() {
    if (!mesh_source || !mesh_target) {
        return;
    }

    grid = new Grid(mesh_source->faces, mesh_target->faces);\
    std::cout << "computing for source" << std::endl;
    grid->ComputeSourceSignedDistanceFunctions(mesh_source);
    std::cout << "computing for target" << std::endl;
    grid->ComputeTargetSignedDistanceFunctions(mesh_target);
    std::cout << "creating" << std::endl;
    grid_show_target = true;
    grid->create(grid_show_target);

    std::cout << "updating " << std::endl;

    this->update();
}
