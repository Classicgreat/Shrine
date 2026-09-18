#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32

#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)

#else

#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)

#endif

#define MAX_DISM 50

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
} SHRINE_PIXEL;

typedef struct {
  Line l;
  int anim;
} DISMANTLE;

void Init_dism(DISMANTLE *d, int p1_x, int p1_y, int p2_x, int p2_y, int anim) {
  (*d).anim = anim;
  (*d).l.start.x = p1_x;
  (*d).l.start.y = p1_y;
  (*d).l.end.x = p2_x;
  (*d).l.end.y = p2_y;
}

SHRINE_PIXEL shrine_pixels[] = {
    {.coord = {11, 0}, .data = '\\'}, {.coord = {14, 0}, .data = '^'},
    {.coord = {17, 0}, .data = '/'},  {.coord = {12, 1}, .data = '/'},
    {.coord = {16, 1}, .data = '\\'}, {.coord = {9, 2}, .data = '\\'},
    {.coord = {12, 2}, .data = '|'},  {.coord = {16, 2}, .data = '|'},
    {.coord = {19, 2}, .data = '/'},  {.coord = {9, 3}, .data = '-'},
    {.coord = {10, 3}, .data = '-'},  {.coord = {11, 3}, .data = '|'},
    {.coord = {12, 3}, .data = '|'},  {.coord = {13, 3}, .data = '|'},
    {.coord = {14, 3}, .data = '|'},  {.coord = {15, 3}, .data = '|'},
    {.coord = {16, 3}, .data = '|'},  {.coord = {17, 3}, .data = '|'},
    {.coord = {18, 3}, .data = '-'},  {.coord = {19, 3}, .data = '-'},
    {.coord = {2, 4}, .data = '\\'},  {.coord = {3, 4}, .data = '\\'},
    {.coord = {4, 4}, .data = '_'},   {.coord = {5, 4}, .data = '_'},
    {.coord = {6, 4}, .data = '_'},   {.coord = {7, 4}, .data = '_'},
    {.coord = {8, 4}, .data = '_'},   {.coord = {9, 4}, .data = '_'},
    {.coord = {10, 4}, .data = '_'},  {.coord = {11, 4}, .data = '!'},
    {.coord = {12, 4}, .data = '@'},  {.coord = {13, 4}, .data = '@'},
    {.coord = {14, 4}, .data = '@'},  {.coord = {15, 4}, .data = '@'},
    {.coord = {16, 4}, .data = '@'},  {.coord = {17, 4}, .data = '!'},
    {.coord = {18, 4}, .data = '_'},  {.coord = {19, 4}, .data = '_'},
    {.coord = {20, 4}, .data = '_'},  {.coord = {21, 4}, .data = '_'},
    {.coord = {22, 4}, .data = '_'},  {.coord = {23, 4}, .data = '_'},
    {.coord = {24, 4}, .data = '_'},  {.coord = {25, 4}, .data = '/'},
    {.coord = {26, 4}, .data = '/'},  {.coord = {3, 5}, .data = '\\'},
    {.coord = {4, 5}, .data = '['},   {.coord = {5, 5}, .data = '['},
    {.coord = {6, 5}, .data = ']'},   {.coord = {7, 5}, .data = '['},
    {.coord = {8, 5}, .data = ']'},   {.coord = {9, 5}, .data = '['},
    {.coord = {10, 5}, .data = ']'},  {.coord = {11, 5}, .data = '['},
    {.coord = {12, 5}, .data = ']'},  {.coord = {13, 5}, .data = '['},
    {.coord = {14, 5}, .data = '@'},  {.coord = {15, 5}, .data = ']'},
    {.coord = {16, 5}, .data = '['},  {.coord = {17, 5}, .data = ']'},
    {.coord = {18, 5}, .data = '['},  {.coord = {19, 5}, .data = ']'},
    {.coord = {20, 5}, .data = '['},  {.coord = {21, 5}, .data = ']'},
    {.coord = {22, 5}, .data = '['},  {.coord = {23, 5}, .data = ']'},
    {.coord = {24, 5}, .data = ']'},  {.coord = {25, 5}, .data = '/'},
    {.coord = {1, 6}, .data = '~'},   {.coord = {3, 6}, .data = '['},
    {.coord = {4, 6}, .data = '|'},   {.coord = {5, 6}, .data = '!'},
    {.coord = {6, 6}, .data = '!'},   {.coord = {7, 6}, .data = '!'},
    {.coord = {8, 6}, .data = '!'},   {.coord = {9, 6}, .data = '!'},
    {.coord = {10, 6}, .data = '!'},  {.coord = {11, 6}, .data = '!'},
    {.coord = {12, 6}, .data = '!'},  {.coord = {13, 6}, .data = '!'},
    {.coord = {14, 6}, .data = '!'},  {.coord = {15, 6}, .data = '!'},
    {.coord = {16, 6}, .data = '!'},  {.coord = {17, 6}, .data = '!'},
    {.coord = {18, 6}, .data = '!'},  {.coord = {19, 6}, .data = '!'},
    {.coord = {20, 6}, .data = '!'},  {.coord = {21, 6}, .data = '!'},
    {.coord = {22, 6}, .data = '!'},  {.coord = {23, 6}, .data = '!'},
    {.coord = {24, 6}, .data = '|'},  {.coord = {25, 6}, .data = ']'},
    {.coord = {27, 6}, .data = '~'},  {.coord = {1, 7}, .data = '|'},
    {.coord = {2, 7}, .data = '~'},   {.coord = {3, 7}, .data = '~'},
    {.coord = {4, 7}, .data = '|'},   {.coord = {5, 7}, .data = '-'},
    {.coord = {6, 7}, .data = '-'},   {.coord = {7, 7}, .data = '-'},
    {.coord = {8, 7}, .data = '-'},   {.coord = {9, 7}, .data = '-'},
    {.coord = {10, 7}, .data = '~'},  {.coord = {11, 7}, .data = '~'},
    {.coord = {12, 7}, .data = '~'},  {.coord = {13, 7}, .data = '~'},
    {.coord = {14, 7}, .data = '~'},  {.coord = {15, 7}, .data = '~'},
    {.coord = {16, 7}, .data = '~'},  {.coord = {17, 7}, .data = '~'},
    {.coord = {18, 7}, .data = '~'},  {.coord = {19, 7}, .data = '-'},
    {.coord = {20, 7}, .data = '-'},  {.coord = {21, 7}, .data = '-'},
    {.coord = {22, 7}, .data = '-'},  {.coord = {23, 7}, .data = '-'},
    {.coord = {24, 7}, .data = '|'},  {.coord = {25, 7}, .data = '~'},
    {.coord = {26, 7}, .data = '~'},  {.coord = {27, 7}, .data = '|'},
    {.coord = {2, 8}, .data = '|'},   {.coord = {3, 8}, .data = '~'},
    {.coord = {4, 8}, .data = '|'},   {.coord = {5, 8}, .data = '-'},
    {.coord = {6, 8}, .data = '-'},   {.coord = {7, 8}, .data = '-'},
    {.coord = {8, 8}, .data = '~'},   {.coord = {9, 8}, .data = '~'},
    {.coord = {10, 8}, .data = '|'},  {.coord = {11, 8}, .data = '|'},
    {.coord = {12, 8}, .data = '|'},  {.coord = {13, 8}, .data = '|'},
    {.coord = {14, 8}, .data = '|'},  {.coord = {15, 8}, .data = '|'},
    {.coord = {16, 8}, .data = '|'},  {.coord = {17, 8}, .data = '|'},
    {.coord = {18, 8}, .data = '|'},  {.coord = {19, 8}, .data = '~'},
    {.coord = {20, 8}, .data = '~'},  {.coord = {21, 8}, .data = '-'},
    {.coord = {22, 8}, .data = '-'},  {.coord = {23, 8}, .data = '-'},
    {.coord = {24, 8}, .data = '|'},  {.coord = {25, 8}, .data = '~'},
    {.coord = {26, 8}, .data = '|'},  {.coord = {4, 9}, .data = '|'},
    {.coord = {5, 9}, .data = '-'},   {.coord = {6, 9}, .data = '~'},
    {.coord = {7, 9}, .data = '|'},   {.coord = {8, 9}, .data = '|'},
    {.coord = {9, 9}, .data = '|'},   {.coord = {19, 9}, .data = '|'},
    {.coord = {20, 9}, .data = '|'},  {.coord = {21, 9}, .data = '|'},
    {.coord = {22, 9}, .data = '~'},  {.coord = {23, 9}, .data = '-'},
    {.coord = {24, 9}, .data = '|'},  {.coord = {4, 10}, .data = '|'},
    {.coord = {5, 10}, .data = '~'},  {.coord = {23, 10}, .data = '~'},
    {.coord = {24, 10}, .data = '|'}, {.coord = {4, 11}, .data = '|'},
    {.coord = {5, 11}, .data = '-'},  {.coord = {6, 11}, .data = '~'},
    {.coord = {7, 11}, .data = '|'},  {.coord = {8, 11}, .data = '|'},
    {.coord = {9, 11}, .data = '|'},  {.coord = {12, 11}, .data = '&'},
    {.coord = {13, 11}, .data = '&'}, {.coord = {14, 11}, .data = '&'},
    {.coord = {15, 11}, .data = '&'}, {.coord = {16, 11}, .data = '&'},
    {.coord = {19, 11}, .data = '|'}, {.coord = {20, 11}, .data = '|'},
    {.coord = {21, 11}, .data = '|'}, {.coord = {22, 11}, .data = '~'},
    {.coord = {23, 11}, .data = '-'}, {.coord = {24, 11}, .data = '|'},
    {.coord = {2, 12}, .data = '|'},  {.coord = {3, 12}, .data = '~'},
    {.coord = {4, 12}, .data = '|'},  {.coord = {5, 12}, .data = '-'},
    {.coord = {6, 12}, .data = '-'},  {.coord = {7, 12}, .data = '-'},
    {.coord = {8, 12}, .data = '~'},  {.coord = {9, 12}, .data = '~'},
    {.coord = {10, 12}, .data = '|'}, {.coord = {11, 12}, .data = '|'},
    {.coord = {12, 12}, .data = '|'}, {.coord = {13, 12}, .data = '|'},
    {.coord = {14, 12}, .data = '|'}, {.coord = {15, 12}, .data = '|'},
    {.coord = {16, 12}, .data = '|'}, {.coord = {17, 12}, .data = '|'},
    {.coord = {18, 12}, .data = '|'}, {.coord = {19, 12}, .data = '~'},
    {.coord = {20, 12}, .data = '~'}, {.coord = {21, 12}, .data = '-'},
    {.coord = {22, 12}, .data = '-'}, {.coord = {23, 12}, .data = '-'},
    {.coord = {24, 12}, .data = '|'}, {.coord = {25, 12}, .data = '~'},
    {.coord = {26, 12}, .data = '|'}, {.coord = {1, 13}, .data = '|'},
    {.coord = {2, 13}, .data = '~'},  {.coord = {3, 13}, .data = '~'},
    {.coord = {4, 13}, .data = '|'},  {.coord = {5, 13}, .data = '-'},
    {.coord = {6, 13}, .data = '-'},  {.coord = {7, 13}, .data = '-'},
    {.coord = {8, 13}, .data = '-'},  {.coord = {9, 13}, .data = '-'},
    {.coord = {10, 13}, .data = '~'}, {.coord = {11, 13}, .data = '~'},
    {.coord = {12, 13}, .data = '~'}, {.coord = {13, 13}, .data = '~'},
    {.coord = {14, 13}, .data = '~'}, {.coord = {15, 13}, .data = '~'},
    {.coord = {16, 13}, .data = '~'}, {.coord = {17, 13}, .data = '~'},
    {.coord = {18, 13}, .data = '~'}, {.coord = {19, 13}, .data = '-'},
    {.coord = {20, 13}, .data = '-'}, {.coord = {21, 13}, .data = '-'},
    {.coord = {22, 13}, .data = '-'}, {.coord = {23, 13}, .data = '-'},
    {.coord = {24, 13}, .data = '|'}, {.coord = {25, 13}, .data = '~'},
    {.coord = {26, 13}, .data = '~'}, {.coord = {27, 13}, .data = '|'},
    {.coord = {1, 14}, .data = '~'},  {.coord = {3, 14}, .data = '@'},
    {.coord = {4, 14}, .data = '@'},  {.coord = {5, 14}, .data = '@'},
    {.coord = {6, 14}, .data = '@'},  {.coord = {7, 14}, .data = '@'},
    {.coord = {8, 14}, .data = '@'},  {.coord = {9, 14}, .data = '@'},
    {.coord = {10, 14}, .data = '@'}, {.coord = {11, 14}, .data = '@'},
    {.coord = {12, 14}, .data = '@'}, {.coord = {13, 14}, .data = '@'},
    {.coord = {14, 14}, .data = '@'}, {.coord = {15, 14}, .data = '@'},
    {.coord = {16, 14}, .data = '@'}, {.coord = {17, 14}, .data = '@'},
    {.coord = {18, 14}, .data = '@'}, {.coord = {19, 14}, .data = '@'},
    {.coord = {20, 14}, .data = '@'}, {.coord = {21, 14}, .data = '@'},
    {.coord = {22, 14}, .data = '@'}, {.coord = {23, 14}, .data = '@'},
    {.coord = {24, 14}, .data = '@'}, {.coord = {25, 14}, .data = '@'},
    {.coord = {27, 14}, .data = '~'}, {.coord = {2, 15}, .data = '@'},
    {.coord = {3, 15}, .data = '@'},  {.coord = {4, 15}, .data = '@'},
    {.coord = {5, 15}, .data = '@'},  {.coord = {6, 15}, .data = '@'},
    {.coord = {7, 15}, .data = '@'},  {.coord = {8, 15}, .data = '@'},
    {.coord = {9, 15}, .data = '@'},  {.coord = {10, 15}, .data = '@'},
    {.coord = {11, 15}, .data = '@'}, {.coord = {12, 15}, .data = '@'},
    {.coord = {13, 15}, .data = '@'}, {.coord = {14, 15}, .data = '@'},
    {.coord = {15, 15}, .data = '@'}, {.coord = {16, 15}, .data = '@'},
    {.coord = {17, 15}, .data = '@'}, {.coord = {18, 15}, .data = '@'},
    {.coord = {19, 15}, .data = '@'}, {.coord = {20, 15}, .data = '@'},
    {.coord = {21, 15}, .data = '@'}, {.coord = {22, 15}, .data = '@'},
    {.coord = {23, 15}, .data = '@'}, {.coord = {24, 15}, .data = '@'},
    {.coord = {25, 15}, .data = '@'}, {.coord = {26, 15}, .data = '@'}};
int total_pixels = sizeof(shrine_pixels) / sizeof(shrine_pixels[0]);
int WIDTH, HEIGHT;

int colide(int x, int y, DISMANTLE dismantle) {
  float epsilon = 0.5;
  float epsilon_2 = 0.2;

  if (dismantle.anim < 4 && dismantle.anim >= 2) {
    epsilon *= 2;
  }

  if ((float)((x - dismantle.l.start.x) * (x - dismantle.l.start.x) +
              (y - dismantle.l.start.y) * (y - dismantle.l.start.y)) <=
          epsilon * epsilon ||
      (float)((x - dismantle.l.end.x) * (x - dismantle.l.end.x) +
              (y - dismantle.l.end.y) * (y - dismantle.l.end.y)) <=
          epsilon * epsilon) {
    if (dismantle.anim >= 3 &&
        ((float)((x - dismantle.l.start.x) * (x - dismantle.l.start.x) +
                 (y - dismantle.l.start.y) * (y - dismantle.l.start.y)) <=
             epsilon_2 * epsilon_2 ||
         (float)((x - dismantle.l.end.x) * (x - dismantle.l.end.x) +
                 (y - dismantle.l.end.y) * (y - dismantle.l.end.y)) <=
             epsilon_2 * epsilon_2)) {
      return 2;
    }
    return 1;
  }

  if (dismantle.l.start.x - dismantle.l.end.x == 0) {
    if (x >= dismantle.l.start.x - epsilon &&
        x <= dismantle.l.end.x + epsilon &&
        y >= dismantle.l.start.y - epsilon &&
        y <= dismantle.l.end.y + epsilon) {
      if (dismantle.anim >= 3 && x >= dismantle.l.start.x - epsilon_2 &&
          x <= dismantle.l.end.x + epsilon_2 &&
          y >= dismantle.l.start.y - epsilon_2 &&
          y <= dismantle.l.end.y + epsilon_2) {
        return 2;
      }
      return 1;
    }
    return 0;
  }

  float k = ((float)(dismantle.l.start.y - dismantle.l.end.y)) /
            ((dismantle.l.start.x - dismantle.l.end.x));
  int b = dismantle.l.start.y - k * dismantle.l.start.x;

  if (x > dismantle.l.start.x && x < dismantle.l.end.x &&
      (float)y > ((float)(k * x + b) - epsilon) &&
      (float)y < ((float)(k * x + b) + epsilon)) {
    if (dismantle.anim >= 3 && x > dismantle.l.start.x &&
        x < dismantle.l.end.x && (float)y > ((float)(k * x + b) - epsilon_2) &&
        (float)y < ((float)(k * x + b) + epsilon_2)) {
      return 2;
    }
    return 1;
  }
  return 0;
}

int update(int *shrine_x, int *shrine_y, int *dismantles, DISMANTLE *dism,
           int w, int h) {
  int delay_ms = 70;

  erase();
  refresh();

  for (int i = 0; i < total_pixels; i++) {
    int x = *shrine_x + shrine_pixels[i].coord.x;
    int y = *shrine_y + shrine_pixels[i].coord.y;
    if (x >= 0 && x < w && y >= 0 && y < h) {
      mvprintw(y, x, "%c", shrine_pixels[i].data);
    }
  }

  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      for (int d = 0; d < (*dismantles); ++d) {
        int c = colide(x, y, dism[d]);
        if (c > 0) {
          //
          char sumb;
          switch (c) {
          case 1:
            sumb = '`';
            break;
          case 2:
            sumb = '*';
            break;
          }
          mvprintw(y, x, "%c", sumb);
          break;
          //
        }
      }
    }
  }

  for (int d = 0; d < (*dismantles); ++d) {
    if (dism[d].anim > 0) {
      --dism[d].anim;
    } else {
      Init_dism(&(dism[d]), rand() % WIDTH, rand() % HEIGHT, rand() % WIDTH,
                rand() % HEIGHT, rand() % 6 + 1);
    }
  }

  if ((*shrine_y) > h / 2) {
    (*shrine_y) -= 1;
  } else if ((*dismantles) + 1 < MAX_DISM && rand() % 20 == 0) {
    (*dismantles)++;
  }

  refresh();
  SLEEP_MS(delay_ms);
  return 1;
}

int main() {
  srand((size_t)time(NULL));

  initscr();
  noecho();
  cbreak();
  curs_set(0);

  getmaxyx(stdscr, HEIGHT, WIDTH);

  int shrine_x = WIDTH / 2 - 15;
  int shrine_y = HEIGHT;
  int dismantles = 0;
  DISMANTLE *dism;
  dism = (DISMANTLE *)malloc(MAX_DISM * sizeof(DISMANTLE));
  for (int d = 0; d < MAX_DISM; ++d) {
    Init_dism(&(dism[d]), 0, 0, 0, 0, 0);
  }

  int prog = 1;
  while (prog) {
    prog = update(&shrine_x, &shrine_y, &dismantles, dism, WIDTH, HEIGHT);
  }

  endwin();
  return 0;
}
