
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>

#include "validator.h"

// Function to validate decimal numbers
double getValidFloat(const char *message,
                     double min, double max)
{
    char input[100];
    char *end;
    double value;

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nInput ended. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        // Check for long input
        if (input[0] != '\0' &&
            input[strlen(input) - 1] != '\n')
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Invalid input! Try again.\n");
            continue;
        }

        errno = 0;
        value = strtod(input, &end);

        while (isspace((unsigned char)*end))
            end++;

        if (end == input || *end != '\0' ||
            errno == ERANGE || !isfinite(value))
        {
            printf("Invalid number! Enter a valid decimal.\n");
            continue;
        }

        if (value < min || value > max)
        {
            printf("Value must be between %.2f and %.2f.\n",
                   min, max);
            continue;
        }

        return value;
    }
}

// Function to validate integer numbers
int getValidInt(const char *message,
                int min, int max)
{
    char input[100];
    char *end;
    long value;

    while (1)
    {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\nInput ended. Exiting.\n");
            exit(EXIT_FAILURE);
        }

        if (input[0] != '\0' &&
            input[strlen(input) - 1] != '\n')
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Invalid input! Try again.\n");
            continue;
        }

        errno = 0;
        value = strtol(input, &end, 10);

        while (isspace((unsigned char)*end))
            end++;

        if (end == input || *end != '\0' ||
            errno == ERANGE ||
            value < INT_MIN || value > INT_MAX)
        {
            printf("Invalid input! Enter a whole number.\n");
            continue;
        }

        if (value < min || value > max)
        {
            printf("Enter a value between %d and %d.\n",
                   min, max);
            continue;
        }

        return (int)value;
    }
}
