#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 circle: add a circle to the edge matrix - 
	    takes 3 arguments (cx, cy, r)
	 hermite: add a hermite curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 bezier: add a bezier curve to the edge matrix -
	    takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)

	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 xrotate: create an x-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 yrotate: create an y-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 zrotate: create an z-axis rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 1 argument (theta)
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];

  color c; 
  c.red = 0;
  c.green = 0;
  c.blue = MAX_COLOR;
  
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
//Reading Variables
  double x0, y0, z0, x1, y1, z1, x2, y2, x3, y3;
    x0 = 0;
    y0 = 0;
        z0 = 0;
        x1 = 0;
        y1 = 0;
        z1 = 0;
        x2 = 0;
        y2 = 0;
        x3 = 0;
        y3 = 0;
  double cx, cy, r;
        cx = 0;
        cy = 0;
        r = 0;
  double x, y, z;
        x = 0;
        y = 0;
        z = 0;
  double theta;
        theta = 0;
  double step = 500;

  while ( fgets(line, 255, f) != NULL ) {

    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);  

    if (strcmp(line, "line") == 0 ) {
    	sscanf(fgets(line, 255, f), "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);
    	//printf("Read in from line:\n");
    	//printf("%lf %lf %lf %lf %lf %lf\n", x0, y0, z0, x1, y1, z1);
		add_edge(pm, x0, y0, z0, x1, y1, z1);
		//printf("a line has been added\n");
    }

    else if (strcmp(line, "circle") == 0) {
		sscanf(fgets(line, 255, f), "%lf %lf %lf", &cx, &cy, &r);	
		add_circle(pm, cx, cy, r, step);
    }

    else if (strcmp(line, "hermite") == 0) { //not done
    	//takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
		sscanf(fgets(line, 255, f), "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
		add_curve(pm, x0, y0, x1, y1, x2, y2, x3, y3, step, HERMITE_MODE);
    }

    else if (strcmp(line, "bezier") == 0) {  //not done
		sscanf(fgets(line, 255, f), "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
		add_curve(pm, x0, y0, x1, y1, x2, y2, x3, y3, step, BEZIER_MODE);
    }

    else if (strcmp(line, "ident") == 0) {
    	ident(transform);
    	printf("IDENT MATRIX\n");
    	print_matrix(transform);
    }

    else if (strcmp(line, "scale") == 0) {
		sscanf(fgets(line, 255, f), "%lf %lf %lf", &x, &y, &z);
		printf("Read in from scale:\n");
    	printf("%lf %lf %lf\n", x, y, z);
		struct matrix *s  = make_scale(x, y, z);
    	matrix_mult(s, transform);
    	printf("TRANSFORM MATRIX\n");
    	print_matrix(transform);
    }

    else if (strcmp(line, "translate") == 0) {
		sscanf(fgets(line, 255, f), "%lf %lf %lf", &x, &y, &z);	    	
		struct matrix *s  = make_translate(x, y, z);
		print_matrix(s);
    	matrix_mult(s, transform);
    	print_matrix(transform);
    }
    
    else if (strcmp(line, "xrotate") == 0) {
    	sscanf(fgets(line, 255, f), "%lf", &theta);
    	theta *= M_PI / 180;
    	struct matrix *s = make_rotX(theta);
    	matrix_mult(s, transform);
    }

    else if (strcmp(line, "yrotate") == 0) {
    	sscanf(fgets(line, 255, f), "%lf", &theta);
    	theta *= M_PI / 180;
    	struct matrix *s = make_rotY(theta);
    	matrix_mult(s, transform);
    }

    else if (strcmp(line, "zrotate") == 0) {
    	sscanf(fgets(line, 255, f), "%lf", &theta);
    	theta *= M_PI / 180;
    	struct matrix *s = make_rotZ(theta);
    	matrix_mult(s, transform);
    }

    else if (strcmp(line, "apply") == 0) {
    	matrix_mult(transform, pm);
    	printf("PM\n");
    	print_matrix(pm);
    	draw_lines(pm, s, c);
    }

    else if (strcmp(line, "display") == 0) {
      clear_screen(s);
      draw_lines(pm,s,c);
      display(s);
    }

   	else if (strcmp(line, "save") == 0) {
   		save_extension(s, "pic_curves");
    }

    else if (strcmp(line, "quit") == 0) {
    	exit(0);
    }
  }
}

  
