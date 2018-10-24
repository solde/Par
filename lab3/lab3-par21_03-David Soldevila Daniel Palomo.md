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
    }
</style>

# LAB 3

## Introduction

<div class="page">

## Task decomposition analysis for Mergesort

### Divide and conquer

### Task decomposition analysis with Tareador

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

The multisort program parallise as expected untill 16 cores, when , the dependences between the multisort marge calls, as seen in the tareador capture of the tasks, doesn't allow for more. The efficiency drops going further than 16 threads (i.e fig: efficiency plot and 32,64 captures) Proving that adding more cpus is pointless.



<div class="page">
