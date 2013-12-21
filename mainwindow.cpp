#include "mainwindow.h"
#include <QtGui>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //createMenuBar();
    createGridGroupBox();

    menuBar = new QMenuBar;
    LoadMenu = new QMenu("&Load");
    loadGosper1 = LoadMenu->addAction(tr("Gosper 1"));
    loadGosper2 = LoadMenu->addAction(tr("Gosper 2"));
    loadGosper3 = LoadMenu->addAction(tr("Gosper 3"));
    loadGlider1 = LoadMenu->addAction(tr("Glider 1"));
    loadGlider2 = LoadMenu->addAction(tr("Glider 2"));
    loadGlider3 = LoadMenu->addAction(tr("Glider 3"));
    loadPulsar1 = LoadMenu->addAction(tr("Pulsar 1"));
    loadPulsar2 = LoadMenu->addAction(tr("Pulsar 2"));
    loadPulsar3 = LoadMenu->addAction(tr("Pulsar 3"));
    loadclock   = LoadMenu->addAction(tr("Clock"));
    loadcross = LoadMenu->addAction(tr("Cross"));
    loadsquare = LoadMenu->addAction(tr("Square"));
    loadquasar = LoadMenu->addAction(tr("Quasar"));
    loadbaker = LoadMenu->addAction(tr("Baker"));
    loadpentadecathlon = LoadMenu->addAction(tr("Pentadecathlon"));
    loadsavefile = LoadMenu->addAction(tr("Load Save File"));
    menuBar->addMenu(LoadMenu);


    // Drawing panel
    PanelGL = new MyPanelOpenGL;
    PanelGL->setMinimumSize(400,400);
    resetW();
    horizontalGroupBox = new QGroupBox(tr("Main Panel and Buttons"));
    horizontalLayout = new QHBoxLayout;
    horizontalLayout->setMenuBar(menuBar);
    horizontalLayout->addWidget(gridGroupBox);
    horizontalLayout->addWidget(PanelGL);
    horizontalGroupBox->setLayout(horizontalLayout);
    setCentralWidget(horizontalGroupBox);

    horizontalGroupBox->adjustSize();
    //horizontalLayout->SetMaximumSize;
    setWindowTitle(tr("Eli's Conway's Game of Life OpenGL"));
    connect(startButton, SIGNAL(clicked()), PanelGL, SLOT(startGeneration()));
    connect(stepButton, SIGNAL(clicked()), PanelGL, SLOT(stepGeneration()));
    connect(stopButton, SIGNAL(clicked()), PanelGL, SLOT(stopGeneration()));
    connect(randomizeButton, SIGNAL(clicked()), PanelGL, SLOT(randomize()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetW()));
    connect(speedSlider, SIGNAL(valueChanged(int)), PanelGL, SLOT(changeDelay(int)));
    connect(speedSlider, SIGNAL(valueChanged(int)), MainWindow::lcdNumber, SLOT(display(int)));
    connect(RGB_0_1, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_1_1(double)));
    connect(RGB_0_2, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_1_2(double)));
    connect(RGB_0_3, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_1_3(double)));
    connect(RGB_1_1, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_2_1(double)));
    connect(RGB_1_2, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_2_2(double)));
    connect(RGB_1_3, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_2_3(double)));
    connect(RGB_2_1, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_3_1(double)));
    connect(RGB_2_2, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_3_2(double)));
    connect(RGB_2_3, SIGNAL(valueChanged(double)), this, SLOT(changeRGB_3_3(double)));
    connect(Outfilename_edit, SIGNAL(textChanged(QString)), PanelGL, SLOT(changeOutFile(QString)));
    connect(saveScreen, SIGNAL(clicked()), PanelGL, SLOT(saveToOutFile()));
    connect(Infilename_edit, SIGNAL(textChanged(QString)), PanelGL, SLOT(changeInFile(QString)));
    connect(loadScreen, SIGNAL(clicked()), PanelGL, SLOT(loadFromInFile()));
    connect(twoDimSet, SIGNAL(clicked()), this, SLOT(setScreenMatrix()));
    // Load files!
    connect(loadGlider1, SIGNAL(triggered()), this, SLOT(loadglider_1()));
    connect(loadGlider2, SIGNAL(triggered()), this, SLOT(loadglider_2()));
    connect(loadGlider3, SIGNAL(triggered()), this, SLOT(loadglider_3()));
    connect(loadGosper1, SIGNAL(triggered()), this, SLOT(loadgosper_1()));
    connect(loadGosper2, SIGNAL(triggered()), this, SLOT(loadgosper_2()));
    connect(loadGosper3, SIGNAL(triggered()), this, SLOT(loadgosper_3()));
    connect(loadPulsar1, SIGNAL(triggered()), this, SLOT(loadpulsar_1()));
    connect(loadPulsar2, SIGNAL(triggered()), this, SLOT(loadpulsar_2()));
    connect(loadPulsar3, SIGNAL(triggered()), this, SLOT(loadpulsar_3()));
    connect(loadsquare,  SIGNAL(triggered()), this, SLOT(loadsquare_m()));
    connect(loadquasar,  SIGNAL(triggered()), this, SLOT(loadquasar_m()));
    connect(loadclock, SIGNAL(triggered()), this, SLOT(loadclock_m()));
    connect(loadcross, SIGNAL(triggered()), this, SLOT(loadcross_m()));
    connect(loadbaker, SIGNAL(triggered()), this, SLOT(loadbaker_m()));
    connect(loadpentadecathlon, SIGNAL(triggered()), this, SLOT(loadpentadecathlon_m()));
    connect(loadsavefile, SIGNAL(triggered()), this, SLOT(loadsavefile_m()));
    // Show gutters
    connect(checkShowGutters, SIGNAL(clicked(bool)), PanelGL, SLOT(displayGutters(bool)));
    connect(checkShowGrid, SIGNAL(clicked(bool)), PanelGL, SLOT(displayGrid(bool)));

    // HELP
    connect(help_button, SIGNAL(clicked()), PanelGL, SLOT(displayHelp()));

    //close
    connect(close_button, SIGNAL(clicked()), this, SLOT(close()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetW()
{
    changeRGB_1_1(0.0);
    changeRGB_1_2(0.0);
    changeRGB_1_3(0.0);
    changeRGB_2_1(0.0);
    changeRGB_2_2(0.2);
    changeRGB_2_3(0.0);
    changeRGB_3_1(0.0);
    changeRGB_3_2(0.0);
    changeRGB_3_3(0.2);
    PanelGL->resetWorld();
    update();
    PanelGL->updateGL();
}

void MainWindow::loadglider_1()
{
    loadFromIni("./inis/glider1.ini");
}
void MainWindow::loadglider_2()
{
    loadFromIni("./inis/glider2.ini");
}
void MainWindow::loadglider_3()
{
    loadFromIni("./inis/glider3.ini");
}
void MainWindow::loadgosper_1()
{
    loadFromIni("./inis/gosper1.ini");
}
void MainWindow::loadgosper_2()
{
    loadFromIni("./inis/gosper2.ini");
}
void MainWindow::loadgosper_3()
{
    loadFromIni("./inis/gosper3.ini");
}
void MainWindow::loadpulsar_1()
{
    loadFromIni("./inis/pulsar1.ini");
}
void MainWindow::loadpulsar_2()
{
    loadFromIni("./inis/pulsar2.ini");
}
void MainWindow::loadpulsar_3()
{
    loadFromIni("./inis/pulsar3.ini");
}
void MainWindow::loadsquare_m()
{
    loadFromIni("./inis/square.ini");
}
void MainWindow::loadquasar_m()
{
    loadFromIni("./inis/quasar.ini");
}
void MainWindow::loadclock_m()
{
    loadFromIni("./inis/clock.ini");
}
void MainWindow::loadcross_m()
{
    loadFromIni("./inis/cross.ini");
}
void MainWindow::loadbaker_m()
{
    loadFromIni("./inis/baker.ini");
}
void MainWindow::loadpentadecathlon_m()
{
    loadFromIni("./inis/pentadecathlon.ini");
}
void MainWindow::loadsavefile_m()
{
    loadFromIni(Outfilename_edit->text());
}

void MainWindow::loadFromIni(QString QTempString)
{
    MainWindow::Infilename_edit->setText(QTempString);
    this->update();
}

void MainWindow::setScreenMatrix()
{
    PanelGL->setMAX(MainWindow::twoDimSpin->value());
    this->update();
}


//------------------------- COLOR FUNCTIONS! ----------------------------------//
void MainWindow::changeRGB_1_1(double rgb){
    PanelGL->GLred_0 = (GLfloat)rgb;
    this->RGB_0_1->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_1_2(double rgb){
    PanelGL->GLgreen_0  = (GLfloat)rgb;
    this->RGB_0_2->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_1_3(double rgb){
    PanelGL->GLblue_0  = (GLfloat)rgb;
    this->RGB_0_3->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_2_1(double rgb){
    PanelGL->GLred_1  = (GLfloat)rgb;
    this->RGB_1_1->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_2_2(double rgb){
    PanelGL->GLgreen_1  = (GLfloat)rgb;
    this->RGB_1_2->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_2_3(double rgb){
    PanelGL->GLblue_1  = (GLfloat)rgb;
    this->RGB_1_3->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_3_1(double rgb){
    PanelGL->GLred_2  = (GLfloat)rgb;
    this->RGB_2_1->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_3_2(double rgb){
    PanelGL->GLgreen_2  = (GLfloat)rgb;
    this->RGB_2_2->setValue(rgb);
    PanelGL->updateGL();
    update();
}

void MainWindow::changeRGB_3_3(double rgb){
    PanelGL->GLblue_2  = (GLfloat)rgb;
    this->RGB_2_3->setValue(rgb);
    PanelGL->updateGL();
    update();
}


void MainWindow::createGridGroupBox()
{
    // CLOSE!
    close_button = new QPushButton("Close!");

    help_button = new QPushButton("&Help");

    // Menu
    menuBar = new QMenuBar;

    // Sliders
    speedSlider = new QSlider;
    speedSlider->setToolTip("Changes the speed of the cycles of generation.");
    lcdNumber = new QLCDNumber;
    lcdNumber->setToolTip("Displays the current delay.");
    sliderLabel = new QLabel(tr("<span style=\"font-size:10pt; font-weight:500; color:#aa00aa;\">Speed Delay</span>"));
    speedSlider->setMinimum(0);
    speedSlider->setMaximum(450);
    speedSlider->setTickInterval(15);

    // Push Buttons
    startButton = new QPushButton("&Start");
    startButton->setToolTip("Starts generation");
    stopButton = new QPushButton("S&top");
    stopButton->setToolTip("Stop at the current generation.");
    stepButton = new QPushButton("St&ep");
    stepButton->setToolTip("One generation ahead.");
    resetButton = new QPushButton("&Reset color/world");
    stopButton->setToolTip("Reset the screen and color");
    randomizeButton = new QPushButton("Randomi&ze");
    stopButton->setToolTip("Up to 18 * MAX * 4 cells generated.");


    // Color name label set ----------0
    ColorNameLabel0 = new QLabel(tr("<span style=\"font-size:8pt; font-weight:400; color:#000000;\">Color Set for Dead Cells</span>"));
    labelColor_r_0 = new QLabel(tr("<span style=\"font-size:6pt; font-weight:2500; color:#ff0000;\">Red</span>"));
    labelColor_g_0 = new QLabel("<span style=\"font-size:6pt; font-weight:2500; color:#00aa00;\">Green</span>");
    labelColor_b_0 = new QLabel("<span style=\"font-size:6pt; font-weight:2500; color:#0000ff;\">Blue</span>");
    RGB_0_1 = new QDoubleSpinBox;
    RGB_0_2 = new QDoubleSpinBox;
    RGB_0_3 = new QDoubleSpinBox;

    RGB_0_1->setDecimals(1);
    RGB_0_1->setMaximum(2.0);
    RGB_0_1->setMinimum(0.0);
    RGB_0_1->setSingleStep(.1);
    RGB_0_2->setDecimals(1);
    RGB_0_2->setMaximum(2.0);
    RGB_0_2->setMinimum(0.0);
    RGB_0_2->setSingleStep(.1);
    RGB_0_3->setDecimals(1);
    RGB_0_3->setMaximum(2.0);
    RGB_0_3->setMinimum(0.0);
    RGB_0_3->setSingleStep(.1);

    RGB_0_1->setValue(0.2);
    RGB_0_2->setValue(0.6);
    RGB_0_3->setValue(0.2);


    // Color name label set ----------1
    ColorNameLabel1 = new QLabel(tr("<span style=\"font-size:8pt; font-weight:400; color:#000000;\">Color Set for Live #1 Cells</span>"));
    labelColor_r_1 = new QLabel(tr("<span style=\"font-size:6pt; font-weight:2500; color:#ff0000;\">Red</span>"));
    labelColor_g_1 = new QLabel("<span style=\"font-size:6pt; font-weight:2500; color:#00aa00;\">Green</span>");
    labelColor_b_1 = new QLabel("<span style=\"font-size:6pt; font-weight:2500; color:#0000ff;\">Blue</span>");
    RGB_1_1 = new QDoubleSpinBox;
    RGB_1_2 = new QDoubleSpinBox;
    RGB_1_3 = new QDoubleSpinBox;

    RGB_1_1->setDecimals(1);
    RGB_1_1->setMaximum(2.0);
    RGB_1_1->setMinimum(0.0);
    RGB_1_1->setSingleStep(.1);
    RGB_1_2->setDecimals(1);
    RGB_1_2->setMaximum(2.0);
    RGB_1_2->setMinimum(0.0);
    RGB_1_2->setSingleStep(.1);
    RGB_1_3->setDecimals(1);
    RGB_1_3->setMaximum(2.0);
    RGB_1_3->setMinimum(0.0);
    RGB_1_3->setSingleStep(.1);

    RGB_1_1->setValue(0.0);
    RGB_1_2->setValue(0.0);
    RGB_1_3->setValue(2.0);

    // Color name label set ----------2
    ColorNameLabel2 = new QLabel(tr("<span style=\"font-size:8pt; font-weight:400; color:#000000;\">Color Set for Live #2 Cells</span>"));
    labelColor_r_2 = new QLabel(tr("<span style=\"font-size:6pt; font-weight:2500; color:#ff0000;\">Red</span>"));
    labelColor_g_2 = new QLabel("<span style=\"font-size:6pt; font-weight:2500; color:#00aa00;\">Green</span>");
    labelColor_b_2 = new QLabel("<span style=\"font-size:6pt; font-weight:2500; color:#0000ff;\">Blue</span>");
    RGB_2_1 = new QDoubleSpinBox;
    RGB_2_2 = new QDoubleSpinBox;
    RGB_2_3 = new QDoubleSpinBox;

    RGB_2_1->setDecimals(1);
    RGB_2_1->setMaximum(2.0);
    RGB_2_1->setMinimum(0.0);
    RGB_2_1->setSingleStep(.1);
    RGB_2_2->setDecimals(1);
    RGB_2_2->setMaximum(2.0);
    RGB_2_2->setMinimum(0.0);
    RGB_2_2->setSingleStep(.1);
    RGB_2_3->setDecimals(1);
    RGB_2_3->setMaximum(2.0);
    RGB_2_3->setMinimum(0.0);
    RGB_2_3->setSingleStep(.1);

    RGB_2_1->setValue(2.0);
    RGB_2_2->setValue(0.0);
    RGB_2_3->setValue(0.0);

    // Infile
    infile_label = new QLabel("Input File: ");
    Infilename_edit = new QLineEdit("Matrix_save.txt");
    // Load Screen
    loadScreen = new QPushButton ("Lo&ad screen");
    loadScreen ->setToolTip("Loads pattern starting from 1,1.");

    // Outfile
    outfile_label = new QLabel("Output File: ");
    Outfilename_edit = new QLineEdit("Matrix_save.txt");
    // Save Screen
    saveScreen = new QPushButton ("Sa&ve screen");
    saveScreen ->setToolTip("Saves entire screen.");

    // Check box for gutters
    showGutters_label = new QLabel("Show Gutters? ");
    checkShowGutters = new QCheckBox;
    checkShowGutters->setToolTip("Checked means show gutters");
    checkShowGutters->setChecked(1);

    // Check box for grid
    showGrid_label = new QLabel("Show Grid? ");
    checkShowGrid = new QCheckBox;
    checkShowGrid->setToolTip("Checked means show grid, not allowed for sizes > 150x150");
    checkShowGrid->setChecked(0);

    // To change the size of the array
    twoDimSize = new QLabel("<span style=\"font-size:6pt; font-weight:600; color:#000000;\">Change World Size 4-1500</span>");
    twoDimSpin = new QSpinBox;
    twoDimSet  = new QPushButton ("Clear and Set the Screen Size!");
    twoDimSpin->setToolTip("Min 4, Max 1500(SO SLOW)");
    twoDimSpin->setMinimum(4);
    twoDimSpin->setMaximum(1500);
    twoDimSpin->setValue(200);
    twoDimSpin->setSingleStep(10);


    verticalSpacer = new QSpacerItem(20, ((this->height()/2)+5));
    // Box to hold it all
    gridGroupBox = new QGroupBox(tr("Grid: Buttons and Spinners"));
    // Layout for buttons, slider, labels, and another for RGB spiners
    QGridLayout *gridLayout = new QGridLayout;




    // Column 0 (1)
    gridLayout->addWidget(speedSlider,0,0,4,2); // spans four rows... hopefully 0,1,2,3 and one col
    gridLayout->addWidget(sliderLabel,5,0,1,2); // Label across 3 cols
    gridLayout->addWidget(lcdNumber,5,2,1,1);   // Put LCD under it

    // Column 1 (2)
    gridLayout->addWidget(startButton, 0, 1, 1, 2);
    gridLayout->addWidget(stepButton,  1, 1, 1, 2);
    gridLayout->addWidget(stopButton,  2, 1, 1, 2);
    gridLayout->addWidget(randomizeButton,3, 1, 1, 2);
    gridLayout->addWidget(resetButton, 4, 1, 1, 2);

    // RGB 0 Label name, spans col 0-2
    gridLayout->addWidget(ColorNameLabel0, 6, 0, 1, 5);
    // Column R spinners
    gridLayout->addWidget(labelColor_r_0, 7, 0, 1, 1);
    gridLayout->addWidget(RGB_0_1, 8, 0, 1, 1);
    // Column G spinners
    gridLayout->addWidget(labelColor_g_0, 7, 1, 1, 1);
    gridLayout->addWidget(RGB_0_2, 8, 1, 1, 1);
    // Column B spinners
    gridLayout->addWidget(labelColor_b_0, 7, 2, 1, 1);
    gridLayout->addWidget(RGB_0_3, 8, 2, 1, 1);

    // RGB 0 Label name, spans col 0-2
    gridLayout->addWidget(ColorNameLabel1, 9, 0, 1, 5);
    // Column R spinners
    gridLayout->addWidget(labelColor_r_1, 10, 0, 1, 1);
    gridLayout->addWidget(RGB_1_1, 11, 0, 1, 1);
    // Column G spinners
    gridLayout->addWidget(labelColor_g_1, 10, 1, 1, 1);
    gridLayout->addWidget(RGB_1_2, 11, 1, 1, 1);
    // Column B spinners
    gridLayout->addWidget(labelColor_b_1, 10, 2, 1, 1);
    gridLayout->addWidget(RGB_1_3, 11, 2, 1, 1);

    // RGB 0 Label name, spans col 0-2
    gridLayout->addWidget(ColorNameLabel2, 12, 0, 1, 5);
    // Column R spinners
    gridLayout->addWidget(labelColor_r_2, 13, 0, 1, 1);
    gridLayout->addWidget(RGB_2_1, 14, 0, 1, 1);
    // Column G spinners
    gridLayout->addWidget(labelColor_g_2, 13, 1, 1, 1);
    gridLayout->addWidget(RGB_2_2, 14, 1, 1, 1);
    // Column B spinners
    gridLayout->addWidget(labelColor_b_2, 13, 2, 1, 1);
    gridLayout->addWidget(RGB_2_3, 14, 2, 1, 1);

    // Files
    gridLayout->addWidget(infile_label, 15,0,1,1);
    gridLayout->addWidget(Infilename_edit, 15,1,1,2);
    gridLayout->addWidget(loadScreen, 16, 0, 1, 2);

    gridLayout->addWidget(outfile_label, 17,0,1,1);
    gridLayout->addWidget(Outfilename_edit, 17,1,1,2);
    gridLayout->addWidget(saveScreen, 18, 0, 1, 2);

    // Show Gutters
    gridLayout->addWidget(showGutters_label, 19, 0, 1, 1);
    gridLayout->addWidget(checkShowGutters,19,1,1,1);

    // Show Grid
    gridLayout->addWidget(showGrid_label, 20, 0, 1, 1);
    gridLayout->addWidget(checkShowGrid,20,1,1,1);


    // How Big? Dynamic!!!!
    gridLayout->addWidget(twoDimSize, 27, 0, 1, 2);
    gridLayout->addWidget(twoDimSpin, 27, 2, 1, 1);
    gridLayout->addWidget(twoDimSet,  28, 0, 1, 3);
    gridLayout->addWidget(help_button, 30,0,1,1);
    gridLayout->addWidget(close_button, 31,0,1,1);
    // Spacer for bottom rows
    gridLayout->addItem(verticalSpacer,32,0,2,1);
    gridGroupBox->setLayout(gridLayout); // Puts the layout in the box!
    //gridGroupBox->setBaseSize(85,500);
    gridGroupBox->setMaximumWidth(220);
}



//// Basic! Don't Touch!
//Ui::MainWindow *ui;
//QWidget *centralWidget;
//QWidget *widget;
//QToolBar *mainToolBar;
//QStatusBar *statusBar;


