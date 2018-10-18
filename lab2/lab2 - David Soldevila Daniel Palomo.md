<style>
    h1{
        color: red;
        text-align: center;
        font-size: 50px;
        text-shadow: 2px 2px #ffcccc;
    }
    h2{
        font-weight: bold;
    }
    img {
        height: auto;
        width: 100%;
    }
    note {
        font-weight: lighter;
        background-color: rgba(100, 100, 100, 0.3);
    }
    p{
        text-align: justify;
    }
    img{
        width: 100%;
        height: auto;
    }
    .center{
        width: 50%;
        height: auto;
        margin-left: auto;
        margin-right: auto;
    }
</style>

Plots: Add comments
       Add plot tags

# Lab 2

## Introduction

In lab 2 we are going to work with Mandelbrot Set. It is a set of complex numbers. With an algorithm to compute Mandelbrot Set we are going to obvserve how to paralelize in diferent ways. Point method and Row method are the methods we are going to work with.

<div class="page">

## Task decomposition and granularity analysis
First of all, take a look at the task dependence graph of point decomposition. At left is the glaphycal version, we can obvserve that it cannot be paraleliced. However the non display (right window) version is paraleliced. Then, whe can conclude, som part of display code is causing some dependences.
<img src=./amb_col.png> 

Using row decomposition, the result is similar then the previous version. Display version still have the same problem, a data dependence, and non-display version is paralel. Obviously the granularity is bigger. Unlike point decomposition, now we are compting a big number of points wiht only one task. It is going to reduce overhead time but increasing the granularity.

<img src=./sense_col.png>

code:

```
(...)       
    for (row = 0; row < height; ++row) {
        tareador_start_task("mandelbrot row loop"); //Row decomposition
        for (col = 0; col < width; ++col) {
            tareador_start_task("mandelbrot col loop"); //Point decompsition

(...)
```

Scale cxolor and display point causes dependency in the graphycal version. It uses vars.

The granularity in point method is smaller and with 64 threads the execution time is much lower than using row method. If you have enough cpus, the point strategy is more adequate, as it allows for a lower time, however, row method it's worth if you have less cores. You will reduce overhead time.

### Code protection:

X11 use a variable, named X11_COLO_fake, with dependences. With openMP you can declare critical regions. That protects your code while parallelize from decoherences.

```
(...)
#if _DISPLAY_
            /* Scale color and display point  */
            
            long color = (long) ((k-1) * scale_color) + min_color;
            if (setup_return == EXIT_SUCCESS) {
				#pragma omp critical (X11)
				{
                XSetForeground (display, gc, color);
                XDrawPoint (display, win, gc, col, row);
				}
            }
#else
(...)
```

<div class="page">

## Point decomposition in OpenMP

With point decomposition, one task is created for every leaf of the three task sequentilly. 

The following plots are the time plot and speed up plot which shows the dependence between time or speed up and the number of cores used while the execxution of the program.

<img src="2_time.png" class="center"><img src="2_SU.png" class="center">
<note> Elapsed tiem and Speed Up plot for point decomposition version of mandel-omp.c program </note>

It appears that after 8 threads the speed-up and the execution time plots begin to normalize. Thats cause the mandelbot only saves a great portion of execution time until 8 cores, after that, we only get small bonuses. 

Speed up doesn’t increase any more, then time also doesn’t increase. We can conclude the maximum number of cores that the program can deal with is 5 in that kind of palatalization. 

<div class="page">

## Row decomposition in OpenMP

We have used parallelization pragmas in the row loop. Due to that we got a task descompostion based in rows. This helps reducing overhead time added to the executable. However that coluld translate in a greater execution time. We would see this with the analysis of scalability.



```
(...)
    #pragma omp parallel
	#pragma omp single
	#pragma omp taskloop firstprivate(row) num_tasks(800)
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            complex z, c;
(...)
```
