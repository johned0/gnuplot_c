// Image display example

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define IMAGE_X_DIMENSION           256
#define IMAGE_Y_DIMENSION           256
#define IMAGE_SIZE                  (IMAGE_X_DIMENSION * IMAGE_Y_DIMENSION)

// Declare global variables and arrays
unsigned char image [IMAGE_SIZE];

int main(void)
{
    FILE            *fp;
    h_GPC_Plot *hImage;                             // Plot object
    int i;
    unsigned char *pImage = image;

    hImage =                                        // Initialize plot
        gpc_init_image ("Image",                    // Plot title
                        IMAGE_X_DIMENSION,          // X-axis length
                        IMAGE_Y_DIMENSION,          // Y-axis length
                        0,                          // Minimum Z value
                        256,                        // Maximum Z value
                        GPC_MONOCHROME,             // Colour mode
                        GPC_KEY_ENABLE);            // Legend / key mode

    if (hImage == NULL)                             // Graph creation failed - e.g is server running ?
    {
        printf ("\nGraph creation failure\n");
        exit (1);
    }

    if((fp = fopen ("lena.dat", "rb")) == NULL )
    {
        printf ("\nThe image file (%s) could not be opened correctly", "lena.dat");
        exit (10);
    }

    for (i = 0; i < IMAGE_Y_DIMENSION; i++)
    {
        fread (pImage + (i * IMAGE_X_DIMENSION), sizeof (unsigned char), IMAGE_X_DIMENSION, fp);
    }

    if ((fclose (fp)) != 0 )
    {
        printf ("\nThe image file (%s) could not be closed correctly", "lena.dat");
        exit (10);
    }

    printf ("Image display\n");

    gpc_plot_image (hImage,                         // Graph handle
                    pImage,                         // Data array
                    "Lena");                        // Dataset title

    printf ("\nHit <CR> to continue ....\n"); getchar ();   // Clear keyboard buffer and wait for <CR>

    gpc_close (hImage);

    return (0);
}


