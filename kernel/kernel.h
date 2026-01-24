#ifndef KERNEL_H
#define KERNEL_H

#define BUFFER_SIZE 256
#define HISTORY_SIZE 50

void kernel_main(void);
void calculator_loop(void);
double evaluate_expression(const char* expr);
void shutdown_system(void);
void reboot_system(void);

#endif
