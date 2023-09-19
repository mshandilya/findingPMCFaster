# GRASP Club Induction Assignment
This repository contains all the files related to the solution of [@mshandilya](mailto:mrigankashekhar.shandilya@iitgn.ac.in) for the Club Induction Assignment of GRASP, IIT Gandhinagar in the year 2023. The various files contained in this repository are described below.

Before you start reading further, if you are unfamiliar with any of the terms used in this repository, you can look for them in the [glossary](#glossary). If you feel you significantly lack knowledge in one or more of the topics, check out the [resources](#resources).

## Problem Statement
Does there exist a partition of the vertex set of the Graph ($G$) into disjoint exhaustive sets ($X, Y$) such that every vertex in $X$ has exactly one neighbour in $Y$ and vice versa?

### Diving into the problem statement
The problem statement requires us to create a mathematical mapping (or function) between two sets such that it's a one-to-one and onto mapping (or function). In this approach, we can ignore the edges that connect vertices in the same partitioning. So, our task is to divide the vertices into two **disjoint**, **exhaustive**, and **equivalent** sets where a **one-to-one** and **onto** function is possible to map.

### How is the problem approached in the concerned research paper
The solution to the problem is attempted to be built from the ground up using singular (or small) updates to the solution and sometimes branching off to more than one possible solution, **repeating** the process until a final solution is built or it becomes evident that a solution cannot be built.

## The Research Paper [🔗](https://doi.org/10.1007/978-3-031-34347-6_26)
Let's not repeat what's already written in the paper's abstract. Since we only intend to implement the method of finding perfect matching cuts (the kind of mapping we described above), let's consider everything that is said in the paper as a black box in order to ignore the calculations, proofs, and rationality behind everything.

Our purpose now becomes much simpler. We just need to understand what the paper intends us to do in order to form a solution for the problem.

### Notations
As uninteresting as it may be, this part is extremely important to actually understand what's going on in the rest of the paper. Since it is well defined in the paper itself, we can skip this part (readers are expected to acquaint themselves with it before moving ahead).

P.S. Don't be scared of the unusual symbols used. If it helps, take them as any unused letters of the English Alphabet.

### Structure of the algorithm:
If you now move on to read part 3 of the paper, you'd find that the partial solution they use to build up the final solution undergoes two kinds of processes:
* Reduction Rules: These are basically the singular updates we talked about, which lead a single partial solution to a slightly more complete partial solution. *i.e.* from **one partial** solution, we get **one** slightly more complete **partial** solution.
* Branching Conditions: When a partial solution can't be reduced any further to a more complete partial solution, we try to create **more than one** (slightly more complete) partial solution from it, one or more of which might lead to a final answer.

### Reduction Rules
Something important to notice at this point is that our problem requires just a *yes/no* answer. So when we find our partial solution violating one of the conditions set by the problem or when we find that no further updates can be made without violating any of those conditions, we can simply return *no* for an answer. This is highlighted in the eight reduction rules present in the paper. These reduction rules are applied to a single partial solution and can be applied in any order. 

A reduced solution is defined as one in which none of the reduction rules can be applied anymore.

### Branching Conditions
Depending on the condition that is first satisfied by our **reduced** partial solution, it may be branched into two separate partial solutions on which the process of reduction and branching needs to be applied **recursively**. Important things to note here are that the branching conditions make use of boolean operators to find out their own answer based on the answer of the branched partial solutions (here, answer: whether or not a PMC exists). Also, the branching conditions necessarily need to be checked in the **same order** as listed in the paper.

*But till when will this be done recursively?*
If you've ever heard of recursion, you must know there are base cases. The base case here, when the recursion stops, is when none of the branching conditions are satisfied.

### The Base Case
When none of the branching conditions are satisfied, we now need to check if the current partial solution can, in fact, lead to a complete solution. As mentioned in the paper (after the branching conditions), at this point, every vertex but the saturated ones form part of a cycle. Since we need to divide the vertices equally in the two partitionings, we need to check that **the length of each such cycle is even**. If it is, then it is possible to form a solution, and the answer is *yes*. Otherwise, it's *no*.

### Solving the bigger problem
So, we have now successfully checked if a partial solution can ever lead to a complete solution, but how do we generate these partial solutions to exhaustively consider every situation that may arise for the bigger problem of Perfect Matching Cuts? The answer is in the paper itself (check just before the branching conditions).

## Resources
* General Concepts:
  * [Recursion](https://www.geeksforgeeks.org/introduction-to-recursion-data-structure-and-algorithm-tutorials/)
* Graph Theory:
  * [Introduction](https://youtu.be/mi2Fd0W8TaM)
  * [Representation](https://youtu.be/Y-6IGzmlG4Y)
  * [Depth First Search](https://youtu.be/0n4UtSm5HtE)
  * [Identifying Cycles](https://www.geeksforgeeks.org/detect-cycle-undirected-graph/)
  * [Matchings](https://youtu.be/chdr2aj4FUc)

## Glossary
* Branching
  : Making different types of updates to the solution that may result in different final solutions. *i.e.* branching to different solution paths
* Edge
  : An edge in a graph is a link or connection between two vertices showing a mathematical or logical relationship between them.
* Graph
  : A graph comprises vertices (or nodes) that may or may not be connected to each other by edges (or links).
* Reduction
  : Making the solution space smaller by eliminating formations (here, prospective solutions) which would violate the conditions set by the problem 
* Return
  : Stop the processing being done, ignore the further steps and simply give back (return) the answer.
* Vertex
  : A vertex in a graph is a point that represents a certain entity or value.
