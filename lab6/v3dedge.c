/****************************************************************/
/* Example VisX4 program v3df                                   */
/*            Compute function on a 3D image structure          */
/* Syntax:                                                      */
/*        v3df if=infile of=outfile [-v]                        */
/****************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{    "if=",    0,   " input file  v3dmean: compute local mean"},
{    "of=",    0,   " output file "},
{    "-v",     0,   " visible flag"},
{     0,       0,    0}
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  VFLAG  par[2].val

int
main(argc, argv)
int argc;
char *argv[];
{
V3fstruct (im);
V3fstruct (tm);
int        x,y,z;               /* index counters                 */
int        xx,yy,zz;            /* window index counters          */
int        sum;
int        temp;
float grad;

int gy[3][3][3], gx[3][3][3], gz[3][3][3];
int hx[3] = {1,2,1}, hy[3] = {1,2,1}, hz[3] = {1,2,1};
int hpx[3]={1,0,-1}, hpy[3]={1,0,-1},hpz[3]={1,0,-1};
int sumx,sumy,sumz;
int m,n,k;

    VXparse(&argc, &argv, par); /* parse the command line         */

    V3fread( &im, IVAL);        /* read 3D image                  */
    if ( im.type != VX_PBYTE || im.chan != 1) { /* check  format  */
       fprintf (stderr, "image not byte type or single channel\n");
       exit (1);
    }   
   
    V3fembed(&tm, &im, 1,1,1,1,1,1); /* temp image copy with border */
    if(VFLAG){
       fprintf(stderr,"bbx is %f %f %f %f %f %f\n", im.bbx[0],
                 im.bbx[1],im.bbx[2],im.bbx[3],im.bbx[4],im.bbx[5]);
    }

for(m=0; m<=2; m++)
   for(n=0; n<=2; n++)
      for(k=0; k<=2;k++){
	   gx[m][n][k] = hpx[m]*hy[n]*hz[k];
	   gy[m][n][k] = hx[m]*hpy[n]*hz[k];
	   gz[m][n][k] = hx[m]*hy[n]*hpz[k];
}

    for (z = im.zlo; z <= im.zhi; z++) {/* for all pixels */
      for (y = im.ylo; y <= im.yhi; y++) {
        for (x = im.xlo; x <= im.xhi; x++) {
             int arr[26];
	     int i = 0;
             int j=0;
             for (zz = -1; zz <= 1; zz++) {/* compute the function */
               for (yy = -1; yy <= 1; yy++) {
                 for (xx = -1; xx <= 1; xx++) {
                     arr[i] = tm.u[z+zz][y+yy][x+xx];
		     i++;
                 }   
               }   
             }   
             for(i=0; i<27; i++){
		for(j=0; j<27; j++){
		    if(arr[i] > arr[j]){
			temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		     }
		}
	   }
		tm.u[z][y][x] = arr[13];

        }
      }
   }

    for (z = im.zlo; z <= im.zhi; z++) {/* for all pixels */
      for (y = im.ylo; y <= im.yhi; y++) {
        for (x = im.xlo; x <= im.xhi; x++) {
             sumx=0, sumy=0, sumz=0;
             for (zz = -1; zz <= 1; zz++) {/* compute the function */
               for (yy = -1; yy <= 1; yy++) {
                 for (xx = -1; xx <= 1; xx++) {
                     sumx += gx[zz+1][yy+1][xx+1]*tm.u[z-zz][y-yy][x-xx];
		     sumy += gy[zz+1][yy+1][xx+1]*tm.u[z-zz][y-yy][x-xx];
                     sumz += gz[zz+1][yy+1][xx+1]*tm.u[z-zz][y-yy][x-xx];
			
                 }   
               }   
             }   
             sumx/=16; sumy/=16; sumz/=16;
	     temp = sqrt(sumx*sumx + sumy*sumy + sumz*sumz);
	     im.u[z][y][x] = temp > 70?255:50;
        }
      }
   }
   V3fwrite (&im, OVAL);
   exit(0);
}
