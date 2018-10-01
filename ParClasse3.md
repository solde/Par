<style>
    h1{
        color: red;
        text-align: center;
        font-size: 50px;
        text-shadow: 2px 2px blue;
    }
    h2{
        font-weight: bold;
    }
    img {
        height: auto;
        width: 50%;
    }
    note {
        font-weight: lighter;
    }
    p{
        text-align: justify;
    }
</style>


# Par teoria S3

## SpeedUp vs Efficiency

SpeedUp (S<sub>p</sub>): relative reduction of execution time whn using P processors with respect sequential

Efficiency (Eff<sub>p</sub>): it is a measure of the fraction of time for which processing element is usefull

## Escalability

> - Strong: resources x2 -> scalability x2
> - Weaak: resources x2 w. proportional work

## Amdahl's law

> φ (Par_Fraction) = T<sub>seq_time_of_par_part</sub> / T<sub>seq_exec</sub>

<center><img src="./amdahl.png"></center>

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