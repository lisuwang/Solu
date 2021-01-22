
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <maper.h>


char *img_basename(char *filename)
{
  char *newM, *part;
  int len, dex;

  len = strlen(filename);
  dex = len - 1;
  while (dex > -1)
  {
    if (filename[dex] == '/')
    {
      break;
    }
    else
    {
      dex--;
    }
  }
  dex++;
  part = &(filename[dex]);
  len = strlen(part);
  newM = (char *) malloc ((unsigned) ((len + 1) * sizeof (char)));
  strcpy(newM, part);
  return(newM);
}


IMAGE *img_alloc()
{
  IMAGE *newM;

  newM = (IMAGE *) malloc (sizeof (IMAGE));
  if (newM == NULL)
  {
    printf("IMGALLOC: Couldn't allocate image structure\n");
    return (NULL);
  }
  newM->rows = 0;
  newM->cols = 0;
  newM->name = NULL;
  newM->data = NULL;
  return (newM);
}


IMAGE *img_creat(char *name, int nr, int nc)
{
  int i, j;
  IMAGE *newM;

  newM = img_alloc();
  newM->data = (int *) malloc ((unsigned) (nr * nc * sizeof(int)));
  newM->name = img_basename(name);
  newM->rows = nr;
  newM->cols = nc;
  for (i = 0; i < nr; i++)
  {
    for (j = 0; j < nc; j++)
    {
      img_setpixel(newM, i, j, 0);
    }
  }
  return (newM);
}


void img_free(IMAGE* img)
{
  if (img->data) free ((char *) img->data);
  if (img->name) free ((char *) img->name);
  free ((char *) img);
}


void img_setpixel(IMAGE *img,int r, int c, int val)
{
  int nc;

  nc = img->cols;
  img->data[(r * nc) + c] = val;
}


int img_getpixel(IMAGE *img, int r, int c)
{
  int nc;

  nc = img->cols;
  return (img->data[(r * nc) + c]);
}


IMAGE *img_open(char *filename)
{
  IMAGE *newM;
  FILE *pgm;
  char line[512], intbuf[100], ch;
  int type, nc, nr, maxval, i, j, k, found;

  newM = img_alloc();
  if ((pgm = fopen(filename, "r")) == NULL)
  {
    printf("IMGOPEN: Couldn't open '%s'\n", filename);
    return(NULL);
  }

  newM->name = img_basename(filename);

  /*** Scan pnm type information, expecting P5 ***/
  fgets(line, 511, pgm);
  sscanf(line, "P%d", &type);
  if (type != 5 && type != 2)
  {
    printf("IMGOPEN: Only handles pgm files (type P5 or P2)\n");
    fclose(pgm);
    return(NULL);
  }

  /*** Get dimensions of pgm ***/
  fgets(line, 511, pgm);
  sscanf(line, "%d %d", &nc, &nr);
  newM->rows = nr;
  newM->cols = nc;

  /*** Get maxval ***/
  fgets(line, 511, pgm);
  sscanf(line, "%d", &maxval);
  if (maxval > 255)
  {
    printf("IMGOPEN: Only handles pgm files of 8 bits or less\n");
    fclose(pgm);
    return(NULL);
  }

  newM->data = (int *) malloc ((unsigned) (nr * nc * sizeof(int)));
  if (newM->data == NULL)
  {
    printf("IMGOPEN: Couldn't allocate space for image data\n");
    fclose(pgm);
    return(NULL);
  }

  if (type == 5)
  {

    for (i = 0; i < nr; i++) {
      for (j = 0; j < nc; j++) {
        img_setpixel(newM, i, j, fgetc(pgm));
      }
    }

  }
  else if (type == 2)
  {

    for (i = 0; i < nr; i++) {
      for (j = 0; j < nc; j++) {

        k = 0;  found = 0;
        while (!found) {
          ch = (char) fgetc(pgm);
          if (ch >= '0' && ch <= '9') {
            intbuf[k] = ch;  k++;
      } else {
            if (k != 0) {
              intbuf[k] = '\0';
              found = 1;
        }
      }
    }
        img_setpixel(newM, i, j, atoi(intbuf));

      }
    }

  } else {
    printf("IMGOPEN: Fatal impossible error\n");
    fclose(pgm);
    return (NULL);
  }

  fclose(pgm);
  return (newM);
}


int img_write(IMAGE *img, char *filename)
{
  int i, j, nr, nc, k, val;
  FILE *iop;

  nr = ROWS(img);  nc = COLS(img);
  iop = fopen(filename, "w");
  fprintf(iop, "P2\n");
  fprintf(iop, "%d %d\n", nc, nr);
  fprintf(iop, "255\n");

  k = 1;
  for (i = 0; i < nr; i++) {
    for (j = 0; j < nc; j++) {
      val = img_getpixel(img, i, j);
      if ((val < 0) || (val > 255)) {
        printf("IMG_WRITE: Found value %d at row %d col %d\n", val, i, j);
        printf("           Setting it to zero\n");
        val = 0;
      }
      if (k % nc){
        fprintf(iop, "%d ", val);
      } else {
        fprintf(iop, "%d\n", val);
      }
      k++;
    }
  }
  fprintf(iop, "\n");
  fclose(iop);
  return (1);
}

