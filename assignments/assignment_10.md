### Points: 5
### Assignment

Create a new project.

Translate to the center of the screen

Create a method that takes the arguments: ofPoint origin, int numTriangles;

The function should:
* Translate to "origin"
* Rotate around origin in steps of 360/numTriangles 
* Draw a triangle (pointing outwards) at each step
* For each triangle drawn, if numTriangles > 1 call yourself with the arguments (tip of drawn triangle, numTriangles - 1);

Call the function in draw with the arguments (center of screen, 4)
Create a slider to modify the number called within the domain of 1 - 20

Use numTriangles to refine triangle width/height, color, and transparency


## Information
See getHeight() getWidth() in the oFx documentation.  Use them to get the window center value.
