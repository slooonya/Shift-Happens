#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "vector"

#define checkImageWidth 64
#define checkImageHeight 64

using namespace std;

// Car attributes
GLfloat carX = 0.0f;
GLfloat carY = 0.0f;
GLfloat carZ = -0.0f;
GLfloat carTurnAngle = 0.0f;
const GLfloat MAX_TURN_ANGLE = 5.0f;
const GLfloat MAX_CAR_X = 4.0f;
bool turningRight = false;
bool turningLeft = false;
GLfloat carSpeed = 0.0f;
GLfloat wheelRotationAngle = 0.0f;
GLint numSpeedUp = 0;

// NPC spectator attributes
GLfloat armAngle = 0.0f;
bool armUp = true;
GLfloat legAngle = 0.0f;
GLfloat humanPosition = 0.0f;
const GLint NUM_SPECTATORS = 5;
const GLfloat SPECTATOR_SPACING = 200.0f;
const GLfloat WALKING_HUMAN_SPACING = 100.0f;

// Camera control
GLfloat camTurn = 0;

// Game stats and logic
GLint score = 0;
GLint lives = 3;
bool collisionOccured = false;

// Game state
bool inMenu = true;
bool isPaused = false;
bool isGameOver = false;

// NPC cars attributes
GLfloat carNPCcoorZ = 0.0f;
GLfloat carNPCspeed = 0.1f;

// Item attribute
GLfloat itemRotationAngle = 0;

// Boulder (obstacle) attributes 
GLfloat boulder1RotationAngle = 0;
GLfloat boulder1X = 0.1;
char boulder1MoveDirection = 'r';
GLfloat boulder2RotationAngle = 0;
GLfloat boulder2X = 0.1;
char boulder2MoveDirection = 'l';

// Texture variables
vector<GLubyte*>p;
GLuint textures[19];
GLint imagewidth0, imageheight0, pixellength0,
      imagewidth1, imageheight1, pixellength1,
      imagewidth2, imageheight2, pixellength2,
      imagewidth3, imageheight3, pixellength3,
      imagewidth4, imageheight4, pixellength4,
      imagewidth5, imageheight5, pixellength5,
      imagewidth6, imageheight6, pixellength6,
      imagewidth7, imageheight7, pixellength7,
      imagewidth8, imageheight8, pixellength8,
      imagewidth9, imageheight9, pixellength9,
      imagewidth10, imageheight10, pixellength10,
      imagewidth11, imageheight11, pixellength11,
      imagewidth12, imageheight12, pixellength12,
      imagewidth13, imageheight13, pixellength13,
      imagewidth14, imageheight14, pixellength14,
      imagewidth15, imageheight15, pixellength15,
      imagewidth16, imageheight16, pixellength16,
      imagewidth17, imageheight17, pixellength17;
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

// Light parameters to be changed over time
GLfloat elapsedTime = 0.0f;
GLfloat light0_position[] = { 0, 3, 3, 0.0 };
GLfloat light1_position[] = { 0, 80, 84, 0.0 };
GLfloat light2_position[] = { 1, -1, 2, 0.0 };
GLfloat light_diffuse[] = { 1.0f, 0.95f, 0.8f, 1.0f };


//====================== Colors for human torsos and arms =====================
struct Color {
    GLfloat r, g, b;
};

Color colors[] = {
    {1.0f, 0.349f, 0.371f},
    {1.0f, 0.792f, 0.227f},
    {0.541f, 0.784f, 0.149f},
    {0.098f, 0.51f, 0.769f},
    {0.416f, 0.298f, 0.576f}
};

const int NUM_COLORS = 5;
//=============================================================================


//== Bounding boxes for obstacles and the player's car (collision detection) ==
struct BoundingBox {
    GLfloat xMin, xMax;
    GLfloat yMin, yMax;
    GLfloat zMin, zMax;
};

BoundingBox createCarBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 0.65f;
    box.xMax = x + 0.65f;
    box.yMin = y - 0.3f;
    box.yMax = y + 0.3f;
    box.zMin = z - 1.1f;
    box.zMax = z + 1.1f;
    return box;
}

BoundingBox createConesBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 0.5f;
    box.xMax = x + 5.5f;
    box.yMin = y - 0.5f;
    box.yMax = y + 0.5f;
    box.zMin = z - 0.5f;
    box.zMax = z + 0.5f;
    return box;
}

BoundingBox createCrateBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 0.75f;
    box.xMax = x + 0.75f;
    box.yMin = y - 0.75f;
    box.yMax = y + 0.75f;
    box.zMin = z - 0.75f;
    box.zMax = z + 0.75f;
    return box;
}

BoundingBox createSandpileBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 3.0f;
    box.xMax = x + 3.0f;
    box.yMin = y - 2.0f;
    box.yMax = y + 3.0f;
    box.zMin = z - 3.0f;
    box.zMax = z + 3.0f;
    return box;
}

BoundingBox createHoleBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 2.0f;
    box.xMax = x + 2.0f;
    box.yMin = y;
    box.yMax = y + 1.0f;
    box.zMin = z - 2.0f;
    box.zMax = z + 2.0f;
    return box;
}

BoundingBox createBoulder1BoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - boulder1X - 0.5f;
    box.xMax = x - boulder1X + 0.5f;
    box.yMin = y - 0.5f;
    box.yMax = y + 0.5f;
    box.zMin = z - 0.5f;
    box.zMax = z + 0.5f;
    return box;
}

BoundingBox createBoulder2BoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - boulder2X - 0.5f;
    box.xMax = x - boulder2X + 0.5f;
    box.yMin = y - 0.5f;
    box.yMax = y + 0.5f;
    box.zMin = z - 0.5f;
    box.zMax = z + 0.5f;
    return box;
}

BoundingBox createCarNPCBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 0.8f;
    box.xMax = x + 0.8f;
    box.yMin = y - 0.95f;
    box.yMax = y + 0.95f;
    box.zMin = z - 3.0f;
    box.zMax = z + 3.0f;
    return box;
}

BoundingBox createItemBoundingBox(GLfloat x, GLfloat y, GLfloat z) {
    BoundingBox box;
    box.xMin = x - 0.3f;
    box.xMax = x + 0.3f;
    box.yMin = y - 0.3f;
    box.yMax = y + 0.3f;
    box.zMin = z - 0.3f;
    box.zMax = z + 0.3f;
    return box;
}
//==============================================================================


//================ Projection and Viewport Setting =============================
void setOrthoProjection() { // Setting for the menus and text
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 500);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void resetPerspectiveProjection() { // Setting for the 3D scene
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    GLfloat aspectRatio = (GLfloat)width / (GLfloat)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspectRatio, 1.0, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//==============================================================================


//======================== Interativity ========================================
// Keyboard interaction
void keyboardInput(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {                    // Acceleration
        if (numSpeedUp < 10) {
            numSpeedUp++;
            carSpeed += 0.1;
        }
    }
    else if ((key == 'a' || key == 'A') && !isPaused && !isGameOver && !inMenu) {  // Left turn
        turningLeft = true;                            // Initiates car left rotation and movement
        turningRight = false;
    }
    else if (key == 's' || key == 'S') {               // Braking
        if (numSpeedUp > 1) {
            numSpeedUp--;
            carSpeed -= 0.1;
        }
    }
    else if ((key == 'd' || key == 'D') && !isPaused && !isGameOver && !inMenu) {   // Right turn
        turningLeft = false;                           // Initiates car right rotation and movement
        turningRight = true;
    }
    else if ((key == 'p' || key == 'P') && !inMenu && !isGameOver) {  // Game pause
        isPaused = !isPaused;
    }
    else if ((key == 'r' || key == 'R') && !inMenu && !isPaused && isGameOver) {
        lives = 3;                                     // Reseting the game after its over
        carZ = 0;
        carX = 0;
        score = 0;
        numSpeedUp = 0;
        camTurn = 0;
        inMenu = true;
        isGameOver = false;
    }
    else if (key == 'q' || key == 'Q')				   // Close the program
        exit(0);
}

void keyboardInputRelease(unsigned char key, int x, int y) {
    if (key == 'a' || key == 'A')       // Returns the car to face forward when not stirring left anymore
        turningLeft = false; 
    else if (key == 'd' || key == 'D')  // Returns the car to face forward when not stirring right anymore
        turningRight = false;
}

// Arrows interaction
void specialKeys(int key, int x, int y) { 
    if (key == GLUT_KEY_LEFT) {  // Turning camera left
        if (camTurn < 2.5) 
            camTurn += 0.1;
    }
    if (key == GLUT_KEY_RIGHT) { // Turning camera right
        if (camTurn > -2.5)
            camTurn -= 0.1;
    }
}

// Mouse interaction
void mouseInput(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        int viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        y = viewport[3] - y;
        if (x >= 390 && x <= 470 && y >= 230 && y <= 260 && inMenu) {       // Easy difficulty selection
            carSpeed = 0.3f;
            inMenu = false;
        }
        else if (x >= 390 && x <= 470 && y >= 190 && y <= 220 && inMenu) {  // Medium difficulty selection
            carSpeed = 0.7f;
            inMenu = false;
        }
        else if (x >= 390 && x <= 470  && y >= 150 && y <= 180 && inMenu) { // Hard difficulty selection
            carSpeed = 1.0f;
            inMenu = false;
        }
    }
}
//==============================================================================
 

//======================= Texture mapping ======================================
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
    // Reading bitmap images
    GLubyte* pixeldata;
    FILE* pfile;
    fopen_s(&pfile, path, "rb");
    if (pfile == 0) exit(0);

    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);

    pixellength = imagewidth * 3;
    while (pixellength % 4 != 0)pixellength++;
    pixellength *= imageheight;

    pixeldata = (GLubyte*)malloc(pixellength);
    if (pixeldata == 0) exit(0);

    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    p.push_back(pixeldata);

    fclose(pfile);
}

void setupTexture(GLuint textureID, GLubyte* data, GLint width, GLint height) {
    // Settings and parameters for texturing
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void makeCheckImage(void) { // Creates checkerboard pattern
    int i, j, c;
    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkImage[i][j][0] = (GLubyte)c;
            checkImage[i][j][1] = (GLubyte)c;
            checkImage[i][j][2] = (GLubyte)c;
        }
    }
}

void initTextures(void) { // Texture initialization 
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);

    ReadImage("grass.bmp", imagewidth0, imageheight0, pixellength0);
    ReadImage("bark.bmp", imagewidth1, imageheight1, pixellength1);
    ReadImage("road.bmp", imagewidth2, imageheight2, pixellength2);
    ReadImage("sand.bmp", imagewidth3, imageheight3, pixellength3);
    ReadImage("foliage.bmp", imagewidth4, imageheight4, pixellength4);
    ReadImage("crate.bmp", imagewidth5, imageheight5, pixellength5);
    ReadImage("barrier.bmp", imagewidth6, imageheight6, pixellength6);
    ReadImage("flagstone.bmp", imagewidth7, imageheight7, pixellength7);
    ReadImage("building.bmp", imagewidth8, imageheight8, pixellength8);
    ReadImage("building1.bmp", imagewidth9, imageheight9, pixellength9);
    ReadImage("building2.bmp", imagewidth10, imageheight10, pixellength10);
    ReadImage("building3.bmp", imagewidth11, imageheight11, pixellength11);
    ReadImage("cones.bmp", imagewidth12, imageheight12, pixellength12);
    ReadImage("light.bmp", imagewidth13, imageheight13, pixellength13);
    ReadImage("window.bmp", imagewidth14, imageheight14, pixellength14);
    ReadImage("metal.bmp", imagewidth15, imageheight15, pixellength15);
    ReadImage("license.bmp", imagewidth16, imageheight16, pixellength16);
    ReadImage("aluminum.bmp", imagewidth17, imageheight17, pixellength17);
    makeCheckImage();

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(19, &textures[0]);

    setupTexture(textures[0], p[0], imagewidth0, imageheight0);
    setupTexture(textures[1], p[1], imagewidth1, imageheight1);
    setupTexture(textures[2], p[2], imagewidth2, imageheight2);
    setupTexture(textures[3], p[3], imagewidth3, imageheight3);
    setupTexture(textures[4], p[4], imagewidth4, imageheight4);
    setupTexture(textures[5], p[5], imagewidth5, imageheight5);
    setupTexture(textures[6], p[6], imagewidth6, imageheight6);
    setupTexture(textures[7], p[7], imagewidth7, imageheight7);
    setupTexture(textures[8], p[8], imagewidth8, imageheight8);
    setupTexture(textures[9], p[9], imagewidth9, imageheight9);
    setupTexture(textures[10], p[10], imagewidth10, imageheight10);
    setupTexture(textures[11], p[11], imagewidth11, imageheight11);
    setupTexture(textures[12], p[12], imagewidth12, imageheight12);
    setupTexture(textures[13], p[13], imagewidth13, imageheight13);
    setupTexture(textures[14], p[14], imagewidth14, imageheight14);
    setupTexture(textures[15], p[15], imagewidth15, imageheight15);
    setupTexture(textures[16], p[16], imagewidth16, imageheight16);
    setupTexture(textures[17], p[17], imagewidth17, imageheight17);
    setupTexture(textures[18], (GLubyte*)checkImage, checkImageWidth, checkImageHeight);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
//==============================================================================


//======================== Text rendering ======================================
void renderText(GLfloat x, GLfloat y, const char* text) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void renderMainMenu() {
    setOrthoProjection();
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0, 0, 0);   
    glBegin(GL_QUADS);
    glVertex2f(240, 120); // Menu background
    glVertex2f(560, 120);
    glVertex2f(560, 370);
    glVertex2f(240, 370);

    glVertex2f(0, 120);   // Controls background
    glVertex2f(220, 120);
    glVertex2f(220, 370);
    glVertex2f(0, 370);

    glVertex2f(800, 120);  // Tips background
    glVertex2f(580, 120);
    glVertex2f(580, 370);
    glVertex2f(800, 370);
    glEnd();

    glColor3f(0, 1, 0);    // Easy background
    glBegin(GL_QUADS);
    glVertex2f(360, 260);
    glVertex2f(440, 260);
    glVertex2f(440, 230);
    glVertex2f(360, 230);
    glEnd();

    glColor3f(1, 1, 0);    // Medium background
    glBegin(GL_QUADS);
    glVertex2f(360, 220);
    glVertex2f(440, 220);
    glVertex2f(440, 190);
    glVertex2f(360, 190);
    glEnd();

    glColor3f(1, 0, 0);    // Hard difficulty
    glBegin(GL_QUADS);
    glVertex2f(360, 180);
    glVertex2f(440, 180);
    glVertex2f(440, 150);
    glVertex2f(360, 150);
    glEnd();

    glColor3f(1, 1, 1);
    renderText(280, 320, "Welcome to the Racing Game!");
    renderText(280, 280, "Select game difficulty to start:");

    renderText(70, 320, "Controls:");
    renderText(37, 290, "W - speed up");
    renderText(40, 270, "S - slow down");
    renderText(40, 250, "A - car turn left");
    renderText(40, 230, "D - car turn right");
    renderText(40, 200, "< - cam turn left");
    renderText(40, 180, "> - cam turn right");
    renderText(40, 150, "P - pause");

    renderText(670, 320, "Tips:");
    renderText(600, 290, "Try to avoid obstacles");
    renderText(650, 250, "Hit = -1 life");
    renderText(610, 210, "Collect yellow items");
    renderText(640, 170, "Item = +1 life");

    glColor3f(0, 0, 0);
    renderText(380, 240, "Easy");
    renderText(370, 200, "Medium");
    renderText(380, 160, "Hard");

    glPopMatrix();
    resetPerspectiveProjection();
}

void renderPauseMenu() {
    setOrthoProjection();
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0, 0, 0);    // Menu background
    glBegin(GL_QUADS);
    glVertex2f(290, 290);
    glVertex2f(520, 290);
    glVertex2f(520, 190);
    glVertex2f(290, 190);
    glEnd();

    glColor3f(1, 1, 1);
    renderText(350, 250, "Game Paused");
    renderText(330, 210, "Press 'P' to resume");

    glPopMatrix();
    resetPerspectiveProjection();
}

void renderGameOver() {
    setOrthoProjection();
    char strScore[50];
    sprintf_s(strScore, "Score: %d", score);

    glPushMatrix();
    glLoadIdentity();

    glColor3f(0, 0, 0);    // Menu background
    glBegin(GL_QUADS);
    glVertex2f(290, 290);
    glVertex2f(520, 290);
    glVertex2f(520, 140);
    glVertex2f(290, 140);
    glEnd();

    glColor3f(1, 1, 1);
    renderText(360, 250, "Game Over");
    renderText(360, 210, strScore);
    renderText(330, 170, "Press 'R' to restart");

    glPopMatrix();
    resetPerspectiveProjection();
}

void renderScore(int score) {
    setOrthoProjection();
    char strScore[50];
    sprintf_s(strScore, "Score: %d", score);

    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    renderText(650, 450, strScore);
    glPopMatrix();
    resetPerspectiveProjection();
}

void renderLives(int lives) {
    setOrthoProjection();
    glPushMatrix();

    char livesText[20];
    sprintf_s(livesText, "Lives: %d", lives);

    glColor3f(1, 1, 1);
    renderText(50, 450, livesText);

    glPopMatrix();
    resetPerspectiveProjection();
}
//===============================================================================


//=========================== Lighting and Materials ============================
void setupLighting() {
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
}

void setMaterial(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
}

void setShinyMaterial() {
    GLfloat ambient[] = {0.25f, 0.25f, 0.25f, 1.0f};
    GLfloat diffuse[] = {0.4f, 0.4f, 0.4f, 1.0f};
    GLfloat specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat shininess = 90.0f;

    setMaterial(ambient, diffuse, specular, shininess);
}

void setMatteMaterial() {
    GLfloat ambient[] = {0.05f, 0.05f, 0.05f, 1.0f};
    GLfloat diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat shininess = 0.0f;

    setMaterial(ambient, diffuse, specular, shininess);
}
//===============================================================================


//================ Generic functions to texture 3D shapes =======================
void drawTexturedCylinder(GLuint textureID, GLfloat baseRadius, GLfloat topRadius,
                          GLfloat height, GLint slices, GLint stacks) {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Draw the side surface
    gluCylinder(quadric, baseRadius, topRadius, height, slices, stacks);

    // Draw the bottom base
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, 0.0f); // Center point
    for (int i = 0; i <= slices; i++) {
        GLfloat angle = 2.0f * 3.14 * (GLfloat)i / (GLfloat)slices;
        GLfloat x = baseRadius * cosf(angle);
        GLfloat y = baseRadius * sinf(angle);
        glTexCoord2f(x * 0.5f / baseRadius + 0.5f, y * 0.5f / baseRadius + 0.5f);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Draw top base
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, height); // Center point
    for (int i = 0; i <= slices; i++) {
        GLfloat angle = 2.0f * 3.14 * (GLfloat)i / (GLfloat)slices;
        GLfloat x = topRadius * cosf(angle);
        GLfloat y = topRadius * sinf(angle);
        glTexCoord2f(x * 0.5f / topRadius + 0.5f, y * 0.5f / topRadius + 0.5f);
        glVertex3f(x, y, height);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quadric);
}

void drawTexturedSphere(GLuint textureID, GLfloat radius, GLint slices, GLint stacks) {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    gluSphere(quadric, radius, slices, stacks);
    glDisable(GL_TEXTURE_2D);
    gluDeleteQuadric(quadric);
}

void drawTexturedCube(GLuint textureID, GLfloat size) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, size);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0, 1.0); glVertex3f(size, size, -size);
    glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, -size);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(size, -size, -size);
    glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, size);
    glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
    glTexCoord2f(0.0, 1.0); glVertex3f(size, size, -size);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
    glTexCoord2f(1.0, 0.0); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0, 1.0); glVertex3f(-size, size, size);
    glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, -size);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-size, size, -size);
    glTexCoord2f(1.0, 0.0); glVertex3f(size, size, -size);
    glTexCoord2f(1.0, 1.0); glVertex3f(size, size, size);
    glTexCoord2f(0.0, 1.0); glVertex3f(-size, size, size);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, size);
    glTexCoord2f(1.0, 0.0); glVertex3f(size, -size, size);
    glTexCoord2f(1.0, 1.0); glVertex3f(size, -size, -size);
    glTexCoord2f(0.0, 1.0); glVertex3f(-size, -size, -size);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}
//================================================================================


//=============== Functions to draw various parts of the scene ===================
//______________________________ Main surface ____________________________________
void drawGround() { // Drawing tile by tile for the texture to map without pixelation
    setMatteMaterial();
    for (int z = -500; z < 1100; z += 20) { 
        for (int x = -100; x < 100; x += 20) {
                glPushMatrix();
                glTranslatef((GLfloat)x, -3.0f, (GLfloat)-1000.0f + z);
                glScalef(1.0f, 0.01f, 1.0f);
                drawTexturedCube(textures[0], 10.0f);
                glPopMatrix();
        }
    }
}

void drawRoad() { // Drawing tile by tile to avoid pixelation
    setMatteMaterial();

    for (GLfloat z = -1500.0f; z < 100.0f; z += 12.0f) {
        glPushMatrix();
        glTranslatef(0.0f, -8.4f, z);
        drawTexturedCube(textures[2], 6.0f);
        glPopMatrix();
    }
}

void drawSidewalk() { // Drawing tile by tile to avoid pixelation
    setMatteMaterial();

    for (int i = -500; i < 800; i++) {
        GLfloat zPosition = -1000.0f + (i * 5.0f);
        
        glPushMatrix(); // Right side
        glTranslatef(8.15f, -4.7f, zPosition);
        drawTexturedCube(textures[7], 2.5f);
        glPopMatrix();

        glPushMatrix(); // Left side
        glTranslatef(-8.15f, -4.7f, zPosition);
        drawTexturedCube(textures[7], 2.5f);
        glPopMatrix();
    }
}
//________________________________________________________________________________


//________________________________ Buildings _____________________________________
void drawBuilding(int heightIndex, GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(5.0f, (heightIndex + 1) * 5.0f, 5.0f);
    drawTexturedCube(textures[8 + heightIndex], 1.0f);
    glPopMatrix();
}

void drawBuildings() {
    srand(666);
    int randHeights[4];
    for (int j = 0; j < 4; ++j) { // Generating random heights
            randHeights[j] = (rand() % 13) + 12;
    }
    
    setShinyMaterial();
    for (int i = 1; i <= 20; i++){ // Draws rows of buildings on each sides of the road
        for (int j = 0; j < 4; ++j) {
            drawBuilding(j , 16, 2.5, (-i * 100) + (j * 25));    // Right side
            drawBuilding(3 - j, -16, 2.5, (-i * 100) + (j * 25)); // Left side
        } 
    }
}
//________________________________________________________________________________


//__________________________________ Trees _______________________________________
void drawTreeTrunk(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.1, 0.1, 1);
    drawTexturedCylinder(textures[1], 1.0f, 1.0f, 5.0f, 32, 1);
    glPopMatrix();

}

void drawTreeFoliage(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(1.2, 1.2, 1.2);
    drawTexturedSphere(textures[4], 1, 32, 10);
    glPopMatrix();
}

void drawTrees() {
    setMatteMaterial();
    for (int i = 0; i < 70; i++) { // Draws rows of trees on the sidewalks
        // Right side
        drawTreeTrunk(9, 0.2, -10.0f - i * 20);
        drawTreeFoliage(9, 0.7, -10.0f - i * 20);
        drawTreeFoliage(8, 0.2, -10.0f - i * 20);
        drawTreeFoliage(9.5, 0.2, -9.5f - i * 20);

        // Left side
        drawTreeTrunk(-9, 0.2, -10.0f - i * 20);
        drawTreeFoliage(-9, 0.7, -10.0f - i * 20);
        drawTreeFoliage(-8, 0.2, -10.0f - i * 20);
        drawTreeFoliage(-9.5, 0.2, -9.5f - i * 20);
    }
}
//________________________________________________________________________________


//_________________________________ Barriers _____________________________________
void drawBarrier(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.1, 0.1, 1);
    drawTexturedCylinder(textures[6], 1.0, 1.0, 1.0, 32, 1);
    glPopMatrix();
}

void drawBarriers(){
    setShinyMaterial();
    for (int i = 0; i < 140; i++) { // Draws road barriers on the sidewalks
        drawBarrier(6.5, -1.5, -0.5f - i * 10.0f);  // Right side
        drawBarrier(-6.5, -1.5, -0.5f - i * 10.0f); // Left side
    }
}
//________________________________________________________________________________


//_________________________________ Obstacles ____________________________________
void drawCone(GLfloat x, GLfloat y, GLfloat z) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[12]);

    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i <= 20; i++) {
        GLfloat angle = 2.0f * 3.14 * (GLfloat)i / (GLfloat)20;
        GLfloat x = 0.5f * sinf(angle);
        GLfloat y = 0.5f * cosf(angle);

        GLfloat normalX = x * 0.5f;
        GLfloat normalY = y * 0.5f;
        GLfloat normalZ = 0.5f;
        glNormal3f(normalX, normalY, normalZ);

        glTexCoord2f((GLfloat)i / (GLfloat)20, 0.0f);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void drawSandHeap(GLfloat x, GLfloat y, GLfloat z, GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scaleX, scaleY, scaleZ);
    drawTexturedSphere(textures[3], 1, 32, 10);
    glPopMatrix();
}

void drawSandPile(GLfloat x, GLfloat y, GLfloat z) {
    setMatteMaterial();

    drawSandHeap(x, y, z + 1, 2, 2, 2);
    drawSandHeap(x + 1, y, z - 1, 2, 2, 2);
    drawSandHeap(x - 1, y, z - 1, 2, 2, 2);
    drawSandHeap(x, y + 2, z + 1, 1.5, 1.5, 1.5);
}

void drawHole(GLfloat x, GLfloat y, GLfloat z) {
    glDisable(GL_LIGHTING);

    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90, 1, 0, 0);
    glutSolidCone(2, 1, 20, 1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawCrate(GLfloat x, GLfloat y, GLfloat z) {
    setMatteMaterial();

    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.8, 0.75, 0.8);
    drawTexturedCube(textures[5], 1.0f);
    glPopMatrix();
}

void drawBoulder1(GLfloat x, GLfloat y, GLfloat z) {
    glDisable(GL_LIGHTING);

    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(x - boulder1X, y, z);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(boulder1RotationAngle, 0, 0, 1);
    glutSolidDodecahedron();
    glColor3f(0, 0, 0);
    glutWireDodecahedron();
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawBoulder2(GLfloat x, GLfloat y, GLfloat z) {
    glDisable(GL_LIGHTING);

    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(x - boulder2X, y, z);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(boulder2RotationAngle, 0, 0, 1);
    glutSolidDodecahedron();
    glColor3f(0, 0, 0);
    glutWireDodecahedron();
    glPopMatrix();

    glEnable(GL_LIGHTING);
}
//________________________________________________________________________________


//___________________________________ Cars _______________________________________
void drawWheel(GLfloat x, GLfloat y, GLfloat z) {
    glDisable(GL_LIGHTING);

    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(wheelRotationAngle, 0.0f, 0.0f, 1.0f); 
    glutSolidTorus(0.15, 0.17, 10, 50);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawCarLight(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.15f, 0.1f, 0.1f);
    drawTexturedCube(textures[13], 1.0f);
    glPopMatrix();
}

void drawCar() {
    setShinyMaterial();

    glPushMatrix();
    glRotatef(carTurnAngle, 0.0f, -1.0f, 0.0f);

    // Body
    glPushMatrix();
    glTranslatef(0.0f, -0.7f, -3.4f);
    glScalef(1.3f, 0.6f, 2.2f);
    drawTexturedCube(textures[15], 0.5f);
    glPopMatrix();

    // Roof
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, -3.6f);
    glScalef(1.29f, 0.55f, 1.2f);
    drawTexturedCube(textures[15], 0.5f);
    glPopMatrix();
    
    // Window
    glPushMatrix();  // Back
    glTranslatef(0.0f, -0.1f, -3.3f);
    glScalef(0.55f, 0.2f, 0.35f);
    drawTexturedCube(textures[14], 1.0f);
    glPopMatrix();

    glPushMatrix();  // Sides
    glTranslatef(0.0f, -0.1f, -3.6f);
    glScalef(0.66f, 0.2f, 0.3f);
    drawTexturedCube(textures[14], 1.0f);
    glPopMatrix();

    // Lights
    drawCarLight(0.49f, -0.65f, -2.39f);  // Right
    drawCarLight(-0.49f, -0.65f, -2.39f); // Left

    // License Plate
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, -2.3f);
    glScalef(0.5f, 0.25f, 0.1f);
    drawTexturedCube(textures[16], 0.5f);
    glPopMatrix();

    // Bumper
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(0.0f, -0.95f, -3.4f);
    glScalef(1.31f, 0.1f, 2.4f);
    glutSolidCube(1);
    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    // Tires
    drawWheel(-0.61f, -0.95f, -2.8f); // Rear left
    drawWheel(-0.61f, -0.95f, -3.9f); // Front left
    drawWheel(0.61f, -0.95f, -2.8f);  // Rear right
    drawWheel(0.61f, -0.95f, -3.9f);  // Front right

    glPopMatrix();
}

void drawCarNPClight(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.2f, 0.15f, 0.2f);
    drawTexturedSphere(textures[13], 1, 32, 10);
    glPopMatrix();
}

void drawCarNPC(GLfloat x, GLfloat y, GLfloat z) {
    setShinyMaterial();

    // Body
    glPushMatrix();
    glTranslatef(x + 0.0f, y - 0.7f, z - 3.4f);
    glScalef(0.8f, 0.4f, 1.2f);
    drawTexturedCylinder(textures[17], 1, 1, 3, 32, 1);
    glPopMatrix();

    // Roof
    glPushMatrix();
    glTranslatef(x + 0.0f, y - 0.2f, z - 1.5);
    glScalef(0.7f, 0.55f, 1.5f);
    drawTexturedSphere(textures[15], 1, 32, 10);
    glPopMatrix();

    // Lights
    drawCarNPClight(x + 0.5f, y - 0.69f, z + 0.2f); // Right
    drawCarNPClight(x - 0.5f, y - 0.69f, z + 0.2f); // Left

    // Tires
    drawWheel(x - 0.72f, y - 0.95f, z - 2.8f); // Rear left
    drawWheel(x - 0.72f, y - 0.95f, z - 0.2f); // Front left
    drawWheel(x + 0.72f, y - 0.95f, z - 2.8f); // Rear right
    drawWheel(x + 0.72f, y - 0.95f, z - 0.2f); // Front right
}
//________________________________________________________________________________


//___________________________________ Humans _____________________________________
void drawHumanTorso(Color color) {
    glColor3f(color.r, color.g, color.b);
    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(0.5f, 0.9f, 0.3f);
    glutSolidCube(1);

    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
}

void drawHumanHead() {
    glColor3f(0.9f, 0.7f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, 0.95f, 0.0f);
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();
}

void drawHumanArm(int side, GLfloat xOffset, bool isCheering, bool isWalking, GLfloat armAngle, Color color) {
    glColor3f(color.r, color.g, color.b);
    glPushMatrix();
    glTranslatef(xOffset, 0.5f, 0.0f);
    if (isCheering) {
        glRotatef((xOffset < 0 ? 180 + armAngle : -180 - armAngle), 0.0f, 0.0f, 1.0f);
    }
    if (isWalking) {
        glRotatef(((side == 0) ? sin(armAngle) * 20 : -sin(armAngle) * 20), 1.0f, 0.0f, 0.0f);
    }
    glTranslatef(0.0f, -0.4f, 0.0f);
    glScalef(0.1f, 0.8f, 0.1f);
    glutSolidCube(1);

    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
}

void drawHumanLeg(int side, GLfloat xOffset, bool isWalking, GLfloat legAngle) {
    glColor3f(0.2f, 0.2f, 0.8f);
    glPushMatrix();
    glTranslatef(xOffset, -0.1f, 0.0f);
    if (isWalking) {
        glRotatef(((side == 0) ? -sin(legAngle) * 20 : sin(legAngle) * 20), 1.0f, 0.0f, 0.0f);
    }
    glTranslatef(0.0f, -0.4f, 0.0f);
    glScalef(0.1f, 0.8f, 0.1f);
    glutSolidCube(1);

    glColor3f(0, 0, 0);
    glutWireCube(1);
    glPopMatrix();
}

void drawStandingHuman(GLfloat x, GLfloat y, GLfloat z, bool isCheering, Color color) {
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(x, y, z);

    drawHumanTorso(color);
    drawHumanHead();
    drawHumanArm(0, -0.35f, isCheering, false, armAngle, color); // Right
    drawHumanArm(1, 0.35f, isCheering, false, armAngle, color);  // Left
    drawHumanLeg(0, -0.15f, false, 0.0f); // Right
    drawHumanLeg(1, 0.15f, false, 0.0f);   // Left

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawWalkingHuman(GLfloat x, GLfloat y, GLfloat z, Color color) {
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(x, y, z);

    drawHumanTorso(color);
    drawHumanHead();
    drawHumanArm(1, -0.3f, false, true, legAngle, color); // Right
    drawHumanArm(0, 0.3f, false, true, legAngle, color);   // Left
    drawHumanLeg(1, -0.15f, true, legAngle); // Right
    drawHumanLeg(0, 0.15f, true, legAngle);   // Left

    glPopMatrix();

    glEnable(GL_LIGHTING);
}
//________________________________________________________________________________


//___________________________________ Others _____________________________________
void drawItem(GLfloat x, GLfloat y, GLfloat z) {
    glDisable(GL_LIGHTING);

    glColor3f(1, 1, 0);
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(0.3, 0.3, 0.3);
    glRotatef(itemRotationAngle, 0, 1, 0);
    glutSolidIcosahedron();
    glColor3f(0, 0, 0);
    glutWireIcosahedron();
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawTunnel(GLfloat x, GLfloat y, GLfloat z) {
    glDisable(GL_LIGHTING);

    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glColor3f(0, 0, 0); // Large cylinder
    glPushMatrix();
    glTranslatef(x, y - 3.0, z - 800.0f);
    glutSolidCylinder(6, 5, 32, 1);
    glPopMatrix();

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glColor3f(1, 1, 1); // Small cylinder
    glPushMatrix();
    glTranslatef(x, y - 3.0, z - 800.0f);
    glutSolidCylinder(5.5, 4.8, 32, 1);
    glPopMatrix();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glColor3f(0, 0, 0); // Large cylinder
    glPushMatrix();
    glTranslatef(x, y - 3.0, z - 800.0f);
    glutSolidCylinder(6, 5, 32, 1);
    glPopMatrix();

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_LIGHTING);
}

void drawFinishLine(GLfloat x, GLfloat y, GLfloat z) {
    setShinyMaterial();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[18]);
    
    for (int i = 0; i < 7; i++) {
        glPushMatrix();
        glTranslatef(x - 4.7f + 1.5 * i, y, z);
        glScalef(1.0f, 0.1f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 0.0f, 1.0f);
        glEnd();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);
}
//================================================================================


//============================= Collision detection ==============================
bool checkCollision(BoundingBox box1, BoundingBox box2) {
    return (box1.xMin <= box2.xMax && box1.xMax >= box2.xMin) &&
        (box1.yMin <= box2.yMax && box1.yMax >= box2.yMin) &&
        (box1.zMin <= box2.zMax && box1.zMax >= box2.zMin);
}

void checkCollisions() {
    BoundingBox carBox = createCarBoundingBox(carX + 0.65, carY + -1.2f, carZ - 0.4f);

    // Bounding boxes for the obstacles
    BoundingBox coneBox = createConesBoundingBox(0, -2, -100);
    BoundingBox crateBox1 = createCrateBoundingBox(-4, -1.7, -250);
    BoundingBox crateBox2 = createCrateBoundingBox(5, -1.7, -250);
    BoundingBox crateBox3 = createCrateBoundingBox(0, -1.7, -250);
    BoundingBox sandpileBox = createSandpileBoundingBox(-3, -1.5, -400);
    BoundingBox holeBox = createHoleBoundingBox(0, -2.39, -500);
    BoundingBox boulderBox1 = createBoulder1BoundingBox(0, -1.5, -900);
    BoundingBox boulderBox2 = createBoulder2BoundingBox(0, -1.5, -750);
    BoundingBox npcBox1 = createCarBoundingBox(-2, -1.15f, -520 + carNPCcoorZ);
    BoundingBox npcBox2 = createCarNPCBoundingBox(3, -1.15f, -580 + carNPCcoorZ);

    // Bounding boxes for the items
    BoundingBox item1BoundingBox = createItemBoundingBox(0, -1.5, -50);
    BoundingBox item2BoundingBox = createItemBoundingBox(-2, -1.5, -150);
    BoundingBox item3BoundingBox = createItemBoundingBox(2, -1.5, -250);
    BoundingBox item4BoundingBox = createItemBoundingBox(0, -1.5, -350);
    BoundingBox item5BoundingBox = createItemBoundingBox(-2, -1.5, -450);
    BoundingBox item6BoundingBox = createItemBoundingBox(2, -1.5, -550);
    BoundingBox item7BoundingBox = createItemBoundingBox(0, -1.5, -650);
    BoundingBox item8BoundingBox = createItemBoundingBox(-1, -1.5, -750);
    BoundingBox item9BoundingBox = createItemBoundingBox(1, -1.5, -850);
    BoundingBox item10BoundingBox = createItemBoundingBox(0, -1.5, -950);

    // Condition to make sure that the penalty applies only once per collision
    if (!collisionOccured) { // Checking collision with obstacles/items
        if (checkCollision(carBox, coneBox) || 
            checkCollision(carBox, crateBox1) || checkCollision(carBox, crateBox2) || checkCollision(carBox, crateBox3) ||
            checkCollision(carBox, sandpileBox) ||
            checkCollision(carBox, holeBox) ||
            checkCollision(carBox, boulderBox1) || checkCollision(carBox, boulderBox2) ||
            checkCollision(carBox, npcBox1) || checkCollision(carBox, npcBox2)) {
            
            lives--;
            score -= 1000;
            collisionOccured = true;

            if (lives <= 0) {
                isGameOver = true;
            }
        }
        else if (checkCollision(carBox, item1BoundingBox) || checkCollision(carBox, item2BoundingBox) || checkCollision(carBox, item3BoundingBox) ||
                checkCollision(carBox, item4BoundingBox) || checkCollision(carBox, item5BoundingBox) || checkCollision(carBox, item6BoundingBox) ||
                checkCollision(carBox, item7BoundingBox) || checkCollision(carBox, item8BoundingBox) || checkCollision(carBox, item9BoundingBox) ||
                checkCollision(carBox, item10BoundingBox)) {
             
             lives++;
             collisionOccured = true;
        }
    }
    else { 
        if (!(checkCollision(carBox, coneBox) || 
              checkCollision(carBox, crateBox1) || checkCollision(carBox, crateBox2) || checkCollision(carBox, crateBox3) ||
              checkCollision(carBox, sandpileBox) ||
              checkCollision(carBox, holeBox) ||
              checkCollision(carBox, boulderBox1) || checkCollision(carBox, boulderBox2)||
              checkCollision(carBox, npcBox1) || checkCollision(carBox, npcBox2) ||
              checkCollision(carBox, item1BoundingBox) || checkCollision(carBox, item2BoundingBox) || checkCollision(carBox, item3BoundingBox) ||
              checkCollision(carBox, item4BoundingBox) || checkCollision(carBox, item5BoundingBox) || checkCollision(carBox, item6BoundingBox) ||
              checkCollision(carBox, item7BoundingBox) || checkCollision(carBox, item8BoundingBox) || checkCollision(carBox, item9BoundingBox) ||
              checkCollision(carBox, item10BoundingBox))) {

              collisionOccured = false;
        }
    }
}
//================================================================================


// Draws all the parts of the scene to be displayed on the screen (called on each event requiring the window to be repainted)
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camTurn, 0.1, carZ + 5.0, 0.0, 0.0, carZ, 0.0, 1.0, 0.0);

    //--------------------- Game state (menus) -----------------------
    glDisable(GL_LIGHTING);
    if (inMenu) {
        renderMainMenu();
    }

    renderScore(score);
    renderLives(lives);

    if (isPaused) {
        renderPauseMenu();
    }

    if (isGameOver) {
        renderGameOver();
    }
    //-----------------------------------------------------------------

    glEnable(GL_LIGHTING);
    setupLighting();

    //---------------------- Environment ------------------------------
    drawGround();
    drawRoad();
    drawSidewalk();
    drawTrees();
    drawBuildings();
    drawFinishLine(0.0f, -2.3f, -990.0f);
    
    for (int i = 0; i < 20; i++) {
        drawTunnel(0, 0.0, -5 - i);
        drawTunnel(0, 0.0, -5 - i - 1000);
    }

    srand(42);
    for (int i = 0; i < 30; i++) {
        GLfloat zPosition = -100 - i * 20;
        int colorIndex = rand() % NUM_COLORS; // Generating random shirt colors
        Color color = colors[colorIndex];
        bool isCheering = (i % 2 == 0);

        if (i % 2 == 0) {
            drawStandingHuman(7.0f, -1.4, zPosition, isCheering, color);
            drawStandingHuman(-7.0, -1.4f, zPosition, !isCheering, color);

            drawStandingHuman(7.0f, -1.4, zPosition - 1000, isCheering, color);
            drawStandingHuman(-7.0, -1.4f, zPosition - 1000, !isCheering, color);
        }
        else if (i % 3 == 0) {
            drawStandingHuman(7.0f, -1.4, zPosition, isCheering, color);
            drawStandingHuman(-7.0, -1.4f, zPosition, !isCheering, color);

            drawStandingHuman(7.0f, -1.4, zPosition - 1000, isCheering, color);
            drawStandingHuman(-7.0, -1.4f, zPosition - 1000, !isCheering, color);
        }
        else if (i % 5 == 0) {
            drawWalkingHuman(7.0f, -1.4f, zPosition + humanPosition, color);
            drawWalkingHuman(-7.0f, -1.4f, zPosition - humanPosition, color);

            drawWalkingHuman(7.0f, -1.4f, zPosition + humanPosition - 1000, color);
            drawWalkingHuman(-7.0f, -1.4f, zPosition - humanPosition - 1000, color);
        }
    }
    
    drawBarriers();
    
    //-----------------------------------------------------------------
     
    //------------------------ Obstacles ------------------------------
    for (int i = 0; i < 5; i++) {
        drawCone(i, -2, -100);
        drawCone(i, -2, -1100); // Extras for perfect loop
    }

    drawCrate(0, -1.7, -250);
    drawCrate(-4.5, -1.7, -250);
    drawCrate(4.5, -1.7, -250);

    drawSandPile(-3, -1.5, -400);

    drawHole(0, -2.39, -500);

    glPushMatrix();
    glTranslatef(0, 0, carNPCcoorZ);
    drawCarNPC(-2, -1.15, -520);
    drawCarNPC(3, -1.15, -580);
    glPopMatrix();

    drawBoulder1(0, -1.5, -900);
    drawBoulder2(0, -1.5, -750);
    //-----------------------------------------------------------------

    //-------------------- Extra lives ------------------------------
    drawItem(0, -1.5, -50);
    drawItem(-2, -1.5, -150);
    drawItem(2, -1.5, -250);
    drawItem(0, -1.5, -350);
    drawItem(-2, -1.5, -450);
    drawItem(2, -1.5, -550);
    drawItem(0, -1.5, -650);
    drawItem(-1, -1.5, -750);
    drawItem(1, -1.5, -850);
    drawItem(0, -1.5, -950);
    //-----------------------------------------------------------------

    //- Extras to make the loop seamless (items seen in the distance) -
    drawItem(0, -1.5, -1050);
    drawCrate(0, -1.7, -1250);
    drawCrate(-5, -1.7, -1250);
    drawCrate(5, -1.7, -1250);
    drawSandPile(-3, -1.5, -1400);
    //-----------------------------------------------------------------
    
    //--------------------- Player's car ------------------------------
    glPushMatrix();
    glTranslatef(carX, carY + -1.15f, carZ + 1.0f);
    drawCar();
    glPopMatrix();
    //-----------------------------------------------------------------

    glFlush();
    glutSwapBuffers();
}

// Supporting function to make certain objects on screen move/change (update coordinates with the passing of time_interval)
void update(int value) {
    if (!isPaused && !inMenu && !isGameOver) {

        //--------------- Cars animation ---------------------
        // Car position (forward movement)
        carZ -= carSpeed;
        carNPCcoorZ -= carNPCspeed;
        score += carSpeed * 10;

        // Reset car's and humans' position to create a perfect loop (arcade)
        if (carZ < -1000) {
            carZ = 0;
            carNPCcoorZ = 0;
            carSpeed += 0.1;
            humanPosition = 0.0f;
            lives++;
        }

        // Wheel rotation animation
        wheelRotationAngle += -carSpeed * 10;
        if (wheelRotationAngle > 360.0f) {
            wheelRotationAngle -= 360.0f;
        }

        // Car turning (sideways movement)
        if (turningLeft) {
            carTurnAngle = -MAX_TURN_ANGLE; 
            if (carX > -MAX_CAR_X) {
                carX -= 0.1f;
            } 
        }
        else if (turningRight) {
            carTurnAngle = MAX_TURN_ANGLE;
            if (carX < MAX_CAR_X) {
                carX += 0.1f; 
            } 
        }
        else {
            if (carTurnAngle > 0) {
                carTurnAngle -= 2.5f;
                if (carTurnAngle < 0) {
                    carTurnAngle = 0;
                }
            }
            else if (carTurnAngle < 0) {
                carTurnAngle += 2.5f;
                if (carTurnAngle > 0) {
                    carTurnAngle = 0;
                }
            }
        }
        //----------------------------------------------------
         
        //------------- Spectator animation ------------------
        if (armUp) {  // Cheering
            armAngle += 2.0f;
            if (armAngle > 20.0f) {
                armUp = false;
            }
        }
        else {
            armAngle -= 2.0f;
            if (armAngle < -20.0f) {
                armUp = true;
            }
        }

        legAngle += 0.1f;  // Leg movement         
        if (legAngle > 2 * 3.14f) {
            legAngle = 0.0f;
        }

        humanPosition -= 0.02f; // Walking
        //----------------------------------------------------

        //--------------- Boulders animation -----------------
        // Boulder 1 left-right movement
        if (boulder1X > -4 && boulder1MoveDirection == 'r') {
            boulder1X -= 0.1;
            boulder1RotationAngle -= 5;
        }
        else
            boulder1MoveDirection = 'l';

        if (boulder1X < 4 && boulder1MoveDirection == 'l') {
            boulder1X += 0.1;
            boulder1RotationAngle += 5;
        }
        else
            boulder1MoveDirection = 'r';

        // Boulder 2 right-left movement
        if (boulder2X > -4 && boulder2MoveDirection == 'r') {
            boulder2X -= 0.1;
            boulder2RotationAngle -= 5;
        }
        else
            boulder2MoveDirection = 'l';

        if (boulder2X < 4 && boulder2MoveDirection == 'l') {
            boulder2X += 0.1;
            boulder2RotationAngle += 5;
        }
        else
            boulder2MoveDirection = 'r';
        //----------------------------------------------------

        // Item animation
        if (itemRotationAngle < 355)
            itemRotationAngle += 5;
        else itemRotationAngle = 0;

        checkCollisions();

        //--------- Day/Night light and background changing -------
        elapsedTime = glutGet(GLUT_ELAPSED_TIME) / 10000.0f;

        light0_position[0] = cos(elapsedTime) * 10;

        light1_position[1] = 80 + sin(elapsedTime) * 10;
        light2_position[0] = 1 + sin(elapsedTime);

        light_diffuse[0] = 0.5f + 0.5f * sin(elapsedTime);
        light_diffuse[1] = 0.5f + 0.5f * sin(elapsedTime);
        light_diffuse[2] = 0.8f + 0.2f * sin(elapsedTime);

        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);

        GLfloat bgR = 0.1f + 0.5f * sin(elapsedTime);
        GLfloat bgG = 0.1f + 0.5f * sin(elapsedTime);
        GLfloat bgB = 0.2f + 0.3f * sin(elapsedTime);
        
        glClearColor(bgR, bgG, bgB, 1);
        //--------------------------------------------------------
    }
    
    glutPostRedisplay();
    glutTimerFunc(32, update, 1);
}

// Idle callback function (force OpenGL to redraw the current window)
void when_in_mainloop() {
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(235, 85);
    glutInitWindowSize(800, 500);
    glutCreateWindow("3D Modelling");
    glEnable(GL_DEPTH_TEST);

    initTextures();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(when_in_mainloop);
    glutKeyboardFunc(keyboardInput);
    glutKeyboardUpFunc(keyboardInputRelease);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseInput);
    glutTimerFunc(0, update, 1);
    glutMainLoop();
    return 0;
}