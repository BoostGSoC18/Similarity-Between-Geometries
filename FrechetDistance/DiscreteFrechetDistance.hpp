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

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/array.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/geometry/geometries/register/linestring.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>


namespace bnu = boost::numeric::ublas;
namespace bg = boost::geometry;

//Printing the Coupling Matrix
template <typename Mat,typename index>
class PrintMatrix {
private:
	Mat Matri;
	index a;
	index b;
public:
    PrintMatrix(Mat M, index a,index b);
    void print();
};
template <typename Mat,typename index>
PrintMatrix<Mat,index>::PrintMatrix(Mat M, index a1,index b1){
	Matri=M;
	a=a1;
	b=b1;
}
template <typename Mat,typename index>
void PrintMatrix<Mat,index>::print() {
 	std::cout << "Values in Coupling Matrix" << std::endl;
    for (int i = 1; i <=a; i++)
    {
		for(int j=1;j <=b;j++)
    	{
        	std::cout<<" "<<Matri(i,j);
    	}
    	std::cout << std::endl;
    }
}


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

bool isSubSequence(char str1[], char str2[], int m, int n)
{
    // Base Cases
    if (m == 0) return true;
    if (n == 0) return false;
 
    // If last characters of two strings are matching
    if (str1[m-1] == str2[n-1])
        return isSubSequence(str1, str2, m-1, n-1);
 
    // If last characters are not matching
    return isSubSequence(str1, str2, m, n-1);
}

//Calculating distance as per the Cartesian system
template <typename Point>
static inline double  Distance(Point const& P1,Point const& P2)
{
	std::string Str2=typeid(Point).name(),Str1="geographic",Str3="spherical";
	bool flag1=isSubSequence(&Str1[0],&Str2[0],(int)Str1.length(),(int)Str2.length());
	bool flag2=isSubSequence(&Str3[0],&Str2[0],(int)Str3.length(),(int)Str2.length());

	if(flag1)
	{
		double Radius=6371; // in Kliometer
		typedef  bg:: strategy :: distance :: haversine<double> HaverSineDistance;
		return bg::distance(P1,P2,HaverSineDistance(Radius));
	}
	else if(flag2)
	{
		double Radius=1; // radius of sphere
		typedef  bg:: strategy :: distance :: haversine<double> HaverSineDistance;
		return bg::distance(P1,P2,HaverSineDistance(Radius));
	}
	else 
	{

		typedef  bg:: strategy :: distance :: pythagoras<double> PythagorasDistance;

		return bg::distance(P1,P2,PythagorasDistance());
	}	

}

template<typename LineString>
static inline double FrechetDistance(LineString ls1,LineString ls2)
{
	double Dis=0;
	unsigned int  a = boost::size(ls1);
 	std::cout <<"size of linestring1 ="<< a << std::endl;
 	unsigned int  b = boost::size(ls2);
 	std::cout << "size of linestring2 ="<< b << std::endl;
 	bnu::matrix<double>  CoupMat(a+1,b+1,-1);
 	//findin the Coupling
 	//calling Recursion to get the coupling distance
 	double NonFeasible=-100;
 	for(unsigned int i=1;i<=a;i++)
 	{
 		for(unsigned  int j=1;j<=b;j++)
 		{
 			if(i==1 && j==1)
 				CoupMat(i,j)= Distance(ls1[i],ls2[j]);
 			else if(i==1 && j>1)
 				CoupMat(i,j)=max(CoupMat(i,j-1),Distance(ls1[i],ls2[j]));
 			else if(i>1 && j==1)
 				CoupMat(i,j)=max(CoupMat(i-1,j),Distance(ls1[i],ls2[j]));
 			else if(i>1 && j>1)
 				CoupMat(i,j)=max(min(min(CoupMat(i,j-1),CoupMat(i-1,j)),CoupMat(i-1,j-1)),Distance(ls1[i],ls2[j]));
 			else
 				CoupMat(i,j)=NonFeasible;
 		}
 	}

 	//Print CoupLing Matrix

 	
 	PrintMatrix<bnu::matrix<double>,int> A(CoupMat,a,b);
 	A.print();

 	//std::cout << CoupMat << std::endl;
	
	//Final Coupling Distance
	Dis=CoupMat(a,b);
	
	return Dis;
}
