#include "mypanelopengl.h"
#include "WorldFunctions.h"
#include <QMessageBox>
#include <QDebug>
#include <GL/glu.h>

// Change where the base class comes from and what it's attached to
MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent)
{
    MAX = 200;

    World = new int *[MAX];
    for (int i = 0; i < MAX; i++)
        World[i] = new int [MAX];
    initialize_world(World, MAX);

    World_colorOverlay = new int *[MAX];
    for (int i = 0; i< MAX; i++)
        World_colorOverlay[i] = new int [MAX];
    initialize_world(World_colorOverlay, MAX);

    strcpy(outfilename, "Matrix_save.txt");
    strcpy(infilename, "Matrix_save.txt");
    setFocusPolicy(Qt::StrongFocus);
    timer=NULL;
    generation = 0;
    randomized = 0;
    delay = 0;
    r_begin=1;
    c_begin=1;
    cycle_select=0;
    r_end = MAX-2;
    c_end = MAX-2;
    x_initial=-1;
    y_initial=1;
    POI_x = 0;          // WILL BE CHANGED BY --> button
    POI_y = 0;          //      "  "          <-- button
    zoom_in = 1;        // this integer value will be increased, decreased by +/-
    draw_grid = false;
    drawselectionbox = false;
    displayGutters(true);
}


// ------------------ PANEL PAINTING -----------------------------------------//
void MyPanelOpenGL::displayGrid(bool yn)
{
    if (yn)
        draw_grid = true;
    else
        draw_grid = false;
    updateGL();
}

void MyPanelOpenGL::displayGutters(bool yn)
{
    if (yn == 1)
    {
        GLred_0_g = 1.0f;
        GLblue_0_g = 0.0f;
        GLgreen_0_g = 0.0f;
        GLred_1_g = 1.0f;
        GLblue_1_g = 1.0f;
        GLgreen_1_g = 0.0f;
        GLred_2_g = 0.0f;
        GLblue_2_g = 1.0f;
        GLgreen_2_g = 1.0f;
    }
    else
    {
        GLred_0_g = 0.0f;
        GLblue_0_g = 0.0f;
        GLgreen_0_g = 0.0f;
        GLred_1_g = 0.0f;
        GLblue_1_g = 0.0f;
        GLgreen_1_g = 0.0f;
        GLred_2_g = 0.0f;
        GLblue_2_g = 0.0f;
        GLgreen_2_g = 0.0f;
    }
    updateGL();
}

void MyPanelOpenGL::setScale()
{
    MAX_x = this->width();
    MAX_y = this->height();
    if  (MAX_y>MAX_x)
        MAX_scale = MAX_x;
    else
        MAX_scale = MAX_y;
    boxwidth = (2.0000/MAX);
    // 2*(abs(-1)+1), 2=100% of available space!
    // 1,1
    // 1,1
    // x_initial+(j*boxwidth)     , y_initial-(i*boxwidth)
    // x_initial+(j*boxwidth)     , y_initial-((i+1)*boxwidth)
    // x_initial+((j+1)*boxwidth) , y_initial-((i+1)*boxwidth)
    // x_initial+((j+1)*boxwidth) , y_initial-(i*boxwidth)
    //
    //i=0, j=0 (-1,1)(-1,0)(0,0)(0,1)       i=0, j=1 (0,1)(0,0)(1,0)(1,1)
    //i=1, j=0 (-1,0)(-1,-1)(0,-1)(0,0)
}

void MyPanelOpenGL::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DOUBLEBUFFER);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glViewport( 0, 0, (GLint)this->width(), (GLint)this->height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    setScale();
}

void MyPanelOpenGL::resizeGL(int w, int h)
{
    setScale();
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport(0, 0, (GLint)w, (GLint)h );
}

void MyPanelOpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Zoom and pan, each cycle of update
    glTranslatef(0+POI_x, 0+POI_y, 0);
    glScaled(zoom_in, zoom_in, 1);

    // now do the drawing (the display world function, really)
    int   r, c; // row col
    //    std::ofstream color_file ("Colors.txt");
    for (r = 0; r<MAX; r++)
    {
        for (c = 0; c<MAX; c++)
        {
            //mix_colors(World[r][c], World_colorOverlay[r][c], color_file);
            mix_colors(World[r][c], World_colorOverlay[r][c]);

            // WE WILL FIGURE OUT BORDER / GUTTER LATER
            if (r == 0 || c == 0 || r == MAX-1 || c == MAX-1)
                glColor3f(GLred_0_g, GLgreen_0_g, GLblue_0_g);
            else
                glColor3f(factor_0, factor_1, factor_2);

            // DRAW A BOX IN A COLOR SET ABOVE!
            glBegin(GL_QUADS); // draw square
            glVertex2f(x_initial+(c*boxwidth)     , y_initial-(r*boxwidth));
            glVertex2f(x_initial+(c*boxwidth)     , y_initial-((r+1)*boxwidth));
            glVertex2f(x_initial+((c+1)*boxwidth) , y_initial-((r+1)*boxwidth));
            glVertex2f(x_initial+((c+1)*boxwidth) , y_initial-(r*boxwidth));
            glEnd();
            if (draw_grid && MAX <= 150)
            {
                // only do this if it's reasonable size Matrix!
                glLineWidth(boxwidth/MAX/2);
                glColor3f(1.0f,1.0f,1.0f);
                glBegin(GL_LINE_LOOP); // draw line around it
                glVertex2f(x_initial+(c*boxwidth)     , y_initial-(r*boxwidth));
                glVertex2f(x_initial+(c*boxwidth)     , y_initial-((r+1)*boxwidth));
                glVertex2f(x_initial+((c+1)*boxwidth) , y_initial-((r+1)*boxwidth));
                glVertex2f(x_initial+((c+1)*boxwidth) , y_initial-(r*boxwidth));
                glEnd();
            }
            if (drawselectionbox && MAX <= 1000)
            {
                cycle_select++; // This allows it to not get overwhelmed by drawing boxes on boxes!
                if (cycle_select == 25)
                {
                    cycle_select = 0;
                    glLineWidth(boxwidth/MAX/2);
                    if ((GLred_0+0.6 > (GLfloat)1.1) || (GLgreen_0+0.6 > (GLfloat)1.1) || (GLblue_0+0.6 > (GLfloat)1.1))
                        glColor3f(GLred_0-.5, GLgreen_0-.5, GLblue_0-.5);
                    else
                        glColor3f(GLred_0+.5, GLgreen_0+.5, GLblue_0+.5);
                    glBegin(GL_LINE_LOOP); // draw line around it
                    glVertex2f(x_initial+(c_begin*boxwidth)     , y_initial-(r_begin*boxwidth));
                    glVertex2f(x_initial+(c_begin*boxwidth)     , y_initial-((r_end+1)*boxwidth));
                    glVertex2f(x_initial+((c_end+1)*boxwidth) , y_initial-((r_end+1)*boxwidth));
                    glVertex2f(x_initial+((c_end+1)*boxwidth) , y_initial-(r_begin*boxwidth));
                    glEnd();
                }
            }
        }// end for inner loop
    } // outer for loop!
    //color_file.close();
}



void MyPanelOpenGL::mix_colors(int element, int coloroverlay)
{

    // last num
    int intensity_0 = coloroverlay%10;
    // second to last
    int intensity_1 = (((coloroverlay%100)-intensity_0)/10);
    // first
    int intensity_2 = ((coloroverlay-(coloroverlay%100))/100);

    //color_out << "intensity is now:" << intensity_0 << ": " << intensity_1 << ": " << intensity_2 << endl;
    if (element == 0)
    {
        factor_0 = GLred_0;
        factor_1 = GLgreen_0;
        factor_2 = GLblue_0;

        if (intensity_2 < intensity_1)
        {
            int diff = abs(intensity_1 - intensity_0);
            factor_0 += ((factor_0 + GLred_1)  /diff*2);
            factor_1 += ((factor_1 + GLgreen_1)/diff*2);
            factor_2 += ((factor_2 + GLblue_1) /diff*2);
        }
        // else if
        if (intensity_1 < intensity_2)
        {
            int diff = abs(intensity_2 - intensity_0);
            factor_0 += ((factor_0 + GLred_2)  /diff*2);
            factor_1 += ((factor_1 + GLgreen_2)/diff*2);
            factor_2 += ((factor_2 + GLblue_2) /diff*2);
        }
    }
    else if (element == 1)
    {
        factor_0 = GLred_1;
        factor_1 = GLgreen_1;
        factor_2 = GLblue_1;

        if (intensity_2 < intensity_0)
        {
            int diff = abs(intensity_0 - intensity_1);
            factor_0 += ((factor_0 + GLred_0)  /diff*2);
            factor_1 += ((factor_1 + GLgreen_0)/diff*2);
            factor_2 += ((factor_2 + GLblue_0) /diff*2);
        }
        // else if
        if (intensity_0 < intensity_2)
        {
            int diff = abs(intensity_2 - intensity_1);
            factor_0 += ((factor_0 + GLred_2)  /diff*2);
            factor_1 += ((factor_1 + GLgreen_2)/diff*2);
            factor_2 += ((factor_2 + GLblue_2) /diff*2);
        }
    }
    else if (element == 2)
    {
        factor_0 = GLred_2;
        factor_1 = GLgreen_2;
        factor_2 = GLblue_2;

        if (intensity_1 < intensity_0)
        {
            int diff = abs(intensity_0 - intensity_2);
            factor_0 += ((factor_0 + GLred_0)  /diff*2);
            factor_1 += ((factor_1 + GLgreen_0)/diff*2);
            factor_2 += ((factor_2 + GLblue_0) /diff*2);
        }
        // else  if
        if (intensity_0 < intensity_1)
        {
            int diff = abs(intensity_1 - intensity_2);
            factor_0 += ((factor_0 + GLred_1)  /diff*2);
            factor_1 += ((factor_1 + GLgreen_1)/diff*2);
            factor_2 += ((factor_2 + GLblue_1) /diff*2);
        }

    }
    //color_out << "Color is now R:  " << factor_0 << "G: " << factor_1 << "B: " << factor_2 << endl;
}

// Change MAX size
void MyPanelOpenGL::setMAX(int newValue)
{
    //delete old pointers! to make room for ones of new size!
    for ( int i = 0 ; i < MAX ; i++ )
    {
        delete [] World[i];
        delete [] World_colorOverlay[i];
    }
    delete [] World;
    delete [] World_colorOverlay;

    MAX=newValue;
    r_begin = 1;
    c_begin = 1;
    r_end=MAX-2;
    c_end=MAX-2;

    //MAKE NEW WITH NEW MAX
    World = new int *[MAX];
    for (int i = 0; i < MAX; i++)
        World[i] = new int [MAX];
    World_colorOverlay = new int *[MAX];
    for (int i = 0; i < MAX; i++)
        World_colorOverlay[i] = new int [MAX];
    int currentwidth = this->parentWidget()->width();
    int currentheight = this->parentWidget()->height();
    this->parentWidget()->resize(10,10);
    this->parentWidget()->resize(currentwidth, currentheight);
    this->updateGL();
    setScale();
    resetWorld();
}

// --------------------- MOUSE CLICKS SLOTS AND STUFF -------------------------------------//

// Convert matrix elements to spots on the panel
float MyPanelOpenGL::cr_to_xy (int c, char cr)
{
    double screen_size;
    double box_pixels;
    if (cr == 'c')
        screen_size = this->width();
    else
        screen_size = this->height();
    // gets the width OR height of the panel!!!!
    box_pixels = (screen_size*1.0) / (MAX*1.0);
    float xy_val = ceilf(cr * box_pixels + 1);
    return xy_val;
}

// Convert pixels on the panel to spots on the matrix
int MyPanelOpenGL::mouse_x_to_c (int x, char xy)
{
    double screen_size;
    double box_pixels;
    if (xy == 'x')
        screen_size = this->width();
    else
        screen_size = this->height();
    box_pixels = (screen_size*1.0)/ (MAX*1.0);
    int c = floor( (x-1)/box_pixels );  //n*box_pixels=xpixels
    return c;
}

void MyPanelOpenGL::keyPressEvent(QKeyEvent *kevent)
{
    switch (kevent->key())
    {
    case Qt::Key_Right:
    {
        // move screen POI_x right, +10? we'll see...
        POI_x-=.20;
        break;
    }
    case Qt::Key_Left:
    {
        // move screen POI_x left, -10? we'll see...
        POI_x+=.20;
        break;
    }
    case Qt::Key_Up:
    {
        // move screen POI_y up, -10? we'll see...
        POI_y-=.20;
        //qDebug() << "Up, POI_y=" << POI_y;
        break;
    }
    case Qt::Key_Down:
    {
        // backwards...
        // move screen POI_y down, +10? we'll see...
        POI_y+=.20;
        //qDebug() << "Down, POI_y=" << POI_y;
        break;
    }
    case Qt::Key_Plus:
    {
        zoom_in+=.5;
        //qDebug() << "Zoom in, zoom_in=" << zoom_in;
        break;
    }
    case Qt::Key_Minus:
    {
        zoom_in-=.5;
        //qDebug() << "Zoom out, zoom_in=" << zoom_in;
        break;
    }
    case Qt::Key_0:
    {
        zoom_in = 1;
        POI_x = 0;
        POI_y = 0;
        break;
    }
    case Qt::Key_1:
    {
        QString QAboutScreen= QString("Screen info\n Width: %1 and Height: %2 \n POI_y: %3 and POI_x: %4\n")
                .arg(this->width())
                .arg(this->height())
                .arg(POI_y)
                .arg(POI_x);
        QMessageBox::information(this,"Info about Screen", QAboutScreen, QMessageBox::Cancel);
        break;
    }
    } // end Switch
    updateGL();
    update();
} // End Switch!


void MyPanelOpenGL::mouseMoveEvent(QMouseEvent *move_event)
{
    if (pressed_left)
    {
        stopGeneration();
        int pos_x = move_event->localPos().x();
        int pos_y = move_event->localPos().y();
        c_end = mouse_x_to_c(pos_x, 'x');
        r_end = mouse_x_to_c(pos_y, 'y');
        drawselectionbox = true;
    }
    else
    {
        drawselectionbox = false;
    }
    updateGL();
}

void MyPanelOpenGL::mousePressEvent(QMouseEvent *mevent)
{
    if (timer != NULL)
        isRunning = true;
    stopGeneration();
    if ((zoom_in == 1) && (POI_x == 0) && (POI_y == 0))
    {
        // ONLY TAKE MOUSE CLICKS IF NO ZOOM
        switch (mevent->button())
        {
        case Qt::LeftButton:
        {
            switch (mevent->modifiers())
            {
            case Qt::ShiftModifier:
            {
                last_shift_x = mevent->localPos().x();
                last_shift_y = mevent->localPos().y();
                pressed_shift = true;
                pressed_left = false;
                pressed_right = false;
                break;
            }
            case Qt::ControlModifier:
            {
                pressed_left = true;
                pressed_shift = false;
                pressed_right = false;

                mouse_last_x = mevent->localPos().x();
                mouse_last_y = mevent->localPos().y();
                c_begin = mouse_x_to_c(mouse_last_x, 'x');
                r_begin = mouse_x_to_c(mouse_last_y, 'y');
                break;
            } // last case
            } // end modifier switch
            break;// Left button
        } // end case
        case Qt::RightButton:
        {
            switch (mevent->modifiers())
            {
            case Qt::ControlModifier:
            {
                pressed_right = true;
                pressed_left = false;
                pressed_shift = false;
                // we'll insert selected (check boxed or default pattern to the screen at the location.
                mouse_last_x = mevent->localPos().x();
                mouse_last_y = mevent->localPos().y();
                // insert it into matrix at c,r and call updateGL() // repaint() // do this on release!
                break;
            }
            case Qt::Key_Alt:
            {
                QString Q_position= QString("Screen info\n pos.x: %1 and pos.y: %2 AND YOU ARE AWESOME!")
                        .arg(mevent->localPos().x())
                        .arg(mevent->localPos().y());
                QMessageBox::information(this,"Awesome Info:", Q_position, QMessageBox::Cancel);
                break;
            } // last case
            } // End Switch
        } // end case
        default:
            break;
        } // End big switch
    }
    else
    {
        if (mevent->button() == Qt::RightButton)
        {
            QString Q_position= QString("Screen info\n pos.x: %1 and pos.y: %2")
                    .arg(mevent->localPos().x())
                    .arg(mevent->localPos().y());
            QMessageBox::information(this,"Info:", Q_position, QMessageBox::Cancel);
        }
    }
}

void MyPanelOpenGL::mouseReleaseEvent(QMouseEvent *mevent2)
{
    if (timer != NULL)
        isRunning = true;
    stopGeneration();
    if ((zoom_in == 1) && (POI_x == 0) && (POI_y == 0))
    {
        // ONLY TAKE MOUSE CLICKS IF NO ZOOM
        int pos_x = mevent2->localPos().x();
        int pos_y = mevent2->localPos().y();

        switch (mevent2->button())
        {
        case Qt::LeftButton:
        {
            switch (mevent2->modifiers())
            {
            case Qt::ShiftModifier:
            {
                if (pressed_shift == true)
                {
                    c_shift = mouse_x_to_c(last_shift_x, 'x');
                    r_shift = mouse_x_to_c(last_shift_y, 'y');
                    cycle_state(World, r_shift, c_shift);
                    pressed_shift = false;
                }
                break;
            }
            case Qt::ControlModifier:
            {
                if (pressed_left == true)
                {
                    // just to be sure!!!
                    c_end = mouse_x_to_c(pos_x, 'x');
                    r_end = mouse_x_to_c(pos_y, 'y');
                    if (c_begin <= 0)
                        c_begin = 1;
                    else if (c_begin >= MAX-2)
                        c_begin = MAX-2;

                    if (r_begin <= 0)
                        r_begin = 1;
                    else if (r_begin >= MAX-2)
                        r_begin = MAX-2;

                    if (c_end <= 0)
                        c_end = 1;
                    else if (c_end >= MAX-2)
                        c_end = MAX-2;

                    if (r_end <= 0)
                        r_end = 1;
                    else if (r_end >= MAX-2)
                        r_end = MAX-2;

                    if (c_end < c_begin)
                    {
                        int temp1 = c_end;
                        int temp2 = r_end;
                        c_end = c_begin;
                        r_end = r_begin;
                        r_begin=temp2;
                        c_begin=temp1;
                    }

                    QString Question1 = QString("Do you want to save from %1 , %2 to %3 , %4 ?")
                            .arg(c_begin)
                            .arg(r_begin)
                            .arg(c_end)
                            .arg(r_end);

                    if (QMessageBox::question(this, "Save this?", Question1, QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
                        saveToOutFile();

                    // reset!
                    pressed_left = false;
                    drawselectionbox = false;
                    c_begin = 1;
                    r_begin=1;
                    c_end = MAX-2;
                    r_end = MAX-2;
                } // if pressed_left == true end...
                break;
            } // end modifiers last case
            } //end switch
            break; // left button case
        }
        case Qt::RightButton:
        {
            if (pressed_right == true)
            {
                c_begin = mouse_x_to_c(mouse_last_x, 'x');
                r_begin = mouse_x_to_c(mouse_last_y, 'y');

                if (c_begin <= 0)
                    c_begin = 1;
                else if (c_begin >= MAX-2)
                    c_begin = MAX-2;
                if (r_begin <= 0)
                    r_begin = 1;
                else if (r_begin >= MAX-2)
                    r_begin = MAX-2;

                QString Question1 = QString("Do you want to load %3 at %1 , %2?")
                        .arg(c_begin)
                        .arg(r_begin)
                        .arg(infilename);

                if (QMessageBox::question(this, "Load this here?", Question1, QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
                    loadFromInFile();

                c_begin = 1;
                r_begin = 1;
                pressed_right = false;
            }
            break; // Right mouse button case
        } // Right Mouse Button release
        default:
            break;
        } // big SWITCH
    }
    if (isRunning)
    {
        startGeneration();
        isRunning = false;
    }
    repaint();
    updateGL();
}



void MyPanelOpenGL::startGeneration()
{
    if (!timer)
    {
        srand(time(NULL)); // new random seed
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(process()));
        timer->start(delay);
    }
}

void MyPanelOpenGL::stopGeneration()
{
    if (timer)
    {
        delete timer;
        timer = NULL;
    }
    //updateGL();
}

void MyPanelOpenGL::stepGeneration()
{
    // just once! no timer connected
    //calculate_next_generation(World, generation, MAX);
    calculate_next_generation(World, World_colorOverlay, generation, MAX);
    updateGL();
}

void MyPanelOpenGL::process()
{
    //calculate_next_generation(World, generation, MAX);
    calculate_next_generation(World, World_colorOverlay, generation, MAX);
    repaint();
    updateGL();
}


void MyPanelOpenGL::changeDelay(int d)
{
    bool ismoving;
    ismoving = 1;
    // to change delay while moving...
    if (timer == NULL) ismoving = 0;
    stopGeneration();
    delay = d;
    if (ismoving) startGeneration();
    repaint();
    updateGL();
}


void MyPanelOpenGL::randomize()
{
    if (randomized < 16)
    {
        int num_times = 2; // to randomize per press
        for (int i = 0; i < num_times; i++)
            populate_random(World, MAX);
        randomized++;
    }
    repaint();
    updateGL();
}

void MyPanelOpenGL::resetWorld()
{
    stopGeneration();
    randomized = 0;
    generation = 0;
    zoom_in = 1;
    POI_x = 0;
    POI_y = 0;
    initialize_world(World, MAX);
    initialize_world(World_colorOverlay, MAX);
    repaint();
    updateGL();
}


//--------------------------------------- FILE FUNCTIONS ----------------------//

void MyPanelOpenGL::saveToOutFile()
{
    // Do the testing Dr. Wilkinson showed us about opening it ifstream first.
    ifstream outfile2 (outfilename);
    bool overwrite_outfile = false;
    if (!outfile2.fail())
    { // iff exists and no overwite
        // the file exists and we don't want to overwrite it!
        if (QMessageBox::question(this, "Overwrite?", "Do you want to overwrite this file?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            overwrite_outfile=true;
        outfile2.open(outfilename);
    }
    else
    { // doesn't exist, but for logic
        overwrite_outfile = true;
    }
    // doesn't exist or overwrite
    outfile2.close();
    if (overwrite_outfile)
    {
        ofstream outfile(outfilename); // opens file for output... I don't care if it overwrites.
        if (outfile.fail())
        {// maybe I will later...
            QMessageBox::information(this, "Error", "File not saved! get a new name!");
            //exit(0); // This is a more serious error because, that would mean we can't write to where we are!
        }
        else
        {
            outfile << r_end-r_begin+1 << "x"<< c_end-c_begin+1 << endl; // output dimension line.
            int i, j;
            for (i=r_begin; i<=r_end; i++)
            {
                for (j=c_begin; j<=c_end; j++)
                {
                    outfile<< World[i][j];
                }
                outfile << endl;
            }
        }
        outfile.close(); // close it!
    }
}

void MyPanelOpenGL::loadFromInFile()
{
    ifstream infile(infilename); // Open is done by this method.
    if (!infile.fail())
    {
        char two_D_size[42];
        int rows_end, cols_end;
        int cols_from_file;
        int r = r_begin;
        int c = c_begin;

        // Process the header string that contains the size of the matrix
        infile.getline(two_D_size, 42, '\n');
        get_m_size(two_D_size, rows_end, cols_from_file);

        //qDebug() << rows_end << " by " << cols_from_file << "=" << strlen(two_D_size);
        char temp_array[cols_from_file+1]; // Gets entire line from file!
        cols_end = cols_from_file;

        // if it's bigger than MAX-2 when added to c_being
        if ((cols_end+c) >= (MAX-2))
            cols_end = (MAX-2-c);
        if ((rows_end+r) >= (MAX-2))
            rows_end = (MAX-2-r);


        cols_end += c;    // At most MAX-2
        rows_end += r;    // At most MAX-2
        //qDebug() << "NOW:" << rows_end << " by " << cols_end;
        for (; ((r <= rows_end)&&(infile.getline(temp_array, cols_from_file+1, '\n'))) ; r++)
        {
            //qDebug() <<"TEMP:"<< temp_array;
            // pull entire line, show only up to MAX-2
            for (int i = 0; (((c+i) <= MAX-2) && (i < (strlen(temp_array)))); i++)
            {
                World[r][c+i] = (temp_array[i])-48; // 0, 1, or 2 r increases after storage
                //qDebug() <<r<<"+"<<c<<"+"<<i<<"="<<World[r][c+i];
            }
        }
        // r_begin and c_begin will be set by mouse clicks and reset to 1,1
        // therefore it will only be set by mouse, not by button push
    }
    infile.close();
    repaint();
    updateGL();
}

void MyPanelOpenGL::changeOutFile(QString Filename)
{
    strcpy(outfilename, Filename.toStdString().c_str());
}

void MyPanelOpenGL::changeInFile(QString Filename)
{
    strcpy(infilename, Filename.toStdString().c_str());
}

void MyPanelOpenGL::displayHelp()
{
    // Instructions
    QString Instructions1 = "<span style=\"font-size:10pt; font-weight:800; color:#ff0000;\">Help Info...</span><br>";
    Instructions1.append("<span style=\"font-size:10pt; font-weight:250; color:#0000aa;\">*To Save: <b>Ctrl+Left</b> Mouse<br>");
    Instructions1.append(" Click then Drag & Release at end point (Only for arrays less than 1000x1000)<br>");
    Instructions1.append("*To Load: <b>Ctrl+Right</b> Mouse Button to load into a spot on the screen<br>");
    Instructions1.append("   Use preloaded pattern from the Load menu or select your own<br>");
    Instructions1.append("*To change point on/off: <b>Shift+Left</b> Mouse Button<br>");
    Instructions1.append("*<b>+/-</b> to zoom. <b>Arrow</b> keys to move screen<br>");
    Instructions1.append("*0 to reset zoom/position<br>");
    Instructions1.append("*<b>DO NOT TRY TO SAVE/SELECT WHILE ZOOMED</b></span>");

    QMessageBox::information(this, "Help!", Instructions1, QMessageBox::Cancel);
}




