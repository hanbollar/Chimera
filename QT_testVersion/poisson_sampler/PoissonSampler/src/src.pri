INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
#LIBS += -L$$PWD/lib -ltbb

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/mygl.cpp \
    $$PWD/scene/camera.cpp \
    $$PWD/scene/scene.cpp \
    $$PWD/openGL/drawable.cpp \
    $$PWD/openGL/glwidget277.cpp \
    $$PWD/openGL/shaderprogram.cpp \
    $$PWD/raytracing/intersection.cpp \
    $$PWD/scene/transform.cpp \
    $$PWD/tinyobj/tiny_obj_loader.cc \
    $$PWD/cameracontrolshelp.cpp \
    $$PWD/samplers/sampler.cpp \
    $$PWD/scene/mesh.cpp \
    $$PWD/scene/bounds.cpp \
    $$PWD/scene/grid.cpp \
    $$PWD/scene/kdtree.cpp \
    $$PWD/scene/triangle.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/mygl.h \
    $$PWD/scene/camera.h \
    $$PWD/la.h \
    $$PWD/scene/scene.h \
    $$PWD/openGL/drawable.h \
    $$PWD/openGL/glwidget277.h \
    $$PWD/openGL/shaderprogram.h \
    $$PWD/raytracing/intersection.h \
    $$PWD/scene/transform.h \
    $$PWD/tinyobj/tiny_obj_loader.h \
    $$PWD/cameracontrolshelp.h \
    $$PWD/globals.h \
    $$PWD/samplers/sampler.h \
    $$PWD/scene/mesh.h \
    $$PWD/scene/bounds.h \
    $$PWD/scene/grid.h \
    $$PWD/scene/kdtree.h \
    $$PWD/scene/triangle.h \
    $$PWD/scene/tableinfo.h
