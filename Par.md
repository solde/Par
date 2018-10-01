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


## Temps d'execució
    T1 = Σ(Work_node(i))
    Tc = Tasks_crit
    Paralellisme = T1/Tc -> Numero màxim de processadors que es farà servir.
---
Problema 3:

    T1 = 100*8+1000*(1+2+3+4+5+6+7+8)+5*8 = 36840

    Tc = 100+8000*5 = 8105

    P = T1/Tc = 4,54

    <img src=pissarra1.png </img>

