#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* =========================================================================
   STEP 1: BANK RULES STORED IN GLOBAL ARRAYS (INDEXED 1 TO 5)
   Students can easily adjust any bank policy parameters from this section.
   Index 0 is unused; indices 1 to 5 correspond to the respective loan types:
     1: Home Loan
     2: Car Loan
     3: Personal Loan
     4: Education Loan
     5: Business Loan
   ========================================================================= */

// Loan type names
const char *loanNames[6] = {
    "",
    "Home Loan",
    "Car Loan",
    "Personal Loan",
    "Education Loan",
    "Business Loan"
};

// Applicant age limits at application
const int minAge[6] = { 0, 21, 21, 21, 16, 25 };
const int maxAge[6] = { 0, 60, 65, 60, 35, 65 };

// Maximum applicant age allowed at loan maturity (age + tenure in years)
// Home loan allows up to 65 for salaried and 70 for self-employed
const int maxAgeAtLoanEndSalaried[6] = { 0, 65, 70, 65, 65, 70 };
const int maxAgeAtLoanEndSelf[6]     = { 0, 70, 70, 65, 65, 70 };

// Minimum credit score required
const int minCreditScore[6] = { 0, 700, 650, 750, 650, 700 };

// Maximum allowed FOIR (Fixed Obligation to Income Ratio) percentage
const float maxFoirPercent[6] = { 0.0f, 50.0f, 50.0f, 40.0f, 50.0f, 50.0f };

// Loan amount ranges (Rs.)
const double minLoanAmount[6] = { 0.0, 500000.0, 100000.0, 50000.0, 50000.0, 100000.0 };
const double maxLoanAmount[6] = { 0.0, 100000000.0, 5000000.0, 2500000.0, 15000000.0, 5000000.0 };

// Tenure limits (in months)
const int minTenureMonths[6] = { 0, 12, 12, 12, 12, 12 };
const int maxTenureMonths[6] = { 0, 360, 84, 60, 180, 120 };

// Annual interest rate hard limits (%)
const float minRate[6] = { 0.0f, 6.0f, 6.0f, 9.0f, 5.0f, 8.0f };
const float maxRate[6] = { 0.0f, 15.0f, 18.0f, 30.0f, 15.0f, 24.0f };

// Typical interest rate hints (%)
const float typicalRateLow[6]  = { 0.0f, 8.0f, 9.0f, 12.0f, 7.0f, 10.0f };
const float typicalRateHigh[6] = { 0.0f, 10.0f, 12.0f, 18.0f, 10.0f, 15.0f };

// Minimum monthly income required (Rs.)
const float minMonthlyIncome[6] = { 0.0f, 25000.0f, 20000.0f, 20000.0f, 15000.0f, 30000.0f };

/* =========================================================================
   HELPER FUNCTIONS FOR INPUT VALIDATION AND BUFFER CLEARING
   Ensures the program never crashes when invalid characters or ranges are given.
   ========================================================================= */

// Clears leftover newline and characters from stdin buffer
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

// Reads a valid integer between minVal and maxVal, re-asking until valid
int getValidInt(const char *prompt, int minVal, int maxVal) {
    int val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &val) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (val < minVal || val > maxVal) {
            printf("Out of range. Allowed range: %d to %d. Try again.\n", minVal, maxVal);
            continue;
        }
        return val;
    }
}

// Reads a valid float between minVal and maxVal, re-asking until valid
float getValidFloat(const char *prompt, float minVal, float maxVal) {
    float val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &val) != 1) {
            printf("Invalid input. Please enter a valid decimal number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (val < minVal || val > maxVal) {
            printf("Out of range. Allowed range: %.2f to %.2f. Try again.\n", minVal, maxVal);
            continue;
        }
        return val;
    }
}

// Reads a valid double between minVal and maxVal, re-asking until valid
double getValidDouble(const char *prompt, double minVal, double maxVal) {
    double val;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &val) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (val < minVal || val > maxVal) {
            printf("Out of range. Allowed range: %.2f to %.2f. Try again.\n", minVal, maxVal);
            continue;
        }
        return val;
    }
}

// Prompts user for a yes/no answer
int askYesNo(const char *question) {
    char ch;
    while (1) {
        printf("%s (y/n): ", question);
        if (scanf(" %c", &ch) != 1) {
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (ch == 'y' || ch == 'Y') return 1;
        if (ch == 'n' || ch == 'N') return 0;
        printf("Please answer with 'y' or 'n'.\n");
    }
}

/* =========================================================================
   STEP 3: CREDIT SCORE CATEGORIES
   Maps 300-900 into credit categories and handles 0 for education co-applicant.
   ========================================================================= */
void getCreditCategory(int score, char *category) {
    if (score == 0) {
        strcpy(category, "No History");
    } else if (score >= 300 && score <= 549) {
        strcpy(category, "Poor");
    } else if (score >= 550 && score <= 649) {
        strcpy(category, "Fair");
    } else if (score >= 650 && score <= 749) {
        strcpy(category, "Good");
    } else if (score >= 750 && score <= 799) {
        strcpy(category, "Very Good");
    } else if (score >= 800 && score <= 900) {
        strcpy(category, "Excellent");
    } else {
        strcpy(category, "Unknown");
    }
}

/* =========================================================================
   EMI AND LOAN CALCULATION FUNCTIONS
   ========================================================================= */

// Calculates Monthly EMI using standard formula: P * r * (1+r)^n / ((1+r)^n - 1)
float calculateEMI(float principal, float annualRate, int tenureMonths) {
    float monthlyRate = annualRate / (12.0f * 100.0f);
    float factor = powf(1.0f + monthlyRate, tenureMonths);
    float emi = (principal * monthlyRate * factor) / (factor - 1.0f);
    return emi;
}

// Calculates total interest payable over the entire tenure
float calculateTotalInterest(float emi, int tenureMonths, float principal) {
    return (emi * tenureMonths) - principal;
}

// Calculates total payment (principal + interest)
float calculateTotalPayment(float emi, int tenureMonths) {
    return emi * tenureMonths;
}

/* =========================================================================
   AMORTIZATION SCHEDULE GENERATION
   Outputs month-wise principal and interest payments to screen or report file.
   ========================================================================= */
void print_schedule(FILE *out, double principal, double annual_rate,
                    int months, double emi) {
    double monthly_rate = annual_rate / 12.0 / 100.0;
    double balance = principal;
    double total_interest = 0.0;
    double total_paid = 0.0;
    int m;

    fprintf(out, "\n%-6s %-12s %-12s %-12s %-14s\n",
            "Month", "EMI", "Principal", "Interest", "Balance");
    fprintf(out, "------------------------------------------------------------\n");

    for (m = 1; m <= months; m++) {
        double interest = balance * monthly_rate;
        double principal_paid = emi - interest;
        double payment = emi;

        // Last month adjustment to clear any fractional rounding remainder
        if (m == months) {
            principal_paid = balance;
            payment = principal_paid + interest;
        }

        balance -= principal_paid;
        if (balance < 0.005) {
            balance = 0.0;
        }

        total_interest += interest;
        total_paid += payment;

        fprintf(out, "%-6d %-12.2f %-12.2f %-12.2f %-14.2f\n",
                m, payment, principal_paid, interest, balance);
    }

    fprintf(out, "------------------------------------------------------------\n");
    fprintf(out, "Total interest payable : %.2f\n", total_interest);
    fprintf(out, "Total payment          : %.2f\n", total_paid);
}

/* =========================================================================
   STEP 6: DASHBOARD DISPLAY (PRESERVING 50-COLUMN BOX DESIGN)
   Only rendered when the loan application is fully APPROVED.
   ========================================================================= */
void printDashboard(int loanType, const char *eligibilityStatus,
                    float principal, float rate, const char *tenureStr,
                    float emi, float totalInterest, float totalPayment,
                    int creditScore, const char *creditCat,
                    float existingEmis, float foirPercent,
                    const char *coAppName, int courseDuration, int moratoriumMonths, float moratoriumInterest,
                    double assetPrice, double downPayment) {
    printf("\n==================================================\n");
    printf("|              LOAN EMI DASHBOARD                |\n");
    printf("==================================================\n");
    printf("| %-22s : %20s |\n", "Loan Type", loanNames[loanType]);
    printf("| %-22s : %20s |\n", "Eligibility Status", eligibilityStatus);
    printf("| %-22s : %20.2f |\n", "Loan Amount", principal);
    printf("| %-22s : %20.2f |\n", "Annual Interest Rate", rate);
    printf("| %-22s : %20s |\n", "Tenure", tenureStr);

    // Credit score row with category
    char scoreStr[40];
    sprintf(scoreStr, "%d (%s)", creditScore, creditCat);
    printf("| %-22s : %20s |\n", "Credit Score", scoreStr);

    // Existing EMIs and FOIR
    printf("| %-22s : %20.2f |\n", "Existing EMIs", existingEmis);
    char foirStr[30];
    sprintf(foirStr, "%.1f%%", foirPercent);
    printf("| %-22s : %20s |\n", "FOIR Percentage", foirStr);

    // Loan-specific rows
    if (loanType == 1) { // Home Loan
        printf("| %-22s : %20.2f |\n", "Property Value", assetPrice);
        printf("| %-22s : %20.2f |\n", "Down Payment", downPayment);
    } else if (loanType == 2) { // Car Loan
        printf("| %-22s : %20.2f |\n", "Car On-Road Price", assetPrice);
        printf("| %-22s : %20.2f |\n", "Down Payment", downPayment);
    } else if (loanType == 4) { // Education Loan
        printf("| %-22s : %20s |\n", "Co-Applicant Name", coAppName);
        char courseStr[30], moraStr[30];
        sprintf(courseStr, "%d Years", courseDuration);
        sprintf(moraStr, "%d Months", moratoriumMonths);
        printf("| %-22s : %20s |\n", "Course Duration", courseStr);
        printf("| %-22s : %20s |\n", "Moratorium Period", moraStr);
        printf("| %-22s : %20.2f |\n", "Moratorium Interest", moratoriumInterest);
    }

    printf("--------------------------------------------------\n");
    printf("| %-22s : %20.2f |\n", "Monthly EMI", emi);
    printf("| %-22s : %20.2f |\n", "Total Interest Payable", totalInterest);
    printf("| %-22s : %20.2f |\n", "Total Payment", totalPayment);
    printf("==================================================\n");
}

/* =========================================================================
   STEP 7: SAVE REPORT TO FILE (emi_report.txt)
   Saves full approval details or a clear rejection report.
   ========================================================================= */
int saveReportToFile(const char *filename, int isApproved, int loanType,
                     double principal, double rate, const char *tenureStr, int tenureMonths,
                     double emi, double totalInterest, double totalPayment,
                     int checkCount, char checkList[][200], double maxEligibleLoan) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }

    // Current date and time
    time_t now = time(NULL);
    char timeStr[64];
    strcpy(timeStr, ctime(&now));
    timeStr[strcspn(timeStr, "\r\n")] = '\0'; // Remove newline

    fprintf(fp, "============================================================\n");
    fprintf(fp, "               LOAN EMI EVALUATION REPORT                   \n");
    fprintf(fp, "============================================================\n");
    fprintf(fp, "Generated Date & Time : %s\n", timeStr);
    fprintf(fp, "Loan Type             : %s\n", loanNames[loanType]);
    fprintf(fp, "Requested Loan Amount : Rs. %.2f\n", principal);
    fprintf(fp, "Annual Interest Rate  : %.2f%%\n", rate);
    fprintf(fp, "Tenure                : %s\n", tenureStr);
    fprintf(fp, "Application Decision  : %s\n", isApproved ? "APPROVED" : "REJECTED");
    fprintf(fp, "------------------------------------------------------------\n");

    fprintf(fp, "\nELIGIBILITY CHECKS BREAKDOWN:\n");
    int i;
    for (i = 0; i < checkCount; i++) {
        fprintf(fp, "  %s\n", checkList[i]);
    }

    fprintf(fp, "\nMaximum loan amount possible for this tenure: Rs. %.2f\n", maxEligibleLoan);

    if (isApproved) {
        fprintf(fp, "\n------------------------------------------------------------\n");
        fprintf(fp, "FINANCIAL SUMMARY:\n");
        fprintf(fp, "  Monthly EMI            : Rs. %.2f\n", emi);
        fprintf(fp, "  Total Interest Payable : Rs. %.2f\n", totalInterest);
        fprintf(fp, "  Total Payment          : Rs. %.2f\n", totalPayment);
        fprintf(fp, "------------------------------------------------------------\n");
        fprintf(fp, "\nAMORTIZATION SCHEDULE:\n");
        print_schedule(fp, principal, rate, tenureMonths, emi);
    }

    fclose(fp);
    return 0;
}

/* =========================================================================
   MAIN EXECUTION FUNCTION
   ========================================================================= */

int main(void) {
    do {
        /* -------------------------------------------------------------
           SELECT LOAN TYPE
           ------------------------------------------------------------- */
        printf("\n  SELECT LOAN TYPE\n");
        printf("  ================\n");
        printf("  1. Home Loan        (Typical rate: 8%% - 10%% p.a.)\n");
        printf("  2. Car Loan         (Typical rate: 9%% - 12%% p.a.)\n");
        printf("  3. Personal Loan    (Typical rate: 12%% - 18%% p.a.)\n");
        printf("  4. Education Loan   (Typical rate: 7%% - 10%% p.a.)\n");
        printf("  5. Business Loan    (Typical rate: 10%% - 15%% p.a.)\n");

        int loanType = getValidInt("  Enter choice (1-5): ", 1, 5);

        /* -------------------------------------------------------------
           STEP 2: LOAN AMOUNT VALIDATION
           ------------------------------------------------------------- */
        char amountPrompt[120];
        sprintf(amountPrompt, "\nEnter loan amount (Allowed: Rs. %.0f to Rs. %.0f): ",
                minLoanAmount[loanType], maxLoanAmount[loanType]);
        double principal = getValidDouble(amountPrompt, minLoanAmount[loanType], maxLoanAmount[loanType]);

        /* -------------------------------------------------------------
           STEP 2: INTEREST RATE VALIDATION & TYPICAL RANGE WARNING
           ------------------------------------------------------------- */
        float rate;
        while (1) {
            printf("Enter annual interest rate (%%) [Allowed: %.1f%% - %.1f%%, Typical: %.1f%% - %.1f%%]: ",
                   minRate[loanType], maxRate[loanType], typicalRateLow[loanType], typicalRateHigh[loanType]);
            if (scanf("%f", &rate) != 1) {
                printf("Invalid input. Please enter a valid number.\n");
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();

            if (rate < minRate[loanType] || rate > maxRate[loanType]) {
                printf("Error: Interest rate must be between %.1f%% and %.1f%%. Try again.\n",
                       minRate[loanType], maxRate[loanType]);
                continue;
            }

            // Check if within hard limits but outside typical range -> Show yellow warning
            if (rate < typicalRateLow[loanType] || rate > typicalRateHigh[loanType]) {
                printf("\033[1;33mWarning: Entered rate (%.2f%%) is outside typical range (%.1f%% - %.1f%%).\033[0m\n",
                       rate, typicalRateLow[loanType], typicalRateHigh[loanType]);
            }
            break;
        }

        /* -------------------------------------------------------------
           STEP 0: FIX THE TENURE BUG (YEARS READ AS FLOAT)
           Formula: months = (int)(years * 12.0f + 0.5f)
           ------------------------------------------------------------- */
        int tenureChoice;
        float tenureYearsInput = 0.0f;
        int tenureMonthsInput = 0;
        int tenureMonths = 0;
        char tenureStr[60];

        while (1) {
            printf("\nEnter tenure:\n");
            printf("1. Years\n");
            printf("2. Months\n");
            tenureChoice = getValidInt("Choice (1-2): ", 1, 2);

            if (tenureChoice == 1) {
                printf("Enter tenure in years (e.g. 1.5, 5, 20): ");
                if (scanf("%f", &tenureYearsInput) != 1 || tenureYearsInput <= 0.0f) {
                    printf("Invalid input. Tenure must be positive.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();
                // Correct conversion from float years to integer months with rounding
                tenureMonths = (int)(tenureYearsInput * 12.0f + 0.5f);
            } else {
                printf("Enter tenure in months (e.g. 18, 60): ");
                if (scanf("%d", &tenureMonthsInput) != 1 || tenureMonthsInput <= 0) {
                    printf("Invalid input. Tenure must be positive.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();
                tenureMonths = tenureMonthsInput;
                tenureYearsInput = (float)tenureMonths / 12.0f;
            }

            // Validate against loan-specific tenure limits
            if (tenureMonths < minTenureMonths[loanType] || tenureMonths > maxTenureMonths[loanType]) {
                printf("Error: Tenure for %s must be between %d and %d months (%.1f to %.1f years).\n",
                       loanNames[loanType], minTenureMonths[loanType], maxTenureMonths[loanType],
                       minTenureMonths[loanType] / 12.0f, maxTenureMonths[loanType] / 12.0f);
                continue;
            }
            break;
        }

        // Format tenure string cleanly to fit within dashboard bounds
        if (tenureChoice == 1) {
            if ((int)tenureYearsInput == tenureYearsInput) {
                if (tenureYearsInput < 10.0f) {
                    sprintf(tenureStr, "%d Years (%d Months)", (int)tenureYearsInput, tenureMonths);
                } else {
                    sprintf(tenureStr, "%d Yrs (%d Months)", (int)tenureYearsInput, tenureMonths);
                }
            } else {
                sprintf(tenureStr, "%.1f Yrs (%d Months)", tenureYearsInput, tenureMonths);
            }
        } else {
            if (tenureMonths % 12 == 0) {
                if (tenureMonths / 12 < 10) {
                    sprintf(tenureStr, "%d Months (%d Years)", tenureMonths, tenureMonths / 12);
                } else {
                    sprintf(tenureStr, "%d Months (%d Yrs)", tenureMonths, tenureMonths / 12);
                }
            } else {
                sprintf(tenureStr, "%d Months (%.1f Yrs)", tenureMonths, (float)tenureMonths / 12.0f);
            }
        }

        /* -------------------------------------------------------------
           CALCULATE EMI FIRST BEFORE ELIGIBILITY CHECKS
           ------------------------------------------------------------- */
        float emi = calculateEMI((float)principal, rate, tenureMonths);
        float totalInterest = calculateTotalInterest(emi, tenureMonths, (float)principal);
        float totalPayment = calculateTotalPayment(emi, tenureMonths);

        /* -------------------------------------------------------------
           GATHER APPLICANT DETAILS ACCORDING TO LOAN TYPE
           ------------------------------------------------------------- */
        int applicantAge = 0;
        int employmentType = 1; // 1 = Salaried, 2 = Self Employed
        float workExperience = 0.0f;
        float monthlyIncome = 0.0f;
        float existingEmis = 0.0f;
        int creditScore = 0;
        char creditCat[30] = "";

        // Specific fields for certain loan types
        double assetPrice = 0.0;     // Property value or Car on-road price
        double downPayment = 0.0;
        char coAppName[60] = "";     // Education loan
        int coAppAge = 0;
        float coAppIncome = 0.0f;
        float coAppExistingEmis = 0.0f;
        int coAppCreditScore = 0;
        char coAppCreditCat[30] = "";
        int courseDuration = 0;
        int hasCollateral = 0;
        int moratoriumMonths = 0;
        float moratoriumInterest = 0.0f;

        printf("\n--- APPLICANT INFORMATION & ELIGIBILITY DETAILS ---\n");

        if (loanType == 4) {
            /* Education Loan */
            applicantAge = getValidInt("Enter student age (years): ", 1, 100);

            printf("Enter co-applicant (parent/guardian) name: ");
            if (fgets(coAppName, sizeof(coAppName), stdin) != NULL) {
                coAppName[strcspn(coAppName, "\r\n")] = '\0';
            }
            coAppAge = getValidInt("Enter co-applicant age (years): ", 1, 100);
            coAppIncome = getValidFloat("Enter co-applicant monthly income (Rs.): ", 0.0f, 100000000.0f);
            coAppExistingEmis = getValidFloat("Enter co-applicant existing monthly EMIs (Rs.): ", 0.0f, 100000000.0f);

            // Credit score: 0 allowed ONLY for education loan co-applicant
            while (1) {
                printf("Enter co-applicant credit score (300-900, or 0 if no credit history): ");
                if (scanf("%d", &coAppCreditScore) != 1) {
                    printf("Invalid input. Please enter a valid number.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();
                if (coAppCreditScore != 0 && (coAppCreditScore < 300 || coAppCreditScore > 900)) {
                    printf("Credit score must be between 300 and 900 (or 0 for no history). Try again.\n");
                    continue;
                }
                break;
            }
            getCreditCategory(coAppCreditScore, coAppCreditCat);

            courseDuration = getValidInt("Enter course duration in years (1 to 6): ", 1, 6);
            hasCollateral = askYesNo("Is collateral (property or fixed deposit) provided?");

            moratoriumMonths = (courseDuration * 12) + 12;
            moratoriumInterest = (float)(principal * (rate / 100.0f) * (moratoriumMonths / 12.0f));

            // Use co-applicant details for financial checks
            monthlyIncome = coAppIncome;
            existingEmis = coAppExistingEmis;
            creditScore = coAppCreditScore;
            strcpy(creditCat, coAppCreditCat);

        } else if (loanType == 5) {
            /* Business Loan */
            applicantAge = getValidInt("Enter applicant age (years): ", 1, 100);
            workExperience = getValidFloat("Enter years in business: ", 0.0f, 80.0f);
            monthlyIncome = getValidFloat("Enter monthly net business income / profit (Rs.): ", 0.0f, 100000000.0f);
            existingEmis = getValidFloat("Enter existing monthly EMIs (Rs.): ", 0.0f, 100000000.0f);

            while (1) {
                printf("Enter credit score (300-900): ");
                if (scanf("%d", &creditScore) != 1) {
                    printf("Invalid input. Please enter a valid number.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();
                if (creditScore < 300 || creditScore > 900) {
                    printf("Credit score must be between 300 and 900. Try again.\n");
                    continue;
                }
                break;
            }
            getCreditCategory(creditScore, creditCat);

        } else {
            /* Home Loan, Car Loan, Personal Loan */
            applicantAge = getValidInt("Enter applicant age (years): ", 1, 100);

            printf("Select employment type:\n  1. Salaried\n  2. Self Employed\n");
            employmentType = getValidInt("Choice (1-2): ", 1, 2);

            if (loanType == 3 && employmentType == 1) {
                workExperience = getValidFloat("Enter years in current job: ", 0.0f, 60.0f);
            } else {
                workExperience = getValidFloat("Enter total years of work experience: ", 0.0f, 60.0f);
            }

            if (loanType == 1) {
                assetPrice = getValidDouble("Enter property value (Rs.): ", 1.0, 1000000000.0);
                downPayment = assetPrice - principal;
            } else if (loanType == 2) {
                assetPrice = getValidDouble("Enter on-road price of the car (Rs.): ", 1.0, 100000000.0);
                downPayment = assetPrice - principal;
            }

            monthlyIncome = getValidFloat("Enter monthly income (Rs.): ", 0.0f, 100000000.0f);
            existingEmis = getValidFloat("Enter existing monthly EMIs (Rs.): ", 0.0f, 100000000.0f);

            while (1) {
                printf("Enter credit score (300-900): ");
                if (scanf("%d", &creditScore) != 1) {
                    printf("Invalid input. Please enter a valid number.\n");
                    clearInputBuffer();
                    continue;
                }
                clearInputBuffer();
                if (creditScore < 300 || creditScore > 900) {
                    printf("Credit score must be between 300 and 900. Try again.\n");
                    continue;
                }
                break;
            }
            getCreditCategory(creditScore, creditCat);
        }

        /* -------------------------------------------------------------
           STEP 4: EXECUTE ALL ELIGIBILITY CHECKS
           Prints [PASS] in green, [FAIL] in red, [PASS WITH NOTE] in yellow.
           ------------------------------------------------------------- */
        printf("\n==================================================\n");
        printf("              ELIGIBILITY VERIFICATION            \n");
        printf("==================================================\n");

        char checkResults[12][200];
        int checkCount = 0;
        int failCount = 0;
        int foirFailed = 0;

        // Check 1: Applicant Age at Application
        if (applicantAge >= minAge[loanType] && applicantAge <= maxAge[loanType]) {
            sprintf(checkResults[checkCount], "[PASS] Applicant Age: %d years (Required: %d to %d years)",
                    applicantAge, minAge[loanType], maxAge[loanType]);
            printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
        } else {
            sprintf(checkResults[checkCount], "[FAIL] Applicant Age: %d years (Must be between %d and %d years)",
                    applicantAge, minAge[loanType], maxAge[loanType]);
            printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
            failCount++;
        }

        // Check 2: Age at Loan Maturity (Applicant or Co-Applicant)
        if (loanType != 4) {
            float loanTenureYears = (float)tenureMonths / 12.0f;
            float ageAtEnd = applicantAge + loanTenureYears;
            int allowedMaxEnd = (employmentType == 2) ? maxAgeAtLoanEndSelf[loanType] : maxAgeAtLoanEndSalaried[loanType];

            if (ageAtEnd <= allowedMaxEnd) {
                sprintf(checkResults[checkCount], "[PASS] Age at Loan Maturity: %.1f years (Maximum allowed: %d years)",
                        ageAtEnd, allowedMaxEnd);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Age at Loan Maturity: %.1f years (Exceeds maximum limit of %d years)",
                        ageAtEnd, allowedMaxEnd);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else {
            // For education loan: check co-applicant age (25 to 65)
            if (coAppAge >= 25 && coAppAge <= 65) {
                sprintf(checkResults[checkCount], "[PASS] Co-Applicant Age: %d years (Required: 25 to 65 years)", coAppAge);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Co-Applicant Age: %d years (Must be between 25 and 65 years)", coAppAge);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

        // Check 3: Credit Score & Category
        if (loanType == 4 && coAppCreditScore == 0) {
            sprintf(checkResults[checkCount], "[PASS WITH NOTE] Co-Applicant Credit Score: 0 (No History - Bank may request extra documents)");
            printf("\033[1;33mNo credit history found. Bank may ask for extra documents. Proceeding.\033[0m\n");
            printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
        } else {
            int scoreToCheck = (loanType == 4) ? coAppCreditScore : creditScore;
            char *catToCheck = (loanType == 4) ? coAppCreditCat : creditCat;

            if (scoreToCheck >= minCreditScore[loanType]) {
                sprintf(checkResults[checkCount], "[PASS] Credit Score: %d (%s) - Meets minimum requirement of %d",
                        scoreToCheck, catToCheck, minCreditScore[loanType]);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Credit Score: %d (%s) - Below minimum requirement of %d",
                        scoreToCheck, catToCheck, minCreditScore[loanType]);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

        // Check 4: Minimum Monthly Income
        if (monthlyIncome >= minMonthlyIncome[loanType]) {
            sprintf(checkResults[checkCount], "[PASS] Monthly Income: Rs. %.2f (Meets minimum required: Rs. %.2f)",
                    monthlyIncome, minMonthlyIncome[loanType]);
            printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
        } else {
            sprintf(checkResults[checkCount], "[FAIL] Monthly Income: Rs. %.2f (Below minimum required: Rs. %.2f)",
                    monthlyIncome, minMonthlyIncome[loanType]);
            printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
            failCount++;
        }

        // Check 5: FOIR (Fixed Obligation to Income Ratio)
        float totalEmi = emi + existingEmis;
        float actualFoir = (monthlyIncome > 0.0f) ? (totalEmi / monthlyIncome * 100.0f) : 100.0f;

        if (actualFoir <= maxFoirPercent[loanType]) {
            sprintf(checkResults[checkCount], "[PASS] FOIR: %.1f%% (Maximum allowed: %.1f%%, Total EMIs: Rs. %.2f)",
                    actualFoir, maxFoirPercent[loanType], totalEmi);
            printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
        } else {
            sprintf(checkResults[checkCount], "[FAIL] FOIR: %.1f%% (Exceeds maximum limit of %.1f%%, Total EMIs: Rs. %.2f)",
                    actualFoir, maxFoirPercent[loanType], totalEmi);
            printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
            failCount++;
            foirFailed = 1;
        }

        // Check 6: Work Experience / Business Stability
        if (loanType == 1) { // Home Loan
            float reqExp = (employmentType == 1) ? 2.0f : 3.0f;
            if (workExperience >= reqExp) {
                sprintf(checkResults[checkCount], "[PASS] Work Experience: %.1f years (Required: %.1f years)", workExperience, reqExp);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Work Experience: %.1f years (Needs at least %.1f years)", workExperience, reqExp);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 2) { // Car Loan
            float reqExp = (employmentType == 1) ? 1.0f : 2.0f;
            if (workExperience >= reqExp) {
                sprintf(checkResults[checkCount], "[PASS] Work Experience: %.1f years (Required: %.1f years)", workExperience, reqExp);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Work Experience: %.1f years (Needs at least %.1f years)", workExperience, reqExp);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 3) { // Personal Loan
            float reqExp = (employmentType == 1) ? 1.0f : 2.0f;
            if (workExperience >= reqExp) {
                sprintf(checkResults[checkCount], "[PASS] Job Stability: %.1f years (Required: %.1f years)", workExperience, reqExp);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Job Stability: %.1f years (Needs at least %.1f years)", workExperience, reqExp);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 5) { // Business Loan
            if (workExperience >= 2.0f) {
                sprintf(checkResults[checkCount], "[PASS] Business Vintage: %.1f years (Required: 2.0 years)", workExperience);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Business Vintage: %.1f years (Must be in business for at least 2.0 years)", workExperience);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

        // Check 7: Loan-to-Value (LTV) for Home and Car Loans
        if (loanType == 1) { // Home Loan max 80% LTV
            double maxAllowedLoan = 0.80 * assetPrice;
            if (principal <= maxAllowedLoan) {
                sprintf(checkResults[checkCount], "[PASS] Loan-to-Value: Rs. %.2f <= 80%% of Property Value (Rs. %.2f)",
                        principal, maxAllowedLoan);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Loan-to-Value: Loan exceeds 80%% of Property Value (Max allowed: Rs. %.2f)",
                        maxAllowedLoan);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 2) { // Car Loan max 90% LTV
            double maxAllowedLoan = 0.90 * assetPrice;
            if (principal <= maxAllowedLoan) {
                sprintf(checkResults[checkCount], "[PASS] Loan-to-Value: Rs. %.2f <= 90%% of Car Price (Rs. %.2f)",
                        principal, maxAllowedLoan);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Loan-to-Value: Loan exceeds 90%% of Car Price (Max allowed: Rs. %.2f)",
                        maxAllowedLoan);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

        // Check 8: Collateral Requirement for Education Loan (> Rs. 7,50,000)
        if (loanType == 4) {
            if (principal > 750000.0) {
                if (hasCollateral) {
                    sprintf(checkResults[checkCount], "[PASS] Collateral Provided for loan exceeding Rs. 7,50,000");
                    printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
                } else {
                    sprintf(checkResults[checkCount], "[FAIL] Collateral Required: Loans above Rs. 7,50,000 need collateral (None provided)");
                    printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                    failCount++;
                }
            } else {
                sprintf(checkResults[checkCount], "[PASS] No Collateral Required (Loan amount <= Rs. 7,50,000)");
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            }
        }

        /* -------------------------------------------------------------
           STEP 5: MAXIMUM ELIGIBLE LOAN CALCULATION
           Max Allowed EMI = (FOIR% * Income) - Existing EMIs
           Max Loan = Max EMI * ((1+r)^n - 1) / (r * (1+r)^n)
           ------------------------------------------------------------- */
        float maxAllowedEmi = (maxFoirPercent[loanType] / 100.0f * monthlyIncome) - existingEmis;
        double maxEligibleLoan = 0.0;

        if (maxAllowedEmi > 0.0f) {
            double monthlyRate = (rate / (12.0 * 100.0));
            double factor = pow(1.0 + monthlyRate, tenureMonths);
            maxEligibleLoan = maxAllowedEmi * (factor - 1.0) / (monthlyRate * factor);
        } else {
            maxEligibleLoan = 0.0;
        }

        printf("--------------------------------------------------\n");
        printf("Maximum loan you can get for this tenure: Rs. %.2f\n", maxEligibleLoan);
        if (foirFailed) {
            printf("\033[1;33mSuggestion: Try a smaller loan amount or a longer tenure.\033[0m\n");
        }

        /* -------------------------------------------------------------
           FINAL ELIGIBILITY DECISION BANNER
           ------------------------------------------------------------- */
        if (failCount > 0) {
            printf("\n\033[1;31m==================================================\n");
            printf("|             APPLICATION REJECTED               |\n");
            printf("==================================================\033[0m\n");

            // Auto-save rejection report so user can review the exact reasons
            saveReportToFile("emi_report.txt", 0, loanType,
                             principal, rate, tenureStr, tenureMonths,
                             emi, totalInterest, totalPayment,
                             checkCount, checkResults, maxEligibleLoan);
            printf("Rejection summary saved to emi_report.txt\n");

            // Do not show dashboard or amortization schedule if rejected
            continue;
        }

        // All checks passed -> APPLICATION APPROVED
        printf("\n\033[1;32m==================================================\n");
        printf("|              APPLICATION APPROVED              |\n");
        printf("==================================================\033[0m\n");

        /* -------------------------------------------------------------
           STEP 6: RENDER LOAN EMI DASHBOARD (ONLY WHEN APPROVED)
           ------------------------------------------------------------- */
        printDashboard(loanType, "APPROVED",
                       (float)principal, rate, tenureStr,
                       emi, totalInterest, totalPayment,
                       creditScore, creditCat,
                       existingEmis, actualFoir,
                       coAppName, courseDuration, moratoriumMonths, moratoriumInterest,
                       assetPrice, downPayment);

        // Special education loan moratorium notice
        if (loanType == 4) {
            printf("\n\033[1;33mNOTE: Education loans include a moratorium period (course duration plus 12 months). Repayment starts after it ends. Interest may build up during the moratorium if not paid.\033[0m\n");
        }

        /* -------------------------------------------------------------
           SHOW AMORTIZATION TABLE (OPTIONAL)
           ------------------------------------------------------------- */
        if (askYesNo("\nShow month-wise amortization table?")) {
            print_schedule(stdout, principal, rate, tenureMonths, emi);
        }

        /* -------------------------------------------------------------
           STEP 7: SAVE SCHEDULE AND FULL REPORT TO TEXT FILE
           ------------------------------------------------------------- */
        if (askYesNo("\nSave schedule to a text file?")) {
            if (saveReportToFile("emi_report.txt", 1, loanType,
                                 principal, rate, tenureStr, tenureMonths,
                                 emi, totalInterest, totalPayment,
                                 checkCount, checkResults, maxEligibleLoan) == 0) {
                printf("Report saved to emi_report.txt\n");
            } else {
                printf("Error: Could not save report to emi_report.txt\n");
            }
        }

    } while (askYesNo("\nCalculate another loan?"));

    printf("Goodbye!\n");
    return 0;
}
