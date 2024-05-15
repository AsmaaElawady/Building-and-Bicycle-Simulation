#include <GLUT/glut.h>
#include <iostream>
#include <cmath>

using namespace std;
// g++ main.cpp -lfreeglut -lopengl32 -lglu32 -lgdi32
GLfloat cameraX = 0.0f, cameraY = 0.0f, cameraZ = 5.0f;
bool isOpen = false, isOpenW = false;
bool isRotateBike = false, isMoveBike = false;
GLfloat angle = 0.0f;
GLfloat xChange = 0.0f;
float leftWheelAngle = 0.0f;
float rightWheelAngle = 0.0f;
bool isLeftButtonPressed = false, isRightButtonPressed = false;


void changeSize(int w, int h) {
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void DrawQuad(GLfloat A[], GLfloat B[], GLfloat C[], GLfloat D[]) {
    glBegin(GL_POLYGON);
    glVertex3fv(A);
    glVertex3fv(B);
    glVertex3fv(C);
    glVertex3fv(D);
    glEnd();
}

void DrawDoor() {
    glEnable(GL_DEPTH_TEST);
    GLfloat PP1[] = {0.15, -0.8, 0.501};  // right down
    GLfloat PP2[] = {-0.15, -0.8, 0.501}; // left down
    GLfloat PP3[] = {-0.15, -0.3, 0.501};  // left up
    GLfloat PP4[] = {0.15, -0.3, 0.501};   // right up
    glColor3f(0.45, 0.35, 0.25);
    DrawQuad(PP1, PP2, PP3, PP4);

    if (isOpen) {
        // Rotate the door gradually
        glTranslatef(-0.65f, 0.0f, 0.65f); // Move the door to the center of the cuboid
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate by the calculated angle around the y-axis
        glutPostRedisplay();

    }
    glColor3f(0.35, 0.25, 0.15);
    // front side E F G H
    GLfloat FP1[] = {0.15, -0.8, 0.52};  // right down
    GLfloat FP2[] = {-0.15, -0.8, 0.52}; // left down
    GLfloat FP3[] = {-0.15, -0.3, 0.52}; // left up
    GLfloat FP4[] = {0.15, -0.3, 0.52}; // right up
    DrawQuad(FP1, FP2, FP3, FP4);
    // back side A B C D
    GLfloat BP1[] = {0.15, -0.8, 0.502}; // right down
    GLfloat BP2[] = {-0.15, -0.8, 0.502}; // left down
    GLfloat BP3[] = {-0.15, -0.3, 0.502}; // left up
    GLfloat BP4[] = {0.15, -0.3, 0.502}; // right up
    DrawQuad(BP1, BP2, BP3, BP4);
    // down side A B F E
    GLfloat DP1[] = {0.15, -0.8, 0.502};  // right down
    GLfloat DP2[] = {-0.15, -0.8, 0.502}; // left down
    GLfloat DP3[] = {-0.15, -0.8, 0.52}; // left up
    GLfloat DP4[] = {0.15, -0.8, 0.52}; // right up
    DrawQuad(DP1, DP2, DP3, DP4);
    // up side D C G H
    GLfloat UP1[] = {0.15, -0.3, 0.502};  // right down
    GLfloat UP2[] = {-0.15, -0.3, 0.502}; // left down
    GLfloat UP3[] = {-0.15, -0.3, 0.52}; // left up
    GLfloat UP4[] = {0.15, -0.3, 0.52}; // right up
    DrawQuad(UP1, UP2, UP3, UP4);
    // right side D A E H
    GLfloat LP1[] = {0.15, -0.3, 0.502};  // right down
    GLfloat LP2[] = {0.15, -0.8, 0.502}; // left down
    GLfloat LP3[] = {0.15, -0.8, 0.52}; // left up
    GLfloat LP4[] = {0.15, -0.3, 0.52}; // right up
    glColor3f(0.35, 0.25, 0.15);
    DrawQuad(LP1, LP2, LP3, LP4);

    // left side B C G F
    GLfloat RP1[] = {-0.15, -0.8, 0.502}; // right down
    GLfloat RP2[] = {-0.15, -0.3, 0.502}; // left down
    GLfloat RP3[] = {-0.15, -0.3, 0.52}; // left up
    GLfloat RP4[] = {-0.15, -0.8, 0.52}; // right up
    DrawQuad(RP1, RP2, RP3, RP4);
}

pair<GLfloat, GLfloat> getF(char x, int w) {
    if (x == 'r' && (w == 1 || w == 3))
        return {-0.169f, 0.835f};
    if (x == 'r' && (w == 2 || w == 4))
        return {-0.6f, 0.4f};
    if (x == 'l' && (w == 2 || w == 4))
        return {-0.82f, 0.28f};
    if (x == 'l' && (w == 1 || w == 3))
        return {-0.388f, 0.72f};

    return {0.0f, 0.0f};
}

void DrawHalf(GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], char half, int nWin) {
    glEnable(GL_DEPTH_TEST);
    if (isOpenW) {
        pair<GLfloat, GLfloat> ret = getF(half, nWin);
        glTranslatef(ret.first, 0.0f, ret.second);
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glutPostRedisplay();
    }
    // back
    GLfloat B1[] = {V1[0], V1[1], static_cast<GLfloat>(V1[2] - 0.02)};
    GLfloat B2[] = {V2[0], V2[1], static_cast<GLfloat>(V2[2] - 0.02)};
    GLfloat B3[] = {V3[0], V3[1], static_cast<GLfloat>(V3[2] - 0.02)};
    GLfloat B4[] = {V4[0], V4[1], static_cast<GLfloat>(V4[2] - 0.02)};
    // right
    GLfloat R1[] = {V1[0], V1[1], static_cast<GLfloat>(V1[2] - 0.02)};
    GLfloat R2[] = {V1[0], V1[1], V1[2]};
    GLfloat R3[] = {V4[0], V4[1], V4[2]};
    GLfloat R4[] = {V4[0], V4[1], static_cast<GLfloat>(V4[2] - 0.02)};
    // left
    GLfloat L1[] = {V2[0], V2[1], static_cast<GLfloat>(V2[2] - 0.02)};
    GLfloat L2[] = {V2[0], V2[1], V2[2]};
    GLfloat L3[] = {V3[0], V3[1], V3[2]};
    GLfloat L4[] = {V3[0], V3[1], static_cast<GLfloat>(V3[2] - 0.02)};
    // up
    GLfloat U1[] = {V4[0], V4[1], V4[2]};
    GLfloat U2[] = {V3[0], V3[1], V3[2]};
    GLfloat U3[] = {V3[0], V3[1], static_cast<GLfloat>(V3[2] - 0.02)};
    GLfloat U4[] = {V4[0], V4[1], static_cast<GLfloat>(V4[2] - 0.02)};
    // down
    GLfloat D1[] = {V1[0], V1[1], static_cast<GLfloat>(V1[2] - 0.02)};
    GLfloat D2[] = {V2[0], V2[1], V2[2]};
    GLfloat D3[] = {V2[0], V2[1], static_cast<GLfloat>(V2[2] - 0.02)};
    GLfloat D4[] = {V1[0], V1[1], static_cast<GLfloat>(V1[2] - 0.02)};

    glColor3f(0.35, 0.25, 0.15);
    DrawQuad(L1, L2, L3, L4);
    DrawQuad(R1, R2, R3, R4);
    DrawQuad(U1, U2, U3, U4);
    DrawQuad(D1, D2, D3, D4);
    DrawQuad(V1, V2, V3, V4);
    DrawQuad(B1, B2, B3, B4);
}

void DrawWindow(GLfloat V1[], GLfloat V2[], GLfloat V3[], GLfloat V4[], float half, int nWin) {
    glEnable(GL_DEPTH_TEST);
    // right half
    GLfloat R2[] = {half, V2[1], V2[2]};
    GLfloat R3[] = {half, V3[1], V3[2]};
    glPushMatrix();
    DrawHalf(V1, R2, R3, V4, 'r', nWin);
    glPopMatrix();

    // left half
    GLfloat R1[] = {half, V1[1], V1[2]};
    GLfloat R4[] = {half, V4[1], V4[2]};
    glPushMatrix();
    DrawHalf(R1, V2, V3, R4, 'l', nWin);
    glPopMatrix();
}

void DrawWindowBackground(){
    glEnable(GL_DEPTH_TEST);
    GLfloat bw1[] = {0.33, -0.05, 0.501};  // right down
    GLfloat bw2[] = {0.1, -0.05, 0.501}; // left down
    GLfloat bw3[] = {0.1, 0.1, 0.501}; // left up
    GLfloat bw4[] = {0.33, 0.1, 0.501}; // right up

    GLfloat bx1[] = {-0.33, -0.05, 0.501};  // right down
    GLfloat bx2[] = {-0.1, -0.05, 0.501}; // left down
    GLfloat bx3[] = {-0.1, 0.1, 0.501}; // left up
    GLfloat bx4[] = {-0.33, 0.1, 0.501}; // right up

    GLfloat by1[] = {0.33, 0.4, 0.501};  // right down
    GLfloat by2[] = {0.1, 0.4, 0.501}; // left down
    GLfloat by3[] = {0.1, 0.55, 0.501}; // left up
    GLfloat by4[] = {0.33, 0.55, 0.501}; // right up

    GLfloat bz1[] = {-0.33, 0.4, 0.501};  // right down
    GLfloat bz2[] = {-0.1, 0.4, 0.501}; // left down
    GLfloat bz3[] = {-0.1, 0.55, 0.501}; // left up
    GLfloat bz4[] = {-0.33, 0.55, 0.501}; // right up

    glColor3f(0.45, 0.35, 0.25);
    DrawQuad(bw1, bw2, bw3, bw4);
    glColor3f(0.45, 0.35, 0.25);
    DrawQuad(bx1, bx2, bx3, bx4);
    glColor3f(0.45, 0.35, 0.25);
    DrawQuad(by1, by2, by3, by4);
    glColor3f(0.45, 0.35, 0.25);
    DrawQuad(bz1, bz2, bz3, bz4);
}

void DrawBuilding(GLfloat V0[], GLfloat V1[], GLfloat V2[], GLfloat V3[],
                  GLfloat V4[], GLfloat V5[], GLfloat V6[], GLfloat V7[]) {
    glEnable(GL_DEPTH_TEST);
    GLfloat V8[] = {V0[0], static_cast<GLfloat>(V0[1] - 0.1), V0[2]};
    GLfloat V9[] = {V1[0], static_cast<GLfloat>(V1[1] - 0.1), V1[2]};
    GLfloat V10[] = {V5[0], static_cast<GLfloat>(V5[1] - 0.1), V5[2]};
    GLfloat V11[] = {V4[0], static_cast<GLfloat>(V4[1] - 0.1), V4[2]};

    glColor3f(1.0f, 0.9f, 0.85f); // Off-white color
    DrawQuad(V0, V1, V2, V3); // front
    DrawQuad(V4, V5, V6, V7); // back
    DrawQuad(V0, V3, V7, V4); // left
    DrawQuad(V1, V2, V6, V5); // right
    glColor3f(0.35, 0.25, 0.15);
    DrawQuad(V8, V9, V10, V11); // Lower roof

    // the divider between the two floors
    glColor3f(0.35, 0.25, 0.15);
    GLfloat F1[] = {0.5, 0.255, 0.501};  // right down
    GLfloat F2[] = {-0.5, 0.255, 0.501}; // left down
    GLfloat F3[] = {-0.5, 0.3, 0.501}; // left up
    GLfloat F4[] = {0.5, 0.3, 0.501}; // right up
    DrawQuad(F1, F2, F3, F4);
    GLfloat R1[] = {0.501, 0.255, -0.501}; // right divider strip
    GLfloat R2[] = {0.501, 0.255, 0.501};
    GLfloat R3[] = {0.501, 0.3, 0.501};
    GLfloat R4[] = {0.501, 0.3, -0.501};
    DrawQuad(R1, R2, R3, R4);
    GLfloat L1[] = {-0.501, 0.255, -0.501}; // left divider strip
    GLfloat L2[] = {-0.501, 0.255, 0.501};
    GLfloat L3[] = {-0.501, 0.3, 0.501};
    GLfloat L4[] = {-0.501, 0.3, -0.501};
    DrawQuad(L1, L2, L3, L4);
    GLfloat B1[] = {0.5, 0.255, -0.501}; // back divider strip
    GLfloat B2[] = {-0.5, 0.255, -0.501};
    GLfloat B3[] = {-0.5, 0.3, -0.501};
    GLfloat B4[] = {0.5, 0.3, -0.501};
    DrawQuad(B1, B2, B3, B4);
}
void DrawPalestineFlag(){
    glPushMatrix(); // the pole
    glColor3f(0, 0, 0);
    glTranslated(0.48f, -0.2f, 2.0f);
    glScaled(0.01, 0.35, 0.01);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
    
    
    glPushMatrix();// lights holder
    glColor3f(0, 0, 0);
    glTranslated(0.33f, -0.06f, 2.0f);
    glScaled(0.3 ,0.06, 0);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
    
    glPushMatrix();// lights holder
    glColor3f(1, 1, 1);
    glTranslated(0.33f, -0.12f, 2.0f);
    glScaled(0.3 ,0.06, 0);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
    
    glPushMatrix();// lights holder
    glColor3f(0, 0.5, 0.03);
    glTranslated(0.33f, -0.18f, 2.0f);
    glScaled(0.3 ,0.06, 0);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
    
    /// Draw the red triangle
        glPushMatrix();
        glColor3f(0.8, 0.2, 0.2); // Red color
        glBegin(GL_TRIANGLES);
        glVertex3f(0.48f, -0.03f, 2.01f); // Top vertex
        glVertex3f(0.48f, -0.21f, 2.01f); // Bottom right vertex
        glVertex3f(0.33f, -0.13f, 2.01f);
        glEnd();
        glPopMatrix();
        glPushMatrix();
        glColor3f(0.8, 0.2, 0.2); // Red color
        glBegin(GL_TRIANGLES);
        glVertex3f(0.48f, -0.03f, 1.99f); // Top vertex
        glVertex3f(0.48f, -0.21f, 1.99f); // Bottom right vertex
        glVertex3f(0.33f, -0.13f, 1.99f);
        glEnd();
        glPopMatrix();
    
    
}
void bicycle() {
    glPushMatrix(); // Right Wheel
    glColor3f(0, 0, 0);
    glTranslated(0.23, -0.7, 1.3);
    glRotated(rightWheelAngle, 0, 1, 0);
    glutSolidTorus(0.02, 0.08, 20, 20);
    glPopMatrix();

    glPushMatrix(); // Left Wheel
    glColor3f(0, 0, 0);
    glTranslated(-0.22, -0.7, 1.3);
    glRotated(leftWheelAngle, 0, 1, 0);
    glutSolidTorus(0.02, 0.08, 20, 20);
    glPopMatrix();

    glPushMatrix(); // Bicycle bottom horizontal pipe o---o
    glColor3f(0.439f, 0.333f, 0.259f);
    glTranslated(-0.012, -0.69, 1.3);
    glScaled(0.31, 0.02, 0.03);
    glutSolidCube(1);
    glColor3f(0.439f, 0.333f, 0.259f);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix(); //bicycle top horizontal pipe
    glColor3f(0.439f, 0.333f, 0.259f);
    glTranslated(-0.02, -0.5, 1.3);
    glScaled(0.3, 0.03, 0.1);
    glutSolidCube(1);
    glColor3f(0.439f, 0.333f, 0.259f);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix(); //bicycle vertical pipes supportive cube + seat
    glColor3f(0.439f, 0.333f, 0.259f);
    glTranslated(-0.06, -0.54, 1.3);
    glScaled(0.035, 0.3, 0.05);
    glutSolidCube(1);
    glColor3f(0.439f, 0.333f, 0.259f);
    glutWireCube(1);
    glPopMatrix();

    // vertical handle
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslated(0.12, -0.53, 1.3);
    glScaled(0.02, 0.3, 0.03);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
    // horizontal handle
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslated(0.118, -0.39, 1.3);
    glScaled(0.02, 0.02, 0.2);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix(); //paddle
    glColor3f(0, 0, 0);
    glTranslated(0.06, -0.685, 1.3);
    glScaled(0.03, 0.02, 0.26);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
    
    // the flag
    glPushMatrix();
    glTranslated(-0.6f, -0.1f, -0.7f);
    DrawPalestineFlag();
    glPopMatrix();
}

void update(int value) {
    if (isRotateBike) {
        angle += 1.0; // Decrease the angle to rotate clockwise
        if (angle < 360) {
            angle -= 360; // Keep angle within [0, 360] range
        }
        glutPostRedisplay(); // Request redisplay to update the scene
    }

    glutTimerFunc(30,update, 0); // Start the update loop// 60 frames per second
}

void DrawBicycle() {
    // Translate the bicycle to the center of the building
    glPushMatrix();

    // Rotate the bicycle around the y-axis
    glRotatef(angle, 0.0f,  1.0f, 0.0f);
    glTranslatef( 0.0f,0.0f, xChange);
    // Draw the bicycle
    bicycle();
    glPopMatrix();
    
}

void DrawStreetLight(){
    glPushMatrix(); // the pole
    glColor3f(0, 0, 0);
    glTranslated(0.6, -0.2, 0);
    glScaled(0.06, 1.2, 0.04);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();// lights holder
    glColor3f(0, 0, 0);
    glTranslated(0.6, 0.4, 0);
    glScaled(0.06, 0, 0.55);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();// the right light
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslated(0.6, 0.4, -0.15);
    glScaled(0.04, 0.02, 0.15);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    glPushMatrix();// the left light
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslated(0.6, 0.40, 0.17);
    glScaled(0.04, 0.02, 0.15);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
};

void renderScene() {
    GLfloat V[8][3] =
            {
                    {-0.5, 0.8,  0.5},
                    {0.5,  0.8,  0.5},
                    {0.5,  -0.8, 0.5},
                    {-0.5, -0.8, 0.5},
                    {-0.5, 0.8,  -0.5},
                    {0.5,  0.8,  -0.5},
                    {0.5,  -0.8, -0.5},
                    {-0.5, -0.8, -0.5}

            };


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.4, 0.4, 0.35);
    glBegin(GL_QUADS);
    glVertex3f(-5, -0.8, -5);
    glVertex3f(-5, -0.8, 5);
    glVertex3f(5, -0.8, 5);
    glVertex3f(5, -0.8, -5);
    glEnd();

    DrawBuilding(V[0], V[1], V[2], V[3], V[4], V[5], V[6], V[7]);
    DrawWindowBackground();
    // Floor 1 right window
    GLfloat FR1[] = {0.33, -0.05, 0.513}; // right down
    GLfloat FR2[] = {0.1, -0.05, 0.513}; // left down
    GLfloat FR3[] = {0.1, 0.1, 0.513}; // left up
    GLfloat FR4[] = {0.33, 0.1, 0.513}; // right up
    glPushMatrix();
    DrawWindow(FR1, FR2, FR3, FR4, 0.22, 1);
    glPopMatrix();
    // Floor 1 left window
    GLfloat FL2[] = {-0.33, -0.05, 0.513}; // right down
    GLfloat FL1[] = {-0.1, -0.05, 0.513}; // left down
    GLfloat FL4[] = {-0.1, 0.1, 0.513}; // left up
    GLfloat FL3[] = {-0.33, 0.1, 0.513}; // right up
    glPushMatrix();
    DrawWindow(FL1, FL2, FL3, FL4, -0.22, 2);
    glPopMatrix();
    // Floor 2 right window
    GLfloat SR1[] = {0.33, 0.4, 0.513};  // right down
    GLfloat SR2[] = {0.1, 0.4, 0.513}; // left down
    GLfloat SR3[] = {0.1, 0.55, 0.513}; // left up
    GLfloat SR4[] = {0.33, 0.55, 0.513}; // right up
    glPushMatrix();
    DrawWindow(SR1, SR2, SR3, SR4, 0.22, 3);
    glPopMatrix();
    // Floor 2 left window
    GLfloat SL1[] = {-0.1, 0.4, 0.513};  // right down
    GLfloat SL2[] = {-0.33, 0.4, 0.513}; // left down
    GLfloat SL3[] = {-0.33, 0.55, 0.513}; // left up
    GLfloat SL4[] = {-0.1, 0.55, 0.513}; // right up
    glPushMatrix();
    DrawWindow(SL1, SL2, SL3, SL4, -0.22, 4);
    glPopMatrix();

    glPushMatrix();
    DrawDoor();
    glPopMatrix();

    glPushMatrix();
    DrawBicycle();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.6f);
    DrawStreetLight();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': // Move camera left
            cameraX -= 0.1f;
            break;
        case 'd': // Move camera right
            cameraX += 0.1f;
            break;
        case 'w': // Move camera up
            cameraY += 0.1f;
            break;
        case 's': // Move camera down
            cameraY -= 0.1f;
            break;
        case 'q': // Move camera backward
            cameraZ -= 0.1f;
            break;
        case 'e': // Move camera forward
            cameraZ += 0.1f;
            break;
        case 'o': // open door
            isOpen = true;
            break;
        case 'O': // open windows
            isOpenW = true;
            break;
        case 'c': // close door
            isOpen = false;
            break;
        case 'C': // close windows
            isOpenW = false;
            break;
        case 'f': // go forward
            xChange+= 0.1;
            isMoveBike = true;
            break;
        case 'b': // go backward
            xChange -= 0.1;
            isMoveBike = true;
            break;
        case 'r': // rotate right wheel
            rightWheelAngle = 15.0;
            break;
        case 'l': // rotate left wheel
            leftWheelAngle = 15.0;
            break;
        case 'p':
            isRotateBike = false;
        default:
            break;
    }
    glutPostRedisplay();
}

void handleMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isLeftButtonPressed = true;
            isRightButtonPressed = false;
            isRotateBike = true;
        }
        else if (state == GLUT_UP) {
            isLeftButtonPressed = false;
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON){
        if (state == GLUT_DOWN) {
            isRightButtonPressed = true;
            isLeftButtonPressed = false;
            isRotateBike = false;
        }
        else if (state == GLUT_UP) {
            isRightButtonPressed = false;
        }
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowPosition(250, 50);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CG Project");
    glClearColor(0.5f, 0.7f, 0.0f, 1.0f);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutTimerFunc(10,update, 0);
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouseClick);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
