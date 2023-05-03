//
// Created by yuval on 5/2/2023.
//
#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>




typedef struct Student{
    char* StudentID;
    char* name;
    char* surName;

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
    int StudentArraySize;

    course* course;
    int courseArraySize;

    student* hackers;
    int hackersArraySize;
}*EnrollmentSystem;

student createNewStudent();
void initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students);


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    EnrollmentSystem sys = malloc(sizeof(*sys));
    if(sys == NULL){
        return NULL;
    }
    sys->StudentArraySize = 1;
    sys->courseArraySize = 1;
    sys->hackersArraySize = 1;
    sys->myStudents = malloc(sizeof(student));
    if(sys->myStudents == NULL){
        free(sys);
        return NULL;
    }
    sys->course = malloc(sizeof(course));
    if(sys->course == NULL){
        free(sys->myStudents);
        free(sys);
        return NULL;
    }
    sys->hackers = malloc(sizeof(student));
    if(sys->hackers == NULL){
        free(sys->myStudents);
        free(sys->course);
        free(sys);
        return NULL;
    }
    initStudentArrayOfEnrollmentSystem(sys, students);
    if(sys->myStudents == NULL){
        free(sys->myStudents);
        free(sys->course);
        free(sys);
        return NULL;
    }





    return sys;

}


void initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students){
    int studentsNum, discardDouble, discardInt, nameLength = 0;
    int namesSize = 20;
    char currChar, garbage = '0';
    student s = NULL;
char firstChar = fgetc(students);
    while(firstChar != EOF){
        currChar = firstChar;
        //check if need to realloc the array of students
        if(studentsNum == sys->StudentArraySize){
            sys->StudentArraySize *= 2;
            sys->myStudents = realloc(sys->myStudents, sizeof(student) * sys->StudentArraySize);
            if(sys->myStudents == NULL){
                return;
            }
        }// end of realloc check and memory handling

        //create new student
        s = createNewStudent();
        if(s == NULL || s->StudentID == NULL || s->name == NULL || s->surName == NULL){
            return;
        }
        //store the student in the array
        sys->myStudents[studentsNum] = s;

        //get the student ID
        s->StudentID[0] = currChar;
        for (int i = 1; i < 9; i++) {
            s->StudentID[i] = currChar;
            currChar = fgetc(students);
        }

        //discard the space, total credits, GPA and space at the end
        fscanf(students, " %d %lf ", &discardInt, &discardDouble);

        //get the name
        for(int i = 0 ;currChar != ' '; i++){
            if(nameLength == namesSize){
                namesSize *= 2;
                s->name = realloc(s->name, sizeof(char) * namesSize);
                if(s->name == NULL){
                    free(s->StudentID);
                    free(s->name);
                    return;
                }
            }
            currChar = fgetc(students);
            s->name[i] = currChar;
            nameLength++;
        }
        //discard the space
        currChar = '0';

        //get the surname
        namesSize = 20;
        nameLength = 0;
        for(int i = 0 ; currChar != ' '; i++){
            if(nameLength == namesSize){
                namesSize *= 2;
                s->surName = realloc(s->surName, sizeof(char) * namesSize);
                if(s->surName == NULL){
                    free(s->StudentID);
                    free(s->name);
                    free(s->surName);
                    return;
                }
            }
            currChar = fgetc(students);
            s->surName[i] = currChar;
            nameLength++;
        }
        //
        studentsNum++;
        firstChar = fgetc(students);
    }
}

student createNewStudent(){
    student newStudent = malloc(sizeof(*newStudent));
    if(newStudent == NULL){
        return NULL;
    }
    newStudent->StudentID = malloc(sizeof(char) * 9);
    if(newStudent->StudentID == NULL){
        free(newStudent);
        return NULL;
    }
    newStudent->name = malloc(sizeof(char) * 20);
    if(newStudent->name == NULL){
        free(newStudent->StudentID);
        free(newStudent);
        return NULL;
    }
    newStudent->surName = malloc(sizeof(char) * 20);
    if(newStudent->surName == NULL){
        free(newStudent->StudentID);
        free(newStudent->name);
        free(newStudent);
        return NULL;
    }

}


void initCoursesArrayOfSystem(EnrollmentSystem sys, FILE* courses) {

}

void initHackersArrayOfSystem(EnrollmentSystem sys, FILE* hackers){

}