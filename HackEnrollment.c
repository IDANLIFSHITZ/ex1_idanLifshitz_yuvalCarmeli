//
// Created by yuval on 5/2/2023.
//
#include "IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ID_SIZE 10

#define FRIEND 20
#define  ENEMY (-20)
#define NEITHER 0

#define COURSE_SUCCESS_TH 2

enum {SPACE, NEXT_LINE, END_OF_FILE};

typedef struct Student
{
    char* StudentID;
    char* name;
    char* surName;

    //hacker parameters, null terminated
    int* desiredCourses;
    char** friendsId;
    char** enemiesId;
}*student;


typedef struct course
{
    IsraeliQueue queue;
    int courseNumber;
    int size;
}*course;


typedef struct EnrollmentSystem
{
    student* myStudents;
    int StudentArraySize;

    course* courses;
    int courseArraySize;

    student* hackers;
    int hackersArraySize;
}*EnrollmentSystem;

student createNewStudent();
course createNewCourse();



EnrollmentError initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students);
EnrollmentError initCoursesArrayOfSystem(EnrollmentSystem sys, FILE* courses);
EnrollmentError initHackersArrayOfSystem(EnrollmentSystem sys, FILE* hackers);

EnrollmentError InitHackerParams(student hacker, FILE* hackers);

int getHackerPosInStudentArray(EnrollmentSystem sys, char* hackerID);

void addFriendshipFunctions(IsraeliQueue q);

int getIDFromFile(char studentID[ID_SIZE], FILE* file2Read);
course getCourse(course* courses, int numOfCourses, int courseNum);
student getStudent(student* students, int studentArraySize, char* currStudentID);
EnrollmentError initAnIDArray(char** arr, FILE* hackers);


EnrollmentError destroyStudent(student s);
EnrollmentError destroyStudentArrayContent(student* arr, int size);

EnrollmentError destroyCourse(course c);
EnrollmentError destroyCourseArrayContent(course* arr, int size);

EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem sys);
EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem sys);

EnrollmentError destroyStringsArray(char** arr);




int compFunc(student s1, student s2);

int friendshipFuncHackerFile(student s1, student s2);
int friendshipFuncHackerFileHelper(student s1, student s2);

int friendshipFuncNameDist(student s1, student s2);
int calcNameDiff(char* name1, char* name2);

int friendshipFuncIDSubtract(student s1, student s2);

bool isInCourse(course course2Check, student student2Find);
bool checkSatisfiedHacker(int countSuccessCourses, student hacker);
void printFailedHacker(FILE* out, student hacker2Print);
void printCourses2File(EnrollmentSystem sys, FILE* out);
void printCourse(course course2Print , FILE* out);




EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    int errorResult;
    EnrollmentSystem sys = malloc(sizeof(*sys));
    if(sys == NULL){
        return NULL;
    }
    sys->StudentArraySize = 1;
    sys->courseArraySize = 1;
    sys->hackersArraySize = 1;
    sys->myStudents = malloc(sizeof(student));
    if(sys->myStudents == NULL)
    {
        free(sys);
        return NULL;
    }
    sys->courses = malloc(sizeof(course));
    if(sys->courses == NULL)
    {
        free(sys->myStudents);
        free(sys);
        return NULL;
    }
    sys->hackers = malloc(sizeof(student));
    if(sys->hackers == NULL)
    {
        free(sys->myStudents);
        free(sys->courses);
        free(sys);
        return NULL;
    }
    //so far so good with no memories, now we need to initialize the arrays


    errorResult = initStudentArrayOfEnrollmentSystem(sys, students);
    if(errorResult != SUCCESS)
    {
        destroyEnrollmentSystemArrays(sys);
    }
    // so far so good with no memory leaks, now we need to initialize the courses array

    errorResult = initCoursesArrayOfSystem(sys, courses);
    if(errorResult != SUCCESS)
    {
        if (errorResult == ALLOC_FAILED)
        {
            destroyStudentArrayContent(sys->myStudents, sys->StudentArraySize);
        }
        destroyEnrollmentSystemArrays(sys);
    }
    // so far so good with no memory leaks, now we need to initialize the hackers array

    errorResult = initHackersArrayOfSystem(sys, hackers);

    if (errorResult != SUCCESS){
        destroyEnrollmentSystemArraysContent(sys);
        destroyEnrollmentSystemArrays(sys);
        return NULL;
    }

    return sys;

}


EnrollmentError initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students){
    if (sys == NULL || students == NULL)
    {
        return BAD_PARAM; // need to free sys arrays on caller
    }

    int studentsNum = 0, discardInt, nameLength = 0;
    double discardDouble = 0;
    int namesSize = 20;
    char currChar;
    char* tempName = NULL;
    student s = NULL;
    student* temp = NULL;
    char firstChar = (char)fgetc(students);
    while(firstChar != EOF)
    {
        currChar = firstChar;

        //check if need to reallocate the array of students
        if(studentsNum == sys->StudentArraySize)
        {
            sys->StudentArraySize++;
            temp = realloc(sys->myStudents, sizeof(student) * sys->StudentArraySize);
            if(temp == NULL){
                sys->StudentArraySize--;
                destroyStudentArrayContent(sys->myStudents, sys->StudentArraySize-1);
                return ALLOC_FAILED; //only need to free sys on the caller
            }
            sys->myStudents = temp;
        }// end of realloc check and memory handling


        //create new student
        s = createNewStudent();
        if(s == NULL){
            destroyStudentArrayContent(sys->myStudents, sys->StudentArraySize-1);
            return ALLOC_FAILED; //no need to free, only have to free sys on the caller
        }


        //get the student ID
        s->StudentID[0] = currChar;
        for (int i = 1; i < 9; i++) {
            currChar = (char)fgetc(students);
            s->StudentID[i] = currChar;

        }
        s->StudentID[9] = '\0';

        //discard the space, total credits, GPA and space at the end
        fscanf(students, " %d %lf ", &discardInt, &discardDouble);

        //get the name
        nameLength = 0;
        currChar = (char)fgetc(students);
        for(int i = 0; currChar != ' '; i++)
        {
            if(nameLength == namesSize-1)
            {
                namesSize *= 2;
                tempName = realloc(s->name, sizeof(char) * namesSize);
                if(tempName == NULL)
                {
                    destroyStudentArrayContent(sys->myStudents, sys->StudentArraySize-1);
                    free(s->StudentID);
                    free(s->name);
                    free(s);
                    return ALLOC_FAILED;
                }
                s->name = tempName;
            }
            s->name[i] = currChar;
            nameLength++;
            currChar = (char)fgetc(students);
        }

        s->name[nameLength] = '\0'; //null terminate the name

        //get the surname
        currChar = '0';
        namesSize = 20;
        nameLength = 0;
        currChar = (char)fgetc(students);
        for(int i = 0 ; currChar != ' '; i++){
            if(nameLength == namesSize-1){
                namesSize *= 2;
                s->surName = realloc(s->surName, sizeof(char) * namesSize);
                if(s->surName == NULL){
                    destroyStudentArrayContent(sys->myStudents, sys->StudentArraySize-1);
                    destroyStudent(s);
                    return ALLOC_FAILED; //no need to free, only have to free sys on the caller
                }
            }

            s->surName[i] = currChar;
            nameLength++;
            currChar = (char)fgetc(students);
        }
        s->surName[nameLength] = '\0';

        //store the student in the array
        sys->myStudents[studentsNum] = s;
        studentsNum++;

        for (int i = 0;firstChar != '\n'; i++) {
            firstChar = (char)fgetc(students);
        }

        firstChar = (char)fgetc(students);
    }
    return SUCCESS;
}


EnrollmentError initCoursesArrayOfSystem(EnrollmentSystem sys, FILE* courses) {
    if (sys == NULL || courses == NULL)
    {
        return BAD_PARAM ; // need to free sys arrays on caller
    }
    int courseSize, courseNumber = 0;
    int courseAmount = 0;
    course* temp = NULL;
    course newCourse = NULL;


    while(fscanf(courses,"%d %d\n", &courseNumber, &courseSize)!= EOF)
    {
        //check if need to reallocate the array of courses
        if(courseAmount == sys->courseArraySize)
        {
            sys->courseArraySize++;
            temp = realloc(sys->courses, sizeof(course) * sys->courseArraySize);
            if(temp == NULL)
            {
                sys->courseArraySize--;
                destroyCourseArrayContent(sys->courses, sys->courseArraySize-1);
                return ALLOC_FAILED; // on caller destroy student array content and use destroy sys arrays
            }
            sys->courses = temp;
        }

        //create new course and assign the values to it
        newCourse = createNewCourse();
        if(newCourse == NULL)
        {
            return ALLOC_FAILED;
        }
        newCourse->courseNumber = courseNumber;
        newCourse->size = courseSize;

        sys->courses[courseAmount] = newCourse;
        courseAmount++;
    }
    return SUCCESS;
}


EnrollmentError initHackersArrayOfSystem(EnrollmentSystem sys, FILE* hackers) {
    int hackersNum, hackerPos, errorResult = 0;
    char currChar;
    student hacker = NULL;
    student* temp = NULL;
    char hackerID[10];
    char firstChar = fgetc(hackers);
    fseek(hackers, -1L, SEEK_CUR); // move backwards one character if file stream.
    while (firstChar != EOF) {

        //if the array is full, need to realloc
        if (hackersNum == sys->hackersArraySize) {
            sys->hackersArraySize++;
            temp = realloc(sys->hackers, sizeof(hackers) * sys->hackersArraySize);
            if (temp == NULL) {
                sys->hackersArraySize--;
                return ALLOC_FAILED; //need to free all sys arrays content and arrays on caller
            }
            sys->hackers = temp;
        }//end of realloc


        //get the hacker ID
        getIDFromFile(hackerID, hackers);

        //get the hacker position in the student array
        hackerPos = getHackerPosInStudentArray(sys, hackerID);
        hacker = sys->myStudents[hackerPos];

        //parse all of the hacker information from the file. (courses, friends, enemies)
        errorResult = InitHackerParams(hacker, hackers);
        if(errorResult == ALLOC_FAILED)
        {
            return ALLOC_FAILED; //need to free all sys arrays content and arrays on caller
        }
        //get the backslash n
        firstChar = fgetc(hackers);


        //store the hacker in the hackers array
        sys->hackers[hackersNum] = sys->myStudents[hackerPos];
        //increment the hackers number
        hackersNum++;
    }
    return SUCCESS;
}

student createNewStudent(){
    student newStudent = malloc(sizeof(*newStudent));
    if(newStudent == NULL){
        return NULL;
    }
    newStudent->StudentID = malloc(sizeof(char) * ID_SIZE);
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
    newStudent->desiredCourses = NULL;
    newStudent->friendsId = NULL;
    newStudent->enemiesId = NULL;

}

course createNewCourse(){

    course newCourse = malloc(sizeof(course));

    if(newCourse == NULL){
        return NULL;
    }

    newCourse->courseNumber = 0;

    newCourse->size = 0;

    FriendshipFunction* friendshipFunctionArr = malloc(sizeof(FriendshipFunction));
    if(friendshipFunctionArr == NULL){
        free(newCourse);
        return NULL;
    }

    friendshipFunctionArr[0] = NULL;
    //need to fill compFunc

    newCourse->queue = IsraeliQueueCreate(friendshipFunctionArr, (int (*)(void*, void*))compFunc, 20, 0);
    if(newCourse->queue == NULL){
        free(friendshipFunctionArr);
        free(newCourse);
        return NULL;
    }
    return newCourse;
}


EnrollmentError InitHackerParams(student hacker, FILE* hackers){
    char currChar = '0';
    int* temp = NULL;
    int errorResult = 0;
    //allocate desired courses array
    hacker->desiredCourses = malloc(sizeof(int) * 1);
    if (hacker->desiredCourses == NULL) {
        return ALLOC_FAILED; //need to free
    }

    hacker->desiredCourses[0] = 0; // zero for the end of the array

    //get the first course
    int numOfCourses = 0;
    for(int i = 0; currChar != '\n'; i++){

        //if the array is full, need to realloc
        if (hacker->desiredCourses[i] == 0)
        {
            numOfCourses++;
            temp = realloc(hacker->desiredCourses,sizeof(int) * (numOfCourses+1));
            if (temp == NULL) {
                return ALLOC_FAILED;
            }
            hacker->desiredCourses = temp;
        }//end of realloc


        hacker->desiredCourses[numOfCourses] = 0;
        fscanf(hackers, "%d", &hacker->desiredCourses[i]);
        currChar = fgetc(hackers);
    }
    //currChar is backslash n

    //get friends ID
    hacker->friendsId = malloc(sizeof(char*) * 1);
    if (hacker->friendsId == NULL) {
        return ALLOC_FAILED;
    }

    hacker->friendsId[0] = NULL; // zero for the end of the array

    errorResult = initAnIDArray(hacker->friendsId, hackers);
    if (errorResult == ALLOC_FAILED) {
        return ALLOC_FAILED;
    }

    //get enemies ID
    hacker->enemiesId = malloc(sizeof(char*) * 1);
    if (hacker->enemiesId == NULL) {
        return ALLOC_FAILED;
    }

    hacker->enemiesId[0] = NULL; // zero for the end of the array

    errorResult = initAnIDArray(hacker->enemiesId, hackers);
    if (errorResult == ALLOC_FAILED) {
        return ALLOC_FAILED;
    }
    return SUCCESS;
}


EnrollmentError initAnIDArray(char** arr, FILE* hackers) {
    char currChar = '0';
    int elementsNumber = 0;
    char** temp = NULL;
    for (int i = 0; currChar != '\n'; i++) {

        //if the array is full, need to realloc
        if (arr[i] == NULL) {
            elementsNumber++;
            temp = realloc(arr, sizeof(int) * (elementsNumber+1));
            if (arr == NULL) {
                return ALLOC_FAILED;
            }
            arr = temp;
        }//end of realloc


        arr[i] = malloc(sizeof(char) * ID_SIZE);
        if (arr[i] == NULL)
        {
            return ALLOC_FAILED;
        }

        arr[elementsNumber] = NULL;
        getIDFromFile(arr[i], hackers);
        currChar = fgetc(hackers);
    }
    return SUCCESS;
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
    fgetc(queues);
    if (feof(queues) == 0) // if file is not empty.
    {
        rewind(queues); // rewind stream pointer to start of file.
    }
    else // file is empty.
    {
        return NULL;
    }
    while (!isEOF) // while file not ended.
    {
        int currCourseNum = 0, currEnd = SPACE;
        fscanf(queues, "%d", &currCourseNum); // get course name.

        // gets course struct.
        course currCourse = getCourse(sys->courses, sys->courseArraySize, currCourseNum);
        if (currCourse == NULL)
        {
            // the course does not exist, error.
        }
        char currStudentID[ID_SIZE] = "";
        while (currEnd == SPACE) // while in current word.
        {
            currEnd = getIDFromFile(currStudentID, queues); // gets current student ID from file.
            fgetc(queues);
            student currStudent = getStudent(sys->myStudents, sys->StudentArraySize, currStudentID);
            if (currStudent == NULL)
            {
             // the student does not exist, error.
            }
            IsraeliQueueEnqueue(currCourse->queue, currStudent); // add student to course.
        }
        addFriendshipFunctions(currCourse->queue);
        if (currEnd == NEXT_LINE) // if there is another line in file.
        {
            continue;
        }
        else if (currEnd == END_OF_FILE) // if file ended.
        {
            isEOF = true;
        }
        else // error in file read.
        {
            // error handle.
        }
    }
}

course getCourse(course* courses, int numOfCourses, int courseNum)
{
    course currCourse;
    for (int i = 0; i < numOfCourses; i++) // for course in courses.
    {
        if (courses[i]->courseNumber == courseNum) // if current course is desired course.
        {
            return  courses[i];
        }
    }
    return NULL;
}

student getStudent(student* students, int studentArraySize, char* studentID)
{
    for (int i = 0; i < studentArraySize; i++) // for student in students.
    {
        if (strcmp(students[i]->StudentID, studentID) == 0)
        {
            return students[i];
        }
    }
    return NULL;
}

void addFriendshipFunctions(IsraeliQueue queue)
{
    IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncHackerFile);
    IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncNameDist);
    IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncIDSubtract);
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    bool isFailed = false;
    for (int i = 0; i < sys->hackersArraySize && !isFailed; i++)
    {
        int countSuccessCourses = 0;
        for (int j = 0; sys->hackers[i]->desiredCourses[j] != NULL; j++)
        {
            course currCourse = getCourse(sys->courses, sys->courseArraySize, sys->hackers[i]->desiredCourses[j]);
            IsraeliQueueEnqueue(currCourse->queue, sys->hackers[i]);
            if (isInCourse(currCourse, sys->hackers[i])) // returns true if hacker in course.
            {
                countSuccessCourses++;
            }
        }
        if (!checkSatisfiedHacker(countSuccessCourses, sys->hackers[i]))
        {
            isFailed = true;
            printFailedHacker(out, sys->hackers[i]);
            return;
        }
    }
    printCourses2File(sys, out);
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
        if (strcmp(s2->StudentID, s1->friendsId[i]) == 0) // if s2 is friend.
        {
            return FRIEND;
        }
    }
    for (int i = 0; s1->enemiesId[i] != NULL; i++)
    {
        if (strcmp(s2->StudentID, s1->friendsId[i]) == 0) // if s2 is enemy.
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
    for (int i = 0; i < 9; i++) //for digits in ID of s1 and s2.
    {
        id1 *= 10;
        id2 *= 10;
        id1 += s1->StudentID[i];
        id2 += s2->StudentID[i];
    }
    return abs(id1-id2); // returns abs of difference between IDs.
}

int compFunc(student s1, student s2)
{
    return (!strcmp(s1->StudentID, s2->StudentID)); // compare between student IDs.
}

int getIDFromFile(char studentID[ID_SIZE], FILE* file2Read)
{
    for (int i = 0; i < ID_SIZE-1; i++)
    {
        studentID[i] = fgetc(file2Read);
    }
    studentID[ID_SIZE-1] = 0;
    char lastChar = fgetc(file2Read);
    fseek(file2Read, -1L, SEEK_CUR); // move backwards one character if file stream.
    if (lastChar == ' ')
    {
        return SPACE;
    }
    else if (lastChar == '\n')
    {
        return NEXT_LINE;
    }
    else if (lastChar == EOF)
    {
        return END_OF_FILE;
    }
    else
    {
        return ERROR;
    }
}

bool isInCourse(course course2Check, student student2Find)
{
    IsraeliQueue clonedQueue = IsraeliQueueClone(course2Check->queue);
    for (int count = 0; count < IsraeliQueueSize(clonedQueue); count++) // for Nodes in queue.
    {
        student currStudent = IsraeliQueueDequeue(clonedQueue);
        count++;
        if (compFunc(currStudent, student2Find) && count < course2Check->size)
        {
            return true;
        }
    }
    return false;
}

bool checkSatisfiedHacker(int countSuccessCourses, student hacker)
{
    if (countSuccessCourses == COURSE_SUCCESS_TH - 1 && hacker->desiredCourses[0] != NULL
    && hacker->desiredCourses[1] == NULL)
    {
        return true;
    }
    else if (countSuccessCourses >= COURSE_SUCCESS_TH)
    {
        return true;
    }
    return false;
}

void printFailedHacker(FILE* out, student hacker2Print)
{
    char message2Print[100] = "Cannot satisfy constraints for";
    strcpy(message2Print, hacker2Print->StudentID);
    fprintf(out, "%s", message2Print);
    return;
}

void printCourses2File(EnrollmentSystem sys, FILE* out)
{
    for (int i = 0; sys->courses[i] != NULL; i++)
    {
        if (sys->courses[i]->size != 0)
        {
            printCourse(sys->courses[i], out);
        }
    }

    return;
}

void printCourse(course course2Print , FILE* out)
{
    fprintf(out, "%d", course2Print->courseNumber);
    IsraeliQueue clonedQueue = IsraeliQueueClone(course2Print->queue);
    student currStudent = IsraeliQueueDequeue(course2Print->queue);
    for (currStudent; currStudent != NULL; currStudent = IsraeliQueueDequeue(course2Print->queue))
    {
        fprintf(out, " ");
        fprintf(out, "%s", currStudent->StudentID);
    }

    fprintf(out, "\n");
    return;
}

EnrollmentError destroyStudentArrayContent(student* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroyStudent(arr[i]);
    }
    free(arr);
}
EnrollmentError destroyStudent(student s){
    free(s->StudentID);
    free(s->name);
    free(s->surName);
    if (s->friendsId != NULL) {
        destroyStringsArray(s->friendsId);
    }
    if (s->desiredCourses != NULL) {
        destroyStringsArray(s->enemiesId);
    }
    free(s);
}

EnrollmentError destroyCourseArrayContent(course* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroyCourse(arr[i]);
    }

}

EnrollmentError destroyCourse(course c){
    IsraeliQueueDestroy(c->queue);
    free(c);
}

EnrollmentError destroyStringsArray(char** arr)
{
    int size = 0;
    for (int i = 0; arr[i] != NULL; i++) {
        size++;
    }
    for (int i = 0; i < size; i++){
        free(arr[i]);
    }
    free(arr);
}

EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem sys){
    destroyStudentArrayContent(sys->myStudents, sys->StudentArraySize);
    destroyCourseArrayContent(sys->courses, sys->courseArraySize);
}

EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem sys){
    free(sys->myStudents);
    free(sys->courses);
    free(sys->hackers);
    free(sys);
}

