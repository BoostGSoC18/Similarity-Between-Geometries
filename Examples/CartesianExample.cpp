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

//#include "../FrechetDistance/DiscreteFrechetDistance.hpp"
#include "../HausdorffDistance/DiscreteHausdorffDistance.hpp"

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/include/size.hpp>


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
    typedef model::d2::point_xy<double> point_2d;
    typedef model::linestring<point_2d> linestring_2d;
    linestring_2d ls1,ls2;

    // points can be created using "make" and added to a linestring using the std:: "push_back"
    ls1.push_back(make<point_2d>(1.1, 1.1));

    // points can also be assigned using "assign_values" and added to a linestring using "append"
    point_2d lp;
    assign_values(lp, 2.5, 2.1);
    append(ls1, lp);
    append(ls1,make<point_2d>(4.5,7.0));
 
 
    typedef bg::coordinate_system<point_2d>::type CordType;
    std::cout << typeid(CordType).name() << std::endl;


    
 	const double c[][2] = { {3.1, 3.1}, {4.9, 1.1}, {3.1, 1.9} };
 	append(ls2, c);

// Lines can be streamed using DSV (delimiter separated values)
    #ifdef BOOST_GEOMETRY_DETAIL_DEBUG_FRECHET_OR_SOMETHING
    std::cout << dsv(ls1) << std::endl;
    std::cout << dsv(ls2) << std::endl;
    #endif

    /*
    double FrechDis;
    FrechDis= bg::algorithms :: frechet_distance(ls1,ls2);
    std::cout <<"FrechetDistance= " << FrechDis << std::endl;
    */
    
    double HausDis;
    HausDis= bg::algorithms :: hausdorff_distance(ls1,ls2);
    std::cout <<"HausdorffDistance= " << HausDis << std::endl;
    return 0;
    
 }
