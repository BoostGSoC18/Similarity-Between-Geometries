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
#include <boost/mpl/assert.hpp>
#include <boost/geometry/core/tag.hpp>


namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace hausdorff_distance
{

struct linestring_linestring
{
    template <typename Linestring1, typename Linestring2, typename Strategy>
    static inline typename distance_result
        <
            typename point_type<Linestring1>::type,
            typename point_type<Linestring2>::type,
            Strategy
        >::type apply(Linestring1 const& ls1, Linestring2 const& ls2, Strategy const& strategy)
    {
        // ************************************************
        // Here goes your implementation for linestrings
        // ************************************************
        // Strategy is Pt-Pt distance strategy
        // when you want to get distance between 2 points you call:
        // dist = geometry::distance(p1, p2, strategy);
        // e.g.:
        
        typedef typename distance_result
            <
                typename point_type<Linestring1>::type,
                typename point_type<Linestring2>::type,
                Strategy
            >::type result_type;
        typedef typename boost::range_size<Linestring1>::type size_type;
        
        result_type DisMax=0,DisMin;
        result_type const infinite = std::numeric_limits<double>::max();
        
        size_type  a = boost::size(ls1);
        size_type  b = boost::size(ls2);

        //findin the HausdorffDistance
        for(size_type i=0;i<a;i++)
        {
            DisMin= std::numeric_limits<result_type>::max() ;
                for(size_type j=0;j<b;j++)
                {
                    result_type DisTemp = geometry::distance(range::at(ls1, 0), range::at(ls2, 0), strategy);
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
    }
};

}} // namespace detail::hausdorff_distance
#endif // DOXYGEN_NO_DETAIL

#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{

// Implementation's default entry point
// instantiated if it's not specialized (dispatched) for specific input
template
<
    typename Geometry1,
    typename Geometry2,
    typename Tag1 = typename tag<Geometry1>::type,
    typename Tag2 = typename tag<Geometry2>::type
>
struct hausdorff_distance : not_implemented<Tag1, Tag2>
{};

// Specialization for linestrings using your implementation above
template <typename Linestring1, typename Linestring2>
struct hausdorff_distance<Linestring1,Linestring2,linestring_tag,linestring_tag>
    : detail::hausdorff_distance::linestring_linestring
{};

} // namespace dispatch
#endif // DOXYGEN_NO_DISPATCH

// Algorithm overload using explicitly passed Pt-Pt distance strategy
template <typename Geometry1, typename Geometry2, typename Strategy>
inline typename distance_result
        <
            typename point_type<Geometry1>::type,
            typename point_type<Geometry2>::type,
            Strategy
        >::type
hausdorff_distance(Geometry1 const& g1, Geometry2 const& g2, Strategy const& strategy)
{
    return dispatch::hausdorff_distance<Geometry1, Geometry2>::apply(g1, g2, strategy);
}

// Algorithm overload using default Pt-Pt distance strategy
template <typename Geometry1, typename Geometry2>
inline typename distance_result
        <
            typename point_type<Geometry1>::type,
            typename point_type<Geometry2>::type
        >::type
hausdorff_distance(Geometry1 const& g1, Geometry2 const& g2)
{
    typedef typename strategy::distance::services::default_strategy
              <
                  point_tag, point_tag,
                  typename point_type<Geometry1>::type,
                  typename point_type<Geometry2>::type
              >::type strategy_type;
    
    return hausdorff_distance(g1, g2, strategy_type());
}

}} // namespace boost::geometry
