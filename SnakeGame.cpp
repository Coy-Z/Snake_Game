#include "SnakeGame.h"

// Declaring Classes
enum class Direction
{
    up, down, left, right
};

enum SnakeOrApple
{
    s = 0, // Snake
    a = 1, // Apple
    h = 2  // Head of snake
};

class SnakePos
{
private:
    std::deque<std::vector<int>> positions;
public:
    void reset()
    {
        positions.clear();
        std::vector<int> intStart1{ 0,0 };
        std::vector<int> intStart2{ -1,0 };
        positions.push_back(intStart1);
        positions.push_back(intStart2);
    }
    SnakePos()
    {
        reset();
    }
    void eat(Direction dir)
    {
        std::vector<int> newPos = positions[0];
        if (dir == Direction::up) // Up
        {
            newPos[1] += 1;
            if (newPos[1] > NUMY / 2 - 1)
            {
                newPos[1] *= -1;
            }
        }
        else if (dir == Direction::down) // Down
        {
            if (newPos[1] <= -NUMY / 2)
            {
                newPos[1] *= -1;
            }
            newPos[1] -= 1;
        }
        else if (dir == Direction::left) // Left
        {
            if (newPos[0] <= -NUMX / 2)
            {
                newPos[0] *= -1;
            }
            newPos[0] -= 1;
        }
        else // Right
        {
            newPos[0] += 1;
            if (newPos[0] > NUMX / 2 - 1)
            {
                newPos[0] *= -1;
            }
        }

        positions.push_front(newPos);
    }

    void retract()
    {
        positions.pop_back();
    }

    bool overlapCheck()
    {
        for (auto it = ++positions.begin(); it != positions.end(); ++it)
        {
            if (positions[0] == *it) return true;
        }
        return false;
    }

    const std::deque<std::vector<int>> getPositions()
    {
        return positions;
    }

    const std::vector<int> getPosition(unsigned int index)
    {
        return positions[index];
    }

    ~SnakePos() {}
};

class ApplePos
{
private:
    std::vector<int> position; // Actual position scaled up to be from [-5, 4] in integers.
public:
    void newPosition()
    {
        std::srand(std::time(NULL));
        int randx = (rand() % NUMX) - NUMX / 2;
        std::srand(std::time(NULL) + 1);
        int randy = (rand() % NUMY) - NUMY / 2;
        position = { randx,randy };
    }

    ApplePos()
    {
        this->newPosition();
    }

    const std::vector<int> getPosition()
    {
        return position;
    }

    ~ApplePos() {}
};

// Prototype of Global Snake and Apple Objects
SnakePos Snake;
ApplePos Apple;

// Global Variable Class
class GlobalVariables
{
private:
    bool help;
    bool paused;
    bool game_over;
    int game_speed;
    unsigned int score;
    unsigned int high_score;
    Direction in_direction;
    int test_counter;
public:
    GlobalVariables()
    {
        help = false;
        paused = false;
        game_over = false;
        game_speed = 1;
        score = 0;
        high_score = 0;
        in_direction = Direction::right;
        test_counter = 0;
    }

    // help access
    void helpSet(const bool var) { help = var; }
    const bool helpGet() const { return help; }

    // paused access
    void pausedSet(const bool var) { paused = var; }
    const bool pausedGet() const { return paused; }

    // game_over access
    void gameOverSet(const bool var) { game_over = var; }
    const bool gameOverGet() const { return game_over; }

    // game_speed access
    void gameSpeedSet(const int var) { game_speed = var; }
    const int gameSpeedGet() const { return game_speed; }

    // score access
    void scoreSet(const unsigned int var) { score = var; }
    void incrementScore() { ++score; }
    const unsigned int scoreGet() const { return score; }
    

    // high_score access
    void highScoreSet(const unsigned int var) { high_score = var; }
    const unsigned int highScoreGet() const { return high_score; }
    void updateHighScore() { high_score = score; }

    // inDirection access
    void inDirectionSet(const Direction var) { in_direction = var; }
    const Direction inDirectionGet() const { return in_direction; }

    // test_counter access
    void testCounterSet(const int var) { test_counter = var; }
    const int testCounterGet() const { return test_counter; }
};

// Initialise Global Variables
GlobalVariables globalVariables;

// ** Drawing Objects **
// Draws a rectangle of width dx and height dy.
void drawRect(std::vector<int> coords, SnakeOrApple val)
{
    double x = coords[0] / 5.;
    double y = coords[1] / 5.;
    if (val == 0)
    {
        glColor3d(0., 1., 0.);
    }
    else if (val == 1)
    {
        glColor3d(1., 0., 0.);
    }
    else
    {
        glColor3d(0., 0.8, 0.);
    }
    glBegin(GL_POLYGON);
    glVertex2d(x, y);
    glVertex2d(x + DX, y);
    glVertex2d(x + DX, y + DY);
    glVertex2d(x, y + DY);
    glEnd();
}
// Uses drawRect to draw the snake.
void drawSnake(SnakePos snake)
{
    std::deque<std::vector<int>> positions = snake.getPositions();
    for (auto it = positions.begin() + 1; it != positions.end(); ++it)
    {
        drawRect(*it, SnakeOrApple::s);
    }
    drawRect(positions[0], SnakeOrApple::h);
}
// Uses drawRect to draw the apple.
void drawApple(ApplePos apple)
{
    drawRect(apple.getPosition(), SnakeOrApple::a);
}

// ** Printing and Help **
// Prints string at location (x,y) in a bitmap font
void glutPrint(double x, double y, std::string str, void* FONT = GLUT_BITMAP_HELVETICA_12)
{
    glColor3f(1., 1., 1.);
    glRasterPos2d(x, y);
    for (int i = 0; i < str.length(); i++)
    {
        glutBitmapCharacter(FONT, str[i]);
    }
}

// Displaying the help prompt.
void display_help()
{
    if (globalVariables.helpGet())
    {
        std::string helpString1("Press arrow keys to change direction.");
        std::string helpString2("Hit the space bar to pause and play.");
        std::string helpString3("Click 'r' to reset the game.");
        std::string helpString4("Smash esc to quit.");
        std::string objString1("Try to eat as many apples as you can");
        std::string objString2("and DON'T BITE YOURSELF!");
        std::vector<std::string> stringVec{ helpString1, helpString2, helpString3, helpString4, objString1, objString2 };
        for (int i = 0; i < stringVec.size(); ++i)
        {
            glutPrint(-0.4, -0.3 - i * 0.1, stringVec[i]);
        }
    }
    else
    {
        glutPrint(-0.2, -0.9, "Press 'h' for help.");
    }
}

// ** Keyboard Input **
// Special keys
void specialKeyControl(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: // Left Arrow
        // std::cout << "Left Pressed" << std::endl;
        globalVariables.inDirectionSet(Direction::left);
        break;
    case GLUT_KEY_UP: // Up Arrow
        // std::cout << "Up Pressed" << std::endl;
        globalVariables.inDirectionSet(Direction::up);
        break;
    case GLUT_KEY_RIGHT: // Right Arrow
        // std::cout << "Right Pressed" << std::endl;
        globalVariables.inDirectionSet(Direction::right);
        break;
    case GLUT_KEY_DOWN: // Down Arrow
        // std::cout << "Down Pressed" << std::endl;
        globalVariables.inDirectionSet(Direction::down);
        break;
    }
}

// Character keys
void keyControl(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'h': case 'H':
        // std::cout << "H Pressed" << std::endl;
        if (globalVariables.helpGet())
        {
            globalVariables.helpSet(false);
        }
        else
        {
            globalVariables.helpSet(true);
        }
        break;
    case 'r': case 'R':
        // std::cout << "R Pressed" << std::endl;
        globalVariables.gameOverSet(false);
        globalVariables.scoreSet(0);
        Snake.reset();
        globalVariables.inDirectionSet(Direction::right);
        Apple.newPosition();
        break;
    case 27:
        // std::cout << "Esc Pressed" << std::endl;
        exit(0);
        break;
    case 32:
        // std::cout << "Space Pressed" << std::endl;
        if (globalVariables.pausedGet())
        {
            globalVariables.pausedSet(false);
        }
        else
        {
            globalVariables.pausedSet(true);
        }
        break;
    }
}

// ** Updating Frames
// The GLUT idle loop function.
void updateGame()
{
    unsigned long delay;

    // Speed-controlled delay
    if (globalVariables.gameSpeedGet() > 0)
    {
        delay = MAX_DELAY; // This will be used for game speed variation at a later date.
#ifdef _WIN32
        Sleep(delay / 1000); // milliseconds
#else
        usleep((useconds_t)delay); // microseconds
#endif
    }
    else
    {
        return;
    }
    
    // Update SnakePos object
    if (!globalVariables.gameOverGet() && !globalVariables.pausedGet())
    {
        Snake.eat(globalVariables.inDirectionGet());
        if (Snake.overlapCheck())
        {
            globalVariables.gameOverSet(true);
        }
        else if (Snake.getPosition(0) == Apple.getPosition())
        {
            globalVariables.incrementScore();
            Apple.newPosition();
        }
        else
        {
            Snake.retract();
        }
    }
    glutPostRedisplay();
}   

// ** Window Initialisation **
// Initialising Function for GLUT
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

// Display Function
void display()
{
    // Clear all pixels
    glClear(GL_COLOR_BUFFER_BIT);

    if (globalVariables.gameOverGet())
    {
        if (globalVariables.scoreGet() > globalVariables.highScoreGet()) globalVariables.updateHighScore();
        // Print Game Over Text
        glutPrint(-0.2, 0.5, "Game Over!", GLUT_BITMAP_HELVETICA_18);

        // Print Final Score
        glutPrint(-0.1, 0.2, "Score: " + std::to_string(globalVariables.scoreGet()));

        // Restart Game
        glutPrint(-0.2, -0.1, "Press 'r' to restart.");
    }

    else
    {
        // Draw objects
        drawSnake(Snake);
        drawApple(Apple);

        // Print score
        glutPrint(-0.1, 0.9, "Score: " + std::to_string(globalVariables.scoreGet()));
    }

    // Print help prompt
    display_help();

    // Print high score
    glutPrint(-0.9, 0.9, "High Score: " + std::to_string(globalVariables.highScoreGet()));
    // Testing
    // glutPrint(-0.9, 0.9, std::to_string(test_counter));

    // Flushing from back buffer
    glutSwapBuffers();
}

// Ensuring the window cannot be resized
void windowReshaping(int width, int height)
{
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

int main(int argc, char* argv[])
{
    // Initialise GLUT with command-line parameters.
	glutInit(&argc, argv);
    // Set display mode.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    // Set the window size and position.
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
    // Initialise GLUT
    init();
    // Create the window.
	window = glutCreateWindow("Snake Game");
    // Set buffer.
    glDrawBuffer(GL_BACK);
    // Implement display.
    glutDisplayFunc(display);
    // Maintaining constant window size.
    glutReshapeFunc(windowReshaping);
    // Idle loop.
    glutIdleFunc(updateGame);
    // Implement keyboard control.
    glutKeyboardFunc(keyControl);
    glutSpecialFunc(specialKeyControl);

    // Begin GLUT main loop.
    glutMainLoop();

    return 0;
}