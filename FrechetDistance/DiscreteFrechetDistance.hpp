/*  
        Boost (Gsoc '18)
        SimmilarityBetweenGeometries
        Author : Yaghyavardhan Sing Khangarot
        Institute: IIIT Hyderabad
*/

#include <algorithm> 
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>
#include <limits>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/range.hpp>


namespace bnu = boost::numeric::ublas;
namespace bg = boost::geometry;


template <typename DataType>
static inline DataType max(DataType a,DataType b)
{
	return (a>b)?a:b;
}

template <typename ElementType>
static inline ElementType min(ElementType a,ElementType b)
{	
	return (a<b)?a:b;
}


//Calculating distance as per the Strategy Type
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


namespace FrechetDistance
{
template<typename LineString>
static inline double FrechDist(LineString ls1,LineString ls2)
{
	double Dis=0;
	unsigned int  a = boost::size(ls1);
 	std::cout <<"size of linestring1 ="<< a << std::endl;
 	unsigned int  b = boost::size(ls2);
 	std::cout << "size of linestring2 ="<< b << std::endl;

 	//bnu::matrix<double>  CoupMat(a+1,b+1,-1);
  double CoupMat[a][b];
  for(unsigned int i = 0; i <a; i++)
    for(unsigned int j = 0; j <b; j++)
      CoupMat[i][j] = -1;
    
 	//findin the Coupling
 	//calling Recursion to get the coupling distance
 	double NonFeasible=-100;
 	for(unsigned int i=0;i<a;i++)
 	{
 		for(unsigned  int j=0;j<b;j++)
 		{
 			if(i==0 && j==0)
 				CoupMat[i][j]= Distance(bg::range::at(ls1,i),bg::range::at(ls2,j));
 			else if(i==0 && j>0)
 				CoupMat[i][j]=max(CoupMat[i][j-1],Distance(bg::range::at(ls1,i),bg::range::at(ls2,j)));
 			else if(i>0 && j==0)
 				CoupMat[i][j]=max(CoupMat[i-1][j],Distance(bg::range::at(ls1,i),bg::range::at(ls2,j)));
 			else if(i>0 && j>0)
 				CoupMat[i][j]=max(min(min(CoupMat[i][j-1],CoupMat[i-1][j]),CoupMat[i-1][j-1]),Distance(bg::range::at(ls1,i),bg::range::at(ls2,j)));
 			else
 				CoupMat[i][j]=NonFeasible;
 		}
 	}

 	//Print CoupLing Matrix

 	for(unsigned int i = 0; i <a; i++)
  {
    for(unsigned int j = 0; j <b; j++)
      std::cout << CoupMat[i][j] << " ";
    std::cout << std::endl;
  }

	
	//Final Coupling Distance
	Dis=CoupMat[a-1][b-1];
	
	return Dis;
};

}
