//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// score
#define SCORE 0

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
    
    // instantiate bricks
    initBricks(window);
    
    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // make a random number for velocity
    double velocityX = drand48();
    double velocityY = -3;
    
    // put the scoreboard at the screen
    updateScoreboard(window, label, points);
    
    // wait for click before begin
    waitForClick();
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // move ball along x-axis
        move(ball, velocityX * 2, velocityY);

        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = -velocityX;
        }

        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            velocityX = -velocityX;
        }
        
        // check if the ball hit the ground
        else if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives -= 1;
            // set the ball in the center of the window
            setLocation(ball, WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS);
            // if lives are greater or equals than 1 wait a click for resume the game 
            if(lives >= 1)
            {
                waitForClick();
            }
        }
        
        // check if the ball hit the top
        else if (getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        // linger before moving again
        pause(10);
        
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, 500);
            }
        }
        
        // check if the ball collides with some objet and return the object hited
        GObject object = detectCollision(window, ball);

        // if we hit one object
        if (object != NULL)
        {
            // if the object was a GRect
            if (strcmp(getType(object), "GRect") == 0)
            {
                // if was the paddle
                if (object == paddle)
                {
                    velocityY = -velocityY;
                }
                // if was a brick
                else
                {
                    // add 1 to the points
                    points += 1;
                    // update the score
                    updateScoreboard(window, label, points);
                    bricks -= 1;
                    velocityY = -velocityY;
                    removeGWindow(window, object);
                }
            }
        }
    }
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{   
    //set the initial position for the first brick
    int brickY = 40;
    //set the initial color for the first row of the wall
    string color = "gray";
    for(int i = 0; i < 5; i++)
    {
        //when the row change the color also change
        switch (i)
        {
            case 1:
                color = "green";
                break;
            case 2:
                color = "red";
                break;
            case 3:
                color = "yellow";
                break;
            case 4:
                color = "orange";
                break;
        }
        //set the initial position in x for the first brick in each row
        int brickX = 25;
        for(int j = 0; j < 10; j++)
        {
            GRect brick = newGRect(brickX, brickY, 30, 10);
            setFilled(brick, true);
            setColor(brick, color);
            add(window, brick);
            brickX += 35;   //change the position x of the brick
        }
        brickY += 17;   //change the position y of the brick
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH/2 - RADIUS, HEIGHT/2 - RADIUS, RADIUS*2, RADIUS*2);
    setFilled(ball, true);
    setColor(ball, "red");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(162, 500, 72, 10);
    setFilled(paddle, true);
    setColor(paddle, "black");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-30");
    //double x = (getWidth(window) - getWidth(label))/2;
    //setLocation(label, x, 200);
    setColor(label, "cyan");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
