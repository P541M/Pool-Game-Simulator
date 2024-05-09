#include "phylib.h"

//Part I: Functions
/*This function will allocate memory for a new phylib_object, set its type to
PHYLIB_STILL_BALL and transfer the information provided in the function parameters into the
structure. It will return a pointer to the phylib_object. If the malloc function fails, it will
return NULL (before trying to store the function parameters in the (non-existent) structure).*/
phylib_object *phylib_new_still_ball(unsigned char number, phylib_coord *pos){
    //Allocating space for a new phylib_object
    phylib_object *newObject = malloc(sizeof(phylib_object));
    
    //Checking if the memory allocation worked
    if (newObject == NULL){
        return NULL;
    }
    //Setting the struct with the params passed through the function
    else{
        newObject->type = PHYLIB_STILL_BALL;
        newObject->obj.still_ball.number = number;
        newObject->obj.still_ball.pos = *pos;
    }
    //Returns the created object (still ball)
    return newObject;
}

/*These functions will do the same thing as the phylib_new_still_ball function for their
respective structures.*/
phylib_object *phylib_new_rolling_ball(unsigned char number, phylib_coord *pos, phylib_coord *vel, phylib_coord *acc){
    //Allocating space for a new phylib_object
    phylib_object *newObject = malloc(sizeof(phylib_object));
    
    //Checking if the memory allocation worked
    if (newObject == NULL){
        return NULL;
    }
    //Setting the struct with the params passed through the function
    else{
        newObject->type = PHYLIB_ROLLING_BALL;
        newObject->obj.rolling_ball.number = number;
        newObject->obj.rolling_ball.pos = *pos;
        newObject->obj.rolling_ball.vel = *vel;
        newObject->obj.rolling_ball.acc = *acc;
    }
    //Returns the created object (rolling ball)
    return newObject;
}

phylib_object *phylib_new_hole(phylib_coord *pos){
    //Allocating space for a new phylib_object
    phylib_object *newObject = malloc(sizeof(phylib_object));
    
    //Checking if the memory allocation worked
    if (newObject == NULL){
        return NULL;
    }
    //Setting the struct with the params passed through the function
    else{
        newObject->type = PHYLIB_HOLE;
        newObject->obj.hole.pos = *pos;
    }
    //Returns the created object (new hole)
    return newObject;
}

phylib_object *phylib_new_hcushion(double y){
    //Allocating space for a new phylib_object
    phylib_object *newObject = malloc(sizeof(phylib_object));
    
    //Checking if the memory allocation worked
    if (newObject == NULL){
        return NULL;
    }
    //Setting the struct with the params passed through the function
    else{
        newObject->type = PHYLIB_HCUSHION;
        newObject->obj.hcushion.y = y;
    }
    //Returns the created object (horizontal cushion)
    return newObject;
}

phylib_object *phylib_new_vcushion(double x){
    //Allocating space for a new phylib_object
    phylib_object *newObject = malloc(sizeof(phylib_object));
    
    //Checking if the memory allocation worked
    if (newObject == NULL){
        return NULL;
    }
    //Setting the struct with the params passed through the function
    else{
        newObject->type = PHYLIB_VCUSHION;
        newObject->obj.vcushion.x = x;
    }
    //Returns the created object (vertical cushion)
    return newObject;
}

/*This function will allocate memory for a table structure, returning NULL if the memory
allocation fails. The member variable, time, will be set to 0.0. It will then assign the values of
its array elements to pointers to new objects created by the phylib_new_* functions provided
above. Specifically, it will add elements in this order:
1) a horizontal cushion at y=0.0;
2) a horizontal cushion at y=PHYLIB_TABLE_LENGTH;
3) a vertical cushion at x=0.0;
4) a vertical cushion at x=PHYLIB_TABLE_WIDTH;
5) 6 holes: positioned in the four corners where the cushions meet and two more
midway between the top holes and bottom holes.
The remaining pointers will all be set to NULL.*/
phylib_table *phylib_new_table(void){
    //Allocating space for a new phylib_table
    phylib_table *newTable = malloc(sizeof(phylib_table));
    
    //Checking if the memory allocation worked
    if (newTable == NULL){
        return NULL;
    }
    //Fills in the array with the given objects
    else{
        
        //For setting the coordinates
        phylib_coord holePosition;

        //Setting time in the table struct to 0.0
        newTable->time = 0.0;

        //Setting the horizontal cushions
        newTable->object[0] = phylib_new_hcushion(0.0);
        newTable->object[1] = phylib_new_hcushion(PHYLIB_TABLE_LENGTH);

        //Setting the vertical cushions
        newTable->object[2] = phylib_new_vcushion(0.0);
        newTable->object[3] = phylib_new_vcushion(PHYLIB_TABLE_WIDTH);

        //Sets the 6 holes for the table
        holePosition.x = 0.0;
        holePosition.y = 0.0;
        newTable->object[4] = phylib_new_hole(&holePosition);

        holePosition.x = 0.0;
        holePosition.y = PHYLIB_TABLE_WIDTH;
        newTable->object[5] = phylib_new_hole(&holePosition);

        holePosition.x = 0.0;
        holePosition.y = PHYLIB_TABLE_LENGTH;
        newTable->object[6] = phylib_new_hole(&holePosition);

        holePosition.x = PHYLIB_TABLE_WIDTH;
        holePosition.y = 0.0;
        newTable->object[7] = phylib_new_hole(&holePosition);

        holePosition.x = PHYLIB_TABLE_WIDTH;
        holePosition.y = PHYLIB_TABLE_WIDTH;
        newTable->object[8] = phylib_new_hole(&holePosition);

        holePosition.x = PHYLIB_TABLE_WIDTH;
        holePosition.y = PHYLIB_TABLE_LENGTH;
        newTable->object[9] = phylib_new_hole(&holePosition);

        //Sets remaining elements of array to NULL using NULL as it's stopping point
        for (int i = 10; i < PHYLIB_MAX_OBJECTS; i++) {
            newTable->object[i] = NULL;
        }
    }
    //Returns the newly created table
    return newTable;
}










//Part II: Functions
/*This function should allocate new memory for a phylib_object. Save the address of that
object at the location pointed to by dest, and copy over the contents of the object from the
location pointed to by src. Hint, you can use memcpy to make this a one-step operation that
works for any type of phylib_object. If src points to a location containing a NULL pointer,
then the location pointed to by dest should be assigned the value of NULL.*/
void phylib_copy_object( phylib_object **dest, phylib_object **src ){
    //Checks if the source is NULL
    if(*src == NULL){
        //Sets destination to NULL if so
        *dest = NULL; 
    }
    //Allocates memory for destination and will copy source to it
    else{
        *dest = malloc(sizeof(phylib_object));
        //Checks if memory allocation worked
        if(*dest == NULL){
            return;
        }
        //Memory allocation worked! YAYY
        else{
            //Uses source and copies it to destination
            memcpy(*dest, *src, sizeof(phylib_object));
        }
    }
}

/*This function should allocate memory for a new phylib_table, returning NULL if the malloc
fails. Then the contents pointed to by table should be copied to the new memory location and
the address returned.*/
phylib_table *phylib_copy_table( phylib_table *table ){
    //Allocated memory for a new table
    phylib_table *newTable = malloc(sizeof(phylib_table));

    //Checks if the memory allocation worked
    if (newTable == NULL){
        return NULL;
    }
    //Will copy the objects from the param table and put it into the new one
    else{
        //Sets the time in the struct from the old table
        newTable->time = table->time;
        //For loop to iterate through the objects and set them
        for(int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
            phylib_copy_object(&newTable->object[i], &table->object[i]);
        }
    }
    //Returns the new table that was copied
    return newTable;
}

/*This function should iterate over the object array in the table until it finds a NULL pointer. It
should then assign that pointer to be equal to the address of object. If there are no NULL
pointers in the array, the function should do nothing.*/
void phylib_add_object( phylib_table *table, phylib_object *object ){
    //For loop to iterate through the whole array and look for a NULL
    for(int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        //Checks if the object at the index is NULL
        if(table->object[i] == NULL){
            //Once a null is found the object will be assigned
            table->object[i] = object;
            break;
        }
    }
}

/*This function should free every non-NULL pointer in the object array of table. It should then
also free table as well.*/
void phylib_free_table( phylib_table *table ){
    //For loop to iterate through the table array
    for(int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
        //Checks every object that is not NULL
        if(table->object[i] != NULL){
            //If found, will free the object
            free(table->object[i]);
        }
    }
    //NO MEMORY LEAKS HEREEE!!
    free(table);
}

/*This function should return the difference between c1 and c2. That is the result’s x value
should be c1.x-c2.x and similarly for y.*/
phylib_coord phylib_sub( phylib_coord c1, phylib_coord c2 ){
    //Creates coord and saves it as value
    phylib_coord value;

    //Calculations done accordingly
    value.x = c1.x - c2.x;
    value.y = c1.y - c2.y;

    //Return the final value
    return value;
}

/*This function should return the length of the vector/coordinate c. You can calculate this length
by using Pythagorean theorem. Important, you must not use the exp function from the math
library. That function is designed for raising values to a real power and is extremely inefficient
for something as simple as squaring a value.*/
double phylib_length( phylib_coord c ){
    //Sets the a and b
    double valueA; 
    double valueB;
    //The c value
    double finalValue;

    //Calculations done accordingly
    valueA = (c.x * c.x);
    valueB = (c.y * c.y);
    finalValue = sqrt(valueA + valueB);

    //Returns C, the final value
    return finalValue;
}

/*This function should compute the dot-product between two vectors. Hint: the dot product is
equal to the sum of: the product of the x-values and the product of the y-values.*/
double phylib_dot_product( phylib_coord a, phylib_coord b ){
    //For X
    double valueA;
    //For Y
    double valueB;
    //Final return value
    double finalValue;

    //Calculations done accordingly
    valueA = a.x * b.x;
    valueB = a.y * b.y;
    finalValue = valueA + valueB;

    //Return the final value
    return finalValue;
}

/*Reference doc for information*/
double phylib_distance( phylib_object *obj1, phylib_object *obj2 ){
    //Gets the center position of the first ball
    phylib_coord ball1Center = obj1->obj.rolling_ball.pos;

    //For setting the cushion coordinate
    double cushionCoord = 0.0;
    //Final return value
    double distance = 0.0;
    
    //Checks if the object1 is not a rolling ball
    if(obj1->type != PHYLIB_ROLLING_BALL){
        return -1.0;
    }
    /*If obj2 is another BALL (ROLLING or STILL), then compute the distance between the
    centres of the two balls and subtract two radii (i.e. one PHYLIB_BALL_DIAMETER).*/
    else if((obj2->type == PHYLIB_ROLLING_BALL) || (obj2->type == PHYLIB_STILL_BALL)){
        //Declared for later usage
        phylib_coord ball2Center;

        //Did this to get rid of pedantic warnings
        //Sets them to 0 by default
        ball2Center.x = 0.0;
        ball2Center.y = 0.0;

        //Checks if object 2 is a rolling ball
        if(obj2->type == PHYLIB_ROLLING_BALL){
            //Updates ball2Center accordingly
            ball2Center = obj2->obj.rolling_ball.pos;
        }
        //Will correspond to a still ball
        else{
            //Updates ball2Center accordingly
            ball2Center = obj2->obj.still_ball.pos;
        }

        //Calculates the distance using the centers and length function, subtracts two radii (PHYLIB_BALL_DIAMETER)
        distance = phylib_length(phylib_sub(ball1Center, ball2Center)) - PHYLIB_BALL_DIAMETER;
        //Returns Distance
        return distance;
    }
    /*If obj2 is a HOLE, then compute the distance between the centre of the ball and the
    hole and subtract the HOLE_RADIUS.*/
    else if(obj2->type == PHYLIB_HOLE){
        //Declared for later usage
        phylib_coord ball2Center;
        
        //Did this to get rid of pedantic warnings
        //Sets them to 0 by default
        ball2Center.x = 0.0;
        ball2Center.y = 0.0;

        //Updates ball2Center accordingly
        ball2Center = obj2->obj.hole.pos;
        //Calculates the distance using the centers and length function, subtracts the radius (PHYLIB_HOLE_RADIUS)
        distance = phylib_length(phylib_sub(ball1Center, ball2Center)) - PHYLIB_HOLE_RADIUS;
        //Returns Distance
        return distance;
    }
    /*If obj2 is a CUSHION calculate the distance between the centre of the ball and the
    CUSION and subtract the BALL_RADIUS. Hint: use abs since the ball might be left/above
    or right/below to CUSHION.*/
    else if((obj2->type == PHYLIB_HCUSHION) || (obj2->type == PHYLIB_VCUSHION)){
        //Checks if object 2 is a horizontal cushion
        if(obj2->type == PHYLIB_HCUSHION){
            //Sets cushionCoord to the corresponding value (y)
            cushionCoord = obj2->obj.hcushion.y;
            //Calculates the distance using the coord and ball center, subtracts the radius as well
            distance = fabs(ball1Center.y - cushionCoord) - PHYLIB_BALL_RADIUS;
        }
        //Checks if object 2 is a vertical cushion
        else if(obj2->type == PHYLIB_VCUSHION){
            //Sets cushionCoord to the corresponding value (x)
            cushionCoord = obj2->obj.vcushion.x;
            //Calculates the distance using the coord and ball center, subtracts the radius as well
            distance = fabs(ball1Center.x - cushionCoord) - PHYLIB_BALL_RADIUS;
        }
        //Returns Distance
        return distance;
    }
    /*Return -1.0 if obj2 isn’t any valid type.*/
    else{
        return -1.0;
    }
}










//Part III: Functions
/*Reference doc for information*/
void phylib_roll( phylib_object *new, phylib_object *old, double time ){
    //Checks if both new and old are rolling balls
    if((new->type == PHYLIB_ROLLING_BALL) && (old->type == PHYLIB_ROLLING_BALL)){
        //Saves the velocity (x and y) before changing them
        double newValBeforeChangeX = new->obj.rolling_ball.vel.x;
        double newValBeforeChangeY = new->obj.rolling_ball.vel.y;

        //Function call to calculate position using formula
        new_position(new, old, time);
        //Function call to calculate velocity using formula
        new_velocity(new, old, time);

        //Will multiply the new * newValBeforeChangeX and if negative will be less than 0 (for x)
        if((new->obj.rolling_ball.vel.x * newValBeforeChangeX) < 0){
            //Sets corresponding values to 0
            new->obj.rolling_ball.vel.x = 0.0;
            new->obj.rolling_ball.acc.x = 0.0;
        }
        //Will multiply the new * newValBeforeChangeY and if negative will be less than 0 (for y)
        if((new->obj.rolling_ball.vel.y * newValBeforeChangeY) < 0){
            //Sets corresponding values to 0
            new->obj.rolling_ball.vel.y = 0.0;
            new->obj.rolling_ball.acc.y = 0.0;
        }
    }
}

/*Reference doc for information*/
unsigned char phylib_stopped( phylib_object *object ){
    //Calculates ball speed using the phylib_length function
    double ballSpeed = phylib_length(object->obj.rolling_ball.vel);

    //Checks if it is less than PHYLIB_VEL_EPSILON
    if(ballSpeed < PHYLIB_VEL_EPSILON){
        //Changes it to a still ball
        object->type = PHYLIB_STILL_BALL;
        //Updates values accordingly
        object->obj.still_ball.number = object->obj.rolling_ball.number;
        object->obj.still_ball.pos = object->obj.rolling_ball.pos;

        //Returns 1 if converted
        return 1;
    }
    //Returns 0 if no conversion
    return 0;
}

/*Reference doc for information*/
void phylib_bounce( phylib_object **a, phylib_object **b ){
    //For case 5
    phylib_coord r_ab;
    phylib_coord v_rel;
    phylib_coord n;
    double v_rel_n;

    //For computing the speeds of object a and b
    double speedOfa;
    double speedOfb;

    //CASE 1: b is a HCUSHION:
    /*In this case the y velocity and y acceleration of a are reversed (negated). This is the
    physical principle of angle of incidence equals angle of reflection.*/
    if((*b)->type == PHYLIB_HCUSHION){
        //Function call to negate
        horizontal_bounce(a);
    }
    //CASE 2: b is a VCUSION:
    /*In this case the x velocity and x acceleration of a are reversed (negated). This is the
    physical principle of angle of incidence equals angle of reflection.*/
    else if((*b)->type == PHYLIB_VCUSHION){
        //Function call to negate
        vertical_bounce(a);
    }
    //CASE 3: b is a HOLE:
    /*In this case, free the memory of a and set it to NULL. This represents the ball falling off
    the table.*/
    else if((*b)->type == PHYLIB_HOLE){
        //FREEEEEEEEEEEE
        free(*a);
        *a = NULL;
        return;
    }
    //CASE 4: b is a STILL_BALL:
    /*In this case, “upgrade” the STILL_BALL to a ROLLING BALL and proceed directly to CASE
    5 (do not collect $200). HINT: if you leave out the break statement at the end of a case
    condition, the code will continue with the next case.*/
    else if((*b)->type == PHYLIB_STILL_BALL){
        //Function call to update the ball
        upgrade_ball_rolling(b);
    }
    //CASE 5: b is a ROLLING_BALL:
    /*Reference doc for information*/
    if((*b)->type == PHYLIB_ROLLING_BALL){

        //Computes the position between a and b
        r_ab = phylib_sub((*b)->obj.rolling_ball.pos, (*a)->obj.rolling_ball.pos);
        //Computes the velocity between a and b
        v_rel = phylib_sub((*a)->obj.rolling_ball.vel, (*b)->obj.rolling_ball.vel);
        //Calculates the normal vector
        n.x = r_ab.x / phylib_length(r_ab);
        n.y = r_ab.y / phylib_length(r_ab);
        //Calculates the ratio of relative velocity
        v_rel_n = phylib_dot_product(v_rel, n);

        //Update velocities of object a
        (*a)->obj.rolling_ball.vel.x -= v_rel_n * n.x;
        (*a)->obj.rolling_ball.vel.y -= v_rel_n * n.y;
        //Update velocities of object b
        (*b)->obj.rolling_ball.vel.x += v_rel_n * n.x;
        (*b)->obj.rolling_ball.vel.y += v_rel_n * n.y;

        //Compute speeds of object a and b
        speedOfa = phylib_length((*a)->obj.rolling_ball.vel);
        speedOfb = phylib_length((*b)->obj.rolling_ball.vel);

        //Function calls to update acc
        update_acc(a, speedOfa);
        update_acc(b, speedOfb);
    }
}

/*Reference doc for information*/
unsigned char phylib_rolling( phylib_table *t ){
    //Declared for later usage
    unsigned char rollingBallAmount = 0;

    //For loop to go through the array of objects
    for(int i = 0; i < PHYLIB_MAX_OBJECTS ; i++){
        //Checks if the object at the index is not a NULL and is a rolling ball
        if((t->object[i] != NULL) && (t->object[i]->type == PHYLIB_ROLLING_BALL)){
            //Increments counter
            rollingBallAmount++;
        }
    }
    //Returns final amount of rolling balls
    return rollingBallAmount;
}

phylib_table *phylib_segment(phylib_table *table){
    //Declared for later usage
    double time = 0.0;

    //Function call to error check if rolling ball and if copy worked
    phylib_table *newTable = check_rolling_check_copy(table);
    if(newTable == NULL){
        return NULL;
    }

    //While loop to run till PHYLIB_MAX_TIME is reached
    while(time <= PHYLIB_MAX_TIME){
        //Sets timer
        time += PHYLIB_SIM_RATE;
        //For loop to iterate through the array of objects
        for(int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
            //Simplifies code, for current object
            phylib_object *currObj1 = newTable->object[i];

            //Checks if NULL and is a rolling ball
            if(currObj1 != NULL && currObj1->type == PHYLIB_ROLLING_BALL){
                //ROLLINGGGGG
                phylib_roll(currObj1, table->object[i], time);
            }
        }

        //For loop to iterate through the array of objects
        for(int i = 0; i < PHYLIB_MAX_OBJECTS; i++){
            //Simplifies code, for current object
            phylib_object *currObj2 = newTable->object[i];

            //Checks if NULL and is a rolling ball
            if(currObj2 != NULL && currObj2->type == PHYLIB_ROLLING_BALL){
                // Will check if the rolling ball has stopped
                if(phylib_stopped(currObj2) == 1){
                    // Update time
                    newTable->time += time;
                    // Returns the table if so
                    return newTable;
                }
                // For loop to go through the array of objects
                // Will also check for collision
                for(int j = 0; j < PHYLIB_MAX_OBJECTS; j++){
                    // Will check if NULL and if the distance between objects are less than 0.0
                    if(newTable->object[j] != NULL && i != j && phylib_distance(currObj2, newTable->object[j]) < 0.0){
                        // BOUNCE! COLLISION DETECTEDDDD
                        phylib_bounce(&newTable->object[i], &newTable->object[j]);
                        // Update time
                        newTable->time += time;
                        // Returns final updated table
                        return newTable;
                    }
                }
            }
        }
    }
    // Returns final updated table
    return newTable;
}










//Functions proceeding are helper functions, mainly used for cleaning the code
/*For phylib_roll, will compute the new pos using the formula*/
void new_position(phylib_object *new, phylib_object *old, double time){
    new->obj.rolling_ball.pos.x = old->obj.rolling_ball.pos.x + old->obj.rolling_ball.vel.x * time + 0.5 * old->obj.rolling_ball.acc.x * time * time;
    new->obj.rolling_ball.pos.y = old->obj.rolling_ball.pos.y + old->obj.rolling_ball.vel.y * time + 0.5 * old->obj.rolling_ball.acc.y * time * time;
}

/*For phylib_roll, will compute the new vel using the formula*/
void new_velocity(phylib_object *new, phylib_object *old, double time){
    new->obj.rolling_ball.vel.x = old->obj.rolling_ball.vel.x + old->obj.rolling_ball.acc.x * time;
    new->obj.rolling_ball.vel.y = old->obj.rolling_ball.vel.y + old->obj.rolling_ball.acc.y * time;
}

/*For phylib_bounce case 1*/
void horizontal_bounce(phylib_object **a) {
    //Negates by multiplying by negative 1
    (*a)->obj.rolling_ball.vel.y = (-1.0) * (*a)->obj.rolling_ball.vel.y;
    (*a)->obj.rolling_ball.acc.y = (-1.0) * (*a)->obj.rolling_ball.acc.y;
}

/*For phylib_bounce case 2*/
void vertical_bounce(phylib_object **a) {
    //Negates by multiplying by negative 1
    (*a)->obj.rolling_ball.vel.x = (-1.0) * (*a)->obj.rolling_ball.vel.x;
    (*a)->obj.rolling_ball.acc.x = (-1.0) * (*a)->obj.rolling_ball.acc.x;
}

/*For phylib_bounce case 4*/
void upgrade_ball_rolling(phylib_object **b) {
    //Saving values to later set
    double tempBallNum = (*b)->obj.still_ball.number;
    phylib_coord tempBallPos = (*b)->obj.still_ball.pos;

    //Changes type to rolling ball
    (*b)->type = PHYLIB_ROLLING_BALL;
    //Sets values accordingly
    (*b)->obj.rolling_ball.number = tempBallNum;
    (*b)->obj.rolling_ball.pos = tempBallPos;
    (*b)->obj.rolling_ball.vel.x = (*b)->obj.rolling_ball.vel.x = 0.0;
    (*b)->obj.rolling_ball.vel.y = (*b)->obj.rolling_ball.vel.y = 0.0;
    (*b)->obj.rolling_ball.acc.x = (*b)->obj.rolling_ball.acc.x = 0.0;
    (*b)->obj.rolling_ball.acc.y = (*b)->obj.rolling_ball.acc.y = 0.0;
}

/*For phylib_bounce case 5*/
void update_acc(phylib_object **object, double speedOfObject) {
    if (speedOfObject > PHYLIB_VEL_EPSILON) {
        (*object)->obj.rolling_ball.acc.x = (-1.0) * ((*object)->obj.rolling_ball.vel.x) / speedOfObject * PHYLIB_DRAG;
        (*object)->obj.rolling_ball.acc.y = (-1.0) * ((*object)->obj.rolling_ball.vel.y) / speedOfObject * PHYLIB_DRAG;
    }
}

/*For phylib_segment, will do simple error checking before proceeding */
phylib_table *check_rolling_check_copy(phylib_table *table){
    //Uses phylib_rolling to check if there are rolling balls
    if(phylib_rolling(table) == 0){
        // Returns NULL if none
        return NULL;
    }
    //Makes a copy of the param table using the phylib_copy_table function
    phylib_table *newTable = phylib_copy_table(table);
    //Checks to see if it worked
    if(newTable == NULL){
        //Returns NULL if not
        return NULL;
    }
    //Returns the new copied table
    return newTable;
}










//A2 Function(s)
char *phylib_object_string(phylib_object *object){
    static char string[80];
    if(object == NULL){
        snprintf(string, 80, "NULL;");
        return string;
    }
    switch (object->type){
    case PHYLIB_STILL_BALL:
        snprintf(string, 80,
                "STILL_BALL (%d,%6.1lf,%6.1lf)",
                object->obj.still_ball.number,
                object->obj.still_ball.pos.x,
                object->obj.still_ball.pos.y);
        break;
    case PHYLIB_ROLLING_BALL:
        snprintf(string, 80,
                "ROLLING_BALL (%d,%6.1lf,%6.1lf,%6.1lf,%6.1lf,%6.1lf,%6.1lf)",
                object->obj.rolling_ball.number,
                object->obj.rolling_ball.pos.x,
                object->obj.rolling_ball.pos.y,
                object->obj.rolling_ball.vel.x,
                object->obj.rolling_ball.vel.y,
                object->obj.rolling_ball.acc.x,
                object->obj.rolling_ball.acc.y);
        break;
    case PHYLIB_HOLE:
        snprintf(string, 80,
                "HOLE (%6.1lf,%6.1lf)",
                object->obj.hole.pos.x,
                object->obj.hole.pos.y);
        break;
    case PHYLIB_HCUSHION:
        snprintf(string, 80,
                "HCUSHION (%6.1lf)",
                object->obj.hcushion.y);
        break;
    case PHYLIB_VCUSHION:
        snprintf(string, 80,
                "VCUSHION (%6.1lf)",
                object->obj.vcushion.x);
        break;
    }
    return string;
}
