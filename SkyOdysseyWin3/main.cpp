#include <GL/glut.h>
#include <cmath>
#define PI 3.14159f
#include <algorithm>
#include <cstdlib> // For srand, rand
#include <ctime>   // For time

int i;
float cloudOffset = -1.0f;
float planeX = 1.0f;
float planeY = 1.0f;
float speed = 0.006;
float angle = 0.0;
bool isAscending = false;
bool hasFlown = false;
float planeAngle = 0.0f;
bool initialMovementDone = false;
float groundLevel = -0.5f;
// Function to draw a filled rectangle
void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Function to draw a filled triangle
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

// Function to draw a filled circle
void drawCircle(float cx, float cy, float r, float rColor, float gColor, float bColor) {
    glColor3f(rColor, gColor, bColor);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159 / 180;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void drawWheel(float x, float y, float radius) {
    glColor3f(0.0, 0.0, 0.0); // Black
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= 30; i++) {
        float angle = 2.0 * M_PI * i / 30;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}


void drawPlane(float x, float y, float angle) {
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);

    // Plane body
    glBegin(GL_POLYGON); // Use GL_POLYGON for a more defined shape
    glColor3f(2.7, 2.7,0.9); // White
    glVertex2f(-0.34, 0.0);
    glVertex2f(-0.3, 0.07);
    glVertex2f(-0.18, 0.12);
    glVertex2f(0.18, 0.12);
    glVertex2f(0.3, 0.07);
    glVertex2f(0.3, 0.0);
    glVertex2f(0.25, -0.07);
    glVertex2f(-0.25, -0.07);
    glEnd();

    // Red stripe
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0); // Red
    glVertex2f(-0.31, -0.025);
    glVertex2f(0.28, -0.025);
    glVertex2f(0.28, -0.035);
    glVertex2f(-0.3, -0.035);
    glEnd();

    // Left Wing
    glBegin(GL_TRIANGLES);
    glColor3f(2.7, 2.7,0.9); // White
    glVertex2f(-0.03, -0.03);
    glVertex2f(0.07, -0.03);
    glVertex2f(0.2, -0.3);
    glEnd();

    // Right Wing
    /*glBegin(GL_TRIANGLES);
    glColor3f(1.0, 1.0, 1.0); // White
    glVertex2f(0.33, 0.13);
    glVertex2f(0.32, 0.1);
    glVertex2f(0.4, 0.07);
    glEnd();
*/
    // Windows
    glColor3f(0.54, 0.77, 0.8); // White

    glBegin(GL_QUADS);
    glVertex2f(-0.2, 0.07);
    glVertex2f(-0.15, 0.07);
    glVertex2f(-0.15, 0.03);
    glVertex2f(-0.2, 0.03);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-0.1, 0.07);
    glVertex2f(-0.05, 0.07);
    glVertex2f(-0.05, 0.03);
    glVertex2f(-0.1, 0.03);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.0, 0.07);
    glVertex2f(0.05, 0.07);
    glVertex2f(0.05, 0.03);
    glVertex2f(0.0, 0.03);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.1, 0.07);
    glVertex2f(0.15, 0.07);
    glVertex2f(0.15, 0.03);
    glVertex2f(0.1, 0.03);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.2, 0.07);
    glVertex2f(0.25, 0.07);
    glVertex2f(0.25, 0.03);
    glVertex2f(0.2, 0.03);
    glEnd();

    // Tail
    glBegin(GL_TRIANGLES);
    glColor3f(2.7, 2.7,0.9); // Yellowish White
    glVertex2f(0.25, 0.07);
    glVertex2f(0.3, 0.0);
    glVertex2f(0.35, 0.2);
    glEnd();

    // Wheels
    drawWheel(-0.2, -0.08, 0.018);
    drawWheel(0.0, -0.08, 0.018);
    //drawWheel(0.08, -0.18, 0.018);

    glPopMatrix();
}

// Function to display text on the screen
void renderBitmapString(float x, float y, float z, void *font, char *string) {
    char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Drawing Of Birds
void drawCircle(float x, float y, float radius);

// Birds
void circle_bird(float x1, float y1, float r) {
    int i;

    GLfloat x = x1;
    GLfloat y = y1;
    GLfloat radius = r;
    int triangleAmount = 100; // # of lines used to draw circle

    // Get the aspect ratio of the window
    int width2 = glutGet(GLUT_WINDOW_WIDTH);
    int height2 = glutGet(GLUT_WINDOW_HEIGHT);
    GLfloat aspect2 = (GLfloat)height2 / (GLfloat)width2; // Aspect ratio (height/width)

    // GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;

    glColor3ub(80, 80, 80);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {
        glVertex2f(x + (radius * cos(i * twicePi / triangleAmount) * aspect2),
                     y + (radius * sin(i * twicePi / triangleAmount)));
    }
    glEnd();
}


void circle(float x1, float y1, float r, char c) {
    int i;
    // Get the aspect ratio of the window
    int width2 = glutGet(GLUT_WINDOW_WIDTH);
    int height2 = glutGet(GLUT_WINDOW_HEIGHT);
    GLfloat aspect2 = (GLfloat)height2 / (GLfloat)width2; // Aspect ratio (height/width)

    GLfloat x = x1;
    GLfloat y = y1;
    GLfloat radius = r;
    int triangleAmount = 100; // # of lines used to draw circle

    // GLfloat radius = 0.8f; //radius
    GLfloat twicePi = 2.0f * PI;
    // glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // center of circle
    for (i = 0; i <= triangleAmount; i++) {

        if (c == 's') {
            glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
                         y + (radius * sin(i * twicePi / triangleAmount)));

        } else {
            glVertex2f(x + (radius * cos(i * twicePi / triangleAmount)),
                         y + (radius * sin(i * twicePi / triangleAmount)));

        }
    }
    glEnd();
}





// Function to draw a bird (V-shape)
void draw_bird(float xAdjust, float yAdjust, int birdType) {

    int wingColorR = 255;
    int wingColorG = 140;
    int wingColorB = 0;
    int bodyColorR = 255;
    int bodyColorG = 255;
    int bodyColorB = 0;
    int beakColorR = 255;
    int beakColorG = 0;
    int beakColorB = 0;


    if (birdType == 1) {  // Green birds
        wingColorR = 0;
        wingColorG = 215;
        wingColorB = 0;
        bodyColorR = 0;
        bodyColorG = 100; // Darker green for body
        bodyColorB = 0;
    } else if (birdType == 2) { // Black birds
        wingColorR = 0;
        wingColorG = 0;
        wingColorB = 0;
        bodyColorR = 50; // Slightly lighter black for body
        bodyColorG = 50;
        bodyColorB = 50;
        beakColorR = 255;
        beakColorG = 255;
        beakColorB = 0;
    }

    glColor3ub(wingColorR, wingColorG, wingColorB);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f + xAdjust, 0.508f + yAdjust);
    glVertex2f(-0.5f + xAdjust, 0.492f + yAdjust);
    glVertex2f(-0.48f + xAdjust, 0.50f + yAdjust);
    glEnd();

    glColor3ub(bodyColorR, bodyColorG, bodyColorB);
    glBegin(GL_QUADS);
    glVertex2f(-0.51f + xAdjust, 0.49f + yAdjust);
    glVertex2f(-0.53f + xAdjust, 0.55f + yAdjust);
    glVertex2f(-0.53f + xAdjust, 0.52f + yAdjust);
    glVertex2f(-0.52f + xAdjust, 0.47f + yAdjust);
    glEnd();

    glColor3ub(0, 0, 0);
    circle_bird(-0.5 + xAdjust, 0.5 + yAdjust, 0.01);

    glColor3ub(beakColorR, beakColorG, beakColorB); // Beak color
    circle(-0.52 + xAdjust, 0.49 + yAdjust, 0.015, 'l');

    glColor3ub(bodyColorR, bodyColorG, bodyColorB);
    glBegin(GL_POLYGON);
    glVertex2f(-0.52f + xAdjust, 0.49f + yAdjust);
    glVertex2f(-0.55f + xAdjust, 0.47f + yAdjust);
    glVertex2f(-0.535f + xAdjust, 0.465f + yAdjust);
    glVertex2f(-0.55f + xAdjust, 0.43f + yAdjust);
    glVertex2f(-0.515f + xAdjust, 0.485f + yAdjust);
    glEnd();
}

float birdX[8] = {-0.06f, -0.06f, -0.12f, -0.12f, -0.07f, -0.07f, -0.12f, -0.12f};
float birdY[8] = {0.05f, 0.09f, 0.03f, 0.07f, 0.25f, 0.29f, 0.24f, 0.21f};
int birdColorType[8] = {0, 1, 2, 0, 1, 2, 0, 1};
float birdMoveX[8] = {0.008f, 0.008f, 0.005f, 0.008f, 0.005f, 0.005f, 0.008f, 0.005f}; // Movement speed for each bird
bool birdMovingRight[8] = {true, true, true, true, true, true, true, true};       // Direction of movement

bool isUserControlling = false;
bool isLanding = false;
bool isEnteringScene = false;

void update(int value) {
    cloudOffset += 0.002f;
    if (cloudOffset > 1.2f) {
        cloudOffset = -1.2f;
    }

    if (isEnteringScene) {
        // Plane enters the scene at 45-degree angle
        float angle = 45.0f;
        float angle_rad = angle * M_PI / 180.0f;

        planeX -= speed * cos(angle_rad);
        planeY -= speed * sin(angle_rad);

        // Stop entering once it reaches inside the scene
        if (planeX <= -0.28f) {
            isEnteringScene = false;
            initialMovementDone = true;
        }
    }
        if (isUserControlling && !isLanding) {
        // Allow user control
        float angle = 45.0f;
        float angle_rad = angle * M_PI / 180.0f;

        planeX -= speed * cos(angle_rad);
        planeY -= speed * sin(angle_rad);
        planeAngle -= 1.0f;  // Adjust rotation
    }

    if (isLanding) {
        // Landing behavior: Gradually decrease planeY
        if (planeY > groundLevel) {
            planeY -= speed; // Slowly descend
            planeAngle = 0.0f; // Align to flat position when landing
        } else {
            isLanding = false; // Stop landing when reaching ground
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}
void keyboard(int key, int x, int y) {
    switch (key) {
         case GLUT_KEY_DOWN:
            isEnteringScene = true; // Start entering scene
            break;
    }
}
// Function to draw the airport scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Sky Background
    drawRectangle(-1.0, 1.0, 1.0, -0.2, 0.757, 0.929, 0.969 );



    // Draw Hills
     drawTriangle(0.8, -0.2, 1.0, -0.2, 0.9, 0.11, 0.125, 0.439, 0.0472);
     drawRectangle(0.75, -0.2, 1.0, 0.02, 0.757, 0.929, 0.969);
     drawTriangle(0.8, -0.2, 1.0, -0.2, 0.9, 0.11, 0.035, 0.329, 0.067);
     drawTriangle(0.85, -0.2, 1.0, -0.2, 1.0, 0.15, 0.125, 0.439, 0.0472);
     drawTriangle(0.36, -0.2, 0.56, -0.2, 0.47, -0.07, 0.035, 0.329, 0.067);
     drawTriangle(0.6, -0.2, 0.8, -0.2, 0.7, 0.01, 0.125, 0.439, 0.0472);
     drawTriangle(0.45, -0.2, 0.7, -0.2, 0.55, -0.04, 0.125, 0.439, 0.0472);
     drawTriangle(-0.1, -0.2, 0.1, -0.2, 0.0, -0.04, 0.035, 0.329, 0.067);
     drawTriangle(-0.05, -0.2, 0.12, -0.2, 0.04, -0.07, 0.125, 0.439, 0.0472);
     drawTriangle(0.2, -0.2, 0.46, -0.2, 0.34, -0.07, 0.125, 0.439, 0.0472);
     drawTriangle(0.1, -0.2, 0.26, -0.2, 0.17, -0.07, 0.035, 0.329, 0.067);
     drawTriangle(0.02, -0.2, 0.24, -0.2, 0.1, -0.05, 0.125, 0.439, 0.0472);
     drawTriangle(0.36, -0.2, 0.56, -0.2, 0.47, -0.07, 0.035, 0.329, 0.067);


     //light green
     drawTriangle(0.36, -0.2, 0.40, -0.2, 0.47, -0.07, 0.373, 0.702, 0.251);
     drawTriangle(0.21, -0.2, 0.24, -0.2, 0.1, -0.05, 0.373, 0.702, 0.251);
     drawTriangle(0.2, -0.2, 0.26, -0.2, 0.34, -0.07, 0.373, 0.702, 0.25);
     drawTriangle(0.10, -0.2, 0.12, -0.2, 0.04, -0.07, 0.373, 0.702, 0.25);
     drawTriangle(0.099, -0.2, 0.1, -0.2, 0.0, -0.04,  0.373, 0.702, 0.25);
     drawTriangle(0.67, -0.2, 0.7, -0.2, 0.55, -0.04,0.373, 0.702, 0.25);
     drawTriangle(0.78, -0.2, 0.8, -0.2, 0.7, 0.01, 0.373, 0.702, 0.25);
     drawTriangle(0.85, -0.2, 0.87, -0.2, 1.0, 0.15, 0.373, 0.702, 0.25);
     drawTriangle(0.8, -0.2, 0.82, -0.2, 0.9, 0.11,  0.373, 0.702, 0.25);


     //draw plane
     //draw triangle
    // drawTriangle(0.75, 0.4, 0.82, 0.3, 0.7, 0.2, 1.0, 1.0, 1.0);







    // Draw  Left Clouds
    drawCircle(-0.825, 0.8, 0.075, 1.0, 1.0, 1.0);
    drawCircle(-0.7, 0.8, 0.075, 1.0, 1.0, 1.0);
    drawCircle(-0.577, 0.8, 0.075, 1.0, 1.0, 1.0);
    drawCircle(-0.775, 0.88, 0.07, 1.0, 1.0, 1.0);
    drawCircle(-0.625, 0.9, 0.07, 1.0, 1.0, 1.0);
    drawCircle(-0.650, 0.9, 0.07, 1.0, 1.0, 1.0);




    //Draw Right Clouds
    drawCircle(0.03, 0.75, 0.075, 1.0, 1.0, 1.0);
    drawCircle(0.15, 0.75, 0.075, 1.0, 1.0, 1.0);
    drawCircle(0.25, 0.75, 0.075, 1.0, 1.0, 1.0);
    drawCircle(0.2, 0.82, 0.075, 1.0, 1.0, 1.0);
    drawCircle(0.13, 0.8, 0.064, 1.0, 1.0, 1.0);
    drawCircle(0.08, 0.82, 0.075, 1.0, 1.0, 1.0);



    // Draw Sun (Moved to the right)
    drawCircle(0.7, 0.83, 0.15, 1.0, 0.85, 0.1) ;// Yellow Sun in the sky

    // Draw Airport Terminal shifted to the left

   drawRectangle(-0.9, -0.3, -0.85, 0.3, 0.906, 0.918, 0.922);
   drawRectangle(-0.93, 0.2, -0.82, 0.3, 0.906, 0.918, 0.922);
   drawTriangle(-1.0, 0.2, -0.93, 0.3, -0.9, 0.2,  0.757, 0.929, 0.969);
   drawTriangle(-0.85, 0.2, -0.82, 0.3, -0.7, 0.2, 0.757, 0.929, 0.969);
   drawRectangle(-0.93, 0.28, -0.82, 0.3,  0.2, 0.6, 0.8);
   drawRectangle(-0.93, 0.3, -0.82, 0.31,   0.906, 0.918, 0.922);
   drawRectangle(-0.93, 0.31, -0.82, 0.32,  0.2, 0.6, 0.8);
   drawRectangle(-0.93, 0.32, -0.82, 0.33,   0.906, 0.918, 0.922);

   drawCircle(-0.875, 0.345, 0.015, 0.2, 0.6, 0.8);
   drawCircle(-0.875, 0.17, 0.005, 0.2, 0.6, 0.8);
   drawCircle(-0.875, 0.13, 0.005, 0.2, 0.6, 0.8);
   drawCircle(-0.875, 0.09, 0.005, 0.2, 0.6, 0.8);
   drawCircle(-0.875, 0.05, 0.005, 0.2, 0.6, 0.8);
   drawCircle(-0.875, 0.01, 0.005, 0.2, 0.6, 0.8);
   drawCircle(-0.875, 0.344, 0.005, 1.0, 0.0, 0.0);


   drawRectangle(-1.0, -0.01, -0.15, -0.25, 0.3, 0.4, 0.4);
   drawRectangle(-1.0, -0.04, -0.15, -0.25,0.761, 0.914, 0.969); // Main building




    // Glass Windows
   drawRectangle(-1.0, -0.3, -0.3, -0.05, 0.2, 0.6, 0.8);
   drawRectangle(-1.0, -0.15, -0.3, -0.14, 1.0, 1.0, 1.0);
   drawRectangle(-1.0, -0.05, -0.3, -0.04, 1.0, 1.0, 1.0);
   drawRectangle(-0.95, -0.3, -0.955, -0.05, 1.0, 1.0, 1.0);
   drawRectangle(-0.85, -0.3, -0.855, -0.05, 1.0, 1.0, 1.0);
   drawRectangle(-0.75, -0.3, -0.755, -0.05, 1.0, 1.0, 1.0);
   drawRectangle(-0.65, -0.3, -0.655, -0.05, 1.0, 1.0, 1.0);
   drawRectangle(-0.55, -0.3, -0.555, -0.05, 1.0, 1.0, 1.0);
   drawRectangle(-0.45, -0.3, -0.455, -0.05, 1.0, 1.0, 1.0);
   drawRectangle(-0.35, -0.3, -0.355, -0.05, 1.0, 1.0, 1.0);// window


    drawCircle(-0.2, -0.25, 0.25, 0.3, 0.4, 0.4);
    drawCircle(-0.2, -0.25, 0.24, 1.0, 1.0, 1.0);
    drawCircle(-0.2, -0.25, 0.23, 0.761, 0.914, 0.969);




    // Add Glass Doors
    drawRectangle(-0.3, -0.07, -0.1, -0.3,  0.53, 0.81, 0.98); // Center door
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-0.20, -0.07);
    glVertex2f(-0.20, -0.25);

    glVertex2f(-0.30, -0.07);
    glVertex2f(-0.30, -0.25);

    glVertex2f(-0.10, -0.07);
    glVertex2f(-0.10, -0.25);

    glVertex2f(-0.30, -0.07);
    glVertex2f(-0.10, -0.07);// Door separator

    glVertex2f(-0.185, -0.17);
    glVertex2f(-0.185, -0.14);

    glVertex2f(-0.215, -0.17);
    glVertex2f(-0.215, -0.14);


    glEnd();



    // Draw birds in the sky
      for (int i = 0; i < 5; i++) {
        birdX[i] += birdMoveX[i]; // Move right
        if (birdX[i] > 1.5f) { // Reset when off-screen
            birdX[i] = -1.2f;
        }

        draw_bird(birdX[i], birdY[i], birdColorType[i]);
    }

    //bird_left1();
    //bird_left2();
    //bird_left3();
    //bird_left4();



    // Display the airport name
    glColor3f(0.0, 0.0, 0.0); // Black text
    renderBitmapString(-0.8f, 0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "PARO INTERNATIONAL AIRPORT");






    // Draw Runway
    drawRectangle(-1.0, -0.65, 1.0, -0.27, 0.2, 0.2, 0.2);
    drawRectangle(-1.0, -1.0, -0.25, -0.27, 0.2, 0.2, 0.2);
    drawRectangle(-1.0, -0.25, 1.0, -0.27, 1.0, 1.0, 1.0);
    drawRectangle(-0.25, -0.65, 1.0, -0.63, 1.0, 1.0, 1.0);
    drawTriangle(-0.68, -1.0, -0.25, -1.0, -0.25, -0.63, 1.0, 1.0, 1.0);
    drawTriangle(-0.65, -1.0, -0.15, -1.0, -0.25, -0.65, 0.65, 0.45, 0.25 );

 glPushMatrix();
    glTranslatef(planeX + 0.4, planeY - 0.02, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);  // Rotate around the new pivot
    //glTranslatef(-0.15, 0.05, 0.0); // Move back to original position

    drawPlane(0.0, 0.0, 0.0);
    glPopMatrix();
//drawPlane(0.6, 0.29, 0.6);


    // Draw Runway Centerline
    glColor3f(1, 1, 1);
    glLineWidth(4);
    glBegin(GL_LINES);
    for (float i = -1.0; i < 1.0; i += 0.3) {
        glVertex2f(i, -0.4);
        glVertex2f(i + 0.15, -0.4);
    }
    glEnd();
    glLineWidth(1);




 glutSwapBuffers();
    glFlush();



}




// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800,600);
    glutCreateWindow("Airport Runway in OpenGL");
glClearColor(0.65, 0.45, 0.25, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);
     glutSpecialFunc(keyboard);
glutTimerFunc(10, update, 0);
    glutMainLoop();
    return 0;
}
