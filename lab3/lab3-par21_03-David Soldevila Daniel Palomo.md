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
    h3{
        font-weight: bold;
    }
    img {
        height: auto;
        width: 100%;
    }
    note {
        font-weight: lighter;
        background-color: rgba(100, 100, 100, 0.3);
        font-style: italic;
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
        display: block;
    }
    .mini{
        width: 30%;
        height: auto;
        margin-left: auto;
        margin-right: auto;
        display: block;
    }
</style>

# LAB 3

## Introduction

In this laboratory we are going to study the implementation of "divide and conquer" strategy using OpneMP paralization. **(ha de continuar)**

<div class="page">

## Task decomposition analysis for Mergesort

### Divide and conquer

### Task decomposition analysis with Tareador

```
ADD CODE
```
**link code**

We have added a tareador task in each of the recursive tasks, to fully visualize the possible parallelizations of the code.

<img src="tareador_captures/1c.png">
<note>Trace of multisort-tareador using 1 core</note>

<img src="tareador_captures/2c.png">
<note>Trace of multisort-tareador using 2 core</note>

<img src="tareador_captures/4c.png">
<note>Trace of multisort-tareador using 4 core</note>

<img src="tareador_captures/8c.png">
<note>Trace of multisort-tareador using 8 core</note>

<img src="tareador_captures/16c.png">
<note>Trace of multisort-tareador using 16 core</note>

<img src="tareador_captures/32c.png">
<note>Trace of multisort-tareador using 32 core</note>

<img src="tareador_captures/64c.png">
<note>Trace of multisort-tareador using 64 core</note>


|NUM CPUs|time (ns)|SPEED UP|EFFICIENCY|
|---|---|---|---|
|1|20334411001|1|0.999360066616823|
|2|10173716001|1.99872013323365|0.99938619627572|
|4|5086725001|3.99754478510288|0.99938619627572|
|8|2550595001|7.97241858979085|0.996552323723856|
|16|1289922001|15.7640624667507|0.985253904171916|
|32|1289909001|15.7642213406029|0.492631916893841|
|64|1289909001|15.7642213406029|0.246315958446921|

The following pots represents number of cores vs. plot of time, speed up and efficiency, respectively.

<img src="tareador_captures/cores_vs_time.png">
<note>Number of cores vs. execution time plot</note>

<img src="tareador_captures/cores_vs_speedup.png">
<note>Number of cores vs. speed up plot</note>

<img src="tareador_captures/cores_vs_efficiency.png">
<note>Number of cores vs. efficiency plot</note>

The multisort program parallizes ideally untill 16 cores, when , the dependences between the multisort marge calls, as seen in the tareador capture of the tasks, doesn't allow for more. The efficiency drops going further than 16 threads (i.e fig: efficiency plot and 32,64 captures) Proving that adding more CPUs is pointless.

<div class="page">

## Shared-memory parallelization with OpenMP tasks

### Task cut–off mechanism

Tree stragey code:
```
void merge(long n, T left[n], T right[n], T result[n*2], long start, long length) {
    if (length < MIN_MERGE_SIZE*2L) {
        // Base case
        basicmerge(n, left, right, result, start, length);
    } else {
        // Recursive decomposition
        #pragma omp taskgroup
        {
            #pragma omp task
            merge(n, left, right, result, start, length/2);
            #pragma omp task
            merge(n, left, right, result, start + length/2, length/2);
        }
    }
}

void multisort(long n, T data[n], T tmp[n]) {
    if (n >= MIN_SORT_SIZE*4L)
    {
        #pragma omp taskgroup
        {
            #pragma omp task
            multisort(n/4L, &data[0], &tmp[0]);
            #pragma omp task
            multisort(n/4L, &data[n/4L], &tmp[n/4L]);
            #pragma omp task
            multisort(n/4L, &data[n/2L], &tmp[n/2L]);
            #pragma omp task
            multisort(n/4L, &data[3L*n/4L], &tmp[3L*n/4L]);
        }
        #pragma omp taskgroup
        {
            #pragma omp task
            merge(n/4L, &data[0], &data[n/4L], &tmp[0], 0, n/2L);
            #pragma omp task
            merge(n/4L, &data[n/2L], &data[3L*n/4L], &tmp[n/2L], 0, n/2L);
            //#pragma omp taskwait
        }
        #pragma omp task
        merge(n/2L, &tmp[0], &tmp[n/2L], &data[0], 0, n);
	}
    else
    {
		basicsort(n, data);
	}
}
```
[multisort-omp-cutoff.c](codes/multisort-omp-tree.c)

<img class="mini" src="paraver_captures_s2/tree_paraver_1.png">
<note> Paraver capture of number of tasks created using tree strategy</note>

Lear stragegy code:
```
void multisort(long n, T data[n], T tmp[n]) {
        if (n >= MIN_SORT_SIZE*4L) {
                // Recursive decomposition
                multisort(n/4L, &data[0], &tmp[0]);
                multisort(n/4L, &data[n/4L], &tmp[n/4L]);
                multisort(n/4L, &data[n/2L], &tmp[n/2L]);
                multisort(n/4L, &data[3L*n/4L], &tmp[3L*n/4L]);

                merge(n/4L, &data[0], &data[n/4L], &tmp[0], 0, n/2L);
                merge(n/4L, &data[n/2L], &data[3L*n/4L], &tmp[n/2L], 0, n/2L);

                merge(n/2L, &tmp[0], &tmp[n/2L], &data[0], 0, n);
	} 
    else {
		// Base case
		#pragma omp taskgroup
		{
			#pragma omp task
			basicsort(n, data);
		}
		//#pragma omp taskwait
	}
}
```
[multisort-omp-cutoff.c](codes/multisort-omp-leaf.c)

<img class="mini" src="paraver_captures_s2/leaf_paraver_1.png">
<note> Paraver capture of number of tasks created using leaf strategy</note>

Cut-off strategy code:

```
void merge(long n, T left[n], T right[n], T result[n*2], long start, long length) {
        if (length < MIN_MERGE_SIZE*2L) {
                // Base case
                basicmerge(n, left, right, result, start, length);
        } else {
                if(NUM_TASKS < CUTOFF){
					#pragma omp taskgroup
					{	
						NUM_TASKS++;
						#pragma omp task
						merge(n, left, right, result, start, length/2);
						NUM_TASKS++;
						#pragma omp task
						merge(n, left, right, result, start + length/2, length/2);
					}
					NUM_TASKS -= 2;
				}
				else{
					merge(n, left, right, result, start, length/2);
					merge(n, left, right, result, start + length/2, length/2);
				}
        }
}

void multisort(long n, T data[n], T tmp[n]) {
        if (n >= MIN_SORT_SIZE*4L) {
                // Recursive decomposition
                if(NUM_TASKS < CUTOFF){
					#pragma omp taskgroup
					{
						NUM_TASKS++;
						#pragma omp task
						multisort(n/4L, &data[0], &tmp[0]);
						NUM_TASKS++;
						#pragma omp task
						multisort(n/4L, &data[n/4L], &tmp[n/4L]);
						NUM_TASKS++;
						#pragma omp task
						multisort(n/4L, &data[n/2L], &tmp[n/2L]);
						NUM_TASKS++;
						#pragma omp task
						multisort(n/4L, &data[3L*n/4L], &tmp[3L*n/4L]);
					}
					NUM_TASKS -= 4;
					#pragma omp taskgroup
					{
						NUM_TASKS++;
						#pragma omp task
						merge(n/4L, &data[0], &data[n/4L], &tmp[0], 0, n/2L);
						NUM_TASKS++;
						#pragma omp task
						merge(n/4L, &data[n/2L], &data[3L*n/4L], &tmp[n/2L], 0, n/2L);
						//#pragma omp taskwait
					}
					NUM_TASKS -= 2;
					
					NUM_TASKS++;
					#pragma omp task
					merge(n/2L, &tmp[0], &tmp[n/2L], &data[0], 0, n);
				}
				else{
					// Recursive decomposition
					multisort(n/4L, &data[0], &tmp[0]);
					multisort(n/4L, &data[n/4L], &tmp[n/4L]);
					multisort(n/4L, &data[n/2L], &tmp[n/2L]);
					multisort(n/4L, &data[3L*n/4L], &tmp[3L*n/4L]);

					merge(n/4L, &data[0], &data[n/4L], &tmp[0], 0, n/2L);
					merge(n/4L, &data[n/2L], &data[3L*n/4L], &tmp[n/2L], 0, n/2L);

					merge(n/2L, &tmp[0], &tmp[n/2L], &data[0], 0, n);
				}
	} else {
		if(NUM_TASKS >= CUTOFF){
			#pragma omp task
			basicsort(n, data);
			#pragma omp taskwait
		}
		else{
			basicsort(n, data);
		}
	}
}
```
[multisort-omp-cutoff.c](codes/multisort-omp-cutoff.c)

<img class="mini" src="paraver_captures_s2/cutoff_paraver_1.png">
<note> Paraver capture of number of tasks created using cut-off strategy</note>