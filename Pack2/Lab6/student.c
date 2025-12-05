#include "student.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


StatusCode readStudentsFromFile(const char *filename, Student **students, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) return ERR_FILE_OPEN;

    *count = 0;
    int capacity = 10;
    *students = (Student*)malloc(capacity * sizeof(Student));
    if (!*students) {
        fclose(file);
        return ERR_MEMORY;
    }

    Student temp;
    while (fscanf(file, "%u %49s %49s %19s", &temp.id,
                  temp.name, temp.surname, temp.group) == 4) {
        
        if (!isValidId(temp.id) || 
            !isValidName(temp.name) || 
            !isValidName(temp.surname)) {
            free(*students);
            fclose(file);
            return ERR_INVALID_INPUT;
        }

        for (int i = 0; i < EXAMS_COUNT; i++) {
            unsigned int grade;
            if (fscanf(file, "%u", &grade) != 1 || grade > 100) {
                free(*students);
                fclose(file);
                return ERR_INVALID_INPUT;
            }
            temp.exams[i] = (unsigned char)grade;
        }

        if (*count >= capacity) {
            capacity *= 2;
            Student *newStudents = (Student*)realloc(*students, capacity * sizeof(Student));
            if (!newStudents) {
                free(*students);
                fclose(file);
                return ERR_MEMORY;
            }
            *students = newStudents;
        }

        (*students)[(*count)++] = temp;
    }

    fclose(file);
    return OK;
}


StatusCode findStudentById(const Student *students, int count,
                           unsigned int id, Student *result) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            *result = students[i];
            return OK;
        }
    }
    return ERR_NOT_FOUND;
}


StatusCode findStudentsByField(const Student *students, int count,
                               const char *field, int fieldType,
                               Student **results, int *resultCount) {
    *resultCount = 0;
    *results = (Student*)malloc(count * sizeof(Student));
    if (!*results) return ERR_MEMORY;

    for (int i = 0; i < count; i++) {
        int match = 0;
        switch (fieldType) {
            case 1:
                match = (strcmp(students[i].surname, field) == 0);
                break;
            case 2:
                match = (strcmp(students[i].name, field) == 0);
                break;
            case 3:
                match = (strcmp(students[i].group, field) == 0);
                break;
            default:
                free(*results);
                return ERR_INVALID_INPUT;
        }
        if (match) {
            (*results)[(*resultCount)++] = students[i];
        }
    }

    if (*resultCount == 0) {
        free(*results);
        return ERR_NOT_FOUND;
    }

    *results = (Student*)realloc(*results, (*resultCount) * sizeof(Student));
    return OK;
}


StatusCode sortStudents(Student *students, int count,
                        int (*comparator)(const void*, const void*)) {
    if (count <= 0) return ERR_EMPTY_LIST;
    qsort(students, count, sizeof(Student), comparator);
    return OK;
}


int compareById(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return (sa->id > sb->id) - (sa->id < sb->id);
}


int compareBySurname(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->surname, sb->surname);
}


int compareByName(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->name, sb->name);
}


int compareByGroup(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->group, sb->group);
}


double calculateAverage(const unsigned char *grades, int count) {
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += grades[i];
    }
    return sum / count;
}


StatusCode writeStudentToTraceFile(const Student *student, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) return ERR_FILE_OPEN;

    double avg = calculateAverage(student->exams, EXAMS_COUNT);
    fprintf(file, "ID: %u, Name: %s, Surname: %s, Group: %s, Average: %.2f\n",
            student->id, student->name, student->surname,
            student->group, avg);
    fclose(file);
    return OK;
}


StatusCode writeTopStudentsToTraceFile(const Student *students, int count,
                                       double threshold, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) return ERR_FILE_OPEN;

    fprintf(file, "\nStudents with average above %.2f:\n", threshold);
    for (int i = 0; i < count; i++) {
        double avg = calculateAverage(students[i].exams, EXAMS_COUNT);
        if (avg - threshold > EPS) {
            fprintf(file, "%s %s (Avg: %.2f)\n",
                    students[i].surname, students[i].name, avg);
        }
    }
    fclose(file);
    return OK;
}


int isValidName(const char *str) {
    if (!str || strlen(str) == 0) return 0;
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && str[i] != '-') return 0;
    }
    return 1;
}

int isValidId(unsigned int id) {
    return id > 0;
}

int isValidGrade(unsigned char grade) {
    return grade <= 100;
}