/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    MyGL *mygl;
    QPushButton *loadSourceObj;
    QPushButton *loadTargetObj;
    QPushButton *sourceVOLUME;
    QPushButton *creategrid;
    QPushButton *stepinterp;
    QCheckBox *checkBox_viewSourceOBJ;
    QSpinBox *viewWhichVoxel;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(657, 747);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QStringLiteral("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QStringLiteral("mygl"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(mygl->sizePolicy().hasHeightForWidth());
        mygl->setSizePolicy(sizePolicy);
        mygl->setMinimumSize(QSize(618, 432));

        verticalLayout->addWidget(mygl);

        loadSourceObj = new QPushButton(centralWidget);
        loadSourceObj->setObjectName(QStringLiteral("loadSourceObj"));

        verticalLayout->addWidget(loadSourceObj);

        loadTargetObj = new QPushButton(centralWidget);
        loadTargetObj->setObjectName(QStringLiteral("loadTargetObj"));

        verticalLayout->addWidget(loadTargetObj);

        sourceVOLUME = new QPushButton(centralWidget);
        sourceVOLUME->setObjectName(QStringLiteral("sourceVOLUME"));

        verticalLayout->addWidget(sourceVOLUME);

        creategrid = new QPushButton(centralWidget);
        creategrid->setObjectName(QStringLiteral("creategrid"));

        verticalLayout->addWidget(creategrid);

        stepinterp = new QPushButton(centralWidget);
        stepinterp->setObjectName(QStringLiteral("stepinterp"));

        verticalLayout->addWidget(stepinterp);

        checkBox_viewSourceOBJ = new QCheckBox(centralWidget);
        checkBox_viewSourceOBJ->setObjectName(QStringLiteral("checkBox_viewSourceOBJ"));
        checkBox_viewSourceOBJ->setEnabled(true);
        checkBox_viewSourceOBJ->setChecked(true);

        verticalLayout->addWidget(checkBox_viewSourceOBJ);

        viewWhichVoxel = new QSpinBox(centralWidget);
        viewWhichVoxel->setObjectName(QStringLiteral("viewWhichVoxel"));
        viewWhichVoxel->setMaximum(2);

        verticalLayout->addWidget(viewWhichVoxel);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 657, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);
        QObject::connect(mygl, SIGNAL(sig_ResizeToCamera(int,int)), MainWindow, SLOT(slot_ResizeToCamera(int,int)));
        QObject::connect(mygl, SIGNAL(sig_DisableGUI(bool)), MainWindow, SLOT(slot_DisableGUI(bool)));
        QObject::connect(loadSourceObj, SIGNAL(clicked()), mygl, SLOT(slot_loadSourceObj()));
        QObject::connect(sourceVOLUME, SIGNAL(clicked()), mygl, SLOT(slot_calculateVolumeOfPoissonObj()));
        QObject::connect(loadTargetObj, SIGNAL(clicked()), mygl, SLOT(slot_loadTargetObj()));
        QObject::connect(checkBox_viewSourceOBJ, SIGNAL(clicked(bool)), mygl, SLOT(slot_viewOBJ(bool)));
        QObject::connect(creategrid, SIGNAL(clicked()), mygl, SLOT(slot_createGrid()));
        QObject::connect(stepinterp, SIGNAL(clicked(bool)), mygl, SLOT(slot_updateInterp()));
        QObject::connect(viewWhichVoxel, SIGNAL(valueChanged(int)), mygl, SLOT(slot_viewWHICHVOXELS(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Poisson Sampler", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionCamera_Controls->setText(QApplication::translate("MainWindow", "Camera Controls", Q_NULLPTR));
        loadSourceObj->setText(QApplication::translate("MainWindow", "Load Source Obj", Q_NULLPTR));
        loadTargetObj->setText(QApplication::translate("MainWindow", "Load Target Obj", Q_NULLPTR));
        sourceVOLUME->setText(QApplication::translate("MainWindow", "Calculate Source Volume", Q_NULLPTR));
        creategrid->setText(QApplication::translate("MainWindow", "Create Grid", Q_NULLPTR));
        stepinterp->setText(QApplication::translate("MainWindow", "step interp", Q_NULLPTR));
        checkBox_viewSourceOBJ->setText(QApplication::translate("MainWindow", "view Source OBJ", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
