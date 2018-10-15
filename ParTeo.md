<style>
    h1{
        color: red;
        text-align: center;
        font-size: 40px;
        text-shadow: 2px 2px #ffcccc;      
    }
    h2{
        font-weight: bold;
    }
    img {
        height: auto;
        width: 100%;
        
    }
    .center{
        width: 50%;
        height: auto;
        margin-left: auto;
        margin-right: auto;
        display: block;
    }
    note {
        font-weight: lighter;
    }
    p{
        text-align: justify;
    }
    .emoji{
        height: 2em;
    }

</style>


# Understanding parallelism

## SpeedUp vs Efficiency

SpeedUp (S<sub>p</sub>): relative reduction of execution time whn using P processors with respect sequential.

Efficiency (Eff<sub>p</sub>): it is a measure of the fraction of time for which processing element is usefull.

## Escalability

> - Strong: resources x2 -> scalability x2
> - Weaak: resources x2 w. proportional work

## Amdahl's law

### Par_Fraction
> φ= T<sub>seq_time_of_par_part</sub> / T<sub>seq_exec</sub>

<img src="./amdahl.png">

Note: <note>If P approach to infinit, φ/P approach to 0, then S<sub>p</sub> = 1/(1-φ).</note>

Ex:

>        seq - 25s
>        par - 50s
>        seq - 25s
>              100s
> φ = 100/50 = 0,5
> SpeedUp <sub>par</sub> = 50/10 = 5
>
> SpeedUp = 100/60 = 1.67

## Sources of overhead
> - tark creation
> - barrier sync
> - tark sync
> - exclusive access to data
> - data sharing 
> - Idleness
> - Computation (extra work to obtain a palallel algorithm)
> - Memory (extra memory to obtain a palallel algorithm)
> - Contention (competition for the access to shared resources)

<center><img src="T1.png"></center>

## How to model data sharing overload?
Example:
### Jacobi solver
> T<sub>calc</sub> = (N<sup>2</sup>/P)*t<sub>boddy</sub>
> 
> T<sub>p</sub> = T<sub>calc</sub> + T<sub>comm</sub>
> 
> Tcomm = 2(t<sub>s</sub>+t<sub>w</sub>*N)

## Data sharing modeling

### Example 4
```
void compute(int n, double *u, double *utmp) {
    int i, j;
    double tmp;
    for (i = 1; i < n-1; i++) {
        for (j = 1; j < n-1; j++) {
            tmp = u[n*(i+1) + j] + u[n*(i-1) + j] + // elements u[i+1][j] and u[i-1][j]
            u[n*i + (j+1)] + u[n*i + (j-1)] - // elements u[i][j+1] and u[i][j-1]
            4 * u[n*i + j]; // element u[i][j]
            u[n*i + j] = tmp/4; // element u[i][j]
        }
    }
}
```
Each cpu with n<sup>2</sup>/P elements. Tasks compute segments of n/c rows by c columns.

Then, time acquire the following form:

<img src="Timeex4.png">
<img src="SpeedUpex4.png" class="center">

<div class=page>

# Task decomposition

<img src="Diagram1.png">

Types:
* Lineal Task decomposition
> Code block or procedure
* Iterative task decomposition
> Iterative constructs
* Recursive task decomposition
> Recursive procedures

## Decomposition Strategies

### Leaf strategy

Create one task squetentally for each leaf of task tree. 

>Less tasks
>
>Less overhead

### Tree strategy

Creater one task for each invocation.

> More Tasks
> 
> More Overhead

## Cut-off control

If tree strategy is in use, whan a certain number of task are created or the granulation si too small or in a certain number of recursive calls; you can change the strategy to Leaf in order to reduce overhead.
