/*  
        Boost (Gsoc '18)
        SimmilarityBetweenGeometries
        Author : Yaghyavardhan Sing Khangarot
        Institute: IIIT Hyderabad
*/

#ifndef BOOST_GEOMETRY_FRECHET_DISTANCE_HPP
#define BOOST_GEOMETRY_FRECHET_DISTANCE_HPP

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
#include <boost/mpl/assert.hpp>




//Calculating distance as per the Strategy Type
template <typename Point>
static inline double Distance(Point const& p1, Point const& p2)
{
    typedef typename boost::geometry::strategy::distance::services::default_strategy
              <
                  boost::geometry::point_tag, boost::geometry::point_tag,
                  Point, Point
              >::type strategy_type;

    return boost::geometry::distance(p1, p2, strategy_type());
}


namespace boost { namespace geometry {

namespace algorithms{

template<typename Geometry1,typename Geometry2>
static inline double frechet_distance(Geometry1 const& ls1,Geometry2 const& ls2)
{
  typedef typename distance_result<Geometry1, Geometry2>::type result_type;
  typedef typename boost::range_size<Geometry1>::type size_type;
	
  size_type  a = boost::size(ls1);
  size_type  b = boost::size(ls2);

  result_type const uninitialized = -1;
  result_type const not_feasible = -100;

 	//Coupling Matrix CoupMat(a+1,b+1,-1);
  result_type CoupMat[a][b];
  for(size_type i = 0; i <a; i++)
    for(size_type j = 0; j <b; j++)
      CoupMat[i][j] = uninitialized;
    
 	//findin the Coupling
 	for(size_type i=0;i<a;i++)
 	{
 		for(size_type j=0;j<b;j++)
 		{
 			if(i==0 && j==0)
 				CoupMat[i][j]= Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j));
 			else if(i==0 && j>0)
 				CoupMat[i][j]=(std::max)(CoupMat[i][j-1],Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j)));
 			else if(i>0 && j==0)
 				CoupMat[i][j]=(std::max)(CoupMat[i-1][j],Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j)));
 			else if(i>0 && j>0)
 				CoupMat[i][j]=(std::max)((std::min)((std::min)(CoupMat[i][j-1],CoupMat[i-1][j]),CoupMat[i-1][j-1]),Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j)));
 			else
 				CoupMat[i][j]=not_feasible;
 		}
 	}

 	//Print CoupLing Matrix

 	for(size_type i = 0; i <a; i++)
  {
    for(size_type j = 0; j <b; j++)
      std::cout << CoupMat[i][j] << " ";
    std::cout << std::endl;
  }
	
	//Final Coupling Distance
	return CoupMat[a-1][b-1];
};

} // namespace algorithms

}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP
