#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define NAME_LEN 50
#define GROUP_LEN 20
#define EXAMS_COUNT 5
#define EPS 1e-9

typedef struct {
    unsigned int id;
    char name[NAME_LEN];
    char surname[NAME_LEN];
    char group[GROUP_LEN];
    unsigned char exams[EXAMS_COUNT];
} Student;

typedef enum {
    OK = 0,
    ERR_FILE_OPEN,
    ERR_MEMORY,
    ERR_INVALID_INPUT,
    ERR_NOT_FOUND,
    ERR_INVALID_COMMAND,
    ERR_INVALID_ID,
    ERR_EMPTY_LIST
} StatusCode;

typedef enum {
    COMPARE_LT = -1,
    COMPARE_EQ = 0,
    COMPARE_GT = 1
} CompareResult;

StatusCode readStudentsFromFile(const char *filename, Student **students, int *count);
StatusCode findStudentById(const Student *students, int count, unsigned int id, Student *result);
StatusCode findStudentsByField(const Student *students, int count,
                               const char *field, int fieldType,
                               Student **results, int *resultCount);
StatusCode sortStudents(Student *students, int count,
                        int (*comparator)(const void*, const void*));

int compareById(const void *a, const void *b);
int compareBySurname(const void *a, const void *b);
int compareByName(const void *a, const void *b);
int compareByGroup(const void *a, const void *b);

double calculateAverage(const unsigned char *grades, int count);
StatusCode writeStudentToTraceFile(const Student *student, const char *filename);
StatusCode writeTopStudentsToTraceFile(const Student *students, int count,
                                       double threshold, const char *filename);

int isValidName(const char *str);
int isValidId(unsigned int id);
int isValidGrade(unsigned char grade);

#endif