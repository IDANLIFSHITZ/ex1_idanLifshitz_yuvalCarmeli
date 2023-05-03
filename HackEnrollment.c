//
// Created by yuval on 5/2/2023.
//
#include "IsraeliQueue.h"

#define ID_SIZE 9

typedef struct Student{
    char* StudentID;
    int totalCredits;
    int gpa;
    char* name;
    char* surName;
    char* city;
    char* department;

    //hacker parameters
    int* desiredCourses;
    char** friendsId;
    char** enemyId;
}*Student;

typedef struct studentArr {
    Student Students;
    int size;
}studentArr;

typedef struct course {

    Israeli
    int size;
}*course;


typedef struct EnrollmentSystem {

    studentArr myStudents;
    int size;

}*EnrollmentSystem;


EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
void hackEnrollment(EnrollmentSystem sys, FILE* out);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    bool isEOF = 0;
    while (!isEOF)
    {
        char* currName = (char*) malloc(ID_SIZE);
        bool isCourseName = true;
        while (fscanf(queues, "%[^\n] ", currName))
        {
            if (isCourseName)
            {
                isCourseName = false;
                sys.
            }
            else

        }



    }
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{

}