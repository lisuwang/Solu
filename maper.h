#ifndef MAPER_H
#define MAPER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <opencv/cv.h>

#define ROWS(img)  ((img)->rows)
#define COLS(img)  ((img)->cols)
#define NAME(img)   ((img)->name)

typedef struct
{
  char *name;
  int rows, cols;
  int *data;
} IMAGE;

char *img_basename(char *filename);
IMAGE *img_alloc();
IMAGE *img_creat(char *name, int nr, int nc);
void img_free(IMAGE* img);
void img_setpixel(IMAGE *img,int r, int c, int val);
int img_getpixel(IMAGE *img, int r, int c);
IMAGE *img_open(char *filename);
int img_write(IMAGE *img, char *filename);
#endif // MAPER_H
