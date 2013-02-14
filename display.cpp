#include "display.h"

display::display(QObject *parent)
{
    initScreen();
    size = .05f;

}

void display::initScreen() {
    for(int x=0;x<256;x++) {
        for(int y=0;y<256;y++) {
            screen[x][y][0] = 1.0f;
            screen[x][y][1] = 1.0f;
            screen[x][y][2] = 1.0f;
        }
    }
}

void display::antiAlias() {
    float sum = 0;
    float tempScreen[256][256][3];
    for(int x=1;x<xmax;x++) {
        for(int y=1;y<ymax;y++) {
            for(int i=0; i<3; i++) {
                sum += screen[x-1][y+1][i];
                sum += screen[x][y+1][i];
                sum += screen[x+1][y+1][i];

                sum += screen[x-1][y][i];
                sum += screen[x][y][i];
                sum += screen[x+1][y][i];

                sum += screen[x-1][y-1][i];
                sum += screen[x][y-1][i];
                sum += screen[x+1][y-1][i];

                if(sum == 9)
                    tempScreen[x][y][i] = 1.0f;
                else
                    tempScreen[x][y][i] = (sum*sum*sum) / (9*9*9);
                sum = 0;
            }
        }
    }
    for(int x=0;x<=xmax;x++) {
        tempScreen[x][0][0] = 1.0f;
        tempScreen[x][0][1] = 1.0f;
        tempScreen[x][0][2] = 1.0f;

        tempScreen[x][ymax][0] = 1.0f;
        tempScreen[x][ymax][1] = 1.0f;
        tempScreen[x][ymax][2] = 1.0f;
    }
    for(int y=0;y<=ymax;y++) {
        tempScreen[0][y][0] = 1.0f;
        tempScreen[0][y][1] = 1.0f;
        tempScreen[0][y][2] = 1.0f;

        tempScreen[xmax][y][0] = 1.0f;
        tempScreen[xmax][y][1] = 1.0f;
        tempScreen[xmax][y][2] = 1.0f;
    }
    memcpy(screen, tempScreen, sizeof(screen));
}

void display::drawDisplay() {

    glPushMatrix();
    //glLoadIdentity();
    //setTranslate();
    //setRotate();
    //setColor();
    //setScale();
    //setMaterial();
    glEnable(GL_COLOR_MATERIAL);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,1.0f,0.0f);
    for(int x=0; x < 256; x++) {
        for(int y=0; y < 256; y++) {
            glColor3f(screen[x][y][0],screen[x][y][1],screen[x][y][2]);
            glVertex3f(x*size, 0, y*size);
            glVertex3f((x+1)*size, 0, y*size);
            glVertex3f((x+1)*size, 0, (y+1)*size);
            glVertex3f(x*size, 0, (y+1)*size);
        }
    }
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();


}

void display::drawDDA(int x0, int y0, int x1, int y1) {
    int dx = abs(x0 - x1);
    int dy = abs(y0 - y1);
    int steps;

    if(dx >= dy) steps = dx;
    else steps = dy;

    float xInc = (x1 - x0) / (float) steps;
    float yInc = (y1 - y0) / (float) steps;

    float x = x0;
    float y = y0;

    for(int i=0; i < steps; i++) {
        screen[(int)round(x)][(int)round(y)][0] = 0.0f;
        screen[(int)round(x)][(int)round(y)][1] = 0.0f;
        screen[(int)round(x)][(int)round(y)][2] = 0.0f;

        x += xInc;
        y += yInc;
    }
    screen[(int)round(x)][(int)round(y)][0] = 0.0f;
    screen[(int)round(x)][(int)round(y)][1] = 0.0f;
    screen[(int)round(x)][(int)round(y)][2] = 0.0f;

}

void display::drawBresCircle(int mx, int my, int radius) {

    int x = 0;
    int y = radius;
    int d = 1-radius;

    for(int i=0;i<3;i++) {
        screen[mx + radius][my][i] = 0.0f;
        screen[mx][my + radius][i] = 0.0f;
        screen[mx - radius][my][i] = 0.0f;
        screen[mx][my - radius][i] = 0.0f;
    }

    while (x < y) {
        if (d < 0) {
            d = d + 2 * x + 3;
            x += 1;
        } else {
            d = d + 2 * (x-y) + 5;
            x += 1;
            y -= 1;
        }
        for(int i=0;i<3;i++) {
            screen[mx + x][my + y][i] = 0.0f;
            screen[mx + x][my - y][i] = 0.0f;
            screen[mx - x][my + y][i] = 0.0f;
            screen[mx - x][my - y][i] = 0.0f;

            screen[mx + y][my + x][i] = 0.0f;
            screen[mx + y][my - x][i] = 0.0f;
            screen[mx - y][my + x][i] = 0.0f;
            screen[mx - y][my - x][i] = 0.0f;

        }
    }
}

void display::lineClipAndDraw(double x0, double y0, double x1, double y1)
{
        // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
        int outcode0 = ComputeOutCode(x0, y0);
        int outcode1 = ComputeOutCode(x1, y1);
        bool accept = false;

        while (true) {
                if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
                        accept = true;
                        break;
                } else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
                        break;
                } else {
                        double x, y;

                        int outcodeOut = outcode0? outcode0 : outcode1;

                        if (outcodeOut & TOP) {           // point is above the clip rectangle
                                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                                y = ymax;
                        } else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
                                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                                y = ymin;
                        } else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
                                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                                x = xmax;
                        } else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
                                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                                x = xmin;
                        }
                        if (outcodeOut == outcode0) {
                                x0 = x;
                                y0 = y;
                                outcode0 = ComputeOutCode(x0, y0);
                        } else {
                                x1 = x;
                                y1 = y;
                                outcode1 = ComputeOutCode(x1, y1);
                        }
                }
        }
        if (accept) {
               drawDDA(x0, y0, x1, y1);
        }
}

int display::ComputeOutCode(double x, double y)
{
        int code;

        code = INSIDE;          // initialised as being inside of clip window

        if (x < xmin)           // to the left of clip window
                code |= LEFT;
        else if (x > xmax)      // to the right of clip window
                code |= RIGHT;
        if (y < ymin)           // below the clip window
                code |= BOTTOM;
        else if (y > ymax)      // above the clip window
                code |= TOP;

        return code;
}
