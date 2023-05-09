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
}*Student;


typedef struct Course
{
    IsraeliQueue queue;
    int courseNumber;
    int size;
}*Course;


typedef struct EnrollmentSystem
{
    Student* myStudents;
    int StudentArraySize;

    Course* courses;
    int courseArraySize;

    Student* hackers;
    int hackersArraySize;

    bool capLettersFlag;
}*EnrollmentSystem;

Student createNewStudent();
Course createNewCourse();


EnrollmentError initStudentArrayOfEnrollmentSystem(EnrollmentSystem sys, FILE* students);
EnrollmentError initCoursesArrayOfSystem(EnrollmentSystem sys, FILE* courses);
EnrollmentError initHackersArrayOfSystem(EnrollmentSystem sys, FILE* hackers);

EnrollmentError InitHackerParams(Student hacker, FILE* hackers);

int getHackerPosInStudentArray(EnrollmentSystem sys, char* hackerID);

void addFriendshipFunctions(IsraeliQueue queue);

int getIDFromFile(char studentID[ID_SIZE], FILE* file2Read);
Course getCourse(Course* courses, int numOfCourses, int courseNum);
Student getStudent(Student* students, int studentArraySize, char* currStudentID);
EnrollmentError initAnIDArray(char** arr, FILE* hackers);


EnrollmentError destroyStudent(Student student);
EnrollmentError destroyStudentArrayContent(Student* arr, int size);

EnrollmentError destroyCourse(Course course);
EnrollmentError destroyCourseArrayContent(Course* arr, int size);

EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem sys);
EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem sys);

EnrollmentError destroyStringsArray(char** strArr);


void removeCapLettersFromNames(Student* students, int studentArraySize);


int compFunc(Student student1, Student student2);

int friendshipFuncHackerFile(Student student1, Student student2);
int friendshipFuncHackerFileHelper(Student student1, Student student2);

int friendshipFuncNameDist(Student student1, Student student2);
int calcNameDiff(char* name1, char* name2);

int friendshipFuncIDSubtract(Student student1, Student student2);

bool isInCourse(Course course2Check, Student student2Find);
bool checkSatisfiedHacker(int countSuccessCourses, Student hacker);
void printFailedHacker(FILE* out, Student hacker2Print);
void printCourses2File(EnrollmentSystem system, FILE* out);
void printCourse(Course course2Print, FILE* out);




EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    int errorResult;
    EnrollmentSystem system = malloc(sizeof(*system));
    if(system == NULL){
        return NULL;
    }
    system->StudentArraySize = 1;
    system->courseArraySize = 1;
    system->hackersArraySize = 1;
    system->myStudents = malloc(sizeof(Student));
    if(system->myStudents == NULL)
    {
        free(system);
        return NULL;
    }
    system->courses = malloc(sizeof(Course));
    if(system->courses == NULL)
    {
        free(system->myStudents);
        free(system);
        return NULL;
    }
    system->hackers = malloc(sizeof(Student));
    if(system->hackers == NULL)
    {
        free(system->myStudents);
        free(system->courses);
        free(system);
        return NULL;
    }
    //so far so good with no memories, now we need to initialize the arrays


    errorResult = initStudentArrayOfEnrollmentSystem(system, students);
    if(errorResult != SUCCESS)
    {
        destroyEnrollmentSystemArrays(system);
    }
    // so far so good with no memory leaks, now we need to initialize the courses array

    errorResult = initCoursesArrayOfSystem(system, courses);
    if(errorResult != SUCCESS)
    {
        if (errorResult == ALLOC_FAILED)
        {
            destroyStudentArrayContent(system->myStudents, system->StudentArraySize);
        }
        destroyEnrollmentSystemArrays(system);
    }
    // so far so good with no memory leaks, now we need to initialize the hackers array

    errorResult = initHackersArrayOfSystem(system, hackers);

    if (errorResult != SUCCESS){
        destroyEnrollmentSystemArraysContent(system);
        destroyEnrollmentSystemArrays(system);
        return NULL;
    }

    return system;

}

EnrollmentError initStudentArrayOfEnrollmentSystem(EnrollmentSystem system, FILE* students){
    if (system == NULL || students == NULL)
    {
        return BAD_PARAM; // need to free sys arrays on caller
    }

    int studentsNum = 0, discardInt, nameLength;
    double discardDouble = 0;
    int namesSize = 20;
    char currChar;
    char* tempName = NULL;
    Student* tempStudent = NULL;
    char firstChar = (char)fgetc(students);
    while(firstChar != EOF)
    {
        currChar = firstChar;

        //check if need to reallocate the array of students
        if(studentsNum == system->StudentArraySize)
        {
            system->StudentArraySize++;
            tempStudent = realloc(system->myStudents, sizeof(Student) * system->StudentArraySize);
            if(tempStudent == NULL){
                system->StudentArraySize--;
                destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
                return ALLOC_FAILED; //only need to free sys on the caller
            }
            system->myStudents = tempStudent;
        }// end of reallocate check and memory handling


        //create new student
        Student newStudent = createNewStudent();
        if(newStudent == NULL){
            destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
            return ALLOC_FAILED; //no need to free, only have to free sys on the caller
        }


        //get the student ID
        newStudent->StudentID[0] = currChar;
        for (int i = 1; i < 9; i++) {
            currChar = (char)fgetc(students);
            newStudent->StudentID[i] = currChar;

        }
        newStudent->StudentID[9] = '\0';

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
                tempName = realloc(newStudent->name, sizeof(char) * namesSize);
                if(tempName == NULL)
                {
                    destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
                    free(newStudent->StudentID);
                    free(newStudent->name);
                    free(newStudent);
                    return ALLOC_FAILED;
                }
                newStudent->name = tempName;
            }
            newStudent->name[i] = currChar;
            nameLength++;
            currChar = (char)fgetc(students);
        }

        newStudent->name[nameLength] = '\0'; //null terminate the name

        //get the surname
        currChar = '0';
        namesSize = 20;
        nameLength = 0;
        currChar = (char)fgetc(students);
        for(int i = 0 ; currChar != ' '; i++){
            if(nameLength == namesSize-1){
                namesSize *= 2;
                newStudent->surName = realloc(newStudent->surName, sizeof(char) * namesSize);
                if(newStudent->surName == NULL){
                    destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
                    destroyStudent(newStudent);
                    return ALLOC_FAILED; //no need to free, only have to free sys on the caller
                }
            }

            newStudent->surName[i] = currChar;
            nameLength++;
            currChar = (char)fgetc(students);
        }
        newStudent->surName[nameLength] = '\0';

        //store the student in the array
        system->myStudents[studentsNum] = newStudent;
        studentsNum++;

        for (int i = 0;firstChar != '\n'; i++) {
            firstChar = (char)fgetc(students);
        }

        firstChar = (char)fgetc(students);
    }
    return SUCCESS;
}

EnrollmentError initCoursesArrayOfSystem(EnrollmentSystem system, FILE* courses) {
    if (system == NULL || courses == NULL)
    {
        return BAD_PARAM ; // need to free sys arrays on caller
    }
    int courseSize, courseNumber = 0;
    int courseAmount = 0;
    Course* tempCourse = NULL;
    Course newCourse = NULL;


    while(fscanf(courses,"%d %d\n", &courseNumber, &courseSize)!= EOF)
    {
        //check if need to reallocate the array of courses
        if(courseAmount == system->courseArraySize)
        {
            system->courseArraySize++;
            tempCourse = realloc(system->courses, sizeof(Course) * system->courseArraySize);
            if(tempCourse == NULL)
            {
                system->courseArraySize--;
                destroyCourseArrayContent(system->courses, system->courseArraySize-1);
                return ALLOC_FAILED; // on caller destroy student array content and use destroy sys arrays
            }
            system->courses = tempCourse;
        }

        //create new course and assign the values to it
        newCourse = createNewCourse();
        if(newCourse == NULL)
        {
            return ALLOC_FAILED;
        }
        newCourse->courseNumber = courseNumber;
        newCourse->size = courseSize;

        system->courses[courseAmount] = newCourse;
        courseAmount++;
    }
    return SUCCESS;
}

EnrollmentError initHackersArrayOfSystem(EnrollmentSystem system, FILE* hackers) {
    int hackersNum, hackerPos, errorResult = 0;
    char currChar;
    Student hacker = NULL;
    Student* tempStudent = NULL;
    char hackerID[10];
    char firstChar = fgetc(hackers);
    fseek(hackers, -1L, SEEK_CUR); // move backwards one character if file stream.
    while (firstChar != EOF) {

        //if the array is full, need to reallocate
        if (hackersNum == system->hackersArraySize)
        {
            system->hackersArraySize++;
            tempStudent = realloc(system->hackers, sizeof(hackers) * system->hackersArraySize);
            if (tempStudent == NULL)
            {
                system->hackersArraySize--;
                return ALLOC_FAILED; //need to free all sys arrays content and arrays on caller
            }
            system->hackers = tempStudent;
        }//end of reallocate


        //get the hacker ID
        getIDFromFile(hackerID, hackers);

        //get the hacker position in the student array
        hackerPos = getHackerPosInStudentArray(system, hackerID);
        hacker = system->myStudents[hackerPos];

        //parse all of the hacker information from the file. (courses, friends, enemies)
        errorResult = InitHackerParams(hacker, hackers);
        if(errorResult == ALLOC_FAILED)
        {
            return ALLOC_FAILED; //need to free all sys arrays content and arrays on caller
        }
        //get the backslash n
        firstChar = fgetc(hackers);


        //store the hacker in the hackers array
        system->hackers[hackersNum] = system->myStudents[hackerPos];
        //increment the hackers number
        hackersNum++;
    }
    return SUCCESS;
}

Student createNewStudent(){
    Student newStudent = malloc(sizeof(*newStudent));
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

Course createNewCourse(){

    Course newCourse = malloc(sizeof(Course));

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

/*
 Create function segment:

 */

EnrollmentError InitHackerParams(Student hacker, FILE* hackers){
    char currChar = '0';
    int* tempDesiredCoursesPtr = NULL;
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

        //if the array is full, need to reallocate
        if (hacker->desiredCourses[i] == 0)
        {
            numOfCourses++;
            tempDesiredCoursesPtr = realloc(hacker->desiredCourses,sizeof(int) * (numOfCourses+1));
            if (tempDesiredCoursesPtr == NULL) {
                return ALLOC_FAILED;
            }
            hacker->desiredCourses = tempDesiredCoursesPtr;
        }//end of reallocate


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
    for (int i = 0; currChar != '\n'; i++)
    {

        //if the array is full, need to reallocate
        if (arr[i] == NULL)
        {
            elementsNumber++;
            temp = realloc(arr, sizeof(int) * (elementsNumber+1));
            if (arr == NULL) {
                return ALLOC_FAILED;
            }
            arr = temp;
        }//end of reallocate


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


int getHackerPosInStudentArray(EnrollmentSystem sys, char* hackerID)
{
    for (int i = 0; i < sys->StudentArraySize; i++)
    {
        if (strcmp(sys->myStudents[i]->StudentID, hackerID) == 0)
        {
            return i;
        }
    }
    return -1;
}

EnrollmentSystem readEnrollment(EnrollmentSystem system, FILE* queues)
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

    if (system->capLettersFlag)
    {
        removeCapLettersFromNames(system->myStudents, system->StudentArraySize);
    }

    while (!isEOF) // while file not ended.
    {
        int currCourseNum = 0, currEnd = SPACE;
        fscanf(queues, "%d", &currCourseNum); // get course name.

        // gets course struct.
        Course currCourse = getCourse(system->courses, system->courseArraySize, currCourseNum);
        if (currCourse == NULL)
        {
            // the course does not exist, error.
        }
        char currStudentID[ID_SIZE] = "";
        while (currEnd == SPACE) // while in current word.
        {
            currEnd = getIDFromFile(currStudentID, queues); // gets current student ID from file.
            fgetc(queues);
            Student currStudent = getStudent(system->myStudents, system->StudentArraySize, currStudentID);
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

Course getCourse(Course* courses, int numOfCourses, int courseNum)
{
    for (int i = 0; i < numOfCourses; i++) // for course in courses.
    {
        if (courses[i]->courseNumber == courseNum) // if current course is desired course.
        {
            return  courses[i];
        }
    }
    return NULL;
}

Student getStudent(Student* students, int studentArraySize, char* studentID)
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

void removeCapLettersFromNames(Student* students, int studentArraySize)
{
    for (int i = 0; i < studentArraySize; i++)
    {
        for (int j = 0; j < strlen(students[i]->name); j++)
        {
            if (students[i]->name[j] >= 'A' && students[i]->name[j] <= 'Z')
            {
                students[i]->name[j] += 'A'-'a';
            }
        }
        for (int j = 0; j < strlen(students[i]->surName); j++)
        {
            if (students[i]->surName[j] >= 'A' && students[i]->surName[j] <= 'Z')
            {
                students[i]->surName[j] += 'A'-'a';
            }
        }
    }
}

void addFriendshipFunctions(IsraeliQueue queue)
{
    IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncHackerFile);
    IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncIDSubtract);
    IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncNameDist);
    return;
}

int friendshipFuncHackerFile(Student student1, Student student2)
{
    int res = 0;
    if (student1->friendsId != NULL)
    {
        res = friendshipFuncHackerFileHelper(student1, student2);
    }
    else if (student2->friendsId != NULL)
    {
        res = friendshipFuncHackerFileHelper(student2, student1);
    }
    return res;
}

int friendshipFuncHackerFileHelper(Student student1, Student student2)
{
    for (int i = 0; student1->friendsId[i] != NULL; i++)
    {
        if (strcmp(student2->StudentID, student1->friendsId[i]) == 0) // if s2 is friend.
        {
            return FRIEND;
        }
    }
    for (int i = 0; student1->enemiesId[i] != NULL; i++)
    {
        if (strcmp(student2->StudentID, student1->friendsId[i]) == 0) // if s2 is enemy.
        {
            return ENEMY;
        }
    }
    return NEITHER;
}

int friendshipFuncNameDist(Student student1, Student student2)
{
    int sum = 0;
    if (strlen(student1->name) >= strlen(student2->name)) // if first student name is longer.
    {
        sum += calcNameDiff(student1->name, student2->name);
    }
    else
    {
        sum += calcNameDiff(student2->name, student1->name);
    }
    if (strlen(student1->surName) >= strlen(student2->surName)) // if first student surname is longer.
    {
        sum += calcNameDiff(student1->surName, student2->surName);
    }
    else
    {
        sum += calcNameDiff(student2->surName, student1->surName);
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
            curr -= name2[i];
        }
        sum += abs(curr);
    }
    return sum;
}

int friendshipFuncIDSubtract(Student student1, Student student2)
{
    int id1 = 0, id2 = 0;
    for (int i = 0; i < 9; i++) //for digits in ID of student1 and student2.
    {
        id1 *= 10;
        id2 *= 10;
        id1 += student1->StudentID[i];
        id2 += student2->StudentID[i];
    }
    return abs(id1-id2); // returns abs of difference between IDs.
}

int compFunc(Student student1, Student student2)
{
    return (!strcmp(student1->StudentID, student2->StudentID)); // compare between student IDs.
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




void hackEnrollment(EnrollmentSystem system, FILE* out)
{
    for (int i = 0; i < system->hackersArraySize; i++)
    {
        int countSuccessCourses = 0;
        for (int j = 0; system->hackers[i]->desiredCourses[j] != NULL; j++)
        {
            Course currCourse = getCourse(system->courses, system->courseArraySize, system->hackers[i]->desiredCourses[j]);
            IsraeliQueueEnqueue(currCourse->queue, system->hackers[i]);
            if (isInCourse(currCourse, system->hackers[i])) // returns true if hacker in course.
            {
                countSuccessCourses++;
            }
        }
        if (!checkSatisfiedHacker(countSuccessCourses, system->hackers[i]))
        {
            printFailedHacker(out, system->hackers[i]);
            return;
        }
    }
    printCourses2File(system, out);
}

bool isInCourse(Course course2Check, Student student2Find)
{
    IsraeliQueue clonedQueue = IsraeliQueueClone(course2Check->queue);
    for (int count = 0; count < IsraeliQueueSize(clonedQueue); count++) // for Nodes in queue.
    {
        Student currStudent = IsraeliQueueDequeue(clonedQueue);
        count++;
        if (compFunc(currStudent, student2Find) && count < course2Check->size)
        {
            return true;
        }
    }
    return false;
}

bool checkSatisfiedHacker(int countSuccessCourses, Student hacker)
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

void printFailedHacker(FILE* out, Student hacker2Print)
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

void printCourse(Course course2Print , FILE* out)
{
    fprintf(out, "%d", course2Print->courseNumber);
    IsraeliQueue clonedQueue = IsraeliQueueClone(course2Print->queue);
    Student currStudent = IsraeliQueueDequeue(clonedQueue);
    for (currStudent; currStudent != NULL; currStudent = IsraeliQueueDequeue(clonedQueue))
    {
        fprintf(out, " ");
        fprintf(out, "%s", currStudent->StudentID);
    }

    fprintf(out, "\n");
    return;
}

EnrollmentError destroyStudentArrayContent(Student* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroyStudent(arr[i]);
    }
    free(arr);
}

EnrollmentError destroyStudent(Student student){
    free(student->StudentID);
    free(student->name);
    free(student->surName);
    if (student->friendsId != NULL)
    {
        destroyStringsArray(student->friendsId);
    }
    if (student->desiredCourses != NULL)
    {
        destroyStringsArray(student->enemiesId);
    }
    free(student);
}

EnrollmentError destroyCourseArrayContent(Course* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroyCourse(arr[i]);
    }

}

EnrollmentError destroyCourse(Course course)
{
    IsraeliQueueDestroy(course->queue);
    free(course);
}

EnrollmentError destroyStringsArray(char** arr)
{
    int size = 0;
    for (int i = 0; arr[i] != NULL; i++)
    {
        size++;
    }
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem system)
{
    destroyStudentArrayContent(system->myStudents, system->StudentArraySize);
    destroyCourseArrayContent(system->courses, system->courseArraySize);
}

EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem system)
{
    free(system->myStudents);
    free(system->courses);
    free(system->hackers);
    free(system);
}

void updateCapLettersFlag(EnrollmentSystem system, bool flag)
{
    system->capLettersFlag = flag;
    return;
}
