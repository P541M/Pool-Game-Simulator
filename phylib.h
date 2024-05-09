#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Constants
#define PHYLIB_BALL_RADIUS (28.5) // mm
#define PHYLIB_BALL_DIAMETER (2 * PHYLIB_BALL_RADIUS)

#define PHYLIB_HOLE_RADIUS (2 * PHYLIB_BALL_DIAMETER)
#define PHYLIB_TABLE_LENGTH (2700.0) // mm
#define PHYLIB_TABLE_WIDTH (PHYLIB_TABLE_LENGTH / 2.0) // mm

#define PHYLIB_SIM_RATE (0.0001) // s
#define PHYLIB_VEL_EPSILON (0.01) // mm/s

#define PHYLIB_DRAG (150.0) // mm/s^2
#define PHYLIB_MAX_TIME (600) // s

#define PHYLIB_MAX_OBJECTS (26)

//Polymorphic object types defined as enum
/*enums are like grouped constants*/
typedef enum {
    PHYLIB_STILL_BALL = 0,
    PHYLIB_ROLLING_BALL = 1,
    PHYLIB_HOLE = 2,
    PHYLIB_HCUSHION = 3,
    PHYLIB_VCUSHION = 4,
} phylib_obj;

//Class representing a vector in 2 dimensions
typedef struct {
    double x;
    double y;
} phylib_coord;

//(Child) Classes representing objects on the table
/*First, a ball that is not in motion (i.e. still). It has a number (cue ball is 0) and a position on the
table.*/
typedef struct {
    unsigned char number;
    phylib_coord pos;
} phylib_still_ball;

/*Next, a ball that is rolling (i.e. not still). It has a number (cue ball is 0) and a position on the
table. It also has a velocity and a (negative) acceleration (due to friction).*/
typedef struct {
    unsigned char number;
    phylib_coord pos;
    phylib_coord vel;
    phylib_coord acc;
} phylib_rolling_ball;

/*Next, one of the 6 holes on the table. It has a position.*/
typedef struct {
    phylib_coord pos;
} phylib_hole;

/*Next, a horizontal cushion (i.e. a cushion along either of the two the short sides of the table). It
has only a y-coordinate.*/
typedef struct {
    double y;
} phylib_hcushion;

/*Rather predictably, a vertical cushion (i.e. a cushion along either of the two the long sides of the
table). It has only an x-coordinate.*/
typedef struct {
    double x;
} phylib_vcushion;

//Polymorphic Parent Class of objects on the table
/*First, we need a C union that can store any of the above classes/structure in the same space.*/
typedef union {
    phylib_still_ball still_ball;
    phylib_rolling_ball rolling_ball;
    phylib_hole hole;
    phylib_hcushion hcushion;
    phylib_vcushion vcushion;
} phylib_untyped;

/*While this union can store an object of any of the above classes/structures, it cannot identify
what the class of the object is, so we need another structure for that.*/
/*Here, the type is the enum indicating the class of the object, and obj is the object itself.
We can now use phylib_object to represent a generic object in the billiards world.*/
typedef struct {
    phylib_obj type;
    phylib_untyped obj;
} phylib_object;

//Finally, the table
/*As the game proceeds there will be multiple table configurations at different points in time, so
each table “knows” its time. There can be a maximum of 26 objects on the table: 15 numbered
balls, 1 cue ball, 4 cushions, and 6 holes.*/
typedef struct {
    double time;
    phylib_object *object[PHYLIB_MAX_OBJECTS];
} phylib_table;










//Part I Functions
/*This function will allocate memory for a new phylib_object, set its type to
PHYLIB_STILL_BALL and transfer the information provided in the function parameters into the
structure. It will return a pointer to the phylib_object. If the malloc function fails, it will
return NULL (before trying to store the function parameters in the (non-existent) structure).*/
phylib_object *phylib_new_still_ball(unsigned char number, phylib_coord *pos);

/*These functions will do the same thing as the phylib_new_still_ball function for their
respective structures.*/
phylib_object *phylib_new_rolling_ball(unsigned char number, phylib_coord *pos, phylib_coord *vel, phylib_coord *acc);
phylib_object *phylib_new_hole(phylib_coord *pos);
phylib_object *phylib_new_hcushion(double y);
phylib_object *phylib_new_vcushion(double x);

/*This function will allocate memory for a table structure, returning NULL if the memory
allocation fails. The member variable, time, will be set to 0.0. It will then assign the values of
its array elements to pointers to new objects created by the phylib_new_* functions provided
above. Specifically, it will add elements in this order:
1) a horizontal cushion at y=0.0;
2) a horizontal cushion at y=PHYLIB_TABLE_LENGTH;
3) a vertical cushion at x=0.0;
4) a vertical cushion at x=PHYLIB_TABLE_WIDTH;
5) 6 holes: positioned in the four corners where the cushions meet and two more midway between the top holes and bottom holes.
The remaining pointers will all be set to NULL.*/
phylib_table *phylib_new_table(void);










//Part II Functions
/*This function should allocate new memory for a phylib_object. Save the address of that
object at the location pointed to by dest, and copy over the contents of the object from the
location pointed to by src. Hint, you can use memcpy to make this a one-step operation that
works for any type of phylib_object. If src points to a location containing a NULL pointer,
then the location pointed to by dest should be assigned the value of NULL.*/
void phylib_copy_object( phylib_object **dest, phylib_object **src );

/*This function should allocate memory for a new phylib_table, returning NULL if the malloc
fails. Then the contents pointed to by table should be copied to the new memory location and
the address returned.*/
phylib_table *phylib_copy_table( phylib_table *table );

/*This function should iterate over the object array in the table until it finds a NULL pointer. It
should then assign that pointer to be equal to the address of object. If there are no NULL
pointers in the array, the function should do nothing.*/
void phylib_add_object( phylib_table *table, phylib_object *object );

/*This function should free every non-NULL pointer in the object array of table. It should then
also free table as well.*/
void phylib_free_table( phylib_table *table );

/*This function should return the difference between c1 and c2. That is the result’s x value
should be c1.x-c2.x and similarly for y.*/
phylib_coord phylib_sub( phylib_coord c1, phylib_coord c2 );

/*This function should return the length of the vector/coordinate c. You can calculate this length
by using Pythagorean theorem. Important, you must not use the exp function from the math
library. That function is designed for raising values to a real power and is extremely inefficient
for something as simple as squaring a value.*/
double phylib_length( phylib_coord c );

/*This function should compute the dot-product between two vectors. Hint: the dot product is
equal to the sum of: the product of the x-values and the product of the y-values.*/
double phylib_dot_product( phylib_coord a, phylib_coord b );

/*Reference doc for information*/
double phylib_distance( phylib_object *obj1, phylib_object *obj2 );










//Part III Functions
/*Reference doc for information*/
void phylib_roll( phylib_object *new, phylib_object *old, double time );

/*Reference doc for information*/
unsigned char phylib_stopped( phylib_object *object );

/*Reference doc for information*/
void phylib_bounce( phylib_object **a, phylib_object **b );

/*Reference doc for information*/
unsigned char phylib_rolling( phylib_table *t );

/*Reference doc for information*/
phylib_table *phylib_segment( phylib_table *table );










//Functions proceeding are helper functions, mainly used for simple calculations and setting
/*For phylib_roll, will compute the new pos and vel using the formula*/
void new_position(phylib_object *new, phylib_object *old, double time);
void new_velocity(phylib_object *new, phylib_object *old, double time);

/*For phylib_bounce cases 1-2*/
void horizontal_bounce(phylib_object **a);
void vertical_bounce(phylib_object **a);

/*For phylib_bounce case 4*/
void upgrade_ball_rolling(phylib_object **b);

/*For phylib_bounce case 5*/
void update_acc(phylib_object **object, double speedOfObject);

/*For phylib_segment, will do simple error checking before proceeding */
phylib_table *check_rolling_check_copy(phylib_table *table);










//A2 Function(s)
char *phylib_object_string( phylib_object *object );
