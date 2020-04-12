/*********************************************************************/
/* vtpeak:     Threshold image between two most sig. hgram peaks     */
/*********************************************************************/

#include "VisXV4.h"          /* VisionX structure include file       */
#include "Vutil.h"           /* VisionX utility header files         */

VXparam_t par[] =            /* command line structure               */
{
{  "if=",   0,   " input file, vtpeak: threshold between hgram peaks"},
{  "of=",   0,   " output file "},
{  "d=",    0,   " min dist between hgram peaks (default 10)"},
{  "-v",    0,   "(verbose) print threshold information"},
{   0,      0,   0} /* list termination */
};
#define  IVAL   par[0].val
#define  OVAL   par[1].val
#define  DVAL   par[2].val
#define  VFLAG  par[3].val

main(argc, argv)
int argc;
char *argv[];
{

    Vfstruct (im);                 /* input image structure          */
    int y,x;                       /* index counters                 */
    int i;
    int j;

    int hist[256];                 /* histogram bins                 */
    int thresh;                    /* threshold                      */
    int maxbin;                    /* maximum histogram bin          */
    int nxtbin;                    /* second maximum bin             */
    int minbin;                    /* minumim histogram bin          */
    int maxa, maxb;       /* second maximum bin above/below maxbin   */
    int dist;                   /* minimum distance between maxima   */
    int cycle = 100000;
    int sum = 0;
    int sum1 = 0;
    int sum2 = 0;
    int num1 = 0;
    int num2 = 0;
    int avg1=0;
    int avg2 = 0;
			     
    VXparse(&argc, &argv, par);    /* parse the command line         */

    dist = 10;                    /* default dist */
    if (DVAL) dist = atoi(DVAL);  /* if d= was specified, get value */
    if (dist < 0 || dist > 255) {
	fprintf(stderr, "d= must be between 0 and 255\nUsing d=10\n");
        dist = 10;
    }

    while ( Vfread( &im, IVAL) ) {
        if ( im.type != VX_PBYTE ) {
              fprintf (stderr, "error: image not byte type\n");
              exit (1);
        }

        /* clear the histogram */
        for (i = 0; i < 256; i++) hist[i] = 0;
 
        /* compute the histogram */
        for (y = im.ylo; y <= im.yhi; y++){
            for (x = im.xlo; x <= im.xhi; x++){
                 hist[im.u[y][x]]++;
                 sum += im.u[y][x];
}
}
thresh = sum /(im.yhi*im.xhi);
  
         /* compute the threshold */
for(i=1; i<cycle; i++){
sum1 = 0;
sum2 = 0;
num1 = 0;
num2 = 0;
for(j=0; j<thresh; j++){
sum1 += j*hist[j];
num1 += hist[j];
}

for(j=thresh; j<=255; j++){
sum2 += j*hist[j];
num2 += hist[j];
}

if(num1 == 0)
avg1 = 0;
else
avg1 = sum1/num1;

if(num2 == 0)
avg2 = 0;
else
avg2 = sum2/num2;

if(thresh == (avg1+avg2)/2)
break;
else
thresh == (avg1+avg2)/2;

}
  

  
        /* apply the threshold */
        for (y = im.ylo; y <= im.yhi; y++) {
            for (x = im.xlo; x <= im.xhi; x++) {
                 if (im.u[y][x] >= thresh) im.u[y][x] = 255;
                 else                      im.u[y][x] = 0;
            }
        }
  
        Vfwrite( &im, OVAL);
    } /* end of every frame section */
    exit(0);
}
