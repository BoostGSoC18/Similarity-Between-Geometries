/*  
        Boost Competency Test (Gsoc '18)
        Author : Yaghyavardhan Sing Khangarot
        Institute: IIIT Hyderabad
*/

#include <algorithm> 
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>
#include <limits>

#include "../include/boost/geometry/algorithms/frechet_distance.hpp"
#include "../include/boost/geometry/algorithms/hausdorff_distance.hpp"

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/geometry/core/tag.hpp>


#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)
namespace bnu = boost::numeric::ublas;
namespace bg = boost::geometry;
BOOST_GEOMETRY_REGISTER_LINESTRING_TEMPLATED(std::vector)
BOOST_GEOMETRY_REGISTER_LINESTRING_TEMPLATED(std::deque)

int main(void)
{
	using namespace boost::geometry;

    // Define a linestring, which is a vector of points, and add some points
    // (I hace  added them deliberately in different ways)
    typedef model::d2::point_xy<double> point_t;
    typedef model::linestring<point_t> linestring_2d;
	typedef bg::model::multi_linestring<linestring_2d> mlinestring_t;
	
	mlinestring_t mls1,mls2;
	mls1.resize(2);				
    
    bg::append(mls1[0], point_t(0.0, 0.0)); 
    bg::append(mls1[0], point_t(0.0, 1.0));
    bg::append(mls1[0], point_t(2.0, 1.0));

    bg::append(mls1[1], point_t(1.0, 0.0)); 
    bg::append(mls1[1], point_t(2.0, 0.0));

    mls2.resize(2);

    bg::append(mls2[0], point_t(0.0, 0.0)); 
    bg::append(mls2[0], point_t(0.0, 1.0));
    bg::append(mls2[0], point_t(2.0, 1.0));

    bg::append(mls2[1], point_t(1.0, 0.0)); 
    bg::append(mls2[1], point_t(2.0, 0.0));


    std::cout << dsv(mls1) << std::endl;
    std::cout << dsv(mls2) << std::endl;
    double l = boost::size(mls1);

    std::cout << l << std::endl;
    typedef typename boost::range_size<mlinestring_t>::type size_type;

    point_t ls=bg::range::at(bg::range::at(mls1,0),0);
    std::cout << dsv(ls) << std::endl;


    typedef typename bg::point_type<linestring_2d> ::type geometry_t;
    linestring_2d ls1,ls2;
    const double c[][2] = { {3.1, 3.1}, {4.9, 1.1}, {3.1, 1.9} };
 	append(ls2, c);
 	const double c2[][2] = { {3.1, 3.1}, {4.9, 1.1}, {3.1, 1.9} };
 	append(ls1, c2);

    /*
    double FrechDis;
    FrechDis= bg:: frechet_distance(ls1,mls2);
    std::cout <<"FrechetDistance= " << FrechDis << std::endl;
    */
    typedef typename distance_result
        <
            typename point_type<linestring_2d>::type,
            typename point_type<mlinestring_t>::type
        >::type result_type;
        result_type a;
      a=  boost::size(mls1);
              typedef typename boost::range_size<mlinestring_t>::type size_type1;
        std::cout << typeid(result_type).name() << std::endl;
        std::cout << a << std:: endl; 
        std::cout << typeid(size_type1).name() << std:: endl;

    double HausDis;
    HausDis= bg:: hausdorff_distance(ls1,mls2);
    std::cout <<"HausdorffDistance= " << HausDis << std::endl;
    
	return 0;
}