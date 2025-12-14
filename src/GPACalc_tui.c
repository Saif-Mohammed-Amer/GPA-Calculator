#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char INTEGER_ARRAY[] = {'0','1','2','3','4','5','6','7','8','9'};


float sum(float arr[], int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}


int isElementInArray(char arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return 1; // Element found
        }
    }
    return 0; // Element not found
}


float getScore() {
    char str[4];
    scanf("%s", &str);

    int isGrade = 0;
    int arraySize = sizeof(str) / sizeof(str[0]);
    for(int i=0; i < arraySize; i++) {
        char character = str[i];
        if((character != 0) && !isElementInArray(INTEGER_ARRAY, 10, character)){
            isGrade = 1;
            break;
        }
    }

    if(!isGrade){
        int percentage = atoi(str);
        if(percentage >= 95){
            return 12.0;
        } else if(percentage >= 90) {
            return 11.5;
        } else if(percentage >= 85) {
            return 11.0;
        } else if(percentage >= 80) {
            return 10.0;
        } else if(percentage >= 75) {
            return 9.0;
        } else if(percentage >= 70) {
            return 8.0;
        } else if(percentage >= 65) {
            return 7.0;
        } else if(percentage >= 60) {
            return 6.0;
        } else if(percentage >= 56) {
            return 5.0;
        } else if(percentage >= 53) {
            return 4.0;
        } else if(percentage >= 50) {
            return 3.0;
        } else {
            return 0.0;
        }
    } else {
        if(strcmp(str, "A+") == 0) {
            return 12.0;
        } else if(strcmp(str, "A") == 0) {
            return 11.5;
        } else if(strcmp(str, "A-") == 0) {
            return 11.0;
        } else if(strcmp(str, "B+") == 0) {
            return 10.0;
        } else if(strcmp(str, "B") == 0) {
            return 9.0;
        } else if(strcmp(str, "B-") == 0) {
            return 8.0;
        } else if(strcmp(str, "C+") == 0) {
            return 7.0;
        } else if(strcmp(str, "C") == 0) {
            return 6.0;
        } else if(strcmp(str, "C-") == 0) {
            return 5.0;
        } else if(strcmp(str, "D+") == 0) {
            return 4.0;
        } else if(strcmp(str, "D") == 0) {
            return 3.0;
        } else {
            return 0.0;
        }
    }

    return 0.0;
}

float getTermGPA(int course_count) {
    float total_score = 0;
    int total_hours = 0;
    int hours;
    for(int i=1;i<=course_count;i++){
        printf("Enter Course %d Mark or Grade: ", i);
        total_score += getScore();
        printf("Enter Course %d Credit Hourse: ", i);
        scanf("%d", &hours);
        total_hours += hours;
        printf("\n");
    }
    return total_score / total_hours;
}

int main(){
    // char grade[3];
    // fgets(grade, sizeof(grade), stdin);
    // char str[] = "12345A";
    // int num;
    // num = atoi(str);
    // printf("THe user inputted '%d'", num);
    // float score = getScore();
    // printf("You got score: %.1f", score);

    int term_count;
    int course_count;

    printf("Enter Number of Terms: ");
    scanf("%d", &term_count);
    printf("\n");

    float gpa[term_count];

    for(int i=0;i<term_count;i++){
        printf("\nEnter How Many Courses Registered in Term %d: ", i+1);
        scanf("%d", &course_count);
        printf("Enter Term %d Courses:\n", i+1);
        gpa[i] = getTermGPA(course_count);
        printf("\n");
    }

    for(int i=0;i<term_count;i++){
        printf("\nSemester %d GPA: %.2f", i+1, gpa[i]);
    }
    float average_gpa = sum(gpa, term_count) / term_count;
    printf("\nOverall GPA: %.2f", average_gpa);
    if(average_gpa >= 3.6) {
        printf("\nExcellent");
    } else if(average_gpa >= 3.0) {
        printf("\nVery Good");
    } else if(average_gpa >= 2.6) {
        printf("\nGood");
    } else if(average_gpa >= 2.0) {
        printf("\nPass");
    } else {
        printf("\nFail.");
    }
}
