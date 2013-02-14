#include "widget.h"

Widget::Widget(QWidget *parent) : QGLWidget(parent) {
    setMouseTracking(true);

    //QObject::connect(this, SIGNAL(drawFlags()), &room2, SLOT(drawroom2()));
    //QObject::connect(this, SIGNAL(drawFlags()), &room1, SLOT(drawroom1()));
    //QObject::connect(this, SIGNAL(drawObject()), &room1, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), &room2, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), &gridA, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), &gridB, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), &light1, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), &light2, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), &light3, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(moveFlags(QVector3D)), &room2, SLOT(move(QVector3D)));
    //QObject::connect(this, SIGNAL(moveFlags(QVector3D)), &room1, SLOT(move(QVector3D)));

    //this->flags.append(&room2);
    //this->flags.append(&room1);
}


void Widget::initializeGL()
{
    collCount=0;
    debug = true;
    ground = new Plane();
    top = new Plane();
    back = new Plane();
    front = new Plane();
    left = new Plane();
    right = new Plane();

    ground->planeNormal.setX(0.0f);
    ground->planeNormal.setY(1.0f);
    ground->planeNormal.setZ(0.0f);

    ground->planePoint.setX(0.0f);
    ground->planePoint.setY(-10.0f);
    ground->planePoint.setZ(0.0f);

    top->planeNormal.setX(0.0f);
    top->planeNormal.setY(-1.0f);
    top->planeNormal.setZ(0.0f);

    top->planePoint.setX(0.0f);
    top->planePoint.setY(10.0f);
    top->planePoint.setZ(0.0f);

    back->planeNormal.setX(0.0f);
    back->planeNormal.setY(0.0f);
    back->planeNormal.setZ(1.0f);

    back->planePoint.setX(0.0f);
    back->planePoint.setY(0.0f);
    back->planePoint.setZ(-10.0f);

    front->planeNormal.setX(0.0f);
    front->planeNormal.setY(0.0f);
    front->planeNormal.setZ(-1.0f);

    front->planePoint.setX(0.0f);
    front->planePoint.setY(0.0f);
    front->planePoint.setZ(10.0f);

    left->planeNormal.setX(1.0f);
    left->planeNormal.setY(0.0f);
    left->planeNormal.setZ(0.0f);

    left->planePoint.setX(-10.0f);
    left->planePoint.setY(0.0f);
    left->planePoint.setZ(0.0f);

    right->planeNormal.setX(-1.0f);
    right->planeNormal.setY(0.0f);
    right->planeNormal.setZ(0.0f);

    right->planePoint.setX(10.0f);
    right->planePoint.setY(0.0f);
    right->planePoint.setZ(0.0f);


    planes.append(ground);
    planes.append(top);
    planes.append(back);
    planes.append(front);
    planes.append(left);
    planes.append(right);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(10);
    timerTick = 0.0;
    particleTick = _particleCount;

    this->camPos.setX(0.0f);
    this->camPos.setY(10.0f);
    this->camPos.setZ(-100.0f);
    this->camDirection.setX(0.0f);
    this->camDirection.setY(0.0f);
    this->camDirection.setZ(0.0f);
    this->xzAngle = 0.0f;
    this->yAngle = 0.0f;

    QString filename  = "cylinder.obj";
    QString filename2 = "room2.obj";
    QString filename3 = "gridA.obj";
    QString filename4 = "gridB.obj";
    QString filename5 = "SCone.obj";

    qDebug() << "Loading object Files\n";

    scene = new flag();
    plane = new flag(scene);
    cylinder = new flag(scene);
    cylinder2 = new flag(cylinder);
    cylinder3 = new flag(cylinder2);
    particleCore = new flag(cylinder2);

    selectedCylinder = cylinder;

    //QObject::connect(this, SIGNAL(drawObject()), cylinder, SLOT(drawObject()));
    //QObject::connect(this, SIGNAL(drawObject()), cylinder2, SLOT(drawObject()));

    loadObj("plane.obj", plane);

    loadObj(filename, cylinder);
    loadMaterial("cylinder.mtl", cylinder);
    cylinder->initTextures();

    loadObj(filename, cylinder2);
    loadMaterial("cylinder.mtl", cylinder2);
    cylinder2->initTextures();

    loadObj(filename, cylinder3);
    loadMaterial("cylinder.mtl", cylinder3);
    cylinder3->initTextures();

    loadObj("sphere.obj", particleCore);

    plane->scale.setX(10.0f);
    plane->scale.setY(10.0f);
    plane->scale.setZ(10.0f);

    plane->location.setY(-10.0f);

    cylinder->location.setY(0.0f);
    cylinder->location.setZ(0.0f);
    light1.location.setY(20.0f);
    light1.location.setX(20.0f);

    light2.location.setZ(-20.0f);
    light2.location.setY(0.0f);
    cylinder2->location.setZ(0.0f);
    cylinder2->location.setY(3.2f);

    particleCore->location.setY(2.0f);

    light3.location.setZ(-10.0f);
    light3.location.setY(20.0f);
    light3.angle = 180.0f;
    light3.orientation.setX(1.0f);

    glEnable(GL_DEPTH_TEST);


   // glEnable(GL_LIGHTING);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glShadeModel(GL_SMOOTH);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    glEnable(GL_TEXTURE_2D);

}

void Widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 1.0f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Widget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer

    glLoadIdentity();
    camera();
    lights();
    //emit drawObject();
    updateParticles();
    drawScene(scene);

    //qDebug() << "cylinder pos " << cylinder.location ;
    //this->renderText(10, 20, "Antti Väyrynen - 1930357", QFont("Helvetica [Cronyx]", 20), 2000);


    GLenum errCode;
    const GLubyte *errString;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: %s" << errCode;
    }
}

void Widget::createParticles() {
    int i = 0;
    flag* particle;

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    for(i=0;i<_particleCount;i++) {

        particle = new flag(scene);
        particle->moving =1;
        particle->lifeSpan = _particleLifeSpan;

        particle->location = getWorldLocation(particleCore);
        randomVelocity(particle);

        loadObj("sphere.obj", particle);
        particles.append(particle);
    }
}

QVector3D Widget::getWorldLocation(flag * object) {
    QMatrix4x4 mat = object->getMatrix();
    QVector3D location;
    QVector4D vect(0,0,0,1);
    flag * obj = object;
    while(obj->parent() != 0) {
        obj = (flag*) obj->parent();
        mat = obj->getMatrix() * mat;
    }
    vect = mat * vect;
    location = vect.toVector3DAffine();
    qDebug() << "particle start location: " << location << "\n";
    return location;
}

void Widget::randomVelocity(flag * particle) {
    qreal xzDirection = randInt(0, 359);
    qreal yDirection = randInt(-90, 90);

    //qDebug() << "xz: " << xzDirection << "     y: " << yDirection << "\n";

    particle->velocity.setZ(cos(xzDirection/180 * 3.141592654f) * cos(yDirection/180 * 3.141592654f) * 1000);
    particle->velocity.setX(sin(xzDirection/180 * 3.141592654f) * cos(yDirection/180 * 3.141592654f)* 1000);
    particle->velocity.setY(sin(yDirection/180 * 3.141592654f)* 1000);

    //qDebug() << "velocity: " << particle->velocity << "\n";

}


void Widget::updateParticles() {

    foreach(flag* particle, particles) {
        //particle->setParent(scene);

        QList<Plane *> k = checkPlaneCollision(particle->location, particle->velocity);
        if(k.count() != 0) {
            //collCount++;
            //qDebug() << "COLLISION COUNT: " << collCount << "\n";
            foreach(Plane * plane, k)
                calcNewVelocity(particle, plane);
        }
        particle->location.setX(particle->location.x() + particle->velocity.x()/_velocityDivider);
        particle->location.setY(particle->location.y() + particle->velocity.y()/_velocityDivider);
        particle->location.setZ(particle->location.z() + particle->velocity.z()/_velocityDivider);

//        if(particle->velocity.length() < _velocityCutoff) {
//            particle->moving = -1;
//            particle->velocity.setX(0.0f);
//            particle->velocity.setY(0.0f);
//            particle->velocity.setZ(0.0f);
//        }

        particle->lifeSpan -= 10;

        if(particle->lifeSpan < 0) {
            particles.removeOne(particle);
            delete particle;
        }
    }
}

void Widget::calcNewVelocity(flag * particle, Plane * plane) {
    QVector3D newNormVelocity;
    QVector3D normal = plane->planeNormal;

    QVector3D normalizedVelocity = particle->velocity.normalized();
    //qDebug() << "normalized veloc: " << normalizedVelocity << "\n";
    qreal lengthVelocity = particle->velocity.length();

//    tempScalar = normalizedVelocity.dotProduct(plane->planePoint, plane->planeNormal);
//    temp3D = normal.operator *=(-2 * tempScalar);
//    newNormVelocity = temp3D.operator +=(normalizedVelocity).normalized();

    newNormVelocity = (-2 * (QVector3D::dotProduct(normalizedVelocity, normal)) *  normal + normalizedVelocity).normalized();
    //qDebug() << "new normal veloc: " << newNormVelocity << "\n";

    if(gravityFlag)
        particle->velocity = newNormVelocity.operator *=( lengthVelocity) *_fraction;
    else
        particle->velocity = newNormVelocity.operator *=( lengthVelocity) ;//* _fraction;
    //qDebug() << "new velocity: " << particle->velocity;
    // R= 2*(-NV dot PlN)*PlN + NV
    // Vnew = -2*(V dot N)*N + V


}

QList<Widget::Plane *> Widget::checkPlaneCollision(QVector3D location, QVector3D velocity) {
    // t= (Xn dot (PointOnPLANE - Raystart)) / (Xn dot Raydirection)
    qreal directionProduct;
    qreal collisionDist;
    QVector3D newLocation;
    qreal newLocCollisionDist;
    QVector3D normVelocity = velocity;
    QVector3D tempVeloc;
    QVector3D tempLoc;
    QList<Plane *> collidingPlanes;
    normVelocity.normalize();
    foreach(Plane * plane, planes) {
        tempVeloc = velocity;
        tempLoc = location;
        directionProduct = QVector3D::dotProduct(normVelocity, plane->planeNormal);
        if (directionProduct == 0)
            continue;

        collisionDist = location.distanceToPlane(plane->planePoint, plane->planeNormal);
        if(collisionDist < 0)
            continue;

        newLocation = tempLoc.operator +=(tempVeloc.operator /=(_velocityDivider));
        newLocCollisionDist = newLocation.distanceToPlane(plane->planePoint, plane->planeNormal);
        if(newLocCollisionDist < 0) {
            //qDebug() << "Collision with plane normal: " << plane->planeNormal ;
            collidingPlanes.append(plane);
        }
    }
    return collidingPlanes;
}

void Widget::drawScene(flag * parent) {
    FlagList children = parent->children();
    glPushMatrix();
    parent->drawObject();
    if(gravityFlag){
        if(parent->moving == 1)
            parent->velocity.setY(parent->velocity.y()-_gravity);
    }
    foreach(flag *flag, children) {
        drawScene(flag);
    }
    glPopMatrix();
}

//Draw objects with grey and then grab the pixel that was clicked and compare it to colors that were used
void Widget::mousePressEvent(QMouseEvent *mouseEvent) {
    /*
    drawPickingMode();

    GLint viewport[4];
    GLubyte pixel[3];

    glGetIntegerv(GL_VIEWPORT,viewport);

    glReadPixels(mouseEvent->x(),viewport[3]-mouseEvent->y(),1,1,
        GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);


    for(int i=0;i<this->flags.length();i++) {
        if(pixel[0] == i) {
            if(mouseEvent->buttons() & Qt::LeftButton) {
                flags[i]->toggleType();
            } else if(mouseEvent->buttons() & Qt::RightButton) {
                flags[i]->toggleMoving();
            }
        }
    }


    for(int i=0;i<this->flags.length();i++) {
        flags[i]->setPicking(-1);
    }
    updateGL();
    */
}

void Widget::timerUpdate() {
    timerTick += 0.2;
    if(timerTick > 360) timerTick = 0.0;
    updateGL();
}



void Widget::camera(void) {

    //qDebug() << "cam pos" << this->camPos;
    //qDebug() << "direction" << this->camDirection;

    /*
    glRotatef(this->camXrot,1.0,0.0,0.0);
    glRotatef(this->camYrot,0.0,1.0,0.0);
    glTranslatef(-this->camPos.x(),-this->camPos.y(),-this->camPos.z());
    */
    if(yAngle )
    camDirection.setZ(cos(xzAngle/180 * 3.141592654f) * cos(yAngle/180 * 3.141592654f));
    camDirection.setX(sin(xzAngle/180 * 3.141592654f) * cos(yAngle/180 * 3.141592654f));
    camDirection.setY(sin(yAngle/180 * 3.141592654f));
    if(camDirection.y() >= 1 || camDirection.y() <= -1) {
        camDirection.setY(0.9999999f);
    }
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(this->camPos.x(),
              this->camPos.y(),
              this->camPos.z(),
              this->camPos.x() + this->camDirection.x(),
              this->camPos.y() + this->camDirection.y(),
              this->camPos.z() + this->camDirection.z(),
              0.0f, 1.0f, 0.0f
              );

}

void Widget::lights(void) {

    //qDebug() << "light1: " << light1.location;
    //qDebug() << "light2: " << light2.location;

    //general attributes
    GLfloat spot_direction_down[] = { 0.0, -1.0, 0.0 };
    GLfloat spot_direction_left[] = { 0.0, 0.0, 1.0 };

    //first light
    GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //white light
    GLfloat light1_position[] = { light1.location.x(),light1.location.y(),light1.location.z(), 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction_down);
    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);

    //second light
    GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //white light
    GLfloat light2_position[] = { light2.location.x(),light2.location.y(),light2.location.z(), 1.0f};
    glLightfv(GL_LIGHT1, GL_SPECULAR, light2_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light2_specular);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction_left);
    glLightfv(GL_LIGHT1, GL_POSITION, light2_position);

    //third light
    GLfloat light3_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //white light
    GLfloat light3_position[] = { light3.location.x(),light3.location.y(),light3.location.z(), 1.0f};
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light3_specular);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light3_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light3_position);

    //ambient light
    GLfloat light7_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    glLightfv(GL_LIGHT7, GL_AMBIENT, light7_ambient);

}

void Widget::mouseMoveEvent(QMouseEvent* event) {
    qreal dx = (event->pos().x()-lastPos.x());
    qreal dy = (event->pos().y()-lastPos.y());
    if(event->buttons() & Qt::LeftButton)
    {
       xzAngle += dx/10;
       yAngle += dy/10;
       if(yAngle >= 90) yAngle = 89.999;
       if(yAngle <= -90) yAngle = -89.999;
       if(xzAngle >= 360) xzAngle -= 360;
       if(xzAngle < 0) xzAngle += 360;
    }

    lastPos = event->pos();
}

void Widget::keyPressEvent(QKeyEvent *keyEvent) {
    QVector3D moveVector;
    if(keyEvent->key() == Qt::Key_Up) {
        yAngle += 1.0f;
        if(yAngle >= 90) yAngle = 89.999;
        if(yAngle <= -90) yAngle = -89.999;
    } else if (keyEvent->key() == Qt::Key_Down) {
        yAngle -= 1.0f;
        if(yAngle >= 90) yAngle = 89.999;
        if(yAngle <= -90) yAngle = -89.999;
    } else if (keyEvent->key() == Qt::Key_Left) {
        if(xzAngle >= 360) xzAngle -= 360;
        if(xzAngle < 0) xzAngle += 360;
        xzAngle += 5.0f;
    } else if (keyEvent->key() == Qt::Key_Right) {
        if(xzAngle >= 360) xzAngle -= 360;
        if(xzAngle < 0) xzAngle += 360;
        xzAngle -= 5.0f;
    } else if (keyEvent->key() == Qt::Key_A) {
        camPos.setX(camPos.x() + camDirection.z());
        camPos.setZ(camPos.z() - camDirection.x());

    } else if (keyEvent->key() == Qt::Key_D) {
        camPos.setX(camPos.x() - camDirection.z());
        camPos.setZ(camPos.z() + camDirection.x());

    } else if (keyEvent->key() == Qt::Key_W) {
        camPos.setX(camPos.x() + camDirection.x());
        camPos.setY(camPos.y() + camDirection.y());
        camPos.setZ(camPos.z() + camDirection.z());

    } else if (keyEvent->key() == Qt::Key_S) {
        camPos.setX(camPos.x() - camDirection.x());
        camPos.setY(camPos.y() - camDirection.y());
        camPos.setZ(camPos.z() - camDirection.z());

    } else if (keyEvent->key() == Qt::Key_1) {
        this->selectedCylinder = cylinder;
    } else if (keyEvent->key() == Qt::Key_2) {
        this->selectedCylinder = cylinder2;
    } else if (keyEvent->key() == Qt::Key_3) {
        this->selectedCylinder = cylinder3;
    } else if (keyEvent->key() == Qt::Key_Y) {
        selectedCylinder->angle += 2.0f;
    } else if (keyEvent->key() == Qt::Key_U) {
        selectedCylinder->angle -= 2.0f;
    } else if (keyEvent->key() == Qt::Key_F) {
        selectedCylinder->orientation.setX(selectedCylinder->orientation.x()+1.0f);
    } else if (keyEvent->key() == Qt::Key_G) {
        selectedCylinder->orientation.setX(selectedCylinder->orientation.x()-1.0f);
    } else if (keyEvent->key() == Qt::Key_H) {
        selectedCylinder->orientation.setY(selectedCylinder->orientation.y()+1.0f);
    } else if (keyEvent->key() == Qt::Key_J) {
        selectedCylinder->orientation.setY(selectedCylinder->orientation.y()-1.0f);
    } else if (keyEvent->key() == Qt::Key_K) {
        selectedCylinder->orientation.setZ(selectedCylinder->orientation.z()+1.0f);
    } else if (keyEvent->key() == Qt::Key_L) {
        selectedCylinder->orientation.setZ(selectedCylinder->orientation.z()-1.0f);
    } else if (keyEvent->key() == Qt::Key_Z) {
        if(gravityFlag)
            gravityFlag = false;
        else
            gravityFlag = true;
    } else if (keyEvent->key() == Qt::Key_X) {
        createParticles();
    } else if (keyEvent->key() == Qt::Key_Escape) {
        QApplication::quit();
    }

    emit moveFlags(moveVector);
}


//Objects are drawn in picking mode with different grey scale colors
void Widget::drawPickingMode() {
    //glDisable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
    glLoadIdentity();
    glDisable(GL_DITHER);
    for(int i=0;i<this->flags.length();i++) {
        flags[i]->setPicking(i);
    }
    emit drawFlags();
    glEnable(GL_DITHER);
    //glEnable(GL_LIGHTING);

}

void Widget::loadMaterial(QString filename, flag * object) {
    qDebug() << filename << "\n";
    QStringList lineSplit;
    QString textureFile;
    QFile file(filename);
    QVector4D qVec4d;
    qreal num;
    if( !file.exists() ) {
        qDebug() << "File does not exist\n";
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        if(line.startsWith("Ka ")) {
            lineSplit = line.split(" ");
            qVec4d.setX(lineSplit.at(1).toFloat());
            qVec4d.setY(lineSplit.at(2).toFloat());
            qVec4d.setZ(lineSplit.at(3).toFloat());
            qVec4d.setW(1.0f);
            object->glAmbient = qVec4d;
        } else if(line.startsWith("Kd ")) {
            lineSplit = line.split(" ");
            qVec4d.setX(lineSplit.at(1).toFloat());
            qVec4d.setY(lineSplit.at(2).toFloat());
            qVec4d.setZ(lineSplit.at(3).toFloat());
            qVec4d.setW(1.0f);
            object->glDiffuse = qVec4d;
        } else if(line.startsWith("Ks ")) {
            lineSplit = line.split(" ");
            qVec4d.setX(lineSplit.at(1).toFloat());
            qVec4d.setY(lineSplit.at(2).toFloat());
            qVec4d.setZ(lineSplit.at(3).toFloat());
            qVec4d.setW(1.0f);
            object->glSpecular = qVec4d;
        } else if(line.startsWith("Ns ")) {
            lineSplit = line.split(" ");
            num = lineSplit.at(1).toFloat();
            object->glShininess = num;
        } else if(line.startsWith("map_Kd ")) {
            lineSplit = line.split(" ");
            textureFile = lineSplit.at(1);
            QImage textureImage;
            textureImage.load(textureFile);
            object->textureImage = QGLWidget::convertToGLFormat(textureImage);
        }
        line = in.readLine();
    }
}

int Widget::randInt(int low, int high) {
    return qrand() % ((high + 1) - low) + low;
}

void Widget::loadObj(QString filename, flag * object) {
    qDebug() << filename << "\n";
    QStringList lineSplit;
    QStringList lineSplit2;
    QVector3D qVec3d;
    QVector2D qVec2d;
    QVector<QVector3D> tempVertices;
    QVector<QVector3D> tempVerticeNormals;
    QVector<QVector2D> tempTextures;
    QFile file(filename);
    if( !file.exists() ) {
        qDebug() << "File does not exist\n";
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;

    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        if(line.startsWith("v ")) {
            lineSplit = line.split(" ");
            qVec3d.setX(lineSplit.at(1).toFloat());
            qVec3d.setY(lineSplit.at(2).toFloat());
            qVec3d.setZ(lineSplit.at(3).toFloat());
            tempVertices.append(qVec3d);
        } else if(line.startsWith("vn ")) {
            lineSplit = line.split(" ");
            qVec3d.setX(lineSplit.at(1).toFloat());
            qVec3d.setY(lineSplit.at(2).toFloat());
            qVec3d.setZ(lineSplit.at(3).toFloat());
            tempVerticeNormals.append(qVec3d);
        } else if(line.startsWith("vt ")) {
            lineSplit = line.split(" ");
            qVec2d.setX(lineSplit.at(1).toFloat());
            qVec2d.setY(lineSplit.at(2).toFloat());
            tempTextures.append(qVec2d);
        } else if(line.startsWith("f ")) {
            lineSplit = line.split(" ");
            for(int i=1; i < 4; i++){
                lineSplit2 = lineSplit.at(i).split("/");
                object->vertices.append(tempVertices.at(lineSplit2.at(0).toUInt()-1));
                object->textures.append(tempTextures.at(lineSplit2.at(1).toUInt()-1));
                object->normals.append(tempVerticeNormals.at(lineSplit2.at(2).toUInt()-1));
            }
        }
        line = in.readLine();
    }

}





