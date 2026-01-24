#include "kernel.h"
#include <stdint.h>


volatile uint16_t* video = (uint16_t*)0xB8000;
uint16_t cursor_pos = 0;

void kputchar(char c) {
    if (c == '\n') {
        cursor_pos += 80 - (cursor_pos % 80);
        return;
    }
    video[cursor_pos++] = (uint8_t)c | (0x07 << 8);
}

void kputs(const char* str) {
    while (*str) kputchar(*str++);
}

int kstrlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void kstrncpy(char* dest, const char* src, int n) {
    int i;
    for (i = 0; i < n && src[i]; i++) dest[i] = src[i];
    for (; i < n; i++) dest[i] = 0;
}

int kstrcmp(const char* a, const char* b) {
    while (*a && (*a == *b)) { a++; b++; }
    return *(unsigned char*)a - *(unsigned char*)b;
}


char kgetchar(void) {
    char c;
    do { 
        asm volatile("int $0x16" : "=a"(c) : "a"(0) ); 
        c = (char)(c & 0xFF);
    } while (!c);
    kputchar(c);
    return c;
}

char* kgets(char* buf, int n) {
    int i = 0;
    while (i < n-1) {
        char c = kgetchar();
        if (c == '\r' || c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = 0;
    kputchar('\n');
    return buf;
}


double kpow(double x, int n) {
    double res = 1;
    for (int i=0;i<n;i++) res *= x;
    return res;
}

double kfactorial(int n) {
    double f = 1;
    for (int i=2;i<=n;i++) f*=i;
    return f;
}

double ksin(double x) {
    double res=0;
    for (int i=0;i<10;i++) res += kpow(-1,i)*kpow(x,2*i+1)/kfactorial(2*i+1);
    return res;
}

double kcos(double x) {
    double res=0;
    for (int i=0;i<10;i++) res += kpow(-1,i)*kpow(x,2*i)/kfactorial(2*i);
    return res;
}

double ktan(double x) { return ksin(x)/kcos(x); }

double ksqrt(double x) {
    double r=x;
    for (int i=0;i<20;i++) r=(r+x/r)/2;
    return r;
}

double klog(double x) {
    double res=0;
    while (x>2) { x/=2; res+=0.69314718; }
    x-=1;
    double term=x;
    for (int i=1;i<20;i++,term*= -x) res+=term/i;
    return res;
}


int kisspace(char c) { return c==' '||c=='\t'||c=='\n'; }
int kisalpha(char c) { return (c>='A'&&c<='Z')||(c>='a'&&c<='z'); }

double kstrtod(const char* str, const char** endptr) {
    double val=0;
    int sign=1;
    if (*str=='-') { sign=-1; str++; }
    while(*str>='0' && *str<='9') { val=val*10+(*str-'0'); str++; }
    if (*str=='.') { str++; double frac=0.1; while(*str>='0' && *str<='9') { val+=(*str-'0')*frac; frac*=0.1; str++; } }
    *endptr=str;
    return val*sign;
}

char history[HISTORY_SIZE][BUFFER_SIZE];
int history_count=0;

double parse_expression(const char** str);

double parse_number(const char** str) {
    while(kisspace(**str)) (*str)++;
    if (**str=='(') { (*str)++; double val=parse_expression(str); if (**str==')') (*str)++; return val; }
    else if (kisalpha(**str)) {
        char func[16]; int i=0;
        while(kisalpha(**str)&&i<15) func[i++]=**str,(*str)++;
        func[i]=0;
        if (**str=='(')(*str)++;
        double arg=parse_expression(str);
        if (**str==')') (*str)++;
        if (kstrcmp(func,"sin")==0) return ksin(arg);
        if (kstrcmp(func,"cos")==0) return kcos(arg);
        if (kstrcmp(func,"tan")==0) return ktan(arg);
        if (kstrcmp(func,"sqrt")==0) return ksqrt(arg);
        if (kstrcmp(func,"log")==0) return klog(arg);
        kputs("Unknown function\n"); return 0;
    } else {
        return kstrtod(*str,str);
    }
}

double parse_term(const char** str) {
    double val=parse_number(str);
    while(1) {
        while(kisspace(**str)) (*str)++;
        if (**str=='*') { (*str)++; val*=parse_number(str); }
        else if (**str=='/') { (*str)++; double d=parse_number(str); if(d!=0) val/=d; else kputs("Error: division by zero\n"); }
        else break;
    }
    return val;
}

double parse_expression(const char** str) {
    double val=parse_term(str);
    while(1) {
        while(kisspace(**str)) (*str)++;
        if (**str=='+') { (*str)++; val+=parse_term(str); }
        else if (**str=='-') { (*str)++; val-=parse_term(str); }
        else break;
    }
    return val;
}

double evaluate_expression(const char* expr) { const char* p=expr; return parse_expression(&p); }


void calculator_loop(void) {
    char input[BUFFER_SIZE];
    while(1) {
        kputs("CalcOS> ");
        kgets(input,BUFFER_SIZE);
        if (kstrlen(input)==0) continue;
        if (kstrcmp(input,"exit")==0||kstrcmp(input,"shutdown")==0) { shutdown_system(); break; }
        if (kstrcmp(input,"reboot")==0) { reboot_system(); break; }
        if (kstrcmp(input,"history")==0) {
            if(history_count==0) kputs("No history yet.\n");
            else { kputs("History:\n"); for(int i=0;i<history_count;i++) kputs(history[i]); }
            continue;
        }
        double result=evaluate_expression(input);
        char buf[32];
        int len=(int)result; int frac=(int)((result-len)*1000000);
        // simple print
        int pos=0;
        if(result<0) { buf[pos++]='-'; result=-result; len=-len; }
        // integer part
        int tmp=len; char num[16]; int n=0;
        if(tmp==0) num[n++]='0';
        while(tmp>0) { num[n++]=tmp%10+'0'; tmp/=10; }
        for(int i=n-1;i>=0;i--) buf[pos++]=num[i];
        buf[pos++]='.';
        // fractional part
        tmp=frac; n=6; while(n--) { buf[pos++]=tmp%10+'0'; tmp/=10; }
        buf[pos]=0;
        kputs(buf);
        kputs("\n");
        if(history_count<HISTORY_SIZE) { kstrncpy(history[history_count],input,BUFFER_SIZE-1); history[history_count][BUFFER_SIZE-1]=0; history_count++; }
    }
}


void shutdown_system(void) { kputs("Shutting down CalculatorOS. Goodbye!\n"); while(1){} }
void reboot_system(void) { kputs("Rebooting CalculatorOS...\n\n"); kernel_main(); }

void kernel_main(void) {
    kputs("Welcome to CalculatorOS!\n");
    kputs("Type expressions or commands: exit, reboot, shutdown, history\n");
    calculator_loop();
}
