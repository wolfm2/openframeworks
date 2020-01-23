### Points: 5
### Assignment

Create a new project.

Create a global ball balls[10];

Create a ball class that has:
* Property: position
* Property: velocity/direction vector
* Property: color
* Property: index
* Method: Constructor
* Method: draw
* Method: update

In the constructor initialize the properties:
* position: the middle of the screen
* velocity/direction vector:  Random between 4-10
* color: start with solid red

In ball::draw:

Draw the circle at the current position with the current color

In ball::update:

If the ball has a collision with another ball or the side of a window, reverse the direction and reduce the velocity by 10%.
If the ball has a velocity of < 1 reinitialize the object in the ball array. (see below)

In update:

Call the update method for each ball object.

In draw:

Call the draw method for each ball object.


Create two unique rules in ball::update. One should affect the ball color, one should affect the ball vector.

Examples: 
* Color gets darker and more transparent as velocity deminishes
* In ball update, check the distance of all other balls.  If the distance to any other ball is below a threshold, modify your vector to gravitate towards or be repelled by that ball. ofVec2f.angle() will help in determining how two circles are oriented.
