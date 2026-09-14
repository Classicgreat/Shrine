#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#ifdef _WIN32
//////////////
#define CLEAR "cls"
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)

void get_console_size(int *cols, int *rows) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        *cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        // если не удалось  значения по умолчанию
        *cols = 80;
        *rows = 25;
    }
}

//////////////
#else
//////////////
#define CLEAR "clear"
#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)

#include <sys/ioctl.h>

void get_console_size(int *cols, int *rows) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        *cols = w.ws_col;
        *rows = w.ws_row;
    } else {
        char *c = getenv("COLUMNS");
        char *r = getenv("LINES");
        *cols = c ? atoi(c) : 80;
        *rows = r ? atoi(r) : 25;
    }
}

//////////////
#endif

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point start;
    Point end;
} Line;

typedef struct {
    Point coord;
    char data;
    int fg;
    int bg;
} SHRINE_PIXEL;


SHRINE_PIXEL shrine_pixels[] = {
    {.coord={ 11, 0 }, .data='\\', .fg=36, .bg=0},
    {.coord={ 14, 0 }, .data='^', .fg=0, .bg=0},
    {.coord={ 17, 0 }, .data='/', .fg=36, .bg=0},
    {.coord={ 12, 1 }, .data='/', .fg=0, .bg=0},
    {.coord={ 16, 1 }, .data='\\', .fg=0, .bg=0},
    {.coord={ 9, 2 }, .data='\\', .fg=36, .bg=0},
    {.coord={ 12, 2 }, .data='|', .fg=0, .bg=0},
    {.coord={ 16, 2 }, .data='|', .fg=0, .bg=0},
    {.coord={ 19, 2 }, .data='/', .fg=36, .bg=0},
    {.coord={ 9, 3 }, .data='-', .fg=36, .bg=0},
    {.coord={ 10, 3 }, .data='-', .fg=36, .bg=0},
    {.coord={ 11, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 12, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 13, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 14, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 15, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 16, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 17, 3 }, .data='|', .fg=0, .bg=0},
    {.coord={ 18, 3 }, .data='-', .fg=36, .bg=0},
    {.coord={ 19, 3 }, .data='-', .fg=36, .bg=0},
    {.coord={ 2, 4 }, .data='\\', .fg=36, .bg=0},
    {.coord={ 3, 4 }, .data='\\', .fg=36, .bg=0},
    {.coord={ 4, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 5, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 6, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 7, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 8, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 9, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 10, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 11, 4 }, .data='!', .fg=0, .bg=0},
    {.coord={ 12, 4 }, .data='@', .fg=0, .bg=0},
    {.coord={ 13, 4 }, .data='@', .fg=0, .bg=0},
    {.coord={ 14, 4 }, .data='@', .fg=0, .bg=0},
    {.coord={ 15, 4 }, .data='@', .fg=0, .bg=0},
    {.coord={ 16, 4 }, .data='@', .fg=0, .bg=0},
    {.coord={ 17, 4 }, .data='!', .fg=0, .bg=0},
    {.coord={ 18, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 19, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 20, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 21, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 22, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 23, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 24, 4 }, .data='_', .fg=0, .bg=0},
    {.coord={ 25, 4 }, .data='/', .fg=36, .bg=0},
    {.coord={ 26, 4 }, .data='/', .fg=36, .bg=0},
    {.coord={ 3, 5 }, .data='\\', .fg=36, .bg=0},
    {.coord={ 4, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 5, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 6, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 7, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 8, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 9, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 10, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 11, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 12, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 13, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 14, 5 }, .data='@', .fg=0, .bg=0},
    {.coord={ 15, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 16, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 17, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 18, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 19, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 20, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 21, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 22, 5 }, .data='[', .fg=0, .bg=0},
    {.coord={ 23, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 24, 5 }, .data=']', .fg=0, .bg=0},
    {.coord={ 25, 5 }, .data='/', .fg=36, .bg=0},
    {.coord={ 1, 6 }, .data='~', .fg=35, .bg=0},
    {.coord={ 3, 6 }, .data='[', .fg=31, .bg=0},
    {.coord={ 4, 6 }, .data='|', .fg=31, .bg=0},
    {.coord={ 5, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 6, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 7, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 8, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 9, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 10, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 11, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 12, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 13, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 14, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 15, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 16, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 17, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 18, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 19, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 20, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 21, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 22, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 23, 6 }, .data='!', .fg=31, .bg=0},
    {.coord={ 24, 6 }, .data='|', .fg=31, .bg=0},
    {.coord={ 25, 6 }, .data=']', .fg=31, .bg=0},
    {.coord={ 27, 6 }, .data='~', .fg=35, .bg=0},
    {.coord={ 1, 7 }, .data='|', .fg=37, .bg=40},
    {.coord={ 2, 7 }, .data='~', .fg=35, .bg=40},
    {.coord={ 3, 7 }, .data='~', .fg=35, .bg=40},
    {.coord={ 4, 7 }, .data='|', .fg=31, .bg=40},
    {.coord={ 5, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 6, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 7, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 8, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 9, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 10, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 11, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 12, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 13, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 14, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 15, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 16, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 17, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 18, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 19, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 20, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 21, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 22, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 23, 7 }, .data='-', .fg=0, .bg=0},
    {.coord={ 24, 7 }, .data='|', .fg=31, .bg=0},
    {.coord={ 25, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 26, 7 }, .data='~', .fg=35, .bg=0},
    {.coord={ 27, 7 }, .data='|', .fg=37, .bg=40},
    {.coord={ 2, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 3, 8 }, .data='~', .fg=35, .bg=40},
    {.coord={ 4, 8 }, .data='|', .fg=31, .bg=40},
    {.coord={ 5, 8 }, .data='-', .fg=0, .bg=0},
    {.coord={ 6, 8 }, .data='-', .fg=0, .bg=0},
    {.coord={ 7, 8 }, .data='-', .fg=0, .bg=0},
    {.coord={ 8, 8 }, .data='~', .fg=35, .bg=0},
    {.coord={ 9, 8 }, .data='~', .fg=35, .bg=0},
    {.coord={ 10, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 11, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 12, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 13, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 14, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 15, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 16, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 17, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 18, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 19, 8 }, .data='~', .fg=35, .bg=40},
    {.coord={ 20, 8 }, .data='~', .fg=35, .bg=40},
    {.coord={ 21, 8 }, .data='-', .fg=0, .bg=0},
    {.coord={ 22, 8 }, .data='-', .fg=0, .bg=0},
    {.coord={ 23, 8 }, .data='-', .fg=0, .bg=0},
    {.coord={ 24, 8 }, .data='|', .fg=31, .bg=0},
    {.coord={ 25, 8 }, .data='~', .fg=35, .bg=0},
    {.coord={ 26, 8 }, .data='|', .fg=37, .bg=40},
    {.coord={ 4, 9 }, .data='|', .fg=31, .bg=0},
    {.coord={ 5, 9 }, .data='-', .fg=0, .bg=0},
    {.coord={ 6, 9 }, .data='~', .fg=35, .bg=0},
    {.coord={ 7, 9 }, .data='|', .fg=37, .bg=40},
    {.coord={ 8, 9 }, .data='|', .fg=37, .bg=40},
    {.coord={ 9, 9 }, .data='|', .fg=37, .bg=40},
    {.coord={ 19, 9 }, .data='|', .fg=37, .bg=40},
    {.coord={ 20, 9 }, .data='|', .fg=37, .bg=40},
    {.coord={ 21, 9 }, .data='|', .fg=37, .bg=40},
    {.coord={ 22, 9 }, .data='~', .fg=35, .bg=40},
    {.coord={ 23, 9 }, .data='-', .fg=0, .bg=0},
    {.coord={ 24, 9 }, .data='|', .fg=31, .bg=0},
    {.coord={ 4, 10 }, .data='|', .fg=31, .bg=0},
    {.coord={ 5, 10 }, .data='~', .fg=35, .bg=0},
    {.coord={ 23, 10 }, .data='~', .fg=35, .bg=0},
    {.coord={ 24, 10 }, .data='|', .fg=31, .bg=0},
    {.coord={ 4, 11 }, .data='|', .fg=31, .bg=0},
    {.coord={ 5, 11 }, .data='-', .fg=0, .bg=0},
    {.coord={ 6, 11 }, .data='~', .fg=35, .bg=0},
    {.coord={ 7, 11 }, .data='|', .fg=37, .bg=40},
    {.coord={ 8, 11 }, .data='|', .fg=37, .bg=40},
    {.coord={ 9, 11 }, .data='|', .fg=37, .bg=40},
    {.coord={ 12, 11 }, .data='&', .fg=31, .bg=0},
    {.coord={ 13, 11 }, .data='&', .fg=31, .bg=0},
    {.coord={ 14, 11 }, .data='&', .fg=31, .bg=0},
    {.coord={ 15, 11 }, .data='&', .fg=31, .bg=0},
    {.coord={ 16, 11 }, .data='&', .fg=31, .bg=0},
    {.coord={ 19, 11 }, .data='|', .fg=37, .bg=40},
    {.coord={ 20, 11 }, .data='|', .fg=37, .bg=40},
    {.coord={ 21, 11 }, .data='|', .fg=37, .bg=40},
    {.coord={ 22, 11 }, .data='~', .fg=35, .bg=40},
    {.coord={ 23, 11 }, .data='-', .fg=0, .bg=0},
    {.coord={ 24, 11 }, .data='|', .fg=31, .bg=0},
    {.coord={ 2, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 3, 12 }, .data='~', .fg=35, .bg=40},
    {.coord={ 4, 12 }, .data='|', .fg=31, .bg=40},
    {.coord={ 5, 12 }, .data='-', .fg=0, .bg=0},
    {.coord={ 6, 12 }, .data='-', .fg=0, .bg=0},
    {.coord={ 7, 12 }, .data='-', .fg=0, .bg=0},
    {.coord={ 8, 12 }, .data='~', .fg=35, .bg=0},
    {.coord={ 9, 12 }, .data='~', .fg=35, .bg=0},
    {.coord={ 10, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 11, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 12, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 13, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 14, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 15, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 16, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 17, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 18, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 19, 12 }, .data='~', .fg=35, .bg=40},
    {.coord={ 20, 12 }, .data='~', .fg=35, .bg=40},
    {.coord={ 21, 12 }, .data='-', .fg=0, .bg=0},
    {.coord={ 22, 12 }, .data='-', .fg=0, .bg=0},
    {.coord={ 23, 12 }, .data='-', .fg=0, .bg=0},
    {.coord={ 24, 12 }, .data='|', .fg=31, .bg=0},
    {.coord={ 25, 12 }, .data='~', .fg=35, .bg=0},
    {.coord={ 26, 12 }, .data='|', .fg=37, .bg=40},
    {.coord={ 1, 13 }, .data='|', .fg=37, .bg=40},
    {.coord={ 2, 13 }, .data='~', .fg=35, .bg=40},
    {.coord={ 3, 13 }, .data='~', .fg=35, .bg=40},
    {.coord={ 4, 13 }, .data='|', .fg=31, .bg=40},
    {.coord={ 5, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 6, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 7, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 8, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 9, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 10, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 11, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 12, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 13, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 14, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 15, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 16, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 17, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 18, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 19, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 20, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 21, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 22, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 23, 13 }, .data='-', .fg=0, .bg=0},
    {.coord={ 24, 13 }, .data='|', .fg=31, .bg=0},
    {.coord={ 25, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 26, 13 }, .data='~', .fg=35, .bg=0},
    {.coord={ 27, 13 }, .data='|', .fg=37, .bg=40},
    {.coord={ 1, 14 }, .data='~', .fg=35, .bg=0},
    {.coord={ 3, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 4, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 5, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 6, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 7, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 8, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 9, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 10, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 11, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 12, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 13, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 14, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 15, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 16, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 17, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 18, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 19, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 20, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 21, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 22, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 23, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 24, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 25, 14 }, .data='@', .fg=37, .bg=0},
    {.coord={ 27, 14 }, .data='~', .fg=35, .bg=0},
    {.coord={ 2, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 3, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 4, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 5, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 6, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 7, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 8, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 9, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 10, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 11, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 12, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 13, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 14, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 15, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 16, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 17, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 18, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 19, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 20, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 21, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 22, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 23, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 24, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 25, 15 }, .data='@', .fg=0, .bg=0},
    {.coord={ 26, 15 }, .data='@', .fg=0, .bg=0},
};
int total_pixels = sizeof(shrine_pixels)/sizeof(shrine_pixels[0]);


int update(int* shrine_x, int* shrine_y, int* dismantles, int w, int h) {
    int delay_ms = 50;

    for (int i = 0; i < total_pixels; i++) {
        int x = *shrine_x + shrine_pixels[i].coord.x;
        int y = *shrine_y + shrine_pixels[i].coord.y;
        if (x >= 0 && x < w && y >= 0 && y < h) {
            printf("\033[%d;%dH", y + 1, x + 1);
            // printf("\n[%i %i]", shrine_pixels[i].fg, shrine_pixels[i].bg);
            if (shrine_pixels[i].fg != 0 || shrine_pixels[i].bg != 0) {
                printf("\033[%d;%dm", shrine_pixels[i].fg, shrine_pixels[i].bg);
            } else {
                printf("\033[0m");
            }
            printf("%c", shrine_pixels[i].data);
            printf("\033[0m");
        }
    }
    // printf("\033[%d;1H", h);

    SLEEP_MS(delay_ms);
    if ((*shrine_y) > h/2) { (*shrine_y) -= 1; }
    else { return 0; }
    system(CLEAR);
    return 1;
}


int main() {
    int WIDTH, HEIGHT;
    get_console_size(&WIDTH, &HEIGHT);
    
    system(CLEAR);

    int shrine_x = WIDTH/2-15;
    int shrine_y = HEIGHT;
    int dismantles = 0;

    int prog = 1;
    while (prog) {
        prog = update(&shrine_x, &shrine_y, &dismantles, WIDTH, HEIGHT);
    }

    return 0;
}
