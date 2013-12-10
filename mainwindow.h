#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QVariant>
#include <QCheckBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QGLWidget>
#include "mypanelopengl.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:

private slots:
    void loadpulsar_3();
    void loadpulsar_2();
    void loadpulsar_1();
    void loadgosper_3();
    void loadgosper_2();
    void loadgosper_1();
    void loadglider_3();
    void loadglider_2();
    void loadglider_1();
    void loadclock_m();
    void loadcross_m();
    void loadbaker_m();
    void loadquasar_m();
    void loadsquare_m();
    void loadpentadecathlon_m();
    void loadsavefile_m();
    void setScreenMatrix();
    void changeRGB_1_1(double rgb);
    void changeRGB_1_2(double rgb);
    void changeRGB_1_3(double rgb);
    void changeRGB_2_1(double rgb);
    void changeRGB_2_2(double rgb);
    void changeRGB_2_3(double rgb);
    void changeRGB_3_1(double rgb);
    void changeRGB_3_2(double rgb);
    void changeRGB_3_3(double rgb);

public slots:
    void resetW(); // Wrapper for reset world to reset spinner boxes.

private:
    // Basic! Don't Touch!
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QWidget *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    // Load from ini file!
    void loadFromIni(QString QTempString);

    // structures
    void createGridGroupBox();
    void createMenuBar();

    // Help
    QPushButton *help_button;

    // Boxes for buttons, slider, labels, and 2nd for color spiners
    QGroupBox *gridGroupBox;
    QGroupBox *horizontalGroupBox;
    //  Main layout
    QHBoxLayout *horizontalLayout;

    // Sliders
    QSlider *speedSlider;
    QLCDNumber *lcdNumber;
    QLabel *sliderLabel;

    // Push Buttons
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *stepButton;
    QPushButton *resetButton;
    int resetw; // to allow dual
    QPushButton *randomizeButton;


    // gutter color
    QCheckBox *checkShowGutters;
    QLabel *showGutters_label;

    // Grid exists
    QLabel *showGrid_label;
    QCheckBox *checkShowGrid;

    // Color name label set ----------0
    QLabel *ColorNameLabel0;
    QLabel *labelColor_r_0;
    QLabel *labelColor_g_0;
    QLabel *labelColor_b_0;
    QDoubleSpinBox *RGB_0_1;
    QDoubleSpinBox *RGB_0_2;
    QDoubleSpinBox *RGB_0_3;

    // Color name label set ----------1
    QLabel *ColorNameLabel1;
    QLabel *labelColor_r_1;
    QLabel *labelColor_g_1;
    QLabel *labelColor_b_1;
    QDoubleSpinBox *RGB_1_1;
    QDoubleSpinBox *RGB_1_2;
    QDoubleSpinBox *RGB_1_3;

    // Color name label set ----------2
    QLabel *ColorNameLabel2;
    QLabel *labelColor_r_2;
    QLabel *labelColor_g_2;
    QLabel *labelColor_b_2;
    QDoubleSpinBox *RGB_2_1;
    QDoubleSpinBox *RGB_2_2;
    QDoubleSpinBox *RGB_2_3;

    // Spacer
    QSpacerItem *verticalSpacer;

    // Drawing panel
    MyPanelOpenGL *PanelGL;

    // File stuff
    QLabel *infile_label;
    QLineEdit *Infilename_edit;
    QPushButton *loadScreen;
    QLabel *outfile_label;
    QLineEdit *Outfilename_edit;
    QPushButton *saveScreen;
    char array_name[100];

    // Size of Array stuff!!!!
    QLabel *twoDimSize;
    QSpinBox *twoDimSpin;
    QPushButton *twoDimSet; // so we don't connect the change of the spinner to anything unless clicked!

    // Menu Bar stuff
    //QString *tempQ;
    QMenu *LoadMenu;
    QAction *loadGosper1;
    QAction *loadGosper2;
    QAction *loadGosper3;

    QAction *loadPulsar1;
    QAction *loadPulsar2;
    QAction *loadPulsar3;

    QAction *loadGlider1;
    QAction *loadGlider2;
    QAction *loadGlider3;

    QAction *loadclock;
    QAction *loadcross;
    QAction *loadbaker;
    QAction *loadquasar;
    QAction *loadsquare;
    QAction *loadpentadecathlon;
    QAction *loadsavefile;

    // QUIT
    QPushButton *close_button;
};

#endif // MAINWINDOW_H
