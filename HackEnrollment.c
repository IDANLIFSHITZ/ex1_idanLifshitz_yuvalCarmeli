//
// Created by yuval on 5/2/2023.
//
#include "IsraeliQueue.h"

typedef struct Student{
    char* StudentID;
    int totalCredits;
    int gpa;
    char* name;
    char* surName;
    char* city;
    char* department

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


typedef struct EnrollementSystem {

    studentArr myStudents;
    int size;

}*EnrollementSystem;
