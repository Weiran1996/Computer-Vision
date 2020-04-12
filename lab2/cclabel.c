/*******************************************************************/
/* vtemp      Compute local max operation on a single byte image   */
/*******************************************************************/

#include "VisXV4.h"           /* VisionX structure include file    */
#include "Vutil.h"            /* VisionX utility header files      */

VXparam_t par[] =             /* command line structure            */
{ /* prefix, value,   description                         */   
{    "if=",    0,   " input file  vtemp: local max filter "},
{    "of=",    0,   " output file "},
{     0,       0,   0}  /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
void dfs(int, int, int); 
Vfstruct (im);                      /* i/o image structure          */
Vfstruct (tm);                      /* temp image structure         */


main(argc, argv)

int argc;
char *argv[];
{
int label = 1;
int        y,x;                     /* index counters               */
  VXparse(&argc, &argv, par);       /* parse the command line       */

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

  for (y = im.ylo ; y <= im.yhi ; y++) {
     for (x = im.xlo; x <= im.xhi; x++)  {
	if(tm.u[y][x] != 0){
           dfs(x, y, label);
		label++;
	}
     }
   }

   Vfwrite(&im, OVAL);             /* write image file                */
   exit(0);
}

/* function to compute the local maximum */
void dfs(int x, int y, int label) 
{	
if(tm.u[y][x] == 0 || x<im.xlo || x>im.xhi || y<im.ylo || y>im.yhi)
return;
else{
im.u[y][x] = label;
tm.u[y][x] = 0;
dfs(x+1, y, label);
dfs(x-1, y, label);
dfs(x, y+1, label);
dfs(x, y-1, label);
}			

}
