#include <stdio.h>
#include "amortization.h"

void print_schedule(FILE *out, double principal, double annual_rate,
                    int months, double emi)
{
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

        /* Last month: clear the exact remaining balance so rounding
           errors don't leave a tiny leftover amount. */
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

int save_schedule_to_file(const char *filename, double principal,
                          double annual_rate, int months, double emi)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return -1;
    }

    fprintf(fp, "===== LOAN EMI CALCULATOR - AMORTIZATION SCHEDULE =====\n");
    fprintf(fp, "Loan amount   : %.2f\n", principal);
    fprintf(fp, "Interest rate : %.2f%% per annum\n", annual_rate);
    fprintf(fp, "Tenure        : %d months\n", months);
    fprintf(fp, "Monthly EMI   : %.2f\n", emi);

    print_schedule(fp, principal, annual_rate, months, emi);

    fclose(fp);
    return 0;
}