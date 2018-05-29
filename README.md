## Similarity Between Geometries [GSoC'18]

Consider 2 Point sets P ⊆ R^d and Q⊆R^d, where the goal is to find the extent of similarity between P and Q. The first question that needs to be answered is: what do we mean by “similar” ? Mathematically, we want to find a mapping σ:P→Q, that has one of the following properties:
1. σ minimize max <sub>p∈P</sub>`||p−σ(p)||` ( minimize maximum distance between mapped points) or
2. σ minimize ∑<sub>p∈P</sub> `||p−σ(p)||`( minimize sum distance between mapped points) or
3. σ minimize ∑<sub>p∈P</sub>`||p−σ(p)||^2` ( minimize sum of squared distance between mapped points).

To  measure the Similarity Between Geometries I will be using different variation/algorithms for calculating Frechet
distance and Hausdorff distance.

### Discrete Frechet Distance
Let `A = (a0, \dots , am−1)` and `B = (b0, \dots , bn−1)` be two sequences of m and n points, respectively, in the plane. The discrete Fr´echet distance `dF(A, B)` between A and B is defined as follows. Fix a distance δ > 0 and consider a directed graph `Gδ(A, B) = (A×B, EA U EB U EAB)`, where the edge sets EA, EB, and EAB are defined
as follows:
EA = `{((a_i, b_j ),(a_{i+1}, bj ))` such that` ||a_i − b_j||, ||a_{i+1} − b_j|| ≤ δ `},
EB = `{((a_i, b_j ),(a_i, b_{j+1}))` such that` ||a_i − b_j||, ||a_i − b_j+|| ≤ δ `} ,
EAB = `{((a_i, b_j ),(a_{i+1}, b_{j+1}))` such that `||a_i − b_j||, ||a_{i+1} − b_j+1||≤ δ` }

Informally, think of A and B as two sequences of stepping stones, and of two frogs, the A-frog and the B-frog, where the A-frog has to visit all the A-stones in order, and the B-frog has to visit all the B-stones in order. The frogs are connected
by a rope of length δ , and are initially placed at a1 and b1, respectively. The jump is allowed only if the distance between the two frogs before the jump as well as after the jump is at most δ. The edges in EA (resp., EB) correspond to the case when the A-frog (resp., B-frog) jumps and the other stays at its current stone, and the edges in EAB correspond to the case when both frogs jump simultaneously. Then dF(A, B) is the smallest δ > 0 for which there exists a sequence
of jumps that gets the frogs to a_m and b_n, respectively.

### Discrete Frechet Distance Using Coupling Measure
To find Similarity between two curves given by discrete points `Thomas Eiter and Heikki Mannila` introduced a variation of Frechet distance as Coupling Measure and in the paper `Computing Discrete Frechet Distance` they  provided an algorithm for finding the Coupling Measure in O(pq) (where p=Number of discrete points in first curve(P);q=Number of descrete points in second curve(Q)).

```c++
 **Pseudo code for finding Coupling Distance** (frechet distance for curve formed by discrete points)

Function dF(P, Q): real;
input:polygonal curves P= (u1, . . . , up) and Q= (v1, . . . , vq).
return:δdF(P, Q)
ca:array[1..p,1..q] of real;
function CouplingMeasure(i, j): real;
	begin
		if ca(i, j)>−1 then return ca(i, j)
		elsif i= 1 and j= 1 then ca(i, j) :=d(u1, v1)
		elsif i >1 and j= 1 then ca(i, j) := max{CouplingMeasure(i−1,1), d(ui, v1)}
		elsif i= 1 and j >1 then ca(i, j) := max{CouplingMeasure(1, j−1), d(u1, vj)}
		elsif i >1 and j >1 then ca(i, j) :=max{min(CouplingMeasure(i−1, j), CouplingMeasure(i−1, j−1), CouplingMeasure(i, j−1)), d(ui, vj)}
		else ca(i, j)=∞
	return ca(i, j);
	end;
  
 function ComputeFrechetDistance() 
	begin
	for i = 1 to p
	do
		for j= 1 to q 
		do ca(i, j) :=−1.0;
	return CouplingMeasure(p, q);
	end.
```
<b>Reference</b>:- [Computing Discrete Fr ́echet Distance by Thomas Eiter and Heikki Mannila](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.90.937)

### Implementation Details for the Discrete Frechet Distance Algorithm

I have implemented an iterative version of the algorithm and  incorporated the   `Cartesian, Spherical_Equatorial, Geographic Coordinate Systems`.I am deciding the strategy type  to calculate point-to-point distance using this patch of code :-
```
template <typename Point>
static inline double Distance(Point const& p1, Point const& p2)
{
    typedef typename bg::strategy::distance::services::default_strategy
              <
                  bg::point_tag, bg::point_tag,
                  Point, Point
              >::type strategy_type;

    return bg::distance(p1, p2, strategy_type());
}
```


### How to use this header: 
  1. Clone/Download this directory (or the file "FrechetDistance.hpp")
  1. Include the header in your code by specifying the absolute or relative path **(#include"/file_path/FrechetDistance.hpp")**
  
 <b> Author By Yaghyavardhan Singh Khangarot </b>
