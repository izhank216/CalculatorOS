#include "kernel.h"
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char history[HISTORY_SIZE][BUFFER_SIZE];
int history_count = 0;

void kernel_main(void) {
    printf("Welcome to CalculatorOS!\n");
    printf("Type expressions to calculate, or commands: exit, reboot, shutdown, history\n");
    calculator_loop();
}

void calculator_loop(void) {
    char input[BUFFER_SIZE];
    while (1) {
        printf("CalcOS> ");
        if (!fgets(input, BUFFER_SIZE, stdin)) continue;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        if (strcmp(input, "exit") == 0 || strcmp(input, "shutdown") == 0) {
            shutdown_system();
            break;
        }
        if (strcmp(input, "reboot") == 0) {
            reboot_system();
            break;
        }
        if (strcmp(input, "history") == 0) {
            if (history_count == 0) printf("No history yet.\n");
            else {
                printf("History:\n");
                for (int i = 0; i < history_count; i++)
                    printf("%d: %s\n", i + 1, history[i]);
            }
            continue;
        }

        double result = evaluate_expression(input);
        printf("= %lf\n", result);

        if (history_count < HISTORY_SIZE) {
            strncpy(history[history_count], input, BUFFER_SIZE - 1);
            history[history_count][BUFFER_SIZE - 1] = 0;
            history_count++;
        }
    }
}

double parse_expression(const char** str);

double parse_number(const char** str) {
    while (isspace(**str)) (*str)++;
    if (**str == '(') {
        (*str)++;
        double val = parse_expression(str);
        if (**str == ')') (*str)++;
        return val;
    } else if (isalpha(**str)) {
        char func[16];
        int i = 0;
        while (isalpha(**str) && i < 15) {
            func[i++] = **str;
            (*str)++;
        }
        func[i] = 0;
        if (**str == '(') (*str)++;
        double arg = parse_expression(str);
        if (**str == ')') (*str)++;
        if (strcmp(func, "sin") == 0) return sin(arg);
        if (strcmp(func, "cos") == 0) return cos(arg);
        if (strcmp(func, "tan") == 0) return tan(arg);
        if (strcmp(func, "sqrt") == 0) return sqrt(arg);
        if (strcmp(func, "log") == 0) return log(arg);
        printf("Unknown function '%s'\n", func);
        return 0;
    } else {
        double val = strtod(*str, (char**)str);
        return val;
    }
}

double parse_term(const char** str) {
    double val = parse_number(str);
    while (1) {
        while (isspace(**str)) (*str)++;
        if (**str == '*') {
            (*str)++;
            val *= parse_number(str);
        } else if (**str == '/') {
            (*str)++;
            double d = parse_number(str);
            if (d != 0) val /= d;
            else printf("Error: division by zero\n");
        } else break;
    }
    return val;
}

double parse_expression(const char** str) {
    double val = parse_term(str);
    while (1) {
        while (isspace(**str)) (*str)++;
        if (**str == '+') {
            (*str)++;
            val += parse_term(str);
        } else if (**str == '-') {
            (*str)++;
            val -= parse_term(str);
        } else break;
    }
    return val;
}

double evaluate_expression(const char* expr) {
    const char* p = expr;
    return parse_expression(&p);
}

void shutdown_system(void) {
    printf("Shutting down CalculatorOS. Goodbye!\n");
    exit(0);
}

void reboot_system(void) {
    printf("Rebooting CalculatorOS...\n\n");
    kernel_main();
}
