//
// Created by yuval on 5/2/2023.
//
#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>

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
}*student;

typedef struct course {
    IsraeliQueue queue;
    int courseNumber;
    int size;
}*course;


typedef struct EnrollmentSystem {
    student* myStudents;
    course* course;
    student* hackers;
}*EnrollmentSystem;


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){

    EnrollmentSystem newSys = (EnrollmentSystem) malloc(sizeof (EnrollmentSystem));
    if()



    //get the students information
    char* StudentID;
    int totalCredits;
    int gpa;
    char* name;
    char* surName;
    char* city;
    char* department;
    while (fscanf(students, "%s %d %d %s %s %s %s", ))
}
