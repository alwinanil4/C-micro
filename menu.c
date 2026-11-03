
#include <stdio.h>
#include "menu.h"

// ANSI color codes
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define RESET   "\033[0m"


#include <stdio.h>
#include "menu.h"
#include "validator.h"

#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define RESET   "\033[0m"

int displayMenu(void)
{
    printf("\n");
    printf(BLUE "====================================\n");
    printf("       LOAN EMI CALCULATOR\n");
    printf("====================================\n" RESET);

    printf(GREEN "1. Calculate EMI\n" RESET);
    printf("2. View EMI Table\n");
    printf("3. View Loan History\n");
    printf("4. Compare Loans\n");
    printf(RED "5. Exit\n" RESET);

    printf("------------------------------------\n");

    return getValidInt(
        YELLOW "Enter your choice (1-5): " RESET,
        1, 5
    );
}

