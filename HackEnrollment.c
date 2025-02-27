#include "IsraeliQueue.h"
#include "HackEnrollment.h"

#include <string.h>

#define ID_SIZE 10
#define NAME_START_SIZE 20

#define FRIEND 20
#define  ENEMY (-20)
#define NEITHER 0

#define COURSE_SUCCESS_TH 2

enum {SPACE, NEXT_LINE, END_OF_FILE};


/*
 * createEnrollment functions:
 */

EnrollmentSystem constructEnrollmentSystem();

EnrollmentError createStudentArrayOfEnrollmentSystem(EnrollmentSystem system, FILE* students);
Student createNewStudent();
EnrollmentError updateStudentParameters(Student newStudent, FILE* studentsFile);

EnrollmentError createCoursesArrayOfSystem(EnrollmentSystem system, FILE* courses);
Course createNewCourse();

EnrollmentError createHackersArrayOfSystem(EnrollmentSystem system, FILE* hackers);
EnrollmentError updateHackerParams(Student hacker, FILE* hackers);

EnrollmentError createAnIDArray(char*** IDArrayPtr, FILE* hackers);
int getHackerPositionInStudentArray(EnrollmentSystem system, char* hackerID);

/*
 * readEnrollment functions:
 */

Course getCourse(Course* courses, int numOfCourses, int courseNum);
Student getStudent(Student* students, int studentArraySize, char* currStudentID);
void removeCapLettersFromNames(Student* students, int studentArraySize);
EnrollmentError addFriendshipFunctions(IsraeliQueue queue);

/*
 * hackEnrollment functions:
 */

int isInCourse(Course course2Check, Student student2Find);
bool checkSatisfiedHacker(int countSuccessCourses, Student hacker);
void printFailedHacker(FILE* out, Student hacker2Print);
void printCourses2File(EnrollmentSystem system, FILE* out);
void printCourse(Course course2Print, FILE* out);

/*
 * friendship functions:
 */

int friendshipFuncHackerFile(Student student1, Student student2);
int friendshipFuncHackerFileHelper(Student student1, Student student2);

int friendshipFuncNameDist(Student student1, Student student2);
int calcNameDiff(char* name1, char* name2);

int friendshipFuncIDSubtract(Student student1, Student student2);

int compFunc(Student student1, Student student2);

/*
 * destroyEnrollment functions:
 */

EnrollmentError destroyStudentArrayContent(Student* studentArray, int size);
EnrollmentError destroyStudent(Student student);

EnrollmentError destroyCourseArrayContent(Course* courseArray, int size);
EnrollmentError destroyCourse(Course course);

EnrollmentError destroyStringsArray(char** stringArray);

EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem system);
EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem system);


/*
 * helper functions:
 */

int getIDFromFile(char studentID[ID_SIZE], FILE* file2Read);
EnrollmentError getNameFromFile(char** name, FILE* file2Read);

/*
 * Code:
 */


/*
 * createEnrollment segment:
 *
 * EnrollmentSystem constructEnrollmentSystem();
 * EnrollmentError createStudentArrayOfEnrollmentSystem(EnrollmentSystem system, FILE* students);
 * Student createNewStudent();
 * EnrollmentError updateStudentParameters(Student newStudent, FILE* studentsFile, EnrollmentSystem system);
 * EnrollmentError createCoursesArrayOfSystem(EnrollmentSystem system, FILE* courses);
 * Course createNewCourse();
 * EnrollmentError createHackersArrayOfSystem(EnrollmentSystem system, FILE* hackers);
 * EnrollmentError updateHackerParams(Student hacker, FILE* hackers);
 * EnrollmentError createAnIDArray(char*** IDArrayPtr, FILE* hackers);
 * int getHackerPositionInStudentArray(EnrollmentSystem system, char* hackerID);
 */

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    // checks for bad parameters.
    if(students == NULL || courses == NULL || hackers == NULL)
    {
        return NULL;
    }

    EnrollmentSystem system = constructEnrollmentSystem();
    if (system == NULL)
    {
        return NULL;
    }

    // add students from file to array.
    int result = createStudentArrayOfEnrollmentSystem(system, students);
    if(result != SUCCESS)
    {
        destroyEnrollmentSystemArrays(system);
        return NULL;
    }

    // add courses from file to array.
    result = createCoursesArrayOfSystem(system, courses);
    if(result != SUCCESS)
    {
        if (result == ALLOC_FAILED)
        {
            destroyStudentArrayContent(system->myStudents, system->StudentArraySize);
        }
        destroyEnrollmentSystemArrays(system);
        return NULL;
    }

    // add hackers from file to array.
    result = createHackersArrayOfSystem(system, hackers);
    if (result != SUCCESS)
    {
        destroyEnrollmentSystem(system);
        return NULL;
    }

    return system;
}

EnrollmentSystem constructEnrollmentSystem()
{
    EnrollmentSystem system = malloc(sizeof(struct EnrollmentSystem_t));
    if(system == NULL) // if memory allocation failed.
    {
        return NULL;
    }
    system->StudentArraySize = 1;
    system->courseArraySize = 1;
    system->hackersArraySize = 1;

    // creates students array.
    system->myStudents = malloc(sizeof(Student));
    if(system->myStudents == NULL)
    {
        free(system);
        return NULL;
    }

    // creates courses array.
    system->courses = malloc(sizeof(Course));
    if(system->courses == NULL)
    {
        free(system->myStudents);
        free(system);
        return NULL;
    }

    // creates hackers array.
    system->hackers = malloc(sizeof(Student));
    if(system->hackers == NULL)
    {
        free(system->myStudents);
        free(system->courses);
        free(system);
        return NULL;
    }

    return system;
}

EnrollmentError createStudentArrayOfEnrollmentSystem(EnrollmentSystem system, FILE* studentsFile)
{
    // if one of the parameters is bad.
    if (system == NULL || studentsFile == NULL)
    {
        return BAD_PARAM;
    }

    int studentsNum = 0;
    Student* tempStudent = NULL;
    char firstChar = (char)fgetc(studentsFile);

    // while file not ended.
    while(firstChar != EOF)
    {
        ungetc(firstChar, studentsFile);

        //check if need to reallocate the array of students
        if(studentsNum == system->StudentArraySize)
        {
            system->StudentArraySize++;
            tempStudent = realloc(system->myStudents, sizeof(Student) * system->StudentArraySize);
            if(tempStudent == NULL)
            {
                system->StudentArraySize--;
                destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
                return ALLOC_FAILED; //only need to free sys on the caller
            }
            system->myStudents = tempStudent;
        }// end of reallocate check and memory handling

        //create new student
        Student newStudent = createNewStudent();
        if(newStudent == NULL)
        {
            destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
            return ALLOC_FAILED;
        }

        //update the student parameters
        int result = updateStudentParameters(newStudent, studentsFile);
        if (result != SUCCESS)
        {
            destroyStudentArrayContent(system->myStudents, system->StudentArraySize-1);
            return ALLOC_FAILED;
        }

        //store the student in the array
        system->myStudents[studentsNum] = newStudent;
        studentsNum++;

        //skip to the next line
        for (int i = 0;firstChar != '\n'; i++)
        {
            firstChar = (char)fgetc(studentsFile);
        }

        firstChar = (char)fgetc(studentsFile);
    }
    return SUCCESS;
}

Student createNewStudent()
{
    Student newStudent = malloc(sizeof(struct Student_t));
    if(newStudent == NULL)
    {
        return NULL;
    }
    newStudent->StudentID = malloc(sizeof(char) * ID_SIZE);
    if(newStudent->StudentID == NULL)
    {
        free(newStudent);
        return NULL;
    }
    newStudent->name = malloc(sizeof(char) * NAME_START_SIZE);
    if(newStudent->name == NULL)
    {
        free(newStudent->StudentID);
        free(newStudent);
        return NULL;
    }
    newStudent->surName = malloc(sizeof(char) * NAME_START_SIZE);
    if(newStudent->surName == NULL)
    {
        free(newStudent->StudentID);
        free(newStudent->name);
        free(newStudent);
        return NULL;
    }
    newStudent->desiredCourses = NULL;
    newStudent->friendsId = NULL;
    newStudent->enemiesId = NULL;
    return  newStudent;
}

EnrollmentError updateStudentParameters(Student newStudent, FILE* studentsFile)
{
    int result, discardInt = 0;
    double discardDouble = 0;

    //get the ID
    getIDFromFile(newStudent->StudentID, studentsFile);

    //discard the space, total credits, GPA and space at the end
    fscanf(studentsFile, " %d %lf ", &discardInt, &discardDouble);

    //get the student's name from the students file
    result = getNameFromFile(&newStudent->name, studentsFile);
    if (result == ALLOC_FAILED)
    {
        free(newStudent->StudentID);
        free(newStudent->name);
        free(newStudent);
        return ALLOC_FAILED;
    }

    //get the student's surname from the students file
    result = getNameFromFile(&newStudent->surName, studentsFile);
    if (result == ALLOC_FAILED)
    {
        destroyStudent(newStudent);
        return ALLOC_FAILED;
    }

    return SUCCESS;
}

EnrollmentError createCoursesArrayOfSystem(EnrollmentSystem system, FILE* courses)
{
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
        }// end of reallocate check and memory handling

        //create new course and assign the values to it
        newCourse = createNewCourse();
        if(newCourse == NULL)
        {
            return ALLOC_FAILED;
        }
        //assigning the course parameters
        newCourse->courseNumber = courseNumber;
        newCourse->size = courseSize;

        system->courses[courseAmount] = newCourse;
        courseAmount++;
    }
    return SUCCESS;
}

Course createNewCourse()
{
    Course newCourse = malloc(sizeof(struct Course_t));

    if(newCourse == NULL)
    {
        return NULL;
    }

    newCourse->courseNumber = 0;
    newCourse->size = 0;

    FriendshipFunction* friendshipFunctionArr = (FriendshipFunction*)malloc(sizeof(*friendshipFunctionArr));
    if(friendshipFunctionArr == NULL)
    {
        free(newCourse);
        return NULL;
    }
    //null terminate the array
    friendshipFunctionArr[0] = NULL;

    //create the queue for the course
    newCourse->queue = IsraeliQueueCreate(friendshipFunctionArr, (int (*)(void*, void*))compFunc, 20, 0);
    free(friendshipFunctionArr);
    if(newCourse->queue == NULL)
    {
        free(newCourse);
        return NULL;
    }
    return newCourse;
}

EnrollmentError createHackersArrayOfSystem(EnrollmentSystem system, FILE* hackers)
{
    int hackerPos, errorResult, hackersNum = 0;
    Student hacker = NULL;
    Student* tempStudentArray = NULL;
    char hackerID[10];

    char firstChar = fgetc(hackers);
    ungetc(firstChar, hackers);

    //while file is not empty
    while (firstChar != EOF)
    {
        //if the array is full, need to reallocate
        if (hackersNum == system->hackersArraySize)
        {
            system->hackersArraySize++;
            tempStudentArray = realloc(system->hackers, sizeof(Student) * system->hackersArraySize);
            if (tempStudentArray == NULL)
            {
                system->hackersArraySize--;
                return ALLOC_FAILED; //need to free all sys arrays content and arrays on caller
            }
            system->hackers = tempStudentArray;
        }// end of reallocate check and memory handling


        //get an hacker ID from the hackers file
        getIDFromFile(hackerID, hackers);

        //get the hacker position in the student array
        hackerPos = getHackerPositionInStudentArray(system, hackerID);
        hacker = system->myStudents[hackerPos];

        //parse all of the hacker information from the file. (courses, friends, enemies)
        errorResult = updateHackerParams(hacker, hackers);
        if(errorResult == ALLOC_FAILED)
        {

            return ALLOC_FAILED; //need to free all sys arrays content and arrays on caller
        }

        //get the backslash n
        firstChar = fgetc(hackers);
        ungetc(firstChar, hackers);

        //store the hacker in the hackers array and increment the hackers number
        system->hackers[hackersNum] = system->myStudents[hackerPos];
        hackersNum++;
    }
    return SUCCESS;
}

EnrollmentError updateHackerParams(Student hacker, FILE* hackers)
{
    char currChar;
    int* tempDesiredCoursesPtr = NULL;
    int errorResult, numOfCourses = 0;

    //allocate desired courses array
    hacker->desiredCourses = malloc(sizeof(int) * 1);
    if (hacker->desiredCourses == NULL)
    {
        return ALLOC_FAILED;
    }

    hacker->desiredCourses[0] = 0; // zero for the end of the array

    //check if there are not any desired courses
    fgetc(hackers);
    currChar = fgetc(hackers);
    if (currChar != '\n')
    {
        ungetc(currChar, hackers);
    }

    //get the hacker's desired courses
    for(int i = 0; currChar != '\n'; i++)
    {

        //if the array is full, need to reallocate
        if (hacker->desiredCourses[i] == 0)
        {
            numOfCourses++;
            tempDesiredCoursesPtr = realloc(hacker->desiredCourses,sizeof(int) * (numOfCourses+1));
            if (tempDesiredCoursesPtr == NULL)
            {
                return ALLOC_FAILED;
            }
            hacker->desiredCourses = tempDesiredCoursesPtr;
        }// end of reallocate check and memory handling


        hacker->desiredCourses[numOfCourses] = 0;
        fscanf(hackers, "%d", &hacker->desiredCourses[i]);
        currChar = fgetc(hackers);
    }
    //currChar is backslash n

    //get friends ID
    hacker->friendsId = malloc(sizeof(char*) * 1);
    if (hacker->friendsId == NULL)
    {
        return ALLOC_FAILED;
    }
    // NULL terminate the friends ID array
    hacker->friendsId[0] = NULL;

    // check empty line, checking if there is a line after the desired courses line
    currChar = fgetc(hackers);
    if (currChar != '\n')
    {
        ungetc(currChar, hackers);
        errorResult = createAnIDArray(&(hacker->friendsId), hackers);
        if (errorResult == ALLOC_FAILED)
        {
            return ALLOC_FAILED;
        }
    }

    // get enemies ID
    hacker->enemiesId = malloc(sizeof(char*) * 1);
    if (hacker->enemiesId == NULL)
    {
        return ALLOC_FAILED;
    }

    // NULL terminate the enemies ID array
    hacker->enemiesId[0] = NULL;

    //check empty line, checking if there is a line after the friends line
    currChar = fgetc(hackers);
    if (currChar != '\n')
    {
        ungetc(currChar, hackers);
        errorResult = createAnIDArray(&(hacker->enemiesId), hackers);
        if (errorResult == ALLOC_FAILED)
        {
            return ALLOC_FAILED;
        }
    }

    return SUCCESS;
}

EnrollmentError createAnIDArray(char*** IDArrayPtr, FILE* hackers)
{
    char currChar = '0';
    int elementsNumber = 0;
    char** tempIDArray = NULL;
    for (int i = 0; currChar != '\n'; i++)
    {

        // if the array is full, need to reallocate
        if ((*IDArrayPtr)[i] == NULL)
        {
            elementsNumber++;
            tempIDArray = realloc((*IDArrayPtr), sizeof(char*) * (elementsNumber+1));
            if (tempIDArray == NULL)
            {
                return ALLOC_FAILED;
            }
            (*IDArrayPtr) = tempIDArray;
        }// end of reallocate check and memory handling


        (*IDArrayPtr)[i] = (char*)malloc(ID_SIZE);
        if ((*IDArrayPtr)[i] == NULL)
        {
            return ALLOC_FAILED;
        }

        getIDFromFile((*IDArrayPtr)[i], hackers);
        currChar = fgetc(hackers);
        (*IDArrayPtr)[elementsNumber] = NULL;
    }
    return SUCCESS;
}

int getHackerPositionInStudentArray(EnrollmentSystem system, char* hackerID)
{
    for (int i = 0; i < system->StudentArraySize; i++)
    {
        if (strcmp(system->myStudents[i]->StudentID, hackerID) == 0)
        {
            return i;
        }
    }
    return -1;
}


/*
 * readEnrollment segment:
 * EnrollmentSystem readEnrollment(EnrollmentSystem system, FILE* queues)
 * Course getCourse(Course* courses, int numOfCourses, int courseNum)
 * Student getStudent(Student* students, int studentArraySize, char* studentID)
 * void removeCapLettersFromNames(Student* students, int studentArraySize)
 * void addFriendshipFunctions(IsraeliQueue queue)
 */

EnrollmentSystem readEnrollment(EnrollmentSystem system, FILE* queues)
{
    if (system == NULL)
    {
        return NULL;
    }
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

    // while file is not empty.
    while (!isEOF)
    {
        int currCourseNum = 0, currEnd = SPACE;
        fscanf(queues, "%d ", &currCourseNum); // get course name.

        // gets course struct.
        Course currCourse = getCourse(system->courses, system->courseArraySize, currCourseNum);
        if (currCourse == NULL)
        {
            return NULL;
        }
        char currStudentID[ID_SIZE] = "";
        while (currEnd == SPACE) // while in current word.
        {
            currEnd = getIDFromFile(currStudentID, queues); // gets current student ID from file.
            fgetc(queues);
            Student currStudent = getStudent(system->myStudents, system->StudentArraySize,
                                             currStudentID);
            if (currStudent == NULL)
            {
                return NULL;
            }
            IsraeliQueueEnqueue(currCourse->queue, currStudent); // add student to course.
        }
        int result = addFriendshipFunctions(currCourse->queue);
        {
            if (result == ALLOC_FAILED)
            {
                return NULL;
            }
        }
        if (currEnd == NEXT_LINE) // if there is another line in file.
        {
            char lastChar = fgetc(queues);
            if (lastChar == EOF) // if file ended.
            {
                isEOF = 1;
            }
            else // if file not ended.
            {
                ungetc(lastChar, queues); // return last char to stream.
            }
            continue;
        }
        else // error in file read.
        {
            return NULL;
        }
    }
    return system;
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
        if (strcmp(students[i]->StudentID, studentID) == 0) // if current student is desired student.
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
                students[i]->name[j] -= 'A'-'a';
            }
        }
        for (int j = 0; j < strlen(students[i]->surName); j++)
        {
            if (students[i]->surName[j] >= 'A' && students[i]->surName[j] <= 'Z')
            {
                students[i]->surName[j] -= 'A'-'a';
            }
        }
    }
}

EnrollmentError addFriendshipFunctions(IsraeliQueue queue)
{

    if (IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncHackerFile)
        == ISRAELIQUEUE_ALLOC_FAILED)
    {
        return ALLOC_FAILED;
    }
    if (IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncNameDist)
        == ISRAELIQUEUE_ALLOC_FAILED)
    {
        return ALLOC_FAILED;
    }
    if (IsraeliQueueAddFriendshipMeasure(queue, (int (*)(void*, void*))friendshipFuncIDSubtract)
        == ISRAELIQUEUE_ALLOC_FAILED)
    {
        return ALLOC_FAILED;
    }
    return SUCCESS;
}

/*
 * hackEnrollment segment:
 * void hackEnrollment(EnrollmentSystem system, FILE* out)
 * int isInCourse(Course course2Check, Student student2Find)
 * bool checkSatisfiedHacker(int countSuccessCourses, Student hacker)
 * void printFailedHacker(FILE* out, Student hacker2Print)
 * void printCourses2File(EnrollmentSystem sys, FILE* out)
 * void printCourse(Course course2Print , FILE* out)
 */

void hackEnrollment(EnrollmentSystem system, FILE* out)
{
    int result;
    for (int i = 0; i < system->hackersArraySize; i++)
    {
        int countSuccessCourses = 0;
        for (int j = 0; system->hackers[i]->desiredCourses[j] != 0; j++)
        {
            Course currCourse = getCourse(system->courses, system->courseArraySize,
                                          system->hackers[i]->desiredCourses[j]);

            //hack the currCourse queue, the hacker's desired course.
            result = IsraeliQueueEnqueue(currCourse->queue, system->hackers[i]);
            if(result != ISRAELIQUEUE_SUCCESS)
            {
                return;
            }
            result = isInCourse(currCourse, system->hackers[i]);
            if (result == 1) // returns true if hacker in course.
            {
                countSuccessCourses++;
            }
            else if (result == -1) // The cloneQueue allocation failed.
            {
                return;
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

int isInCourse(Course course2Check, Student student2Find)
{
    IsraeliQueue clonedQueue = IsraeliQueueClone(course2Check->queue);
    if (clonedQueue == NULL)
    {
        return -1;
    }

    int queueSize = IsraeliQueueSize(clonedQueue);

    for (int count = 0; count < queueSize; count++) // for Nodes in queue.
    {
        Student currStudent = IsraeliQueueDequeue(clonedQueue);
        if (compFunc(currStudent, student2Find) && count < course2Check->size)
        {
            IsraeliQueueDestroy(clonedQueue);
            return true;
        }
    }
    IsraeliQueueDestroy(clonedQueue);
    return false;
}

bool checkSatisfiedHacker(int countSuccessCourses, Student hacker)
{
    if (countSuccessCourses == COURSE_SUCCESS_TH - 1 && hacker->desiredCourses[0] != 0
        && hacker->desiredCourses[1] == 0)
    {
        return true;
    }
    else if (countSuccessCourses >= COURSE_SUCCESS_TH)
    {
        return true;
    }
    else if(hacker->desiredCourses[0] == 0){
        return true;
    }
    return false;
}

void printFailedHacker(FILE* out, Student hacker2Print)
{
    char message2Print[100] = "Cannot satisfy constraints for ";
    strcat(message2Print, hacker2Print->StudentID);
    fprintf(out, "%s\n", message2Print);
}

void printCourses2File(EnrollmentSystem system, FILE* out)
{
    for (int i = 0; i < system->courseArraySize; i++)
    {
        if (IsraeliQueueSize(system->courses[i]->queue) != 0)
        {
            printCourse(system->courses[i], out);
        }
    }
}

void printCourse(Course course2Print , FILE* out)
{
    fprintf(out, "%d", course2Print->courseNumber);
    IsraeliQueue clonedQueue = IsraeliQueueClone(course2Print->queue);
    if (clonedQueue == NULL)
    {
        return;
    }
    Student currStudent = IsraeliQueueDequeue(clonedQueue);
    for (; currStudent != NULL; currStudent = IsraeliQueueDequeue(clonedQueue))
    {
        fprintf(out, " ");
        fprintf(out, currStudent->StudentID);
    }
    IsraeliQueueDestroy(clonedQueue);
    fprintf(out, "\n");
}

/*
 * friendship functions segment:
 * int friendshipFuncHackerFile(Student student1, Student student2):
    gets two students and checks which one is an hacker. returns the result calculated by the helper function.

 * int friendshipFuncHackerFileHelper(Student student1, Student student2):
    gets two students and assumes the first on is an hacker. looping through friends and enemies to find student2.
    returns 20 if found friend, -20 enemy and 0 if neither.

 * int friendshipFuncNameDist(Student student1, Student student2):
    checks to see which student has longer name and surname and gets sum from calcNameDiff. returns adding of sum.

 * int calcNameDiff(char* name1, char* name2):
    gets two names and assumes the first one is longer. calculates the difference between the names and returns it.

 * int friendshipFuncIDSubtract(Student student1, Student student2):
    gets two students and calculates the difference in their IDs and returns it.

 * int compFunc(Student student1, Student student2):
    gets two students and compares their ID's anr returns 1 if equal, 0 else.
 */

int friendshipFuncHackerFile(Student student1, Student student2)
{
    int result = 0;
    if (student1->friendsId != NULL) // if student1 is the hacker.
    {
        result = friendshipFuncHackerFileHelper(student1, student2);
    }
    else if (student2->friendsId != NULL) // if student2 is the hacker.
    {
        result = friendshipFuncHackerFileHelper(student2, student1);
    }
    return result;
}

int friendshipFuncHackerFileHelper(Student student1, Student student2)
{
    for (int i = 0; student1->friendsId[i] != NULL; i++) // for friends in friendsId.
    {
        if (strcmp(student2->StudentID, student1->friendsId[i]) == 0) // if student2 is friend.
        {
            return FRIEND;
        }
    }

    for (int i = 0; student1->enemiesId[i] != NULL; i++) // for enemy in enemiesId.
    {
        if (strcmp(student2->StudentID, student1->enemiesId[i]) == 0) // if student2 is enemy.
        {
            return ENEMY;
        }
    }
    return NEITHER; // not a friend nor enemy.
}

int friendshipFuncNameDist(Student student1, Student student2)
{
    int sum = 0;

    // calculate first name difference.
    if (strlen(student1->name) >= strlen(student2->name)) // if first student name is longer.
    {
        sum += calcNameDiff(student1->name, student2->name);
    }
    else
    {
        sum += calcNameDiff(student2->name, student1->name);
    }

    // calculate surname difference.
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
        int currDiff = name1[i];
        if (i < strlen(name2))
        {
            currDiff -= name2[i];
        }
        sum += abs(currDiff);
    }
    return sum;
}

int friendshipFuncIDSubtract(Student student1, Student student2)
{
    int id1 = 0, id2 = 0;
    for (int i = 0; i < ID_SIZE-1; i++) //for digits in ID of student1 and student2.
    {
        id1 *= 10;
        id2 *= 10;
        id1 += student1->StudentID[i];
        id2 += student2->StudentID[i];
    }
    return abs(id1-id2); // returns absolute value of difference between IDs.
}

int compFunc(Student student1, Student student2)
{
    return (!strcmp(student1->StudentID, student2->StudentID)); // compare between student IDs.
}

/*
 * Free memory functions:
 * EnrollmentError destroyStudentArrayContent(Student* arr, int size)
 * EnrollmentError destroyStudent(Student student)
 * EnrollmentError destroyCourseArrayContent(Course* coursesArray, int size)
 * EnrollmentError destroyCourse(Course course)
 * EnrollmentError destroyStringsArray(char** stringArray)
 * EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem system)
 * EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem system)
 * EnrollmentError destroyEnrollmentSystem(EnrollmentSystem system)
 */

EnrollmentError destroyStudentArrayContent(Student* studentArray, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroyStudent(studentArray[i]);
    }
    return SUCCESS;
}

EnrollmentError destroyStudent(Student student)
{
    free(student->StudentID);
    free(student->name);
    free(student->surName);
    if (student->friendsId != NULL)
    {
        destroyStringsArray(student->friendsId);
    }
    if (student->enemiesId != NULL)
    {
        destroyStringsArray(student->enemiesId);
    }
    if (student->desiredCourses != NULL)
    {
        free(student->desiredCourses);
    }
    free(student);
    return SUCCESS;
}

EnrollmentError destroyCourseArrayContent(Course* courseArray, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroyCourse(courseArray[i]);
    }
    return SUCCESS;
}

EnrollmentError destroyCourse(Course course)
{
    IsraeliQueueDestroy(course->queue);
    free(course);
    return SUCCESS;
}

EnrollmentError destroyStringsArray(char** stringArray)
{
    int size = 0;
    for (int i = 0; stringArray[i] != NULL; i++)
    {
        size++;
    }
    for (int i = 0; i < size; i++)
    {
        free(stringArray[i]);
    }
    free(stringArray);
    return SUCCESS;
}

EnrollmentError destroyEnrollmentSystemArraysContent(EnrollmentSystem system)
{
    destroyStudentArrayContent(system->myStudents, system->StudentArraySize);
    destroyCourseArrayContent(system->courses, system->courseArraySize);
    return SUCCESS;
}

EnrollmentError destroyEnrollmentSystemArrays(EnrollmentSystem system)
{
    free(system->myStudents);
    free(system->courses);
    free(system->hackers);
    free(system);
    return SUCCESS;
}

EnrollmentError destroyEnrollmentSystem(EnrollmentSystem system)
{
    destroyEnrollmentSystemArraysContent(system);
    destroyEnrollmentSystemArrays(system);
    return SUCCESS;
}


/*
 * helper functions:
 * EnrollmentError getNameFromFile(char** name, FILE* file2Read)
 * int getIDFromFile(char studentID[ID_SIZE], FILE* file2Read)
 * void updateCapLettersFlag(EnrollmentSystem system, bool flag)
 */

EnrollmentError getNameFromFile(char** name, FILE* file2Read)
{
    int nameLength = 0;
    int namesSize = NAME_START_SIZE;
    char currChar = (char)fgetc(file2Read);
    char* tempName = NULL;
    for(int i = 0; currChar != ' '; i++)
    {
        //if name is longer than the current size of the name array, increase the size of the array.
        if(nameLength == namesSize-1)
        {
            namesSize *= 2;
            tempName = realloc(*name, sizeof(char) * namesSize);
            if(tempName == NULL)
            {
                return ALLOC_FAILED;
            }
            *name = tempName;
        }// end of reallocate check and memory handling


        (*name)[i] = currChar;
        nameLength++;
        currChar = (char)fgetc(file2Read);
    }
    (*name)[nameLength] = '\0'; //null terminate the name
    return SUCCESS;
}

int getIDFromFile(char studentID[ID_SIZE], FILE* file2Read)
{
    for (int i = 0; i < ID_SIZE-1; i++)
    {
        studentID[i] = fgetc(file2Read);
    }
    studentID[ID_SIZE-1] = 0;
    char lastChar = fgetc(file2Read);
    ungetc(lastChar, file2Read); // move backwards one character in the file stream.
    if (lastChar == ' ')
    {
        return SPACE;
    }
    else if (lastChar == '\n')
    {
        return NEXT_LINE;
    }
    else
    {
        return END_OF_FILE;
    }
}

void updateCapLettersFlag(EnrollmentSystem system, bool flag)
{
    system->capLettersFlag = flag;
}
