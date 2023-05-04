//
// Created by yuval on 5/2/2023.
//
#include "IsraeliQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define ID_SIZE 9

#define FRIEND 20
#define  ENEMY -20
#define NEITHER 0

#define NINTH_DIGIT 1000000

typedef struct Student{
    char* StudentID;
    char* name;
    char* surName;

    //hacker parameters, null terminated
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

    course* courses;
    int courseArraySize;

    student* hackers;
    int hackersArraySize;
}*EnrollmentSystem;

student createNewStudent();
void initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students);
void initCoursesArrayOfSystem(EnrollmentSystem sys, FILE* courses);
void initHackersArrayOfSystem(EnrollmentSystem sys, FILE* hackers);
int getHackerPosInStudentArray(EnrollmentSystem sys, char* hackerID);

int compFunc(student s1, student s2);
int friendshipFuncHackerFile(student s1, student s2);
int friendshipFuncNameDist(student s1, student s2);
int friendshipFuncIDSubtract(student s1, student s2);
int calcNameDiff(char* name1, char* name2);
int friendshipFuncHackerFileHelper(student s1, student s2);


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
    sys->courses = malloc(sizeof(course));
    if(sys->courses == NULL){
        free(sys->myStudents);
        free(sys);
        return NULL;
    }
    sys->hackers = malloc(sizeof(student));
    if(sys->hackers == NULL){
        free(sys->myStudents);
        free(sys->courses);
        free(sys);
        return NULL;
    }
    initStudentArrayOfEnrollmentSystem(sys, students);
    if(sys->myStudents == NULL){
        free(sys->courses);
        free(sys);
        return NULL;
    }
    initCoursesArrayOfSystem(sys, courses);
    if(sys->courses == NULL || sys->courses[0] == NULL){
        free(sys->myStudents);
        free(sys->courses);
        free(sys);
        return NULL;
    }



    return sys;

}


void initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students){
    int studentsNum, discardDouble, discardInt, nameLength = 0;
    int namesSize = 20;
    char currChar = '0';
    student s = NULL;
char firstChar = fgetc(students);
    while(firstChar != EOF){
        currChar = firstChar;
        //check if need to realloc the array of students
        if(studentsNum == sys->StudentArraySize){
            sys->StudentArraySize++;
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
        s->StudentID[9] = '\0';

        //discard the space, total credits, GPA and space at the end
        fscanf(students, " %d %lf ", &discardInt, &discardDouble);

        //get the name
        for(int i = 0 ;currChar != ' '; i++){
            if(nameLength == namesSize-1){
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
        s->name[nameLength] = '\0';
        //discard the space
        currChar = '0';

        //get the surname
        namesSize = 20;
        nameLength = 0;
        for(int i = 0 ; currChar != ' '; i++){
            if(nameLength == namesSize-1){
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
        s->surName[nameLength] = '\0';
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
    newStudent->StudentID = malloc(sizeof(char) * 10);
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

course createNewCourse(){
    course newCourse = malloc(sizeof(*newCourse));
    if(newCourse == NULL){
        return NULL;
    }
    newCourse->courseNumber = 0;
    newCourse->size = 0;

    FriendshipFunction* ff = malloc(sizeof(FriendshipFunction*));
    if(ff == NULL){
        free(newCourse);
        return NULL;
    }
    ff[0] = NULL;
    //need to fill compFunc
    newCourse->queue = IsraeliQueueCreate(ff, NULL, 20, 0);
    if(newCourse->queue == NULL){
        free(newCourse);
        return NULL;
    }
    return newCourse;
}

void initCoursesArrayOfSystem(EnrollmentSystem sys, FILE* courses) {
    int i = 1;
    int courseNum = 0;
    sys->courses[0] = createNewCourse();
    if(sys->courses[0] == NULL || sys->courses[0]->queue == NULL){
        //need to free all the array
        free(sys->courses);
        return;
    }
    courseNum++;

    fscanf(courses,"%d %d\n", sys->courses[0]->courseNumber, sys->courses[0]->size);

    while(fscanf(courses,"%d %d\n", sys->courses[i]->courseNumber, sys->courses[i]->size)!= EOF){
        if(courseNum == sys->courseArraySize){
            sys->courseArraySize++;
            sys->courses = realloc(sys->courses, sizeof(course) * sys->courseArraySize);
            if(sys->courses == NULL){
                //need to free all the array
                free(sys->courses);
                return;
            }
        }
        sys->courses[i] = createNewCourse();
        if(sys->courses[i] == NULL || sys->courses[i]->queue == NULL){
            //need to free all the array
            free(sys->courses);
            return;
        }
        i++;
    }

}

void initHackersArrayOfSystem(EnrollmentSystem sys, FILE* hackers) {
    int hackersNum, hackerPos = 0;
    char currChar = '0';
    char hackerID[10];
    char firstChar = fgetc(hackers);
    while (firstChar != EOF) {
        currChar = firstChar;

        //if the array is full, need to realloc
        if (hackersNum == sys->hackersArraySize) {
            sys->hackersArraySize++;
            sys->hackers = realloc(sys->hackers, sizeof(hackers) * sys->hackersArraySize);
            if (sys->hackers == NULL) {
                //need to free all the array
                free(sys->hackers);
                return;
            }
        }//end of realloc

        //get the hacker ID
        hackerID[0] = currChar;
        for (int i = 1; i < 9; i++) {
            hackerID[i] = currChar;
            currChar = fgetc(hackers);
        }
        hackerID[9] = '\0'; //end of string

        //get the hacker position in the student array and put it in the hackers array
        hackerPos = getHackerPosInStudentArray(sys, hackerID);
        sys->hackers[hackersNum] = sys->myStudents[hackerPos];

        //increament the hackers number
        hackersNum++;

        //get the backslash n
        currChar = fgetc(hackers);
        //allocate desired courses array
        sys->hackers[hackersNum]->desiredCourses = malloc(sizeof(int) * 1);
        if (sys->hackers[hackersNum]->desiredCourses == NULL) {
            //need to free all the array
            free(sys->hackers);
            return;
        }
        //get the first course
        for(int i = 0; currChar != '\n'; i++){
            fscanf(hackers, "%d", sys->hackers[hackersNum]->desiredCourses[i]);
            //if the array is full, need to realloc
            if (i == sys->hackers[hackersNum]->desiredCoursesSize) {
                sys->hackers[hackersNum]->desiredCoursesSize++;
                sys->hackers[hackersNum]->desiredCourses = realloc(sys->hackers[hackersNum]->desiredCourses,
                                                                   sizeof(int) * sys->hackers[hackersNum]->desiredCoursesSize);
                if (sys->hackers[hackersNum]->desiredCourses == NULL) {
                    //need to free all the array
                    free(sys->hackers);
                    return;
                }
            }//end of realloc
        }


    }
}



int getHackerPosInStudentArray(EnrollmentSystem sys, char* hackerID) {
    for (int i = 0; i < sys->StudentArraySize; i++) {
        if (strcmp(sys->myStudents[i]->StudentID, hackerID) == 0) {
            return i;
        }
    }
    return -1;
}

















EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    bool isEOF = 0;
    while (!isEOF)
    {
        char* currName = (char*) malloc(ID_SIZE);
        int currCourseNum = 0;
        bool isCourseName = true;
        while (fscanf(queues, "%[^\n] ", currName))
        {
            if (isCourseName)
            {
                isCourseName = false;
                sys->course.createCourse()
            }
            else

        }



    }
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{

}

int friendshipFuncHackerFile(student s1, student s2)
{
    int res = 0;
    if (s1->friendsId != NULL)
    {
        res = friendshipFuncHackerFile(s1, s2);
    }
    else if (s2->friendsId != NULL)
    {
        res = friendshipFuncHackerFile(s2, s1);
    }
    return res;
}

int friendshipFuncHackerFileHelper(student s1, student s2)
{
    for (int i = 0; s1->friendsId[i] != NULL; i++)
    {
        if (compFunc(s2, s1->friendsId[i])) // if s2 is friend.
        {
            return FRIEND;
        }
    }
    for (int i = 0; s1->enemyId[i] != NULL; i++)
    {
        if (compFunc(s2, s1->friendsId[i])) // if s2 is enemy.
        {
            return ENEMY;
        }
    }
    return NEITHER;
}


int friendshipFuncNameDist(student s1, student s2)
{
    int sum = 0;
    if (strlen(s1->name) >= strlen(s2->name)) // if first student name is longer.
    {
        sum += calcNameDiff(s1->name, s2->name);
    }
    else
    {
        sum += calcNameDiff(s2->name, s1->name);
    }
    if (strlen(s1->surName) >= strlen(s2->surName)) // if first student surname is longer.
    {
        sum += calcNameDiff(s1->surName, s2->surName);
    }
    else
    {
        sum += calcNameDiff(s2->surName, s1->surName);
    }
    return sum;
}

int calcNameDiff(char* name1, char* name2)
{
    int sum = 0;
    for (int i = 0; i < strlen(name1); i++)
    {
        int curr = name1[i];
        if (i < strlen(name2))
        {
            curr += name2[i];
        }
        sum += abs(curr);
    }
    return sum;
}

int friendshipFuncIDSubtract(student s1, student s2)
{
    int id1 = 0, id2 = 0;
    for (int i = 1; i < 9; i++) //for digits 1-8 in ID of s1 and s2.
    {
        id1 *= 10;
        id2 *= 10;
        id1 += s1->StudentID[i];
        id2 += s2->StudentID[i];
    }
    if (s1->StudentID[0] == '-') // if negative ID.
    {
        id1 *= -1;
    }
    else
    {
        id1 += NINTH_DIGIT * s1->StudentID[0];
    }
    if (s2->StudentID[0] == '-') // if negative ID.
    {
        id2 *= -1;
    }
    else
    {
        id2 += NINTH_DIGIT * s1->StudentID[0];
    }

    return abs(id1-id2); // returns abs of difference between IDs.
}

int compFunc(student s1, student s2)
{
    return (!strcmp(s1->StudentID, s2->StudentID)); // compare between student IDs.
}

