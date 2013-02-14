#ifndef FLAG_H
#define FLAG_H

#include <QtOpenGL>
#include <QVector3D>
#include <glu.h>
#include <iostream>
#include <QObject>

class flag;

typedef QList<flag *> FlagList;

class flag : public QObject {
    Q_OBJECT
private:
    static const int _glColor=1;
    static const int _glTextures=2;
public:
    flag(flag *parent);
    flag();
    ~flag() {

    }

    void setColor();
    void setTranslate();
    void setRotate();
    void setPicking(int picking);
    void toggleType();
    void toggleMoving();
    void setScale();
    void setMaterial();
    void initTextures();
    void init();
    FlagList children();
    QMatrix4x4 getMatrix();
    void calcOrientation();
    bool debug;
    bool mipmap;
    int colorMode;
    GLuint textureId;
    int picking;
    int type;
    int moving;
    GLuint * vboIds;   
    QVector3D velocity;
    QVector3D location;
    QVector3D orientation;
    QVector3D color;
    QVector3D scale;
    QVector4D glAmbient, glDiffuse, glSpecular, glEmission;
    QImage textureImage;
    qreal glShininess;
    qreal angle;
    qreal xzOrientation;
    qreal yOrienatation;
    qreal lifeSpan;
    QVector<QVector3D> vertices;
    QVector<QVector3D> normals;
    QVector<QVector2D> textures;
    QVector<GLuint> elements;
public slots:
    //void drawFlag1();
    //void drawFlag2();
    void drawObject();
    //void drawBres();
    void move(QVector3D moveVector);
};

#endif // FLAG_H
