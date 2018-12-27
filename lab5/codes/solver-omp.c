#include "heat.h"

/*
 * Function to copy one matrix into another
 */

void copy_mat (double *u, double *v, unsigned sizex, unsigned sizey)
{
    for (int i=1; i<=sizex-2; i++)
        for (int j=1; j<=sizey-2; j++) 
            v[ i*sizey+j ] = u[ i*sizey+j ];
}

/*
 * Blocked Jacobi solver: one iteration step
 */

double relax_jacobi (double *u, double *utmp, unsigned sizex, unsigned sizey)
{
    double diff, sum=0.0;
	
   
     #pragma omp parallel private(diff) reduction (+:sum) 
     {
		int funci= omp_get_thread_num();
		int thre = omp_get_num_threads();
		int i_start = lowerb(funci, thre, sizex);
		int i_end = upperb(funci, thre, sizex);
		
		for (int i=max(1, i_start); i<= min(sizex-2, i_end); i++) {
			for (int j=1; j<= sizey-2; j++) {
				utmp[i*sizey+j]= 0.25 * ( u[ i*sizey 	 + (j-1) ]+  	 // left
										  u[ i*sizey     + (j+1) ]+  // right
										  u[ (i-1)*sizey + j     ]+  // top
										  u[ (i+1)*sizey + j     ]); // bottom
		
				diff = utmp[i*sizey+j] - u[i*sizey + j];
				sum += diff * diff; 
		
			}
		}
    }
    return sum;
}

/*
 * Blocked Gauss-Seidel solver: one iteration step
 */
double relax_gauss (double *u, unsigned sizex, unsigned sizey)
{
    double unew, diff, sum=0.0;
    
    int chunkx = sizex/omp_get_num_threads();
    int chunky = sizey/omp_get_num_threads();

    int howmany=4;
	  #pragma omp parallel for reduction(+:sum) ordered(2)
	  for(int bx = 0; bx < sizex/chunkx; ++bx){
		  for(int by = 0; by < sizey/chunky; by++){
			  int i_start = bx*chunkx;
			  int i_end = chunkx*(1+bx);
			  int j_start = by*chunky;
			  int j_end = chunky*(1+by);
			  #pragma omp ordered depend(sink:bx,by-1) depend(sink:bx-1, by)
			  {
				  for (int i=max(1, i_start); i<= min(sizex-2, i_end); i++) {
						for (int j=max(1, j_start); j<= min(sizey-2, j_end); j++) {
							unew= 0.25 * ( u[ i*sizey	+ (j-1) ]+  // left
									   u[ i*sizey	+ (j+1) ]+  // right
									   u[ (i-1)*sizey	+ j ]+  // top
									   u[ (i+1)*sizey	+ j ]); // bottom
							diff = unew - u[i*sizey+ j];
							sum += diff * diff; 
							u[i*sizey+j]=unew;
						}
				  }
			  }
		  }
	  }	
			
    return sum;
}
