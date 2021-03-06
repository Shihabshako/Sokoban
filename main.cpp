#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <iostream>
#include <math.h>
#include <GL/glut.h>				// include GLUT library
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <fstream>

using namespace std;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
}



// Global variables
GLfloat man_x = 0;       // x-coordinate of center of Triangle
GLfloat man_y = 0;       // y-coordinate of center of Triangle
GLfloat box_x = -0.3;
GLfloat box_y = 0.0;
int box_index = 0;
bool flag_human = true;
bool flag_box = true;
char level[25][25];
string current_level = "level1.txt";
int current_level_number = 1;


struct points{
   float x;
   float y;
};

/*
    # = wall
    ' '  = blank space
    @ = human
    * = box
    $ = target point
*/

struct points map_coordinate[22][22];
const GLfloat map_coordinate_value[22] = {-3.3, -3.0, -2.7, -2.4, -2.1, -1.8, -1.5, -1.2, -0.9f, -0.6, -0.3, 0.0, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0};
struct points boxes[5];


void initiate_map_coordinate(){
    int i, j, temp_i=0, temp_j=21;

    for(i=0 ; i<22 ; i++){
        temp_i = 0;
        for(j=0 ; j<22 ; j++){
            map_coordinate[i][j].x = map_coordinate_value[j];
            map_coordinate[i][j].y = map_coordinate_value[temp_j];
        }
        temp_j--;
    }

}

bool compareFloatNum(double a, double b){

    // Correct method to compare
    // floating-point numbers
    return fabs(a - b) <= 1.19e-7f;
}

int map_coordinate_value_index(GLfloat value , char direction){
    int i=0, j, k, l;
    if(direction == 'c'){
       for(j=21 ; j>=0; j--){
            if(compareFloatNum(map_coordinate_value[j],value)){
                break;
            }
        }
        return 21-j;

    }else if(direction == 'r'){
        for(k=0 ; k<22; k++){
            if(compareFloatNum(map_coordinate_value[k] , value)){
                i=k;
                break;
            }

        }
        return i;
    }


}

void draw_wall(GLfloat x, GLfloat y){
    glBegin(GL_QUADS);
        glColor3f(0.7f, 0.3f, 0.3f);        // redish
        glVertex2f(x, y);                  // Left vertex
        glVertex2f( x+0.3f, y);            // Top vertex
        glVertex2f( x+0.3f, y+0.3f);       // Right vertex
        glVertex2f(x, y+0.3f);           // Left vertex
    glEnd();
}

void draw_target(GLfloat x, GLfloat y){
    glBegin(GL_QUADS);
        glColor3f(0.0f, 1.0f, 1.0f);        // cyan
        glVertex2f(x, y);                  // Left vertex
        glVertex2f( x+0.3f, y);            // Top vertex
        glVertex2f( x+0.3f, y+0.3f);       // Right vertex
        glVertex2f(x, y+0.3f);           // Left vertex
    glEnd();
}

void draw_human(){
   glBegin(GL_QUADS);                      // Each set of 3 vertices form a triangle

        glColor3f(1.0f, 0.0f, 0.0f);        // Red
        glVertex2f(man_x, man_y);           // Left vertex
        glVertex2f( man_x+0.3f, man_y);            // Top vertex
        glVertex2f( man_x+0.3f, man_y+0.3f);           // Right vertex
        glVertex2f(man_x, man_y+0.3f);           // Left vertex

    glEnd();
   /* glBegin(GL_POLYGON);    // All vertices form a single polygon
        glColor3f(1.0f, 0.0f, 0.0f);
        GLfloat cx = man_x;
        GLfloat cy = man_y;
        GLfloat r = 0.2;
        for (float theta = 0; theta < 360; theta += 5) {
            float x = cx + r * cos(theta/180*M_PI);
            float y = cy + r * sin(theta/180*M_PI);
            glVertex2f(x,y);
        }
    glEnd();*/
}

void draw_boxes(){
   for(int i=0 ; i< box_index ; i++ ){
    GLfloat x = boxes[i].x;
    GLfloat y = boxes[i].y;
    glBegin(GL_QUADS);                      // Each set of 3 vertices form a triangle
        glColor3f(1.0f, 0.7f, 0.1f);        // dark yellow
        glVertex2f(x, y);                   // Left vertex
        glVertex2f( x+0.3f, y);             // Top vertex
        glVertex2f( x+0.3f, y+0.3f);        // Right vertex
        glVertex2f(x, y+0.3f);              // Left vertex

    glEnd();

   }

}

void draw_level(){

    for(int i=0 ; i<22 ; i++){
        for(int j=0 ; j<22 ; j++){
           char ch = level[i][j];
           if(ch == '#'){
              draw_wall(map_coordinate[i][j].x, map_coordinate[i][j].y);
           }else if(ch == '@' && flag_human){
             man_x = map_coordinate[i][j].x;
             man_y = map_coordinate[i][j].y;
             flag_human = false;
           }else if(ch == '$'){
               draw_target(map_coordinate[i][j].x, map_coordinate[i][j].y);
           }else if(ch == '*' && flag_box){
            boxes[box_index].x = map_coordinate[i][j].x;
            boxes[box_index].y = map_coordinate[i][j].y;
            box_index++;
           }
        }
    }
    flag_box = false;

    draw_boxes();
}

int box_coordinate_index(GLfloat x, GLfloat y){
    int i;
    for(i=0 ; i< box_index ; i++){
        if(compareFloatNum(boxes[i].x , x) && compareFloatNum(boxes[i].y , y)){
            return i;
        }
    }
}

bool is_wall_beside(char side){

    char ch;
    int index_i=0, index_j=0;
    //cout << map_coordinate_value_index(man_y, 'c') << "=" << map_coordinate_value_index(man_x, 'r')<< endl;;
    //cout << level[8][8];

    if(side == 'l'){
        GLfloat x = man_x - 0.3f;
        GLfloat y = man_y;
        index_i = map_coordinate_value_index(x, 'r');
        index_j = map_coordinate_value_index(y, 'c');
        ch = level[index_j][index_i];


    }else if(side == 'r'){
        GLfloat x = man_x + 0.3f;
        GLfloat y = man_y;
        index_i = map_coordinate_value_index(x, 'r');
        index_j = map_coordinate_value_index(y, 'c');
        ch = level[index_j][index_i];

    }else if(side == 'u'){
        GLfloat x = man_x ;
        GLfloat y = man_y + 0.3f;
        index_i = map_coordinate_value_index(x, 'r');
        index_j = map_coordinate_value_index(y, 'c');
        ch = level[index_j][index_i];

    }else if(side == 'd'){
        GLfloat x = man_x ;
        GLfloat y = man_y - 0.3f;
        index_i = map_coordinate_value_index(x, 'r');
        index_j = map_coordinate_value_index(y, 'c');
        ch = level[index_j][index_i];
    }

    if(ch == '#'){
        return true;
    }else{
        return false;
    }



}

bool is_box_beside(char side){
    int i;
    if(side == 'l'){
        GLfloat value_x = man_x - 0.3f;
        GLfloat value_y = man_y;
        for( i=0 ; i< box_index ; i++){
           if(compareFloatNum(boxes[i].x , value_x) && compareFloatNum(boxes[i].y , value_y)){
                break;
           }
        }
    }else if(side == 'r'){
        GLfloat value_x = man_x + 0.3f;
        GLfloat value_y = man_y;
        for( i=0 ; i< box_index ; i++){
           if(compareFloatNum(boxes[i].x , value_x) && compareFloatNum(boxes[i].y , value_y)){
                break;
           }
        }
    }else if(side == 'u'){
        GLfloat value_y = man_y + 0.3f;
        GLfloat value_x = man_x;
        for( i=0 ; i< box_index ; i++){
           if(compareFloatNum(boxes[i].x , value_x) && compareFloatNum(boxes[i].y , value_y)){
                break;
           }
        }
    }else if(side == 'd'){
        GLfloat value_y = man_y - 0.3f;
        GLfloat value_x = man_x;
        for( i=0 ; i< box_index ; i++){
           if(compareFloatNum(boxes[i].x , value_x) && compareFloatNum(boxes[i].y , value_y)){
                break;
           }
        }
    }

    if(i == box_index){
        return false;
    }else{
        return true;
    }
}

bool is_box_movable(char side){
    int i, value_x, value_y;
    char ch;
    if(side == 'l'){
         value_x = map_coordinate_value_index(man_x - 0.6f, 'r');
         value_y = map_coordinate_value_index(man_y, 'c');
        ch = level[value_y][value_x];
    }else if(side == 'r'){
         value_x = map_coordinate_value_index(man_x + 0.6f, 'r');
         value_y = map_coordinate_value_index(man_y, 'c');
        ch = level[value_y][value_x];

    }else if(side == 'u'){
         value_x = map_coordinate_value_index(man_x , 'r');
         value_y = map_coordinate_value_index(man_y + 0.6f, 'c');
        ch = level[value_y][value_x];
    }else if(side == 'd'){
         value_x = map_coordinate_value_index(man_x , 'r');
         value_y = map_coordinate_value_index(man_y - 0.6f, 'c');
        ch = level[value_y][value_x];
    }

    if(ch == ' ' || ch == '$' || ch == '@'){
        return true;
    }else if(ch == '*'){
        for(i=0 ; i< box_index ; i++){
           if(compareFloatNum(boxes[i].x , value_x) && compareFloatNum(boxes[i].y , value_y)){
                break;
           }
        }
        if(i == box_index){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool is_man_movable(char side){
    int i, value_x, value_y;
    char ch;
    bool is_box = false;
    if(side == 'l'){
        value_x = map_coordinate_value_index(man_x - 0.3f, 'r');
        value_y = map_coordinate_value_index(man_y, 'c');
        ch = level[value_y][value_x];


    }else if(side == 'r'){
        value_x = map_coordinate_value_index(man_x + 0.3f, 'r');
        value_y = map_coordinate_value_index(man_y, 'c');
        ch = level[value_y][value_x];

    }else if(side == 'u'){
        value_x = map_coordinate_value_index(man_x , 'r');
        value_y = map_coordinate_value_index(man_y + 0.3f, 'c');
        ch = level[value_y][value_x];

    }else if(side == 'd'){
        value_x = map_coordinate_value_index(man_x , 'r');
        value_y = map_coordinate_value_index(man_y - 0.3f, 'c');
        ch = level[value_y][value_x];

    }


    //cout << "value " << value_x << "~" << value_y  << endl;
    //cout <<  endl << endl;
    for(i=0 ; i<box_index ; i++){
            //cout << "box " << boxes[i].x << "~" << boxes[i].y << endl;
        if(map_coordinate_value_index(boxes[i].x, 'r') == value_x && map_coordinate_value_index(boxes[i].y,'c') == value_y){
            is_box = true;
        }
    }

    if(is_box){
        if(side == 'l'){
            value_x = map_coordinate_value_index(man_x - 0.6f, 'r');
            value_y = map_coordinate_value_index(man_y, 'c');
            ch = level[value_y][value_x];


        }else if(side == 'r'){
            value_x = map_coordinate_value_index(man_x + 0.6f, 'r');
            value_y = map_coordinate_value_index(man_y, 'c');
            ch = level[value_y][value_x];

        }else if(side == 'u'){
            value_x = map_coordinate_value_index(man_x , 'r');
            value_y = map_coordinate_value_index(man_y + 0.6f, 'c');
            ch = level[value_y][value_x];

        }else if(side == 'd'){
            value_x = map_coordinate_value_index(man_x , 'r');
            value_y = map_coordinate_value_index(man_y - 0.6f, 'c');
            ch = level[value_y][value_x];

        }
        if(ch != '#' && ch != '$'){
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }

}

void read_file(string filename) {
    int i=0 , j=0;
    unsigned char character = 0;

    FILE* input_file = fopen(filename.c_str(), "r");
    if (input_file == nullptr) {
       cout << "Failed to open file";
    }
    while (!feof(input_file)) {
       character = getc(input_file);
       if(character == '\n'){
        j=0;
        i++;

       }else{
        level[i][j] = character;
        j++;
       }
    }
    fclose(input_file);
}

void is_player_win(){
    bool win = true;
    for(int i=0 ; i < box_index ; i++){
        int x = map_coordinate_value_index(boxes[i].x, 'r');
        int y = map_coordinate_value_index(boxes[i].y, 'c');
        if(level[y][x] != '$'){
            win = false;
        }
    }
    if(win){
       // glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        memset(level, 0, sizeof(level));
        memset(boxes, 0, sizeof(boxes));
        flag_human = true;
        flag_box = true;
        box_index = 0;

        current_level_number++;
        if(current_level_number == 1){
            read_file("level1.txt");

        }else if(current_level_number == 2){
            read_file("level2.txt");

        }else if(current_level_number == 3){
            read_file("level3.txt");
        }

        cout << "Congratulations!!! Level Completed" <<endl;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

   // cout << man_x << "~~~" << man_y << "  ######  " << box_x << "~~" << box_y << endl;
    //glTranslatef(box_x,box_y,0);
    //glTranslatef(man_x,man_y,0);

    //cout << man_x << "~~~" << man_y << endl;

    glPushMatrix();
       glScalef(0.3,0.3,1);  // Scale down the Square
       draw_level();
       draw_human();
    glPopMatrix();

    glutSwapBuffers();      // Swap front and back buffers (of double buffered mode)

    glFlush();  // Render now
    is_player_win();

}

void reset(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    memset(level, 0, sizeof(level));
    memset(boxes, 0, sizeof(boxes));
    flag_human = true;
    flag_box = true;
    box_index = 0;
    current_level_number=1;
    read_file("level1.txt");

}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}

/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:    // ESC key
            exit(0);    // Exit window
            break;
        case 114:
            reset();
            break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeys(int key, int x, int y) {
    //cout << man_x << "~~~" << man_y << endl;
    //cout << man_x << endl;;

    GLfloat v = 0.3f;
    switch (key) {
        case GLUT_KEY_UP:
            if(!is_wall_beside('u')){
                if(is_box_beside('u') && is_box_movable('u')){
                   int i = box_coordinate_index(man_x, man_y + v);
                   boxes[i].y += v;
                }
                if(is_man_movable('u')){
                   man_y += v;  //move up
                }

            }
            break;

        case GLUT_KEY_DOWN:
            if(!is_wall_beside('d')){
                if(is_box_beside('d') && is_box_movable('d')){
                   int i = box_coordinate_index(man_x, man_y - v);
                   boxes[i].y -= v;
                }
                if(is_man_movable('d')){
                   man_y -= v;  // Move down
                }

            }
            break;

        case GLUT_KEY_LEFT:
            if(!is_wall_beside('l')){
                if(is_box_beside('l') && is_box_movable('l')){
                   int i = box_coordinate_index(man_x - v, man_y);
                   boxes[i].x -= v;
                }
                if(is_man_movable('l')){
                   man_x -= v;  // Move left
                }

            }

            break;


        case GLUT_KEY_RIGHT:
            if(!is_wall_beside('r')){
                if(is_box_beside('r') && is_box_movable('r')){
                   int i = box_coordinate_index(man_x + v, man_y);
                   boxes[i].x += v;
                }
                if(is_man_movable('r')){
                    man_x += v;  // Move right
                }

            }
            break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    read_file(current_level);
    initiate_map_coordinate();
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(700, 700);           // Set the window's initial width & height
    glutInitWindowPosition(50, 50);         // Position the window's initial top-left corner
    glutCreateWindow("Key Input");          // Create a window with the given title
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);               // Register callback handler for window re-shape
    glutKeyboardFunc(keyboard);             // Register callback handler for normal-key event
    glutSpecialFunc(specialKeys);           // Register callback handler for special-key event
    initGL();                               // Our own OpenGL initialization
    glutMainLoop();                         // Enter the event-processing loop
    return 0;

    /*

    int m, n;
    for(int i=0 ; i<22 ; i++){
        for(int j=0 ; j<22 ; j++){
            cout << level[i][j] << " ";
        }
        cout << endl;
    }*/




}
