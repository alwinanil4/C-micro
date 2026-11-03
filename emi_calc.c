#include <stdio.h>
#include <math.h>

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