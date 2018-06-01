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


namespace bg = boost::geometry;


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


namespace boost { namespace geometry {
namespace HausdorffDistance
{

template<typename LineString>
static inline double HausDist(LineString ls1,LineString ls2)
{
	double DisMax=0,DisMin;
  double infinite = std::numeric_limits<double>::max();
	unsigned int  a = boost::size(ls1);
 	std::cout <<"size of linestring1 ="<< a << std::endl;
 	unsigned int  b = boost::size(ls2);
 	std::cout << "size of linestring2 ="<< b << std::endl;

 	//findin the Coupling
 	for(unsigned int i=0;i<a;i++)
 	{
    DisMin= std::numeric_limits<double>::max() ;
 		for(unsigned  int j=0;j<b;j++)
 		{
 			double DisTemp = Distance(bg::range::at(ls1,i),bg::range::at(ls2,j));
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

} // Hausdorff Distance

}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP