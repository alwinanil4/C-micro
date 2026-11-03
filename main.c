#include <stdio.h>
#include <math.h>
#include "amortization.h"

/* ---------- Existing functions (unchanged) ---------- */

int validateInputs(float principal, float annualRate, int tenureMonths) {
    if (principal <= 0) {
        printf("Error: Loan amount must be greater than 0.\n");
        return 0;
    }
    if (annualRate <= 0 || annualRate > 100) {
        printf("Error: Interest rate must be between 0 and 100.\n");
        return 0;
    }
    if (tenureMonths <= 0) {
        printf("Error: Tenure must be greater than 0 months.\n");
        return 0;
    }
    return 1;
}

float calculateEMI(float principal, float annualRate, int tenureMonths) {
    float monthlyRate = annualRate / (12 * 100.0);
    float emi = (principal * monthlyRate * pow(1 + monthlyRate, tenureMonths)) /
                (pow(1 + monthlyRate, tenureMonths) - 1);
    return emi;
}

float calculateTotalInterest(float emi, int tenureMonths, float principal) {
    return (emi * tenureMonths) - principal;
}

float calculateTotalPayment(float emi, int tenureMonths) {
    return emi * tenureMonths;
}

void printDashboard(float principal, float rate, int tenureMonths, float emi, float totalInterest, float totalPayment) {
    printf("\n==================================================\n");
    printf("|              LOAN EMI DASHBOARD                |\n");
    printf("==================================================\n");
    printf("| %-22s : %20.2f |\n", "Loan Amount", principal);
    printf("| %-22s : %20.2f |\n", "Annual Interest Rate", rate);
    printf("| %-22s : %20d |\n", "Tenure (Months)", tenureMonths);
    printf("--------------------------------------------------\n");
    printf("| %-22s : %20.2f |\n", "Monthly EMI", emi);
    printf("| %-22s : %20.2f |\n", "Total Interest Payable", totalInterest);
    printf("| %-22s : %20.2f |\n", "Total Payment", totalPayment);
    printf("==================================================\n");
}

/* ---------- Helpers for main ---------- */

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Asks a yes/no question, returns 1 for y/Y, 0 otherwise */
static int askYesNo(const char *question) {
    char ch;
    printf("%s (y/n): ", question);
    if (scanf(" %c", &ch) != 1) {
        return 0;
    }
    clearInputBuffer();
    return (ch == 'y' || ch == 'Y');
}

/* ---------- Main ---------- */

int main(void) {
    do {
        float principal, rate;
        int tenure;

        printf("\nEnter loan amount: ");
        if (scanf("%f", &principal) != 1) { printf("Invalid input.\n"); clearInputBuffer(); continue; }
        printf("Enter annual interest rate (%%): ");
        if (scanf("%f", &rate) != 1) { printf("Invalid input.\n"); clearInputBuffer(); continue; }
        printf("Enter tenure (months): ");
        if (scanf("%d", &tenure) != 1) { printf("Invalid input.\n"); clearInputBuffer(); continue; }
        clearInputBuffer();

        if (!validateInputs(principal, rate, tenure)) {
            continue;
        }

        float emi = calculateEMI(principal, rate, tenure);
        float totalInterest = calculateTotalInterest(emi, tenure, principal);
        float totalPayment = calculateTotalPayment(emi, tenure);

        printDashboard(principal, rate, tenure, emi, totalInterest, totalPayment);

        /* Febine's part: amortization schedule + save to file */
        if (askYesNo("\nShow month-wise amortization table?")) {
            print_schedule(stdout, principal, rate, tenure, emi);
        }

        if (askYesNo("\nSave schedule to a text file?")) {
            if (save_schedule_to_file("emi_schedule.txt", principal, rate, tenure, emi) == 0) {
                printf("Saved to emi_schedule.txt\n");
            } else {
                printf("Error: could not write emi_schedule.txt\n");
            }
        }
    } while (askYesNo("\nCalculate another loan?"));

    printf("Goodbye!\n");
    return 0;
}