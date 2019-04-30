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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QGroupBox *groupBox_2;
    QWidget *layoutWidget_3;
    QGridLayout *gridLayout_4;
    QLabel *label_11;
    QCheckBox *checkBox_viewBVH;
    QLabel *label_13;
    QPushButton *loadPoissonObj;
    QPushButton *poisson;
    QCheckBox *checkBox_viewOBJ;
    QPushButton *poissonVOLUME;
    QPushButton *poissonCOM;
    QPushButton *poissonVERTEXOFFSET;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(651, 747);
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

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMinimumSize(QSize(631, 121));
        groupBox_2->setBaseSize(QSize(631, 121));
        layoutWidget_3 = new QWidget(groupBox_2);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(10, 20, 611, 134));
        gridLayout_4 = new QGridLayout(layoutWidget_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_4->addWidget(label_11, 5, 0, 1, 1);

        checkBox_viewBVH = new QCheckBox(layoutWidget_3);
        checkBox_viewBVH->setObjectName(QStringLiteral("checkBox_viewBVH"));

        gridLayout_4->addWidget(checkBox_viewBVH, 2, 1, 1, 1);

        label_13 = new QLabel(layoutWidget_3);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_4->addWidget(label_13, 1, 1, 1, 1);

        loadPoissonObj = new QPushButton(layoutWidget_3);
        loadPoissonObj->setObjectName(QStringLiteral("loadPoissonObj"));

        gridLayout_4->addWidget(loadPoissonObj, 1, 0, 1, 1);

        poisson = new QPushButton(layoutWidget_3);
        poisson->setObjectName(QStringLiteral("poisson"));

        gridLayout_4->addWidget(poisson, 2, 0, 1, 1);

        checkBox_viewOBJ = new QCheckBox(layoutWidget_3);
        checkBox_viewOBJ->setObjectName(QStringLiteral("checkBox_viewOBJ"));
        checkBox_viewOBJ->setChecked(true);

        gridLayout_4->addWidget(checkBox_viewOBJ, 5, 1, 1, 1);

        poissonVOLUME = new QPushButton(layoutWidget_3);
        poissonVOLUME->setObjectName(QStringLiteral("poissonVOLUME"));

        gridLayout_4->addWidget(poissonVOLUME, 4, 0, 1, 1);

        poissonCOM = new QPushButton(layoutWidget_3);
        poissonCOM->setObjectName(QStringLiteral("poissonCOM"));

        gridLayout_4->addWidget(poissonCOM, 4, 1, 1, 1);

        poissonVERTEXOFFSET = new QPushButton(layoutWidget_3);
        poissonVERTEXOFFSET->setObjectName(QStringLiteral("poissonVERTEXOFFSET"));

        gridLayout_4->addWidget(poissonVERTEXOFFSET, 3, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 651, 21));
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
        QObject::connect(poisson, SIGNAL(clicked()), mygl, SLOT(slot_poissonClicked()));
        QObject::connect(loadPoissonObj, SIGNAL(clicked()), mygl, SLOT(slot_loadPoissonObj()));
        QObject::connect(checkBox_viewBVH, SIGNAL(clicked(bool)), mygl, SLOT(slot_viewPBVH(bool)));
        QObject::connect(checkBox_viewOBJ, SIGNAL(clicked(bool)), mygl, SLOT(slot_viewOBJ(bool)));
        QObject::connect(poissonVOLUME, SIGNAL(clicked()), mygl, SLOT(slot_calculateVolumeOfPoissonObj()));
        QObject::connect(poissonCOM, SIGNAL(clicked()), mygl, SLOT(slot_calculateCenterOfMass()));
        QObject::connect(poissonVERTEXOFFSET, SIGNAL(clicked()), mygl, SLOT(slot_calculateVertexOffset()));

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
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Controls", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        checkBox_viewBVH->setText(QApplication::translate("MainWindow", "View BVH", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        loadPoissonObj->setText(QApplication::translate("MainWindow", "Load Poisson Obj", Q_NULLPTR));
        poisson->setText(QApplication::translate("MainWindow", "Poisson", Q_NULLPTR));
        checkBox_viewOBJ->setText(QApplication::translate("MainWindow", "View OBJ", Q_NULLPTR));
        poissonVOLUME->setText(QApplication::translate("MainWindow", "Calculate Poisson Volume", Q_NULLPTR));
        poissonCOM->setText(QApplication::translate("MainWindow", "Calculate Poisson Center Of Mass", Q_NULLPTR));
        poissonVERTEXOFFSET->setText(QApplication::translate("MainWindow", "Calculate VertexOffset Between Meshes", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
