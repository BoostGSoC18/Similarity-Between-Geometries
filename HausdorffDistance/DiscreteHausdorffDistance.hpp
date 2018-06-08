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
namespace algorithms
{

template<typename Geometry1 ,typename Geometry2>
static inline double hausdorff_distance(Geometry1 ls1,Geometry2 ls2)
{
  typedef typename distance_result<Geometry1, Geometry2>::type result_type;
  typedef typename boost::range_size<Geometry1>::type size_type;

	result_type DisMax=0,DisMin;
  result_type infinite = std::numeric_limits<double>::max();
	
  size_type  a = boost::size(ls1);
 	std::cout <<"size of linestring1 ="<< a << std::endl;
 	size_type  b = boost::size(ls2);
 	std::cout << "size of linestring2 ="<< b << std::endl;

 	//findin the Coupling
 	for(size_type i=0;i<a;i++)
 	{
    DisMin= std::numeric_limits<result_type>::max() ;
 		for(size_type j=0;j<b;j++)
 		{
 			result_type DisTemp = Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j));
      if(DisTemp < DisMax)
        break; //Early Break
      if(DisTemp < DisMin)
        DisMin=DisTemp;
 		}

    if (DisMin > DisMax && infinite > DisMin)
    {
      DisMax = DisMin;
    }
 	}
	
	return DisMax;
};

} // algorithms

}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP