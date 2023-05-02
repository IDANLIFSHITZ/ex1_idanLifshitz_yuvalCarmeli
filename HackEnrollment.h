#ifndef HACKENROLLMENT_H
#define HACKENROLLMENT_H


#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct EnrollmentSystem EnrollmentSystem;

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);




#endif //HACKENROLLMENT_H
