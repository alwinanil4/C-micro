#ifndef AMORTIZATION_H
#define AMORTIZATION_H

#include <stdio.h>

/*
 * Prints the month-wise amortization table to any stream
 * (stdout for the screen, or a FILE* for a text file).
 *
 *   principal   - loan amount (P)
 *   annual_rate - yearly interest rate in percent (e.g. 8.5)
 *   months      - tenure in months (N)
 *   emi         - monthly EMI, already calculated by the EMI function
 */
void print_schedule(FILE *out, double principal, double annual_rate,
                    int months, double emi);

/*
 * Saves the loan summary + full schedule to a text file.
 * Returns 0 on success, -1 if the file could not be opened.
 */
int save_schedule_to_file(const char *filename, double principal,
                          double annual_rate, int months, double emi);

#endif