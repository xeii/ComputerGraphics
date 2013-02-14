#ifndef WIDGET_H
#define WIDGET_H

#include <QtOpenGL>
#include <glu.h>
#include <iostream>
#include "flag.h"
#include "display.h"
#include <QVector3D>
#include <QList>
#include <QObject>
#include <QtDebug>
#include <QTimer>
#include <stdlib.h>
#include <QGLShader>

class Widget : public QGLWidget {
    Q_OBJECT
private:
    static const int _particleCount = 200;
    static const int _particleLifeSpan = 40000;
    static const int _velocityDivider = 10000;
    static const qreal _gravity = 3.0f;
    static const qreal _fraction = 0.7;
    bool gravityFlag;
    int collCount;
    QList<flag*> particles;
    struct Plane {
        QVector3D planePoint;
        QVector3D planeNormal;
    };

public:
    Widget(QWidget *parent = NULL);
    int randInt(int low, int high);
protected:
    bool debug;
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void keyPressEvent(QKeyEvent *keyEvent);
    void drawPickingMode();
    void loadObj(QString filename, flag *object);
    void loadMaterial(QString filename, flag * object);
    void camera(void);
    void lights(void);
    void drawScene(flag *parent);
    void createParticles();
    void updateParticles();
    void LoadShader(QString vshader, QString fshader);
    QList<Widget::Plane *> checkPlaneCollision(QVector3D location, QVector3D velocity);
    void calcNewVelocity(flag * particle, Plane *plane);
    void randomVelocity(flag * particle);
    QVector3D getWorldLocation(flag * object);
    QList <flag*> flags;
    QList <Plane*> planes;
    Plane *ground, *top, *back, *front, *left, *right;
    flag *scene, *cylinder, *cylinder2, *cylinder3, *plane, *particleCore;
    flag light1, light2, light3, light4;
    QVector3D camPos;
    QVector3D camDirection;
    QPoint lastPos;
    float xzAngle; //varies from [0, 360]
    float yAngle; //varies from  [-90, 90]
    qreal timerTick, particleTick;
    flag * selectedCylinder;
    display display1;
signals:
    void drawFlags();
    void moveFlags(QVector3D);
    void drawObject();
public slots:
    void timerUpdate();
};

#endif // WIDGET_H
