#include "flag.h"

flag::flag(flag *parent) : QObject(parent)
{
    init();
}

flag::flag() {
    init();
}

void flag::init() {
    mipmap = true;
    lifeSpan = 0;
    colorMode = _glTextures;
    debug = true;
    this->vboIds = new GLuint[3];
    this->picking = -1;
    this->type = 1;
    this->moving = -1;
    this->velocity.setX(0.0f);
    this->velocity.setY(0.0f);
    this->velocity.setZ(0.0f);
    this->location.setX(0.0f);
    this->location.setY(0.0f);
    this->location.setZ(0.0f);
    this->orientation.setX(0.0f);
    this->orientation.setY(1.0f);
    this->orientation.setZ(0.0f);
    this->color.setX(1.0f);
    this->color.setY(1.0f);
    this->color.setZ(1.0f);
    this->scale.setX(1.0f);
    this->scale.setY(1.0f);
    this->scale.setZ(1.0f);

    //QVector4D is used here to store RGBA(=XYZW) values for different material properties
    this->glAmbient.setX(0.2f), glAmbient.setY(0.2f), glAmbient.setZ(0.2f), glAmbient.setW(1.0f);
    this->glDiffuse.setX(0.8f), glDiffuse.setY(0.8f), glDiffuse.setZ(0.8f), glDiffuse.setW(1.0f);
    this->glSpecular.setX(0.0f), glSpecular.setY(0.0f), glSpecular.setZ(0.0f), glSpecular.setW(1.0f);
    this->glEmission.setX(0.0f), glEmission.setY(0.0f), glEmission.setZ(0.0f), glEmission.setW(1.0f);

    this->glShininess = 0.0f;
    this->angle = 0.0f;
}

void flag::drawObject() {

    GLenum errCode;
    const GLubyte *errString;

    //glPushMatrix();
    //glLoadIdentity();
    setTranslate();
    setRotate();
    if(colorMode == _glColor) {
        glEnable(GL_COLOR_MATERIAL);
        setColor();
    } else {
        glDisable(GL_COLOR_MATERIAL);
    }
    setScale();

    //qDebug() << "Drawing object\n";

    glBindTexture(GL_TEXTURE_2D,textureId);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 8" << errCode;
    }
    setMaterial();
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 9" << errCode;
    }
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < vertices.size();i++) {
        glNormal3f(normals.at(i).x(), normals.at(i).y(), normals.at(i).z());
        glTexCoord2f(textures.at(i).x(), textures.at(i).y());
        glVertex3f(vertices.at(i).x(), vertices.at(i).y(), vertices.at(i).z());
    }
    glEnd();
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 10" << errCode;
    }
    glBindTexture(GL_TEXTURE_2D,0);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 11" << errCode;
    }
    //glPopMatrix();

    if(debug) {

        debug = false;
    }

}

void flag::calcOrientation() {

}

void flag::setMaterial() {

    GLfloat amMat[] = {glAmbient.x(), glAmbient.y(), glAmbient.z(), glAmbient.w()};
    GLfloat diffMat[] = {glDiffuse.x(), glDiffuse.y(), glDiffuse.z(), glDiffuse.w()};
    GLfloat specMat[] = {glSpecular.x(), glSpecular.y(), glSpecular.z(), glSpecular.w()};
    GLfloat emiMat[] = {glEmission.x(), glEmission.y(), glEmission.z(), glEmission.w()};
    GLfloat shin[] = { glShininess };

    glMaterialfv(GL_FRONT, GL_AMBIENT, amMat);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffMat);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specMat);
    glMaterialfv(GL_FRONT, GL_EMISSION, emiMat);
    glMaterialfv(GL_FRONT, GL_SHININESS, shin);

}

void flag::initTextures() {
    GLenum errCode;
    const GLubyte *errString;

    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 1" << errCode;
    }
    glGenTextures(1,&textureId);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 2" << errCode;
    }
    glBindTexture(GL_TEXTURE_2D,textureId);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 3" << errCode;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 4" << errCode;
    }
    if(mipmap)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 5" << errCode;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(),
                 textureImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 textureImage.bits());
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 6" << errCode;
    }
    if(mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    if ((errCode = glGetError()) != GL_NO_ERROR) {
        errString = gluErrorString(errCode);
        qDebug() << "OpenGL Error: 7" << errCode;
    }
}

void flag::setColor(){
    if(this->picking == -1) {
        glColor3f(color.x(),color.y(),color.z());
    } else {
        glColor3ub(this->picking, this->picking, this->picking);
    }
}

void flag::setScale(){
    glScalef(scale.x(), scale.y(), scale.z());
}

void flag::setRotate() {
    glRotatef(angle, orientation.x(), orientation.y(), orientation.z());
}

void flag::setPicking(int picking) {
    this->picking = picking;
}

void flag::toggleType() {
    this->type = this->type * -1;
}

void flag::toggleMoving() {
    this->moving = this->moving * -1;
}

void flag::setTranslate() {

    glTranslatef(this->location.x(),this->location.y(),this->location.z());
}

void flag::move(QVector3D moveVector) {
    if(this->moving == 1) {
        this->location.setX(this->location.x() + moveVector.x());
        this->location.setY(this->location.y() + moveVector.y());
        this->location.setZ(this->location.z() + moveVector.z());
    }
}

QMatrix4x4 flag::getMatrix() {
    this->orientation.normalize();

    QMatrix4x4 mat;
    mat.translate(this->location);
    mat.rotate(this->angle, this->orientation);
    return mat;
}



FlagList flag::children() {
    FlagList children;
    foreach(QObject *obj, QObject::children()) {
        children.append((flag*) obj);
    }
    return children;
}
