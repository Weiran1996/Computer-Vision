/*******************************************************************/
/* vgrow      segment image by region growing                      */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{    "r=",     0,   " region pixel range "},
{    "-p",     0,   " set first pixel value in the region as label value "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  RANGE   par[2].val
#define  FLAG  par[3].val

void setlabel(int, int, int);
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */
int first;
int range;


main(argc, argv)
int argc;
char *argv[];
{
int        y,x;                     /* index counters               */
int label = 1;
  VXparse(&argc, &argv, par);       /* parse the command line       */
  
  range = 8;                        /* default range                */
  if (RANGE) range = atoi(RANGE);     /* assign input value to range  */

  Vfread(&im, IVAL);                /* read image file              */
  Vfembed(&tm, &im, 1,1,1,1);       /* image structure with border  */
  if ( im.type != VX_PBYTE ) {      /* check image format           */
     fprintf(stderr, "vtemp: no byte image data in input file\n");
     exit(-1);
  }


  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
           im.u[y][x] = 0;
     }
   }
   

   for (y = im.ylo ; y<= im.yhi ; y++) {
      for (x = im.xlo ; x <= im.xhi ; x++) {
		if (tm.u[y][x] != 0 && im.u[y][x] == 0) {
			first = tm.u[y][x];
			if (FLAG) setlabel(x, y, first);
			else {
				setlabel(x, y, label);
				if (label == 255) label = 1;
				else label++;
			}
		}
      }
   }

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to compute the local maximum */
void setlabel(int x, int y, int label) 
{						
	im.u[y][x] = label;
	if (tm.u[y][x+1] != 0 && im.u[y][x+1] == 0 && abs(tm.u[y][x+1] - first) < range)
		setlabel(x+1, y, label);
	if (tm.u[y][x-1] != 0 && im.u[y][x-1] == 0 && abs(tm.u[y][x-1] - first) < range)
		setlabel(x-1, y, label);
	if (tm.u[y+1][x] != 0 && im.u[y+1][x] == 0 && abs(tm.u[y+1][x] - first) < range)
		setlabel(x, y+1, label);
	if (tm.u[y-1][x] != 0 && im.u[y-1][x] == 0 && abs(tm.u[y-1][x] - first) < range)
		setlabel(x, y-1, label);
}
