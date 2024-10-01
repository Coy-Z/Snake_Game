#include "SnakeGame.h"

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
    glVertex2d(x + dx, y);
    glVertex2d(x + dx, y + dy);
    glVertex2d(x, y + dy);
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
    if (help)
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
        inDirection = Direction::left;
        break;
    case GLUT_KEY_UP: // Up Arrow
        // std::cout << "Up Pressed" << std::endl;
        inDirection = Direction::up;
        break;
    case GLUT_KEY_RIGHT: // Right Arrow
        // std::cout << "Right Pressed" << std::endl;
        inDirection = Direction::right;
        break;
    case GLUT_KEY_DOWN: // Down Arrow
        // std::cout << "Down Pressed" << std::endl;
        inDirection = Direction::down;
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
        if (help)
        {
            help = false;
        }
        else
        {
            help = true;
        }
        break;
    case 'r': case 'R':
        // std::cout << "R Pressed" << std::endl;
        game_over = false;
        score = 0;
        Snake.reset();
        inDirection = Direction::right;
        Apple.newPosition();
        break;
    case 27:
        // std::cout << "Esc Pressed" << std::endl;
        exit(0);
        break;
    case 32:
        // std::cout << "Space Pressed" << std::endl;
        if (paused)
        {
            //game_speed = game_speed_buffer;
            paused = false;
        }
        else
        {
            //game_speed_buffer = game_speed;
            //game_speed = 0;
            paused = true;
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
    if (game_speed > 0)
    {
        delay = max_delay; // This will be used for game speed variation at a later date.
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
    if (!game_over && !paused)
    {
        Snake.eat(inDirection);
        if (Snake.overlapCheck())
        {
            game_over = true;
        }
        else if (Snake.getPosition(0) == Apple.getPosition())
        {
            ++score;
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

    if (game_over)
    {
        if (score > high_score) high_score = score;
        // Print Game Over Text
        glutPrint(-0.2, 0.5, "Game Over!", GLUT_BITMAP_HELVETICA_18);

        // Print Final Score
        glutPrint(-0.1, 0.2, "Score: " + std::to_string(score));

        // Restart Game
        glutPrint(-0.2, -0.1, "Press 'r' to restart.");
    }

    else
    {
        // Draw objects
        drawSnake(Snake);
        drawApple(Apple);

        // Print score
        glutPrint(-0.1, 0.9, "Score: " + std::to_string(score));
    }

    // Print help prompt
    display_help();

    // Print high score
    glutPrint(-0.9, 0.9, "High Score: " + std::to_string(high_score));
    // Testing
    // glutPrint(-0.9, 0.9, std::to_string(test_counter));

    // Flushing from back buffer
    glutSwapBuffers();
}

// Ensuring the window cannot be resized
void windowReshaping(int width, int height)
{
    glutReshapeWindow(window_width, window_height);
}

int main(int argc, char* argv[])
{
    // Initialise GLUT with command-line parameters.
	glutInit(&argc, argv);
    // Set display mode.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    // Set the window size and position.
	glutInitWindowSize(window_width, window_height);
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