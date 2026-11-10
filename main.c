#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

const char *loanNames[6] = {
    "",
    "Home Loan",
    "Car Loan",
    "Personal Loan",
    "Education Loan",
    "Business Loan"
};

const int minAge[6] = { 0, 21, 21, 21, 16, 25 };
const int maxAge[6] = { 0, 60, 65, 60, 35, 65 };

const int maxAgeAtLoanEndSalaried[6] = { 0, 65, 70, 65, 65, 70 };
const int maxAgeAtLoanEndSelf[6]     = { 0, 70, 70, 65, 65, 70 };

const int minCibilScore[6] = { 0, 650, 650, 700, 700, 700 };

const float maxFoirPercent[6] = { 0.0f, 50.0f, 50.0f, 40.0f, 50.0f, 50.0f };

const double minLoanAmount[6] = { 0.0, 500000.0, 100000.0, 50000.0, 50000.0, 100000.0 };
const double maxLoanAmount[6] = { 0.0, 100000000.0, 5000000.0, 2500000.0, 15000000.0, 5000000.0 };

const int minTenureMonths[6] = { 0, 12, 12, 12, 12, 12 };
const int maxTenureMonths[6] = { 0, 360, 84, 60, 180, 120 };

const float minRate[6] = { 0.0f, 6.0f, 6.0f, 9.0f, 5.0f, 8.0f };
const float maxRate[6] = { 0.0f, 15.0f, 18.0f, 30.0f, 15.0f, 24.0f };

const float typicalRateLow[6]  = { 0.0f, 8.0f, 9.0f, 12.0f, 7.0f, 10.0f };
const float typicalRateHigh[6] = { 0.0f, 10.0f, 12.0f, 18.0f, 10.0f, 15.0f };

const float minMonthlyIncome[6] = { 0.0f, 25000.0f, 20000.0f, 20000.0f, 15000.0f, 30000.0f };

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

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

int getValidCibilScore(const char *prompt) {
    int score;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &score) != 1) {
            printf("Error: Invalid input. Please enter a valid CIBIL score between 300 and 900.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        if (score < 300 || score > 900) {
            printf("Error: CIBIL score must be between 300 and 900. Please try again.\n");
            continue;
        }
        return score;
    }
}

void getCibilCategory(int cibilScore, char *cibilCategory) {
    if (cibilScore >= 750 && cibilScore <= 900) {
        strcpy(cibilCategory, "Excellent");
    } else if (cibilScore >= 700 && cibilScore <= 749) {
        strcpy(cibilCategory, "Good");
    } else if (cibilScore >= 650 && cibilScore <= 699) {
        strcpy(cibilCategory, "Average");
    } else if (cibilScore >= 600 && cibilScore <= 649) {
        strcpy(cibilCategory, "Poor");
    } else if (cibilScore >= 300 && cibilScore <= 599) {
        strcpy(cibilCategory, "Very Poor");
    } else {
        strcpy(cibilCategory, "Invalid");
    }
}

float calculateEMI(float principal, float annualRate, int tenureMonths) {
    double monthlyRate;
    double factor;
    double emi;
    monthlyRate = annualRate / (12.0 * 100.0);
    factor = pow(1.0 + monthlyRate, tenureMonths);
    emi = (principal * monthlyRate * factor) / (factor - 1.0);
    return (float)emi;
}

float calculateTotalInterest(float emi, int tenureMonths, float principal) {
    return (emi * tenureMonths) - principal;
}

float calculateTotalPayment(float emi, int tenureMonths) {
    return emi * tenureMonths;
}

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

void printDashboard(int loanType, const char *loanStatus,
                    float principal, float rate, const char *tenureStr,
                    float emi, float totalInterest, float totalPayment,
                    int cibilScore, const char *cibilCategory,
                    float existingEmis, float foirPercent,
                    const char *coAppName, int courseDuration, int moratoriumMonths, float moratoriumInterest,
                    double assetPrice, double downPayment) {
    char scoreStr[40];
    char foirStr[30];
    char courseStr[30];
    char moraStr[30];

    printf("\n===================================================\n");
    printf("|               LOAN EMI DASHBOARD                |\n");
    printf("===================================================\n");
    printf("| %-22s : %22s |\n", "Loan Type", loanNames[loanType]);
    printf("| %-22s : %22s |\n", "Eligibility Status", loanStatus);
    printf("| %-22s : %22.2f |\n", "Loan Amount", principal);
    printf("| %-22s : %22.2f |\n", "Annual Interest Rate", rate);
    printf("| %-22s : %22s |\n", "Tenure", tenureStr);

    sprintf(scoreStr, "%d (%s)", cibilScore, cibilCategory);
    printf("| %-22s : %22s |\n", "CIBIL Score", scoreStr);

    printf("| %-22s : %22.2f |\n", "Existing EMIs", existingEmis);
    sprintf(foirStr, "%.1f%%", foirPercent);
    printf("| %-22s : %22s |\n", "FOIR Percentage", foirStr);

    if (loanType == 1) {
        printf("| %-22s : %22.2f |\n", "Property Value", assetPrice);
        printf("| %-22s : %22.2f |\n", "Down Payment", downPayment);
    } else if (loanType == 2) {
        printf("| %-22s : %22.2f |\n", "Car On-Road Price", assetPrice);
        printf("| %-22s : %22.2f |\n", "Down Payment", downPayment);
    } else if (loanType == 4) {
        printf("| %-22s : %22s |\n", "Co-Applicant Name", coAppName);
        sprintf(courseStr, "%d Years", courseDuration);
        sprintf(moraStr, "%d Months", moratoriumMonths);
        printf("| %-22s : %22s |\n", "Course Duration", courseStr);
        printf("| %-22s : %22s |\n", "Moratorium Period", moraStr);
        printf("| %-22s : %22.2f |\n", "Moratorium Interest", moratoriumInterest);
    }

    printf("---------------------------------------------------\n");
    printf("| %-22s : %22.2f |\n", "Monthly EMI", emi);
    printf("| %-22s : %22.2f |\n", "Total Interest Payable", totalInterest);
    printf("| %-22s : %22.2f |\n", "Total Payment", totalPayment);
    printf("===================================================\n");
}

int saveReportToFile(const char *filename, const char *loanStatus, int isApprovedOrConditional, int loanType,
                     double principal, double rate, const char *tenureStr, int tenureMonths,
                     double emi, double totalInterest, double totalPayment,
                     int checkCount, char checkList[][200], double maxEligibleLoan) {
    FILE *fp;
    time_t now;
    char timeStr[64];
    int i;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }

    now = time(NULL);
    strcpy(timeStr, ctime(&now));
    timeStr[strcspn(timeStr, "\r\n")] = '\0';

    fprintf(fp, "============================================================\n");
    fprintf(fp, "               LOAN EMI EVALUATION REPORT                   \n");
    fprintf(fp, "============================================================\n");
    fprintf(fp, "Generated Date & Time : %s\n", timeStr);
    fprintf(fp, "Loan Type             : %s\n", loanNames[loanType]);
    fprintf(fp, "Requested Loan Amount : Rs. %.2f\n", principal);
    fprintf(fp, "Annual Interest Rate  : %.2f%%\n", rate);
    fprintf(fp, "Tenure                : %s\n", tenureStr);
    fprintf(fp, "Application Decision  : %s\n", loanStatus);
    fprintf(fp, "------------------------------------------------------------\n");

    fprintf(fp, "\nELIGIBILITY CHECKS BREAKDOWN:\n");
    for (i = 0; i < checkCount; i++) {
        fprintf(fp, "  %s\n", checkList[i]);
    }

    fprintf(fp, "\nMaximum loan amount possible for this tenure: Rs. %.2f\n", maxEligibleLoan);

    if (isApprovedOrConditional) {
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

int main(void) {
    int loanType;
    char amountPrompt[120];
    double principal;
    float rate;
    int tenureChoice;
    float tenureYearsInput;
    int tenureMonthsInput;
    int tenureMonths;
    char tenureStr[60];
    float emi;
    float totalInterest;
    float totalPayment;
    int applicantAge;
    int employmentType;
    float workExperience;
    float monthlyIncome;
    float existingEmis;
    int applicantScore;
    int studentHasCibil;
    char cibilCategory[30];
    double assetPrice;
    double downPayment;
    char coAppName[60];
    int coAppAge;
    float coAppIncome;
    float coAppExistingEmis;
    int coAppCibilScore;
    char coAppCibilCategory[30];
    int courseDuration;
    int hasCollateral;
    int hasGuarantor;
    int moratoriumMonths;
    float moratoriumInterest;
    char checkResults[12][200];
    int checkCount;
    int failCount;
    int conditionalFlag;
    int foirFailed;
    float loanTenureYears;
    float ageAtEnd;
    int allowedMaxEnd;
    float totalEmi;
    float actualFoir;
    float reqExp;
    double maxAllowedLoan;
    float maxAllowedEmi;
    double maxEligibleLoan;
    double monthlyRate;
    double factor;
    char loanStatus[30];
    int displayScore;
    char displayCategory[30];

    do {
        printf("\n  SELECT LOAN TYPE\n");
        printf("  ================\n");
        printf("  1. Home Loan        (Typical rate: 8%% - 10%% p.a.)\n");
        printf("  2. Car Loan         (Typical rate: 9%% - 12%% p.a.)\n");
        printf("  3. Personal Loan    (Typical rate: 12%% - 18%% p.a.)\n");
        printf("  4. Education Loan   (Typical rate: 7%% - 10%% p.a.)\n");
        printf("  5. Business Loan    (Typical rate: 10%% - 15%% p.a.)\n");

        loanType = getValidInt("  Enter choice (1-5): ", 1, 5);

        sprintf(amountPrompt, "\nEnter loan amount (Allowed: Rs. %.0f to Rs. %.0f): ",
                minLoanAmount[loanType], maxLoanAmount[loanType]);
        principal = getValidDouble(amountPrompt, minLoanAmount[loanType], maxLoanAmount[loanType]);

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

            if (rate < typicalRateLow[loanType] || rate > typicalRateHigh[loanType]) {
                printf("\033[1;33mWarning: Entered rate (%.2f%%) is outside typical range (%.1f%% - %.1f%%).\033[0m\n",
                       rate, typicalRateLow[loanType], typicalRateHigh[loanType]);
            }
            break;
        }

        tenureYearsInput = 0.0f;
        tenureMonthsInput = 0;
        tenureMonths = 0;

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

            if (tenureMonths < minTenureMonths[loanType] || tenureMonths > maxTenureMonths[loanType]) {
                printf("Error: Tenure for %s must be between %d and %d months (%.1f to %.1f years).\n",
                       loanNames[loanType], minTenureMonths[loanType], maxTenureMonths[loanType],
                       minTenureMonths[loanType] / 12.0f, maxTenureMonths[loanType] / 12.0f);
                continue;
            }
            break;
        }

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

        emi = calculateEMI((float)principal, rate, tenureMonths);
        totalInterest = calculateTotalInterest(emi, tenureMonths, (float)principal);
        totalPayment = calculateTotalPayment(emi, tenureMonths);

        applicantAge = 0;
        employmentType = 1;
        workExperience = 0.0f;
        monthlyIncome = 0.0f;
        existingEmis = 0.0f;
        applicantScore = 0;
        studentHasCibil = 1;
        strcpy(cibilCategory, "");

        assetPrice = 0.0;
        downPayment = 0.0;
        strcpy(coAppName, "");
        coAppAge = 0;
        coAppIncome = 0.0f;
        coAppExistingEmis = 0.0f;
        coAppCibilScore = 0;
        strcpy(coAppCibilCategory, "");
        courseDuration = 0;
        hasCollateral = 0;
        hasGuarantor = 0;
        moratoriumMonths = 0;
        moratoriumInterest = 0.0f;

        printf("\n--- APPLICANT INFORMATION & ELIGIBILITY DETAILS ---\n");

        if (loanType == 4) {
            applicantAge = getValidInt("Enter student age (years): ", 1, 100);
            studentHasCibil = askYesNo("Does the student have a CIBIL score?");
            if (studentHasCibil == 1) {
                applicantScore = getValidCibilScore("Enter student CIBIL score (300-900): ");
                getCibilCategory(applicantScore, cibilCategory);
            } else {
                applicantScore = 0;
                strcpy(cibilCategory, "No CIBIL Score");
            }

            printf("Enter co-applicant (parent/guardian) name: ");
            if (fgets(coAppName, sizeof(coAppName), stdin) != NULL) {
                coAppName[strcspn(coAppName, "\r\n")] = '\0';
            }
            coAppAge = getValidInt("Enter co-applicant age (years): ", 1, 100);
            coAppIncome = getValidFloat("Enter co-applicant monthly income (Rs.): ", 0.0f, 100000000.0f);
            coAppExistingEmis = getValidFloat("Enter co-applicant existing monthly EMIs (Rs.): ", 0.0f, 100000000.0f);

            if (studentHasCibil == 0) {
                printf("Student has no CIBIL score. Checking co-applicant CIBIL score for eligibility.\n");
                coAppCibilScore = getValidCibilScore("Enter co-applicant CIBIL score (300-900): ");
                getCibilCategory(coAppCibilScore, coAppCibilCategory);
            } else if (applicantScore < 700) {
                printf("Student CIBIL score is below 700. Co-applicant CIBIL score is required.\n");
                coAppCibilScore = getValidCibilScore("Enter co-applicant CIBIL score (300-900): ");
                getCibilCategory(coAppCibilScore, coAppCibilCategory);
            } else {
                coAppCibilScore = applicantScore;
                strcpy(coAppCibilCategory, cibilCategory);
            }

            courseDuration = getValidInt("Enter course duration in years (1 to 6): ", 1, 6);
            hasCollateral = askYesNo("Is collateral (property or fixed deposit) provided?");

            moratoriumMonths = (courseDuration * 12) + 12;
            moratoriumInterest = (float)(principal * (rate / 100.0f) * (moratoriumMonths / 12.0f));

            monthlyIncome = coAppIncome;
            existingEmis = coAppExistingEmis;

        } else if (loanType == 5) {
            applicantAge = getValidInt("Enter applicant age (years): ", 1, 100);
            workExperience = getValidFloat("Enter years in business: ", 0.0f, 80.0f);
            monthlyIncome = getValidFloat("Enter monthly net business income / profit (Rs.): ", 0.0f, 100000000.0f);
            existingEmis = getValidFloat("Enter existing monthly EMIs (Rs.): ", 0.0f, 100000000.0f);
            applicantScore = getValidCibilScore("Enter your CIBIL score (300-900): ");
            getCibilCategory(applicantScore, cibilCategory);

            if (applicantScore >= 600 && applicantScore <= 649) {
                hasGuarantor = askYesNo("Do you have a co-applicant or guarantor?");
            }

        } else {
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
            applicantScore = getValidCibilScore("Enter your CIBIL score (300-900): ");
            getCibilCategory(applicantScore, cibilCategory);

            if (applicantScore >= 600 && applicantScore <= 649) {
                hasGuarantor = askYesNo("Do you have a co-applicant or guarantor?");
            }
        }

        printf("\n==================================================\n");
        printf("              ELIGIBILITY VERIFICATION            \n");
        printf("==================================================\n");

        checkCount = 0;
        failCount = 0;
        conditionalFlag = 0;
        foirFailed = 0;

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

        if (loanType != 4) {
            loanTenureYears = (float)tenureMonths / 12.0f;
            ageAtEnd = applicantAge + loanTenureYears;
            if (employmentType == 2) {
                allowedMaxEnd = maxAgeAtLoanEndSelf[loanType];
            } else {
                allowedMaxEnd = maxAgeAtLoanEndSalaried[loanType];
            }

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
            if (coAppAge >= 25 && coAppAge <= 65) {
                sprintf(checkResults[checkCount], "[PASS] Co-Applicant Age: %d years (Required: 25 to 65 years)", coAppAge);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Co-Applicant Age: %d years (Must be between 25 and 65 years)", coAppAge);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

        if (loanType == 4) {
            if (studentHasCibil == 0) {
                if (coAppCibilScore >= 700 && coAppCibilScore <= 900) {
                    sprintf(checkResults[checkCount], "[CONDITIONAL] Student has no CIBIL score. Co-Applicant CIBIL %d (%s) - Conditionally Approved",
                            coAppCibilScore, coAppCibilCategory);
                    printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
                    conditionalFlag = 1;
                } else {
                    sprintf(checkResults[checkCount], "[FAIL] Student has no CIBIL score and Co-Applicant CIBIL %d (%s) is below 700",
                            coAppCibilScore, coAppCibilCategory);
                    printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                    failCount++;
                }
            } else {
                if (applicantScore >= 750 && applicantScore <= 900) {
                    sprintf(checkResults[checkCount], "[PASS] Student CIBIL Score: %d (%s) - Approved easily",
                            applicantScore, cibilCategory);
                    printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
                } else if (applicantScore >= 700 && applicantScore <= 749) {
                    sprintf(checkResults[checkCount], "[PASS] Student CIBIL Score: %d (%s) - Approved (slightly higher interest may apply)",
                            applicantScore, cibilCategory);
                    printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
                } else if (applicantScore >= 600 && applicantScore <= 699) {
                    if (coAppCibilScore >= 750 && coAppCibilScore <= 900) {
                        sprintf(checkResults[checkCount], "[CONDITIONAL] Student CIBIL %d (%s), Co-Applicant CIBIL %d (%s) - Conditionally Approved",
                                applicantScore, cibilCategory, coAppCibilScore, coAppCibilCategory);
                        printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
                        conditionalFlag = 1;
                    } else if (coAppCibilScore >= 700 && coAppCibilScore <= 749) {
                        sprintf(checkResults[checkCount], "[CONDITIONAL] Student CIBIL %d (%s), Co-Applicant CIBIL %d (%s) - Conditionally Approved (higher interest)",
                                applicantScore, cibilCategory, coAppCibilScore, coAppCibilCategory);
                        printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
                        conditionalFlag = 1;
                    } else if (coAppCibilScore >= 650 && coAppCibilScore <= 699 && hasCollateral == 1) {
                        sprintf(checkResults[checkCount], "[CONDITIONAL] Co-Applicant CIBIL %d (%s) with collateral - Conditionally Approved",
                                coAppCibilScore, coAppCibilCategory);
                        printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
                        conditionalFlag = 1;
                    } else {
                        sprintf(checkResults[checkCount], "[FAIL] Student CIBIL %d (%s) and Co-Applicant CIBIL %d (%s) do not meet requirement",
                                applicantScore, cibilCategory, coAppCibilScore, coAppCibilCategory);
                        printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                        failCount++;
                    }
                } else {
                    sprintf(checkResults[checkCount], "[FAIL] Student CIBIL Score: %d (%s) - Application rejected (Co-Applicant CIBIL: %d - %s)",
                            applicantScore, cibilCategory, coAppCibilScore, coAppCibilCategory);
                    printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                    failCount++;
                }
            }
        } else {
            if (applicantScore >= 750 && applicantScore <= 900) {
                sprintf(checkResults[checkCount], "[PASS] CIBIL Score: %d (%s) - Approve all loan types easily",
                        applicantScore, cibilCategory);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else if (applicantScore >= 700 && applicantScore <= 749) {
                sprintf(checkResults[checkCount], "[PASS] CIBIL Score: %d (%s) - Approved (slightly higher interest rate applies)",
                        applicantScore, cibilCategory);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else if (applicantScore >= 650 && applicantScore <= 699) {
                if (loanType == 1 || loanType == 2) {
                    sprintf(checkResults[checkCount], "[CONDITIONAL] CIBIL Score: %d (%s) - Conditionally Approved (Secured Loan)",
                            applicantScore, cibilCategory);
                    printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
                    conditionalFlag = 1;
                } else {
                    sprintf(checkResults[checkCount], "[FAIL] CIBIL Score: %d (%s) - Only secured loans (Home/Car) approved in 650-699 range",
                            applicantScore, cibilCategory);
                    printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                    failCount++;
                }
            } else if (applicantScore >= 600 && applicantScore <= 649) {
                if (hasGuarantor == 1) {
                    sprintf(checkResults[checkCount], "[CONDITIONAL] CIBIL Score: %d (%s) - Conditionally Approved with Co-Applicant/Guarantor",
                            applicantScore, cibilCategory);
                    printf("\033[1;33m%s\033[0m\n", checkResults[checkCount++]);
                    conditionalFlag = 1;
                } else {
                    sprintf(checkResults[checkCount], "[FAIL] CIBIL Score: %d (%s) - Rejected (requires a co-applicant or guarantor)",
                            applicantScore, cibilCategory);
                    printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                    failCount++;
                }
            } else {
                sprintf(checkResults[checkCount], "[FAIL] CIBIL Score: %d (%s) - Score below 600 results in rejection",
                        applicantScore, cibilCategory);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

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

        totalEmi = emi + existingEmis;
        if (monthlyIncome > 0.0f) {
            actualFoir = (totalEmi / monthlyIncome) * 100.0f;
        } else {
            actualFoir = 100.0f;
        }

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

        if (loanType == 1) {
            if (employmentType == 1) {
                reqExp = 2.0f;
            } else {
                reqExp = 3.0f;
            }
            if (workExperience >= reqExp) {
                sprintf(checkResults[checkCount], "[PASS] Work Experience: %.1f years (Required: %.1f years)", workExperience, reqExp);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Work Experience: %.1f years (Needs at least %.1f years)", workExperience, reqExp);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 2) {
            if (employmentType == 1) {
                reqExp = 1.0f;
            } else {
                reqExp = 2.0f;
            }
            if (workExperience >= reqExp) {
                sprintf(checkResults[checkCount], "[PASS] Work Experience: %.1f years (Required: %.1f years)", workExperience, reqExp);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Work Experience: %.1f years (Needs at least %.1f years)", workExperience, reqExp);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 3) {
            if (employmentType == 1) {
                reqExp = 1.0f;
            } else {
                reqExp = 2.0f;
            }
            if (workExperience >= reqExp) {
                sprintf(checkResults[checkCount], "[PASS] Job Stability: %.1f years (Required: %.1f years)", workExperience, reqExp);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Job Stability: %.1f years (Needs at least %.1f years)", workExperience, reqExp);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        } else if (loanType == 5) {
            if (workExperience >= 2.0f) {
                sprintf(checkResults[checkCount], "[PASS] Business Vintage: %.1f years (Required: 2.0 years)", workExperience);
                printf("\033[1;32m%s\033[0m\n", checkResults[checkCount++]);
            } else {
                sprintf(checkResults[checkCount], "[FAIL] Business Vintage: %.1f years (Must be in business for at least 2.0 years)", workExperience);
                printf("\033[1;31m%s\033[0m\n", checkResults[checkCount++]);
                failCount++;
            }
        }

        if (loanType == 1) {
            maxAllowedLoan = 0.80 * assetPrice;
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
        } else if (loanType == 2) {
            maxAllowedLoan = 0.90 * assetPrice;
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

        maxAllowedEmi = (maxFoirPercent[loanType] / 100.0f * monthlyIncome) - existingEmis;
        maxEligibleLoan = 0.0;

        if (maxAllowedEmi > 0.0f) {
            monthlyRate = (rate / (12.0 * 100.0));
            factor = pow(1.0 + monthlyRate, tenureMonths);
            maxEligibleLoan = maxAllowedEmi * (factor - 1.0) / (monthlyRate * factor);
        } else {
            maxEligibleLoan = 0.0;
        }

        printf("--------------------------------------------------\n");
        printf("Maximum loan you can get for this tenure: Rs. %.2f\n", maxEligibleLoan);
        if (foirFailed) {
            printf("\033[1;33mSuggestion: Try a smaller loan amount or a longer tenure.\033[0m\n");
        }

        if (failCount > 0) {
            strcpy(loanStatus, "REJECTED");
            printf("\n\033[1;31m==================================================\n");
            printf("|             APPLICATION REJECTED               |\n");
            printf("==================================================\033[0m\n");

            saveReportToFile("emi_report.txt", loanStatus, 0, loanType,
                             principal, rate, tenureStr, tenureMonths,
                             emi, totalInterest, totalPayment,
                             checkCount, checkResults, maxEligibleLoan);
            printf("Rejection summary saved to emi_report.txt\n");
            continue;
        } else if (conditionalFlag == 1) {
            strcpy(loanStatus, "CONDITIONALLY APPROVED");
            printf("\n\033[1;33m==================================================\n");
            printf("|       APPLICATION CONDITIONALLY APPROVED       |\n");
            printf("==================================================\033[0m\n");
        } else {
            strcpy(loanStatus, "APPROVED");
            printf("\n\033[1;32m==================================================\n");
            printf("|              APPLICATION APPROVED              |\n");
            printf("==================================================\033[0m\n");
        }

        if (loanType == 4 && (studentHasCibil == 0 || applicantScore < 700)) {
            displayScore = coAppCibilScore;
            strcpy(displayCategory, coAppCibilCategory);
        } else {
            displayScore = applicantScore;
            strcpy(displayCategory, cibilCategory);
        }

        printDashboard(loanType, loanStatus,
                       (float)principal, rate, tenureStr,
                       emi, totalInterest, totalPayment,
                       displayScore, displayCategory,
                       existingEmis, actualFoir,
                       coAppName, courseDuration, moratoriumMonths, moratoriumInterest,
                       assetPrice, downPayment);

        if (loanType == 4) {
            printf("\n\033[1;33mNOTE: Education loans include a moratorium period (course duration plus 12 months). Repayment starts after it ends. Interest may build up during the moratorium if not paid.\033[0m\n");
        }

        if (askYesNo("\nShow month-wise amortization table?")) {
            print_schedule(stdout, principal, rate, tenureMonths, emi);
        }

        if (askYesNo("\nSave schedule to a text file?")) {
            if (saveReportToFile("emi_report.txt", loanStatus, 1, loanType,
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
