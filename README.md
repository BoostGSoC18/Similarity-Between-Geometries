## Similarity Between Geometries [GSoC'18]

Consider 2 Point sets P ⊆ R^d and Q⊆R^d, where the goal is to find the extent of similarity between P and Q. The first question that needs to be answered is: what do we mean by “similar” ? Mathematically, we want to find a mapping σ:P→Q, that has one of the following properties:
1. σ minimize max <sub>p∈P</sub>`||p−σ(p)||` ( minimize maximum distance between mapped points) or
2. σ minimize ∑<sub>p∈P</sub> `||p−σ(p)||`( minimize sum distance between mapped points) or
3. σ minimize ∑<sub>p∈P</sub>`||p−σ(p)||^2` ( minimize sum of squared distance between mapped points).

To  measure the Similarity Between Geometries I will be using different variation/algorithms for calculating Frechet
distance and Hausdorff distance.

### Discrete Frechet Distance
Let `A = (a0, .... , am−1)` and `B = (b0, .... , bn−1)` be two sequences of m and n points, respectively, in the plane. The discrete Fr´echet distance `dF(A, B)` between A and B is defined as follows. Fix a distance δ > 0 and consider a directed graph `Gδ(A, B) = (A×B, EA U EB U EAB)`, where the edge sets EA, EB, and EAB are defined
as follows:
EA = `{((a_i, b_j ),(a_{i+1}, bj ))` such that` ||a_i − b_j||, ||a_{i+1} − b_j|| ≤ δ `},
EB = `{((a_i, b_j ),(a_i, b_{j+1}))` such that` ||a_i − b_j||, ||a_i − b_j+|| ≤ δ `} ,
EAB = `{((a_i, b_j ),(a_{i+1}, b_{j+1}))` such that `||a_i − b_j||, ||a_{i+1} − b_j+1||≤ δ` }

Informally, think of A and B as two sequences of stepping stones, and of two frogs, the A-frog and the B-frog, where the A-frog has to visit all the A-stones in order, and the B-frog has to visit all the B-stones in order. The frogs are connected
by a rope of length δ , and are initially placed at a1 and b1, respectively. The jump is allowed only if the distance between the two frogs before the jump as well as after the jump is at most δ. The edges in EA (resp., EB) correspond to the case when the A-frog (resp., B-frog) jumps and the other stays at its current stone, and the edges in EAB correspond to the case when both frogs jump simultaneously. Then dF(A, B) is the smallest δ > 0 for which there exists a sequence
of jumps that gets the frogs to a_m and b_n, respectively.

### Discrete Frechet Distance Using Coupling Measure
To find Similarity between two curves given by discrete points `Thomas Eiter and Heikki Mannila` introduced a variation of Frechet distance as Coupling Measure and in the paper `Computing Discrete Frechet Distance` they  provided an algorithm for finding the Coupling Measure in O(pq) (where p=Number of discrete points in first curve(P);q=Number of descrete points in second curve(Q)). Currently it works for calculating frechet distance between LineString-LineString. 

```c++
 **Pseudo code for finding Coupling Distance** (frechet distance for curve formed by discrete points)

Function dF(P, Q): real;
input:polygonal curves P= (u1, . . . , up) and Q= (v1, . . . , vq).
return:δdF(P, Q)
ca:array[1..p,1..q] of real;
function InitializeCouplineMatrix() 
	begin
	for i = 1 to p
	do
		for j= 1 to q 
		do ca(i, j) :=−1.0;
	end.
	
function CouplingMeasure(): real;
	begin
	for i = 1 to p
	do 
		for j = 1 to q
		do
			if i= 1 and j= 1 then ca(i, j) :=d(u1, v1)
			elsif i >1 and j= 1 then ca(i, j) := max{CouplingMatrix(i−1,1), d(ui, v1)}
			elsif i= 1 and j >1 then ca(i, j) := max{CouplingMatrix(1, j−1), d(u1, vj)}
			elsif i >1 and j >1 then ca(i, j) :=max{min(CouplingMatrix(i−1, j), CouplingMatrix(i−1, j−1), 								CouplingMatrix(i, j−1)), d(ui, vj)}
			else ca(i, j)=∞      "Not feasible"
		end
	end
	return CouplingMatrix(p, q);
	end;
  
 
```
<b>Reference</b>:- [Computing Discrete Fr ́echet Distance by Thomas Eiter and Heikki Mannila](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.90.937)

### Implementation Details for the Discrete Frechet Distance Algorithm

I have implemented an iterative version of the algorithm and  incorporated the   `Cartesian, Spherical_Equatorial, Geographic Coordinate Systems`. The strategy type  to calculate point-to-point distance is decided using this patch of code :-
```
template <typename Geometry1, typename Geometry2>
inline typename distance_result
        <
            typename point_type<Geometry1>::type,
            typename point_type<Geometry2>::type
        >::type
frechet_distance(Geometry1 const& geometry1, Geometry2 const& geometry2)
{
    typedef typename strategy::distance::services::default_strategy
              <
                  point_tag, point_tag,
                  typename point_type<Geometry1>::type,
                  typename point_type<Geometry2>::type
              >::type strategy_type;
    
    return frechet_distance(geometry1, geometry2, strategy_type());
}
```
or the user can pass their own strategy type using this patch of code:-
```
template <typename Geometry1, typename Geometry2, typename Strategy>
inline typename distance_result
        <
            typename point_type<Geometry1>::type,
            typename point_type<Geometry2>::type,
            Strategy
        >::type
frechet_distance(Geometry1 const& geometry1, Geometry2 const& geometry2, Strategy const& strategy)
{
    return dispatch::frechet_distance<Geometry1, Geometry2>::apply(geometry1, geometry2, strategy);
}
```
### Discrete Hausdorff Distance
Let P and Q be two set of points in R^d. The directed Hausdorff distance from P to Q , denoted by h(P,Q), is `{max_{p in P} min_{q in Q} ||p-q||}`.
The Hausdorff distance between P and Q, denoted by H(P,Q), is `{max{h(P,Q),h(Q,P)}}`.
Intuitively, the function h(P, Q) finds the point p in P that is farthest from any point in Q and measures the distance from p to its nearest neighbor in Q.  Hausdorff distance is a measure of the mismatch between two point-sets. 

```
Pseudo code for Hausdorff distance for curve formed by discrete points
Require: Two finite point sets P, Q.

hausdorff_distance(P(geometry1),Q(geometry2))
{
cmax = 0
for x in P do
	cmin = infinite 
	for y in Q do
		d = Distance(x ,y)
		if d < cmax then
			break
		end if
		if d < cmin then
			cmin = d
		end if
	end for
	if ( cmin > cmax AND inf > cmin ) then
		cmax = cmin
	end if
end for
return cmax
}
```
Using the R-tree Algorithm there is a huge amount of reduction in computational time as the iterative method take `O(N*M)` time where as using R-tree the computationla reduces to `O(N*logN + M*logN) ` (where N is the number of elements in the R-tree). N=Number of points in P; M=Number of points in Q.
```
Discrete Hausdorff Distance Using R-tree
      
result_type dis_max=0;
typedef bgi::rtree<point_t, bgi::linear<4> > rtree_type;
rtree_type rtree(boost::begin(r2), boost::end(r2));
point_t res;

for(size_type i=0;i<a;i++)
{
	size_type found= rtree.query(bgi::nearest(range::at(r1, i),1), &res);	
	result_type dis_min=geometry::distance(range::at(r1,i),res);
	if (dis_min > dis_max )	
	{
	dis_max = dis_min;
	}
}
        
return dis_max;
```
### Hausdorff Distance Extension for range-multi_range and multi_range-multi_range

Discrete Hausdorff Distance algroithm was implemented for Point-MulitPoint, MultiPoint-MultiPoint, LineString-LineString, LineString-MultiLineString and MultiLineString-MultiString using the below mentioned variations in the Hausdorff distance algorithm.
```
For range-multi_range

input: Range(rng);Multi_range(Mrng)

range-multi_range(rng,Mrng)
{
for each Range(rng_i)  in Mrng:
	do 
		temp_dis=hausdorff_distance(rng_i,rng);
		if temp_dis > max_dis
			max_dis = temp_dis;             //max_dis initialized as 0.
	end
return max_dis.
}
```

```
For multi_range-multi_range

input:Multi_range(Mrng1);Multi_range(Mrng2)

multi_range-multi_range(Mrng1,Mrng2)
{
for each Range(rng_i) in Mrng1:
	do
		temp_dis=range-multi_range(rng_i,Mrng2);
		if temp_dis > max_dis
			max_dis = temp_dis;             //max_dis initialized as 0.
	end
return max_dis.
}
```
### Performance testingg with R-tree and with loop(Iterative Method)
These are the results for the same geometries v.s. partially similar v.s. totally different geometries for Linestring and MultiPoint are as follows:-
```
LINESTRING
N=10 ; M=10 ; K=10000  ; N,M=Number of points in the geometries K=Number of times Distance calculated
For Same Geometries
With R-tree => 1.308s,1.298s,1.3s,1.293s
With loop => 0.943s,0.945s,0.946s,0.949s
For Partially Similar Geometries
With R-tree => 1.248s,1.238s,1.31s,1.327s
With loop => 0.944s,0.938s,0.94s,0.935s
For Totally different Geometries
With R-tree => 1.267s,1.309s,1.38s,1.366s
With loop => 0.961s,0.989s,0.975s,0.974s

LINESTRING
N=100 ; M=100 ; K=10000 ; N,M=Number of points in the geometries K=Number of times Distance calculated
For Same Geometries
With R-tree => 16.038s,15.624s,16.43s,15.715s
With loop => 36.867s,38.378s,37.371s,36.381s
For Partially Similar Geometries
With R-tree => 16.084s,16.206s,16.01s,16.249s
With loop => 36.331s,36.166s,35.917s,35.737s
For Totally different Geometries
With R-tree => 17.268s,17.402s,16.768s,17.661s
With loop => 37.29s,36.402s,35.813s,37.542s

LINESTRING
N=1000 ; M=1000 ; K=100 ; N,M=Number of points in the geometries K=Number of times Distance calculated
For Same Geometries
With R-tree => 1.901s,1.884s,1.878s,1.919s
With loop => 31.886s,31.619s,31.298s,31.159s
For Partially Similar Geometries
With R-tree => 2.045s,2.21s,2.232s,2.157s
With loop => 31.477s,32.059s,32.375s,32.178s
For Totally different Geometries
With R-tree => 2.055s,1.892s,2.048s,2.053s
With loop => 31.641s,33.744s,33.546s,32.275s

MULTIPOINT
N=10 ; M=10 ; K=10000 ; N,M=Number of points in the geometries K=Number of times Distance calculated
For Same Geometries
With R-tree => 1.301s,1.275s,1.275s,1.278s
With loop => 1.135s,1.118s,1.111s,1.201s
For Partially Similar Geometries
With R-tree => 1.413s,1.424s,1.386s,1.3s
With loop => 1.016s,1.008,1.004s,1.038s
For Totally different Geometries
With R-tree => 1.322s,1.312s,1.353s,1.369s
With loop => 1.066s,1.119s,1.128s,1.132s

MULTIPOINT
N=100 ; M=100 ; K=10000 ; N,M=Number of points in the geometries K=Number of times Distance calculated
For Same Geometries
With R-tree => 15.676s,15.85s,15.883s,15.769s
With loop => 36.45s,36.429s,36.495s,36.68s
For Partially Similar Geometries
With R-tree => 18.704s,18.601s,17.818s,17.889s
With loop => 38.26s,37.451s,36.484s,37.511s
For Totally different Geometries
With R-tree => 16.72s,17.235s,17.3s,17.104s
With loop => 36.364s,36.454s,36.675s,36.445s

MULTIPOINT
N=1000 ; M=1000 ; K=100 ; N,M=Number of points in the geometries K=Number of times Distance calculated
For Same Geometries
With R-tree => 1.951s,1.946s,1.944s,1.97s
With loop => 30.97s,30.99s,30.923s,30.933s
For Partially Similar Geometries
With R-tree => 2.02s,2.02s,2.041s,2.134s
With loop => 31.062s,31.047s,31.553s,31.096s
For Totally different Geometries
With R-tree => 2.115s,1.943s,2.102s,2.115s
With loop => 31.159s,30.986s,31.464s,31.444s
```
This clearly show that for geometries having size less than 10 Iterative method works better , but as the size increases R-tree Works better.

### How to use this header: 
  1. Include the header in your code by specifying
  **(#include  <boost/geometry/algorthms/discerte_hausdroff_distance.hpp")**
	and call the function as
	```
	result_type h_distance = bg::hausdorff_distance(geometry1,geometry2);
	or
	result_type h_distance = bg::hausdorff_distance(geometry1,geometry2,strategy);
	```
  **(#include <boost/geometry/algorithms/discerte_frechet_distance.hpp")**
  	and call the function as
	```
	result_type f_distance = bg::frechet_distance(geometry1,geometry2);
	or
	result_type f_distance = bg::frechet_distance(geometry1,geometry2,strategy);
	```
or just use
  **(##include <boost/geometry.hpp>)*
  
 <b> Author By Yaghyavardhan Singh Khangarot </b>
