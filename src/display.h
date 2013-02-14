#ifndef DISPLAY_H
#define DISPLAY_H

#include <QtOpenGL>
#include <QVector3D>
#include <glu.h>
#include <iostream>
#include <QObject>

class display : public QObject {
    Q_OBJECT
public:
    display(QObject *parent = NULL);
    float screen[256][256][3];
    float antiAliasedScreen[256][256][3];
    void drawBresCircle(int mx, int my, int radius);
    void drawDisplay();
    void lineClipAndDraw(double x0, double y0, double x1, double y1);
    qreal size;
    static const int INSIDE = 0;
    static const int LEFT = 1;
    static const int RIGHT = 2;
    static const int BOTTOM = 4;
    static const int TOP = 8;
    static const int xmin = 0;
    static const int ymin = 0;
    static const int xmax = 255;
    static const int ymax = 255;
    void antiAlias();
private:
    void initScreen();
    int ComputeOutCode(double x, double y);
    void drawDDA(int x0, int y0, int x1, int y1);

};

#endif // DISPLAY_H
