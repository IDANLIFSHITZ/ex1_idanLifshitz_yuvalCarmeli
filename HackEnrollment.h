#ifndef HACKENROLLMENT_H
#define HACKENROLLMENT_H


#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum { SUCCESS, ALLOC_FAILED, BAD_PARAM, ERROR} EnrollmentError;

/**Error clarification:
 * SUCCESS: Indicates the function has completed its task successfully with no errors.
 * ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * BAD_PARAM: Indicates an illegal parameter was passed.
 * ERROR: Indicates any error beyond the above.
 * */



typedef struct EnrollmentSystem * EnrollmentSystem;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);




#endif //HACKENROLLMENT_H
