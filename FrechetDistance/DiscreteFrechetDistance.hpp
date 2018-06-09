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
#include <boost/geometry/geometries/concepts/check.hpp>
#include <boost/assert.hpp>


template <typename size_type,typename result_type>
class coup_mat
{
public:
    coup_mat(size_type w, size_type h)
        : m_data(w * h,-1), m_width(w), m_height(h)
    {}

    result_type & operator()(size_type i, size_type j)
    {
        BOOST_ASSERT(i < m_width && j < m_height);
        return m_data[j * m_width + i];
    }

private:
    std::vector<result_type> m_data;
    size_type m_width;
    size_type m_height;
};

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

template<typename geometry1,typename geometry2>
static inline double frechet_distance(geometry1 const& ls1,geometry2 const& ls2)
{
  typedef typename distance_result<geometry1, geometry2>::type result_type;
  typedef typename boost::range_size<geometry1>::type size_type;
	
  boost::geometry::detail::throw_on_empty_input(ls1);
  boost::geometry::detail::throw_on_empty_input(ls2);

  size_type  a = boost::size(ls1);
  size_type  b = boost::size(ls2);


 	//Coupling Matrix CoupMat(a,b,-1);
  coup_mat<size_type,result_type>  coup_matrix(a,b);

  result_type const not_feasible = -100;
 	//findin the Coupling
 	for(size_type i=0;i<a;i++)
 	{
 		for(size_type j=0;j<b;j++)
 		{
 			if(i==0 && j==0)
 				coup_matrix(i,j)= Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j));
 			else if(i==0 && j>0)
 				coup_matrix(i,j)=(std::max)(coup_matrix(i,j-1),Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j)));
 			else if(i>0 && j==0)
 				coup_matrix(i,j)=(std::max)(coup_matrix(i-1,j),Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j)));
 			else if(i>0 && j>0)
 				coup_matrix(i,j)=(std::max)((std::min)((std::min)(coup_matrix(i,j-1),coup_matrix(i-1,j)),coup_matrix(i-1,j-1)),Distance(boost::geometry::range::at(ls1,i),boost::geometry::range::at(ls2,j)));
 			else
 				coup_matrix(i,j)=not_feasible;
 		}
 	}

 	//Print CoupLing Matrix
 	for(size_type i = 0; i <a; i++)
  {
    for(size_type j = 0; j <b; j++)
      std::cout << coup_matrix(i,j) << " ";
    std::cout << std::endl;
  }
	
	//Final Coupling Distance
	return coup_matrix(a-1,b-1);
};

} // namespace algorithms

}} // namespace boost::geometry


#endif // BOOST_GEOMETRY_STRATEGIES_DISTANCE_HPP
