/*
 *  A simple nqueens program that counts the number of possible solutions
 *  to the nqueens problem.
 */

#include <alloca.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <memory.h>
#include <sys/time.h>
#include <omp.h>


#ifdef _TAREADOR_
#include <tareador.h>
#endif

double getusec_() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return ((double)time.tv_sec * (double)1e6 + (double)time.tv_usec);
}

#define START_COUNT_TIME stamp = getusec_();
#define STOP_COUNT_TIME stamp = getusec_() - stamp;\
                        stamp = stamp/1e6;

// defaults for arguments
#define BOARD_SIZE 12
#define CUTOFF 8

int size, cutoff;

char *a;                // Solution being explored
char *sol = NULL;       // First solution found
int  sol_count = 0;     // Total number of solutions found

void help_message(char *prog_name) {
    fprintf(stderr, "%s usage:\n", prog_name);
    fprintf(stderr, "  Parameters:\n");
    fprintf(stderr, "    -n<num> : board dimensions (num x num)\n");
    fprintf(stderr, "    -c<num> : task recursivity cutoff\n");
    fprintf(stderr, "  Help:\n");
    fprintf(stderr, "    -h : print this help message\n");
}

void process_args(int argc, char *argv[], int *size, int *cutoff) {
    int i;

    /* process command line arguments */
    // resort to defaults if no arguments are provided
    *size = BOARD_SIZE;
    *cutoff = CUTOFF;
    for (i=1; i<argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
              case 'n': /* set array size parameter */
                if (sscanf(&argv[i][2], "%d", size) != 1 || *size < 1)
                {
                    fprintf(stderr, "%s Warning: argument of -n option unacceptable: %s\n", argv[0], &argv[i][2]);
                    exit(0);
                }
                break;
              case 'c': /* set cuttoff recursivity parameter */
                if (sscanf(&argv[i][2], "%d", cutoff) != 1 || *cutoff< 0)
                {
                    fprintf(stderr, "%s Warning: argument of -c option unacceptable: %s\n", argv[0], &argv[i][2]);
                    exit(0);
                }
                break;
              case 'h': /* print help message */
                help_message(argv[0]);
                exit(0);
                break;
              default:
                fprintf(stderr, "%s: Warning: command-line option ignored: %s\n", argv[0], argv[i]);
                exit(0);
                break;
            }
        } else {
            fprintf(stderr, "%s: Warning: command-line option ignored: %s\n",
                    argv[0], argv[i]);
            exit(0);
        }
    }
}

/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */
int ok(int n, char *a) {
     int i, j;
     char p, q;

     for (i = 0; i < n; i++) {
          p = a[i];

          for (j = i + 1; j < n; j++) {
               q = a[j];
               if (q == p || q == p - (j - i) || q == p + (j - i))
                    return 0;
          }
     }
     return 1;
}

/*
 * <a> is an array of <j> numbers.  The entries of <a> contain
 * queen positions already set.  If there is any extension of <a>
 * to a complete <n> queen setting, puts one of these queen
 * settings (allocated from the heap) in <sol>.  Counts all complete
 * solutions to the problem and updates <sol_count>.
 * Does not side-effect <a>.
 */

#ifdef _TAREADOR_
char stringMessage[256];
#endif

void nqueens(int n, int j, char *a, int d) {
	int i;
	if (n == j) {
	#ifdef _TAREADOR_
		tareador_start_task("Solution");
	#endif
		/* put good solution in heap. */
	#pragma omp critical
	{
		if( sol == NULL ) {
			sol = malloc(n * sizeof(char));
			memcpy(sol, a, n * sizeof(char));
		}
		sol_count += 1;
	}
	#ifdef _TAREADOR_
	tareador_end_task("Solution");
	#endif
	} else {
		/* try each possible position for queen <j> */
		for ( i=0 ; i < n ; i++ ) {
			#ifdef _TAREADOR_
				sprintf(stringMessage,"Trying [%d,%d]",j,i);
				tareador_start_task(stringMessage);
			#endif
			char* b = alloca(sizeof(a));
			memcpy(b, a, sizeof(a));
			#pragma omp task final (d<CUTOFF) mergeable
			{
				b[j] = (char) i;
				if (ok(j + 1, b)) {
					nqueens(n, j + 1, b,d+1);
				} 
			}   
			#ifdef _TAREADOR_
				tareador_end_task(stringMessage);
			#endif
		}
		#pragma omp taskwait 
	}
}


int main(int argc, char *argv[])
{
     int i;

     process_args(argc, argv, &size, &cutoff);

     a = alloca(size * sizeof(char));

     printf( "Parameters:\n" );
     printf( "  board size:              %d\n", size );
     printf( "  recursion cutoff level:  %d\n", cutoff );
     printf( "\n" ) ;
     fflush( stdout );

#ifndef _TAREADOR_
     double stamp;
#endif

#ifdef _TAREADOR_
     tareador_ON ();
#else
     START_COUNT_TIME;
#endif
        #pragma omp parallel
        #pragma omp single
     nqueens(size, 0, a,0);

#ifdef _TAREADOR_
     tareador_OFF ();
#else
     STOP_COUNT_TIME;
#endif

     if (sol != NULL) {
          if (!ok(size, sol)) {
               printf("RESULT IS BAD!!!\n");
               exit(1);
          }
          printf("one solution: ");
          for (i = 0; i < size; i++)
               printf("%2d ", sol[i]);
          printf("\n\n");
          printf("number of solutions: %d\n\n", sol_count);
     } else
          printf("No possible result!\n\n");

#ifndef _TAREADOR_
     printf ("Solution Count Time is %0.6f seconds\n", stamp);
#endif

     return 0;
}
