// Gnuplot/C interface library
// Please ensure that the system path includes an entry for the gnuplot binary folder

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gnuplot_c.h"

#if defined (_MSC_VER)              // Defined by Microsoft compilers
    #include <windows.h>
    #if (GPC_DEBUG == 1)
        // #define GNUPLOT_CMD "gnuplot"                       // Do not pipe the text output to null so that it can be used for debugging
        #define GNUPLOT_CMD "gnuplot > debug.log 2>&1"      // Pipe the text output to debug.log for debugging
    #else
        #define GNUPLOT_CMD "gnuplot > /nul 2>&1"           // Pipe the text output to null for higher performance
    #endif
    #define popen _popen
    #define pclose _pclose
    #define mssleep Sleep
    // #pragma warning(disable:4996)                           // -D "_CRT_SECURE_NO_WARNINGS=1"
#else                               // Use GNU compiler
    #include <unistd.h>
    #include <time.h>
    #if (GPC_DEBUG == 1)
        #define GNUPLOT_CMD "tee debug.log | gnuplot -persist"    // Do not pipe the text output to null so that it can be used for debugging
        // #define GNUPLOT_CMD "gnuplot > debug.log"          // Pipe the text output to debug.log for debugging
    #else
        #if ((__unix) || (__MACH__))                        // Linux / OS X
            #define GNUPLOT_CMD "gnuplot > /dev/null 2>&1"  // Pipe the text output to null for higher performance
        #else
            #define GNUPLOT_CMD "gnuplot > /dev/nul 2>&1"   // Pipe the text output to null for higher performance
        #endif
    #endif
    #define mssleep(u) usleep(u*1000)
#endif

/********************************************************
* Function : gpc_init_2d
*
* Parameters :
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const double scalingMode,
*   const enum gpcPlotSignMode signMode,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the 2d plot
*
********************************************************/

h_GPC_Plot *gpc_init_2d (const char *plotTitle,
    const char *xLabel,
    const char *yLabel,
    const double scalingMode,
    const enum gpcPlotSignMode signMode,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");            // Set border behind plot

    fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);  // Set the X label
    fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);  // Set the Y label
    fprintf (plotHandle->pipe, "set grid x y\n");               // Turn on the grid
    fprintf (plotHandle->pipe, "set tics out nomirror\n");      // Tics format

    fprintf (plotHandle->pipe, "set mxtics 4\n");
    fprintf (plotHandle->pipe, "set mytics 2\n");

    if (keyMode == GPC_KEY_ENABLE)
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n"); // Legend / key location
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");              // Disable legend / key
    }

    if (scalingMode == GPC_AUTO_SCALE)                          // Set the Y axis scaling
    {
        fprintf (plotHandle->pipe, "set autoscale  yfix\n");    // Auto-scale Y axis
    }
    else
    {
        if (signMode == GPC_SIGNED)                             // Signed numbers (positive and negative)
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:%1.3le]\n", -scalingMode, scalingMode);
        }
        else if (signMode == GPC_POSITIVE)                      // 0 to +ve Max
        {
            fprintf (plotHandle->pipe, "set yrange [0.0:%1.3le]\n", scalingMode);
        }
        else                                                    // GPC_NEGAIVE - -ve Min to 0
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:0.0]\n", -scalingMode);
        }
    }

    fflush (plotHandle->pipe);                                  // flush the pipe

    return (plotHandle);
}

/********************************************************
* Function : gpc_init_2d_logscalex
*
* Parameters :
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const double scalingMode,
*   const enum gpcPlotSignMode signMode,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the 2d plot with logscale x axis
*
********************************************************/

h_GPC_Plot *gpc_init_2d_logscalex (const char *plotTitle,
    const char *xLabel,
    const char *yLabel,
    const double scalingMode,
    const enum gpcPlotSignMode signMode,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");            // Set border behind plot

    fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);  // Set the X label
    fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);  // Set the Y label
    fprintf (plotHandle->pipe, "set grid x y\n");               // Turn on the grid
    fprintf (plotHandle->pipe, "set tics out nomirror\n");      // Tics format

    fprintf (plotHandle->pipe, "set logscale x 10\n");          // Set logscale x
    fprintf (plotHandle->pipe, "set grid mxtics xtics\n");
    fprintf (plotHandle->pipe, "set mxtics\n");
    fprintf (plotHandle->pipe, "set mytics 2\n");
//    fprintf (plotHandle->pipe, "set xrange [10:10e3]\n");

    if (keyMode == GPC_KEY_ENABLE)
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n"); // Legend / key location
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");              // Disable legend / key
    }

    if (scalingMode == GPC_AUTO_SCALE)                          // Set the Y axis scaling
    {
        fprintf (plotHandle->pipe, "set autoscale  yfix\n");    // Auto-scale Y axis
    }
    else
    {
        if (signMode == GPC_SIGNED)                             // Signed numbers (positive and negative)
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:%1.3le]\n", -scalingMode, scalingMode);
        }
        else if (signMode == GPC_POSITIVE)                      // 0 to +ve Max
        {
            fprintf (plotHandle->pipe, "set yrange [0.0:%1.3le]\n", scalingMode);
        }
        else                                                    // GPC_NEGAIVE - -ve Min to 0
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:0.0]\n", -scalingMode);
        }
    }

    fflush (plotHandle->pipe);                                  // flush the pipe

    return (plotHandle);
}

/********************************************************
* Function : gpc_plot_2d
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const double *pData,
*   const int graphLength,
*   const char *pDataName,
*   const double xMin,
*   const double xMax,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value :
*   int - error flag
*
* Description : Generate the 2d plot
*
********************************************************/

int gpc_plot_2d (h_GPC_Plot *plotHandle,
    const double *pData,
    const int graphLength,
    const char *pDataName,
    const double xMin,
    const double xMax,
    const char *plotType,
    const char *pColour,
    const enum gpcNewAddGraphMode addMode)

{
    int   i;

    if (addMode == GPC_NEW)                                 // GPC_NEW
    {
        plotHandle->numberOfGraphs = 0;
    }
    else                                                    // GPC_ADD
    {
        plotHandle->numberOfGraphs++;
        if (plotHandle->numberOfGraphs >= (MAX_NUM_GRAPHS - 1)) // Check we haven't overflowed the maximum number of graphs
        {
            return (GPC_ERROR);
        }
    }

    sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].title, "%s", pDataName);
    sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].formatString, "%s lc rgb \"%s\"", plotType, pColour);

    fprintf (plotHandle->pipe, "$DATA%d << EOD\n", plotHandle->numberOfGraphs); // Write data to named data block
    for (i = 0; i < graphLength; i++)
    {
        fprintf (plotHandle->pipe, "%1.3le %1.3le\n", xMin + ((((double)i) * (xMax - xMin)) / ((double)(graphLength - 1))), pData[i]);
    }
    fprintf (plotHandle->pipe, "EOD\n");


    fprintf (plotHandle->pipe, "plot $DATA%d u 1:2 t \"%s\" w %s", 0, plotHandle->graphArray[0].title, plotHandle->graphArray[0].formatString);  // Send start of plot and first plot command
    for (i = 1; i <= plotHandle->numberOfGraphs; i++)       // Send individual plot commands
    {
        fprintf (plotHandle->pipe, ", \\\n $DATA%d u 1:2 t \"%s\" w %s", i, plotHandle->graphArray[i].title, plotHandle->graphArray[i].formatString);  // Set plot format
    }
    fprintf (plotHandle->pipe, "\n");                       // Send end of plot command


//    fprintf (plotHandle->pipe, "refresh\n");
//    fprintf (plotHandle->pipe, "reread\n");
//    fprintf (plotHandle->pipe, "set yticks auto\n");
//    fprintf (plotHandle->pipe, "replot\n");

    fflush (plotHandle->pipe);                              // Flush the pipe

#if GPC_DEBUG
    mssleep (100);                                          // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_init_2d_dual_plot
*
* Parameters :
*   const char *plotTitle,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the 2d plot
*
********************************************************/

h_GPC_Plot *gpc_init_2d_dual_plot (const char *plotTitle,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                     // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");                // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    strcpy (plotHandle->plotTitle, plotTitle);                  // Set plot title in handle
    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");            // Set border behind plot

    fprintf (plotHandle->pipe, "set multiplot layout 2, 1\n");  // Set 2 x 1 multiplot

    if (keyMode == GPC_KEY_ENABLE)
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n"); // Legend / key location
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");              // Disable legend / key
    }

    fflush (plotHandle->pipe);                                  // flush the pipe

    return (plotHandle);
}


/********************************************************
* Function : gpc_plot_2d_dual_plot
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const char *xLabel,
*   const double xMin,
*   const double xMax,
*   const double *pData1,
*   const char *pDataName1,
*   const char *plotType1,
*   const char *pColour1,
*   const char *yLabel1,
*   const double scalingMode1,
*   const enum gpcPlotSignMode signMode1,
*   const double *pData2,
*   const char *pDataName2,
*   const char *plotType2,
*   const char *pColour2,
*   const char *yLabel2,
*   const double scalingMode2,
*   const enum gpcPlotSignMode signMode2,
*   const int graphLength)
*
* Return value :
*   int - error flag
*
* Description : Generate the 2d dual plot
*
********************************************************/

int gpc_plot_2d_dual_plot (h_GPC_Plot *plotHandle,
    const char *xLabel,
    const double xMin,
    const double xMax,
    const double *pData1,
    const char *pDataName1,
    const char *plotType1,
    const char *pColour1,
    const char *yLabel1,
    const double scalingMode1,
    const enum gpcPlotSignMode signMode1,
    const double *pData2,
    const char *pDataName2,
    const char *plotType2,
    const char *pColour2,
    const char *yLabel2,
    const double scalingMode2,
    const enum gpcPlotSignMode signMode2,
    const int graphLength)

{
    int   i;

    fprintf (plotHandle->pipe, "set origin 0.0,0.0\n");
    fprintf (plotHandle->pipe, "set size 1.0,1.0\n");
    fprintf (plotHandle->pipe, "clear\n");                      // Clear the plot

    fprintf (plotHandle->pipe, "set multiplot previous\n");     // Select plot #1

// Plot #1
    fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);  // Set the X label

    fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel1); // Set the Y label
    fprintf (plotHandle->pipe, "set grid x y\n");               // Turn on the grid
    fprintf (plotHandle->pipe, "set tics out nomirror\n");      // Tics format

    fprintf (plotHandle->pipe, "set mxtics 4\n");
    fprintf (plotHandle->pipe, "set mytics 2\n");


    if (scalingMode1 == GPC_AUTO_SCALE)                         // Set the Y axis scaling
    {
        fprintf (plotHandle->pipe, "set autoscale  yfix\n");    // Auto-scale Y axis
    }
    else
    {
        if (signMode1 == GPC_SIGNED)                            // Signed numbers (positive and negative)
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:%1.3le]\n", -scalingMode1, scalingMode1);
        }
        else if (signMode1 == GPC_POSITIVE)                     // 0 to +ve Max
        {
            fprintf (plotHandle->pipe, "set yrange [0.0:%1.3le]\n", scalingMode1);
        }
        else                                                    // GPC_NEGAIVE - -ve Min to 0
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:0.0]\n", -scalingMode1);
        }
    }

    fprintf (plotHandle->pipe, "plot '-' using 1:2 title \"%s\" with %s lc rgb \"%s\"\n", pDataName1, plotType1, pColour1);  // Set plot format
    for (i = 0; i < graphLength; i++)                           // Copy the data to gnuplot
    {
        fprintf (plotHandle->pipe, "%1.3le %1.3le\n", xMin + ((((double)i) * (xMax - xMin)) / ((double)(graphLength - 1))), pData1[i]);
    }
    fprintf (plotHandle->pipe, "e\n");                          // End of dataset



// Plot #2
//    fprintf (plotHandle->pipe, "set multiplot next\n");         // Select plot #2

    fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);  // Set the X label

    fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel2); // Set the Y label
    fprintf (plotHandle->pipe, "set grid x y\n");               // Turn on the grid
    fprintf (plotHandle->pipe, "set tics out nomirror\n");      // Tics format

    fprintf (plotHandle->pipe, "set mxtics 4\n");
    fprintf (plotHandle->pipe, "set mytics 2\n");


    if (scalingMode2 == GPC_AUTO_SCALE)                         // Set the Y axis scaling
    {
        fprintf (plotHandle->pipe, "set autoscale  yfix\n");    // Auto-scale Y axis
    }
    else
    {
        if (signMode2 == GPC_SIGNED)                            // Signed numbers (positive and negative)
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:%1.3le]\n", -scalingMode2, scalingMode2);
        }
        else if (signMode2 == GPC_POSITIVE)                     // 0 to +ve Max
        {
            fprintf (plotHandle->pipe, "set yrange [0.0:%1.3le]\n", scalingMode2);
        }
        else                                                    // GPC_NEGAIVE - -ve Min to 0
        {
            fprintf (plotHandle->pipe, "set yrange [%1.3le:0.0]\n", -scalingMode2);
        }
    }

    fprintf (plotHandle->pipe, "plot '-' using 1:2 title \"%s\" with %s lc rgb \"%s\"\n", pDataName2, plotType2, pColour2);  // Set plot format
    for (i = 0; i < graphLength; i++)                           // Copy the data to gnuplot
    {
        fprintf (plotHandle->pipe, "%1.3le %1.3le\n", xMin + ((((double)i) * (xMax - xMin)) / ((double)(graphLength - 1))), pData2[i]);
    }
    fprintf (plotHandle->pipe, "e\n");                          // End of dataset


    fflush (plotHandle->pipe);                                  // Flush the pipe

#if GPC_DEBUG
    mssleep (100);                                              // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_init_xy
*
* Parameters :
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const double dimension,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the XY plot
*
********************************************************/

h_GPC_Plot *gpc_init_xy (const char *plotTitle,
    const char *xLabel,
    const char *yLabel,
    const double dimension,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    plotHandle->scalingMode = dimension;                    // Set dimension in handle
    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");        // Set border behind plot

    fprintf (plotHandle->pipe, "set xlabel \'%s\' offset graph 0.45,0.45\n", xLabel);
    fprintf (plotHandle->pipe, "set ylabel \'%s\' offset graph 0.55,-0.45\n", yLabel);
    fprintf (plotHandle->pipe, "set nokey\n");

    fprintf (plotHandle->pipe, "unset border\n");
    fprintf (plotHandle->pipe, "set xtics axis nomirror\n");
    fprintf (plotHandle->pipe, "set ytics axis nomirror\n");
    fprintf (plotHandle->pipe, "unset rtics\n");
    fprintf (plotHandle->pipe, "set zeroaxis\n");

    if (keyMode == GPC_KEY_ENABLE)
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n"); // Legend / key location
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");          // Disable legend / key
    }

    //  fprintf (plotHandle->pipe, "set clip\n");           // Enable graph clipping

    fflush (plotHandle->pipe);                              // flush the pipe

    return (plotHandle);
}


/********************************************************
* Function : gpc_plot_xy
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const ComplexRect_s *pData,
*   const int graphLength,
*   const char *pDataName,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value :
*   int - error flag
*
* Description : Generate the XY plot
*
********************************************************/

int gpc_plot_xy (h_GPC_Plot *plotHandle,
    const ComplexRect_s *pData,
    const int graphLength,
    const char *pDataName,
    const char *plotType,
    const char *pColour,
    const enum gpcNewAddGraphMode addMode)

{
    int i;

    if (addMode == GPC_NEW)
    {
        if (plotHandle->scalingMode == GPC_AUTO_SCALE)      // Set the X and Y axis scaling
        {
            fprintf (plotHandle->pipe, "set autoscale xy\n");   // Auto-scale Y axis
        }
        else
        {
            fprintf (plotHandle->pipe, "set xrange[-%1.3le:%1.3le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
            fprintf (plotHandle->pipe, "set yrange[-%1.3le:%1.3le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
        }

        if (plotHandle->numberOfGraphs != 0)
        {
            fprintf (plotHandle->pipe, "unset multiplot\n");    // If there is an existing multiplot then close it
        }
        fprintf (plotHandle->pipe, "set multiplot\n");

        plotHandle->numberOfGraphs = 1;                     // Reset the number of plots
    }
    else                                                    // GPC_ADD
    {
        plotHandle->numberOfGraphs++;                       // Increment the number of graphs
    }

    fprintf (plotHandle->pipe, "plot '-' title \"%s\" with %s lc rgb \"%s\"\n", pDataName, plotType, pColour);  // Set plot format
    for (i = 0; i < graphLength; i++)                       // Copy the data to gnuplot
    {
        fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, pData[i].imag);
    }

    fprintf (plotHandle->pipe, "e\n");                      // End of dataset

    fflush (plotHandle->pipe);                              // Flush the pipe

#if GPC_DEBUG
    mssleep (100);                                          // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_init_pz
*
* Parameters :
*   const char *plotTitle,
*   const double dimension,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the Pole-Zero plot
*
********************************************************/

h_GPC_Plot *gpc_init_pz (const char *plotTitle,
    const double dimension,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    plotHandle->scalingMode = dimension;                    // Set dimension in handle
    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");        // Set border behind plot

    fprintf (plotHandle->pipe, "set size square\n");
    fprintf (plotHandle->pipe, "set tics scale 0.75\n");

    fprintf (plotHandle->pipe, "unset border\n");
    fprintf (plotHandle->pipe, "set xtics axis nomirror\n");
    fprintf (plotHandle->pipe, "set ytics axis nomirror\n");
    fprintf (plotHandle->pipe, "unset rtics\n");
    fprintf (plotHandle->pipe, "set zeroaxis\n");

    fprintf (plotHandle->pipe, "set nokey\n");
    fprintf (plotHandle->pipe, "set xlabel \'Real\' offset graph 0.45,0.45\n");
    fprintf (plotHandle->pipe, "set ylabel \'Imaginary\' offset graph 0.55,-0.45\n");

                                                            // Define line styles for Poles, Zeros and unit circle
    fprintf (plotHandle->pipe, "set style line 1 lc rgb \"#ff0000\" lt 1 lw 1   pt 2 ps 1.5\n");    // Poles - Red
    fprintf (plotHandle->pipe, "set style line 2 lc rgb \"#0000ff\" lt 1 lw 1   pt 6 ps 1.5\n");    // Zeros - Blue
    fprintf (plotHandle->pipe, "set style line 3 lc rgb \"#000000\" lt 1 lw 0.5 pt 5 ps 1.5\n");    // Unit circle - Black

    if (keyMode == GPC_KEY_ENABLE)
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n"); // Legend / key location
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");          // Disable legend / key
    }

    fflush (plotHandle->pipe);                              // flush the pipe

    return (plotHandle);
}


/********************************************************
* Function : gpc_plot_pz
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const ComplexRect_s *pData,
*   const int graphLength,
*   const char *pDataName,
*   const enum gpcPoleZeroMode poleZeroMode,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value :
*   int - error flag
*
* Description : Generate the Pole-Zero plot
*
********************************************************/

int gpc_plot_pz (h_GPC_Plot *plotHandle,
    const ComplexRect_s *pData,
    const int graphLength,
    const char *pDataName,
    const enum gpcPoleZeroMode poleZeroMode,
    const enum gpcNewAddGraphMode addMode)

{
    int i;

    if (addMode == GPC_NEW)
    {
    if (plotHandle->scalingMode == GPC_AUTO_SCALE)          // Set the X and Y axis scaling
    {
        fprintf (plotHandle->pipe, "set xrange[-1.5:1.5]\n");
        fprintf (plotHandle->pipe, "set yrange[-1.5:1.5]\n");
    }
    else
    {
        fprintf (plotHandle->pipe, "set xrange[-%1.3le:%1.3le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
        fprintf (plotHandle->pipe, "set yrange[-%1.3le:%1.3le]\n", plotHandle->scalingMode, plotHandle->scalingMode);
    }

    if (plotHandle->numberOfGraphs != 0)
    {
        fprintf (plotHandle->pipe, "unset multiplot\n");    // If there is an existing multiplot then close it
    }
    fprintf (plotHandle->pipe, "set multiplot\n");

    plotHandle->numberOfGraphs = 1;                         // Reset the number of plots
    }
    else                                                    // GPC_ADD
    {
        plotHandle->numberOfGraphs++;                       // Increment the number of graphs
    }

    fprintf (plotHandle->pipe, "set parametric\n");         // Plot unit circle
    fprintf (plotHandle->pipe, "set angle degree\n");
    fprintf (plotHandle->pipe, "set trange [0:360]\n");
    fprintf (plotHandle->pipe, "set size square\n");
    fprintf (plotHandle->pipe, "unset parametric\n");

    switch (poleZeroMode)
    {
        case GPC_COMPLEX_POLE :
        fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 1\n", pDataName);  // Set plot format
        for (i = 0; i < graphLength; i++)                   // Copy the data to gnuplot
        {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, pData[i].imag);
        }
        break;
    case GPC_CONJUGATE_POLE :
        fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 1\n", pDataName);  // Set plot format
        for (i = 0; i < graphLength; i++)                   // Copy the data to gnuplot
        {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, pData[i].imag);
        }
        for (i = 0; i < graphLength; i++)                   // Copy the data to gnuplot - conjugate pole
        {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, -pData[i].imag);
        }
        break;
    case GPC_COMPLEX_ZERO :
        fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 2\n", pDataName);  // Set plot format
        for (i = 0; i < graphLength; i++)                   // Copy the data to gnuplot
        {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, pData[i].imag);
        }
        break;
    case GPC_CONJUGATE_ZERO :
        fprintf (plotHandle->pipe, "plot '-' title \"%s\" with points ls 2\n", pDataName);  // Set plot format
        for (i = 0; i < graphLength; i++)                   // Copy the data to gnuplot
        {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, pData[i].imag);
        }
        for (i = 0; i < graphLength; i++)                   // Copy the data to gnuplot - conjugate zero
        {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pData[i].real, -pData[i].imag);
        }
        break;
    }

    fprintf (plotHandle->pipe, "e\n");                      // End of dataset

    fflush (plotHandle->pipe);                              // Flush the pipe

#if GPC_DEBUG
    mssleep (100);                                          // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_init_spectrogram
*
* Parameters :
*   const char *plotTitle,
*   const char *xLabel,
*   const char *yLabel,
*   const int xAxisLength,
*   const int yAxisLength,
*   const double yMin,
*   const double yMax,
*   const double zMin,
*   const double zMax,
*   const char *pColourPalette,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the spectrogram plot
*
********************************************************/

h_GPC_Plot *gpc_init_spectrogram (const char *plotTitle,
    const char *xLabel,
    const char *yLabel,
    const int xAxisLength,
    const int yAxisLength,
    const double yMin,
    const double yMax,
    const double zMin,
    const double zMax,
    const char *pColourPalette,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    plotHandle->xAxisLength = xAxisLength;                  // Set X axis length in handle
    plotHandle->yAxisLength = yAxisLength;                  // Set Y axis length in handle
    plotHandle->numberOfGraphs = 0;                         // We have not plotted any columns
    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");        // Set border behind plot

    fprintf (plotHandle->pipe, "set xlabel \"%s\"\n", xLabel);
    fprintf (plotHandle->pipe, "set ylabel \"%s\"\n", yLabel);

    plotHandle->yMin = yMin;                                // Store values for min/max Y, for axis labels
    plotHandle->yMax = yMax;

                                                            // NOTE - Have to add the +/-0.5 to plot all of the pixels and
                                                            // to make peripheral pixels the same size as the internal ones
    fprintf (plotHandle->pipe, "set yrange [%1.3le:%1.3le]\n", yMin - (0.5 * ((yMax - yMin) / yAxisLength)), yMax + (0.5 * ((yMax - yMin) / yAxisLength)));  // Set length of Y axis
    fprintf (plotHandle->pipe, "set zrange [%1.3le:%1.3le] noreverse nowriteback\n", zMin, zMax);

    fprintf (plotHandle->pipe, "set tics out nomirror scale 0.75\n");   // Tics format

    // fprintf (plotHandle->pipe, "unset sur\n");              // Remove surface lines

    if (keyMode == GPC_KEY_ENABLE)                          // Legend / key location
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n");
        // fprintf (plotHandle->pipe, "set cbrange [0:255]\n");
        fprintf (plotHandle->pipe, "unset cblabel\n");
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");          // Disable legend / key
    }

    fprintf (plotHandle->pipe, "%s\n", pColourPalette);     // Colour palette

    fflush (plotHandle->pipe);                              // flush the pipe

#if GPC_DEBUG
    mssleep (100);                                          // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (plotHandle);
}


/********************************************************
* Function : gpc_plot_spectrogram
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const double *pData,
*   const char *pDataName,
*   const double xMin,
*   const double xMax)
*
* Return value :
*   int - error flag
*
* Description : Generate the spectrogram plot
*
********************************************************/

int gpc_plot_spectrogram (h_GPC_Plot *plotHandle,
    const double *pData,
    const char *pDataName,
    const double xMin,
    const double xMax)

{
    int i;

    if (pData == GPC_END_PLOT)                              // End of plot
    {
        fprintf (plotHandle->pipe, "e\n");                  // End of spectrogram dataset
        fflush (plotHandle->pipe);                          // Flush the pipe
        return (GPC_NO_ERROR);
    }

    if (plotHandle->numberOfGraphs  == 0)
    {
        fprintf (plotHandle->pipe, "set xrange [%1.3le:%1.3le]\n", xMin - (0.5 * ((xMax - xMin) / plotHandle->xAxisLength)), xMax + (0.5 * ((xMax - xMin) / plotHandle->xAxisLength)));  // Set length of X axis
        fprintf (plotHandle->pipe, "plot '-' using 1:2:3 title \"%s\" with image\n", pDataName); // Set plot format
    }

    for (i = 0; i < plotHandle->yAxisLength; i++)           // Copy the data to gnuplot
    {
        fprintf (plotHandle->pipe, "%1.3le %1.3le %1.3le\n", xMin + ((((double)plotHandle->numberOfGraphs) * (xMax - xMin)) / ((double)(plotHandle->xAxisLength - 1))),
                                                    plotHandle->yMin + ((((double)i) * (plotHandle->yMax - plotHandle->yMin)) / ((double)(plotHandle->yAxisLength - 1))),
                                                    pData[i]);
    }

    if (plotHandle->numberOfGraphs < (plotHandle->xAxisLength - 1))
    {
        fprintf (plotHandle->pipe, "\n");                   // End of isoline scan
        plotHandle->numberOfGraphs++;                       // Increment number of columns plotted
    }
    else
    {
        fprintf (plotHandle->pipe, "e\n");                  // End of spectrogram dataset
        plotHandle->numberOfGraphs = 0;                     // Reset number of columns plotted for next scan
    }

    fflush (plotHandle->pipe);                              // Flush the pipe

#if GPC_DEBUG
    mssleep (100);                                          // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_init_image
*
* Parameters :
*   const char *plotTitle,
*   const int xAxisLength,
*   const int yAxisLength,
*   const unsigned int zMin,
*   const unsigned int zMax,
*   const char *pColourPalette,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the image plot
*
********************************************************/

h_GPC_Plot *gpc_init_image (const char *plotTitle,
    const int xAxisLength,
    const int yAxisLength,
    const unsigned int zMin,
    const unsigned int zMax,
    const char *pColourPalette,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    plotHandle->xAxisLength = xAxisLength;                  // Set X axis length in handle
    plotHandle->yAxisLength = yAxisLength;                  // Set Y axis length in handle
    plotHandle->numberOfGraphs = 0;                         // We have not plotted any rows
    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot
    fprintf (plotHandle->pipe, "set lmargin at screen %4.8lf\n", PLOT_LMARGIN); // Define the margins so that the graph is 512 pixels wide
    fprintf (plotHandle->pipe, "set rmargin at screen %4.8lf\n", PLOT_RMARGIN);
    fprintf (plotHandle->pipe, "set border back\n");        // Set border behind plot

                                                            // NOTE - Have to add the +/-0.5 to plot all of the pixels and
                                                            // to make peripheral pixels the same size as the internal ones
    fprintf (plotHandle->pipe, "set xrange [-0.5:%1.3le]\n", ((double)xAxisLength)-0.5);   // Set length of X axis
    fprintf (plotHandle->pipe, "set yrange [%1.3le:-0.5]\n", ((double)yAxisLength)-0.5);   // Set length of Y axis
    if ((zMin == GPC_IMG_AUTO_SCALE) && (zMax == GPC_IMG_AUTO_SCALE))
    {
        fprintf (plotHandle->pipe, "set autoscale  zfix\n");    // Auto-scale Z axis
    }
    else
    {
        fprintf (plotHandle->pipe, "set zrange [%u:%u]\n", zMin, zMax);
        fprintf (plotHandle->pipe, "set cbrange [%u:%u]\n", zMin, zMax);
    }

    fprintf (plotHandle->pipe, "set tics out nomirror scale 0.75\n");  // Tics format

    fprintf (plotHandle->pipe, "set view map\n");           // Set splot map view
    fprintf (plotHandle->pipe, "set size square\n");
    fprintf (plotHandle->pipe, "unset title\n");

    if (keyMode == GPC_KEY_ENABLE)                          // Legend / key location
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n");
        //    fprintf (plotHandle->pipe, "set cbrange [0:255]\n");
        fprintf (plotHandle->pipe, "unset cblabel\n");
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");          // Disable legend / key
    }

    fprintf (plotHandle->pipe, "%s\n", pColourPalette);     // Colour palette

    fflush (plotHandle->pipe);                              // flush the pipe

#if GPC_DEBUG
    mssleep (100);                                          // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (plotHandle);
}


/********************************************************
* Function : gpc_plot_image
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const unsigned int *pData,
*   const char *pDataName)
*
* Return value :
*   int - error flag
*
* Description : Generate the image plot
*
********************************************************/

int gpc_plot_image (h_GPC_Plot *plotHandle,
    const unsigned int *pData,
    const char *pDataName)

{
    int i, j;

    fprintf (plotHandle->pipe, "splot '-' matrix title \"%s\" with image\n", pDataName); // Set plot format

    for (j = 0; j < plotHandle->yAxisLength; j++)           // For every row
    {
        for (i = 0; i < plotHandle->xAxisLength; i++)       // For every pixel in the row
        {
          fprintf (plotHandle->pipe, "%u ", pData[i + (j * plotHandle->xAxisLength)]);
        }
        fprintf (plotHandle->pipe, "\n");                   // End of isoline scan
    }
    fprintf (plotHandle->pipe, "\ne\ne\n");                 // End of spectrogram dataset

    fflush (plotHandle->pipe);                              // Flush the pipe

#if GPC_DEBUG
    mssleep (100);                                         // Slow down output so that pipe doesn't overflow when logging results
#endif

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_init_polar
*
* Parameters :
*   const char *plotTitle,
*   const double gMin,
*   const double gMax,
*   const enum gpcKeyMode keyMode)
*
* Return value :
*   h_GPC_Plot - Plot handle
*
* Description : Initialize the polar plot
*
********************************************************/

h_GPC_Plot *gpc_init_polar (const char *plotTitle,
    const double gMin,
    const double gMax,
    const enum gpcKeyMode keyMode)

{
    h_GPC_Plot *plotHandle;                                 // Create plot

    plotHandle = (h_GPC_Plot*) malloc (sizeof (h_GPC_Plot));    // Malloc plot and check for error
    if (plotHandle == NULL)
    {
        return (plotHandle);
    }

    plotHandle->pipe = popen (GNUPLOT_CMD, "w");            // Open pipe to Gnuplot and check for error
    if (plotHandle->pipe == NULL)
    {
        printf ("\n\nGnuplot/C Error\n");
        printf ("Gnuplot/C can not find the required Gnuplot executable.\n");
        printf ("Please ensure you have installed Gnuplot from (http://www.gnuplot.info)\n");
        printf ("and that the executable program is located in the system PATH.\n\n");

        free (plotHandle);
        return (plotHandle);
    }

    strcpy (plotHandle->plotTitle, plotTitle);              // Set plot title in handle

    fprintf (plotHandle->pipe, "set term wxt 0 title \"%s\" size %u, %u\n", plotHandle->plotTitle, CANVAS_WIDTH, CANVAS_HEIGHT); // Set the plot

    fprintf (plotHandle->pipe, "set polar\n");
    fprintf (plotHandle->pipe, "set angle degree\n");
    fprintf (plotHandle->pipe, "set size ratio 1\n");
    fprintf (plotHandle->pipe, "set tmargin 3\n");
    fprintf (plotHandle->pipe, "set bmargin 3\n");
    fprintf (plotHandle->pipe, "set key out vert nobox\n");

    fprintf (plotHandle->pipe, "set style line 1 lc rgb 'gray80' lt -1\n");
    fprintf (plotHandle->pipe, "set grid polar ls 1\n");

    fprintf (plotHandle->pipe, "unset border\n");
    fprintf (plotHandle->pipe, "unset xtics\n");
    fprintf (plotHandle->pipe, "unset ytics\n");

    fprintf (plotHandle->pipe, "minGain=%d\n", (int)gMin);                          // # Minimum gain
    fprintf (plotHandle->pipe, "f_maxGain=%lf\n", gMax);                            // # Maximum gain
    fprintf (plotHandle->pipe, "tickstep = 10\n");                                  // # Ticks every 10 dB
    fprintf (plotHandle->pipe, "numticks = %u\n", (unsigned)((gMax - gMin) / 10.)); // # numticks = r / tickstep :: Don't use divide because numticks is used in for loop and doesn't work
    fprintf (plotHandle->pipe, "f_numticks = %lf\n", (gMax - gMin) / 10.);          // # Floating point numticks


    fprintf (plotHandle->pipe, "set rrange [minGain:0]\n");
    fprintf (plotHandle->pipe, "set rtics tickstep format '' scale 0\n");

    fprintf (plotHandle->pipe, "set label '0°' center at first 0, first -minGain*1.05\n");
    fprintf (plotHandle->pipe, "set label '90°' right at first -minGain*1.05, 0\n");
    fprintf (plotHandle->pipe, "set label '180°' center at first 0, first minGain*1.05\n");
    fprintf (plotHandle->pipe, "set label '270°' left at first minGain*1.05, 0\n");

    fprintf (plotHandle->pipe, "set for [i=.1:numticks] label at first minGain*0.001, first minGain*((i/f_numticks) + 0.007) sprintf(\"%%d dB\", minGain+(i*10))\n");
    fprintf (plotHandle->pipe, "unset raxis\n");

    if (keyMode == GPC_KEY_ENABLE)
    {
        fprintf (plotHandle->pipe, "set key out vert nobox\n"); // Legend / key location
    }
    else
    {
        fprintf (plotHandle->pipe, "unset key\n");              // Disable legend / key
    }

    fflush (plotHandle->pipe);                                  // flush the pipe

    plotHandle->xMin = gMin;
    plotHandle->xMax = gMax;

    return (plotHandle);
}


/********************************************************
* Function : gpc_plot_polar
*
* Parameters :
*   h_GPC_Plot *plotHandle,
*   const double *pAngles,
*   const double *pGains,
*   const int numAngles,
*   const char *pDataName,
*   const char *plotType,
*   const char *pColour,
*   const enum gpcNewAddGraphMode addMode)
*
* Return value :
*   int - error flag
*
* Description : Generate the polar plot
*
********************************************************/

int gpc_plot_polar (h_GPC_Plot *plotHandle,
    const double *pAngles,
    const double *pGains,
    const int numAngles,
    const char *pDataName,
    const char *plotType,
    const char *pColour,
    const enum gpcNewAddGraphMode addMode)

{
    int   i;

    if (addMode == GPC_NEW)                                 // GPC_NEW
    {
        plotHandle->numberOfGraphs = 0;
    }
    else                                                    // GPC_ADD
    {
        plotHandle->numberOfGraphs++;
        if (plotHandle->numberOfGraphs >= (MAX_NUM_GRAPHS - 1)) // Check we haven't overflowed the maximum number of graphs
        {
            return (GPC_ERROR);
        }
    }

    sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].title, "%s", pDataName);
    sprintf (plotHandle->graphArray[plotHandle->numberOfGraphs].formatString, "%s lc rgb \"%s\"", plotType, pColour);


    fprintf (plotHandle->pipe, "$DATA%d << EOD\n", plotHandle->numberOfGraphs); // Write data to named data block
    for (i = 0; i < numAngles; i++)
    {
        if (pGains[i] < plotHandle->xMin) {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pAngles[i], plotHandle->xMin);
        }
        else if (pGains[i] > plotHandle->xMax) {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pAngles[i], plotHandle->xMax);
        }
        else {
            fprintf (plotHandle->pipe, "%1.3le %1.3le\n", pAngles[i], pGains[i]);
        }
    }
    fprintf (plotHandle->pipe, "EOD\n");


    fprintf (plotHandle->pipe, "plot $DATA%d u (-$1+90.):($2-f_maxGain) t \"%s\" w %s", 0, plotHandle->graphArray[0].title, plotHandle->graphArray[0].formatString);  // Send start of plot and first plot command
    for (i = 1; i <= plotHandle->numberOfGraphs; i++)       // Send individual plot commands
    {
        fprintf (plotHandle->pipe, ", \\\n $DATA%d u (-$1+90.):($2-f_maxGain) t \"%s\" w %s", i, plotHandle->graphArray[i].title, plotHandle->graphArray[i].formatString);  // Set plot format
    }
    fprintf (plotHandle->pipe, "\n");                       // Send end of plot command

    fflush (plotHandle->pipe);                              // Flush the pipe

    return (GPC_NO_ERROR);
}


/********************************************************
* Function : gpc_close
*
* Parameters :
*   h_GPC_Plot *plotHandle
*
* Return value :
*   void
*
* Description : Delete the plot and temporary files
*   associated with the handle.
*
********************************************************/

void gpc_close (h_GPC_Plot *plotHandle)
{
    mssleep (100);                                          // Wait - ensures pipes flushed

    fprintf (plotHandle->pipe, "exit\n");                   // Close GNUPlot
    pclose (plotHandle->pipe);                              // Close the pipe to Gnuplot

    free (plotHandle);                                      // Free the plot
}

