#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H
#include <QtGui>
#include <GL/glu.h>
#include <QGLWidget>
#include <QMessageBox>
#include <QApplication>
#include <QTimerEvent>
#include <QTimer>
#include <QDebug>

class MyPanelOpenGL : public QGLWidget
{
    Q_OBJECT

private slots:
    void process(); // it needs control over?
    void startGeneration();
    void stopGeneration();
    void stepGeneration();
    void changeDelay(int d);
    void changeOutFile(QString Filename);
    void changeInFile(QString Filename);
    void saveToOutFile();
    void loadFromInFile();
    void randomize();
    void mousePressEvent(QMouseEvent *mevent);
    void mouseReleaseEvent(QMouseEvent *mevent2);
    void mouseMoveEvent(QMouseEvent *mevent3);
    void keyPressEvent(QKeyEvent *kevent);
    void displayGutters(bool yn);
    void displayGrid(bool yn);
    //void resetColors();


public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    int MAX;
    // can be set on screen
    GLfloat GLred_0;
    GLfloat GLblue_0;
    GLfloat GLgreen_0;
    GLfloat GLred_0_g;
    GLfloat GLblue_0_g;
    GLfloat GLgreen_0_g;

    GLfloat GLred_1;
    GLfloat GLblue_1;
    GLfloat GLgreen_1;
    GLfloat GLred_1_g;
    GLfloat GLblue_1_g;
    GLfloat GLgreen_1_g;

    GLfloat GLred_2;
    GLfloat GLblue_2;
    GLfloat GLgreen_2;
    GLfloat GLred_2_g;
    GLfloat GLblue_2_g;
    GLfloat GLgreen_2_g;


public slots:
    // Things MainWindow needs to be able to use!
    void setMAX(int newValue);
    void resetWorld();
    void displayHelp();

protected:
    QTimer *timer;

    // Mouse Placement
    int   mouse_x_to_c(int x, char xy);
    int   mouse_y_to_r(int y);
    float cr_to_xy(int c, char cr);
    void  setScale();
    void  correctScale();

    // Graphical implementation
    void  initializeGL();
    void  resizeGL(int w, int h);
    void  paintGL();

protected slots:

private:
    // THE WORLD
    int **World;
    int **World_colorOverlay; // will contain an int made up of 0-8 and 0-8
                              //and will realate to an intensity multiplication overlay for 0-8 of it's type, and 0-8 of the other
	
    // ITS GENERATION
    int generation;

    // IS IT MOVING
    bool isRunning;
    int cycle_select;


    // Random stuff
    int randomized;

    // display stuff
    int delay;

    // Mouse Stuff
    bool pressed_right;
    bool pressed_left;
    bool pressed_shift;


    // Mouse Markers
    float mouse_last_y;
    float mouse_last_x;
    float last_shift_x;
    float last_shift_y;

    // Rubber banding
    bool drawselectionbox;
    int x_current_pos;
    int y_current_pos;

    // Cycle Color Marker
    int c_shift;
    int r_shift;

    // show Grid!
    bool draw_grid;

    // floats for placement
    float MAX_y;
    float MAX_x;
    float MAX_scale;
    double boxwidth;
    float x_initial;
    float y_initial;
    float POI_x;          // WILL BE CHANGED BY --> button
    float POI_y;          //      "  "          <-- button
    float zoom_in;      // this integer value will be increased, decreased by UP/DOWN
    float Win_Wide;
    float Win_High;

    // file stuff
    // SAVE
    char outfilename[100];
    char infilename[100];
    int r_begin;
    int c_begin;
    int r_end;
    int c_end;
};

#endif // MYPANELOPENGL_H
