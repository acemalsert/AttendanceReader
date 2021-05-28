/*
 *
 * Author : Ahmet Cemal Sert
 * Title: CMPE252 Homework2
 *
 * */

/*
 * Import Statements
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Constants
 * */
#define MAX_ATTENDEE 500
#define MAX_CHAR 250

/*
 * Function declarations
 * */
void write_txt(FILE *file_ptr, char names_arr[][MAX_ATTENDEE], double duration_time_arr[], int num_of_attendees,
               int min_duration);

void split_name(char full_name[]);

int read_txt(FILE *in_ptr, char line[MAX_CHAR], char *tok_ptr,
             char names_arr[MAX_ATTENDEE][MAX_ATTENDEE],
             char emails_arr[MAX_ATTENDEE][MAX_ATTENDEE], char join_arr[MAX_ATTENDEE][MAX_ATTENDEE],
             double duration_time_arr[MAX_ATTENDEE],
             char leave_arr[MAX_ATTENDEE][MAX_ATTENDEE]);

/*
 * Function definitions
 * */

int read_txt(FILE *in_ptr, char line[MAX_CHAR], char *tok_ptr,
             char names_arr[MAX_ATTENDEE][MAX_ATTENDEE],
             char emails_arr[MAX_ATTENDEE][MAX_ATTENDEE], char join_arr[MAX_ATTENDEE][MAX_ATTENDEE],
             double duration_time_arr[MAX_ATTENDEE],
             char leave_arr[MAX_ATTENDEE][MAX_ATTENDEE]) {

    const char *delim1 = " ";
    const char *delim2 = ",";
    const char *delim3 = ",\n";

    if (in_ptr == NULL) {
        printf("File Not found!");
        return 0;
    }

    char temp_arr[MAX_CHAR];
    int counter;
    int number_of_attendees = 0;
    fgets(line, MAX_CHAR, in_ptr);
    while (fgets(line, MAX_CHAR, in_ptr) != NULL) {
        counter = 0;
        strcpy(temp_arr, line);
        tok_ptr = strtok(temp_arr, delim2);
        while (tok_ptr != NULL) {
            tok_ptr = strtok(NULL, delim2);
            counter++;
        }
        tok_ptr = strtok(line, delim2);
        if (counter == 4) {
            strcpy(names_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            strcpy(join_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            strcpy(leave_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            duration_time_arr[number_of_attendees] = atof(tok_ptr);
        } else if (counter == 5) {
            strcpy(names_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            strcpy(emails_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            strcpy(join_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            strcpy(leave_arr[number_of_attendees], tok_ptr);
            tok_ptr = strtok(NULL, delim3);
            duration_time_arr[number_of_attendees] = atof(tok_ptr);
        }
        number_of_attendees++;
    }

    for (int i = 0; i < number_of_attendees; i++) {
        split_name(names_arr[i]);
    }


    for (int i = 0; i < number_of_attendees; i++) {
        for (int j = i + 1; j < number_of_attendees; ++j) {
            if (strcasecmp(names_arr[i], names_arr[j]) == 0 && strcasecmp(names_arr[i], " ") != 0 &&
                duration_time_arr[j] != 0) {
                strcpy(names_arr[j], delim1);
                duration_time_arr[i] = duration_time_arr[i] + duration_time_arr[j];
                duration_time_arr[j] = 0;
            }
        }
    }
    return number_of_attendees;
}



void split_name(char full_name[]) {
    const char *delim = " ";
    char *tok_ptr;
    char first_name[MAX_CHAR];
    char last_name[MAX_CHAR];
    tok_ptr = strtok(full_name, delim);
    strcpy(first_name, tok_ptr);
    tok_ptr = strtok(NULL, delim);
    strcpy(last_name, tok_ptr);
    tok_ptr = strtok(NULL, delim);

    if (tok_ptr == NULL) {
        strcat(last_name, delim);
        strcat(last_name, first_name);
        strcpy(full_name, last_name);
    } else {
        strcat(first_name, delim);
        strcat(first_name, last_name);
        strcpy(last_name, tok_ptr);
        strcat(last_name, delim);
        strcat(last_name, first_name);
        strcpy(full_name, last_name);
    }
}

void write_txt(FILE *file_ptr, char names_arr[][MAX_ATTENDEE], double duration_time_arr[], int num_of_attendees,
               int min_duration) {
    const char *str = " ";
    const char *format_ptr = "%s %.2lf\n";
    for (int i = 0; i < num_of_attendees; i++) {
        if (strcasecmp(names_arr[i], str) != 0 && duration_time_arr[i] != 0) {
            if (duration_time_arr[i] >= min_duration) {
                fprintf(file_ptr, format_ptr, names_arr[i], duration_time_arr[i]);
            }
        }
    }
}


int main(int argc, char *argv[]) {

    // run with arguments <directory\input1.txt-directory\output1.txt-min_duration>
    char line[MAX_CHAR];
    char tok_ptr[MAX_CHAR];
    char names_arr[MAX_ATTENDEE][MAX_ATTENDEE];
    char emails_arr[MAX_ATTENDEE][MAX_ATTENDEE];
    char leave_arr[MAX_ATTENDEE][MAX_ATTENDEE];
    double duration_time_arr[MAX_ATTENDEE];

    FILE *in_ptr = NULL;
    FILE *out_ptr = NULL;

    int min_duration = atoi(argv[3]);

    in_ptr = fopen(argv[1], "r");
    out_ptr = fopen(argv[2], "w");

    int num_of_attendees = read_txt(in_ptr, line, tok_ptr, names_arr, emails_arr, emails_arr,
                                    duration_time_arr,
                                    leave_arr);

    write_txt(out_ptr, names_arr, duration_time_arr, num_of_attendees, min_duration);
    return 0;
}