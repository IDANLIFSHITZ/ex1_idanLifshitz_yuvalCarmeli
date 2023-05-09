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


/**Struct clarification:
 * struct Student_t: A struct representing a student.
 * */
typedef struct Student_t
{
    char* StudentID;
    char* name;
    char* surName;

    //hacker parameters, null terminated
    int* desiredCourses;
    char** friendsId;
    char** enemiesId;
}*Student;

/**Struct clarification:
 * struct Course_t: A struct representing a course.
 * */
typedef struct Course_t
{
    IsraeliQueue queue;
    int courseNumber;
    int size;
}*Course;

/**Struct clarification:
 * struct EnrollmentSystem: A struct representing the enrollment system.
 * * */
typedef struct EnrollmentSystem_t
{
    Student* myStudents;
    int StudentArraySize;

    Course* courses;
    int courseArraySize;

    Student* hackers;
    int hackersArraySize;

    bool capLettersFlag;
}*EnrollmentSystem;


/**
 * @param students: a file containing the students.
 * @format: <student id> <total credits> <GPA> <name> <surname> <City> <department>\n
 * @param courses: a file containing the courses.
 * @format: <course number> <size>
 * @param hackers: a file containing the hackers.
 * @format: <Student ID>\n<Course Numbers>*\n<Student ID>*\n<Student ID>*\n
 * @return if successful return enrollment system, a struct containing all the information needed
 * for the enrollment system. If failed return NULL.
 * @note: The function assumes the files are in the correct format.
 * This function creates an enrollment system from the files provided.
 */
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);



/**
 * @param system a struct containing all the information needed for the enrollment system.
 * @param flag a flag indicating whether to change the students names into lower case letters or not.
 * @return void.
 * this function changes the students names into lower case letters if flag is true.
 */
void updateCapLettersFlag(EnrollmentSystem system, bool flag);


/**
 * @param system a struct containing all the information needed for the enrollment system.
 * @param queues a file containing the queues of some courses.
 * @format: <course number> <student id> <student id> <student id> ...
 * @return if successful return enrollment system, a struct containing all the information needed
 * with initialized queues to given courses in queues FILE. If failed return NULL.
 * @note: The function assumes the file is in the correct format.
 * this function initializes the queues of the courses in the file.
 */
EnrollmentSystem readEnrollment(EnrollmentSystem system, FILE* queues);


/**
 * @param system a struct containing all the information needed for the enrollment system.
 * @param out a file to write the non-empty courses queues of the enrollment system to.
 * @return void.
 * @note: The function assumes sys is not NULL.
 * this function writes the non-empty courses queues of the enrollment system to the file.
 */
void hackEnrollment(EnrollmentSystem system, FILE* out);


/**
 * @param system a struct containing all the information needed for the enrollment system.
 * @return an ENUM indicating the success of the function.
 * this function destroys the whole enrollment system.
 */
EnrollmentError destroyEnrollmentSystem(EnrollmentSystem system);




#endif //HACKENROLLMENT_H
