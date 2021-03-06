
#include "SideElement.h"
#include "RayLine.h"
#include "LineSegment.h"
#include "MooseRandom.h"
#include "libmesh/fe_interface.h"
#include "libmesh/fe_type.h"

//#include "RandomInterface.h"
using namespace std;

SideElement::SideElement(const Elem *elem, const Point normal, Real transmissivity, Real absorptivity, Real diffuse_reflectivity, Real mirrors_reflectivity) :
 _transmissivity(transmissivity),
 _absorptivity(absorptivity),
 _diffuse_reflectivity(diffuse_reflectivity),
 _mirrors_reflectivity(mirrors_reflectivity),
 _elem(elem),
 _normal(normal)
{
}

RayLine SideElement::sendRay()
{
//	MooseRandom::seed(0);
	Real xi=1.0;
	Real eta=1.0;
	Real theita = 2*pi*MooseRandom::rand();
	Real phi = acos(1 - 2*MooseRandom::rand());

	unsigned int dim = _elem->dim();
	Point p = _elem->centroid();

	if (dim == 2)
	{
		if (_elem->type() == TRI3)
		{
			while(xi+eta>1.0)
			{
				xi=MooseRandom::rand();
				eta=MooseRandom::rand();
			}
		p=FEInterface::map(dim, FEType(), _elem, Point( xi, eta));
		}

		else if (_elem->type() == QUAD4)
		{
			Real S1_percent=((*(_elem->get_node(1)))-(*(_elem->get_node(0))))*((*(_elem->get_node(2)))-(*(_elem->get_node(0))))/(2.0*_elem->volume());
			Real Rs=MooseRandom::rand();

			while(xi+eta>1.0)
			{
				xi=MooseRandom::rand();
				eta=MooseRandom::rand();
			}

			if(Rs<S1_percent)
				p=(1-xi-eta)*(*(_elem->get_node(0)))+xi*(*(_elem->get_node(1)))+eta*(*(_elem->get_node(2)));

			else
				p=(1-xi-eta)*(*(_elem->get_node(0)))+xi*(*(_elem->get_node(2)))+eta*(*(_elem->get_node(3)));
		}

		else
			mooseError("产生随机位置时不支持的网格形状：" << _elem->type());

		Point O1 = p+_normal;

		Point M(cos(theita)*sin(phi),sin(theita)*sin(phi),cos(phi));
		M+=O1;

		if((M-p).size()<TOLERANCE)
		{
			return RayLine(p,_normal);
		}

		else
		{
			return RayLine(p,(M-p).unit());
		}
	}

	else if (dim == 1)
	{
		xi=MooseRandom::rand();
		p=FEInterface::map(dim, FEType(), _elem, Point( (2*xi-1.0), 0.0));
		Point O1 = p+_normal;

		Point M(sin(theita)*sin(phi),cos(phi));
//		Point M(cos(theita),sin(theita));
		M+=O1;

		if((M-p).size()<TOLERANCE)
		{
			return RayLine(p,_normal);
		}

		else
		{
			return RayLine(p,(M-p).unit());
		}
	}

	else // 1D
	{
		return RayLine(p,p+100000*_normal);
	}
}

RayLine SideElement::diffuseReflectRay(RayLine* rayline, Point point)
{
//	MooseRandom::seed(0);
	Real theita = 2*pi*MooseRandom::rand();
	Real phi = acos(1 - 2*MooseRandom::rand());

	unsigned int dim = _elem->dim();

	if (dim == 2)
	{
		Point O1 = point+_normal;

		Point M(cos(theita)*sin(phi),sin(theita)*sin(phi),cos(phi));
		M+=O1;

		if((M-point).size()<TOLERANCE)
		{
			return RayLine(point,_normal);
		}

		else
		{
			return RayLine(point,(M-point).unit());
		}
	}

	else if (dim == 1)
	{
		Point O1 = point+_normal;
//		cout << "_normal:" << _normal << endl;
//		cout << "O1:" << O1 << endl;

//		Point M(cos(theita),sin(theita));
		Point M(sin(theita)*sin(phi),cos(phi));
		M=O1+M;

		if((M-point).size()<TOLERANCE)
		{
			return RayLine(point,_normal);
		}

		else
		{
//			cout << "M:" << M << endl;
//			cout << "point:" << point << endl;
//			cout << "(M-point).unit():" << (M-point).unit() << endl;
			return RayLine(point,(M-point).unit());
		}
	}

	else // 1D
	{
		return RayLine(point,_normal);
	}

}

RayLine SideElement::mirrorsReflectRay(RayLine* rayline, Point point)
{
	Point normal_in=rayline->_normal;
	Point normal_out=normal_in-_normal*(2*(_normal*normal_in)/(normal_in.size()*_normal.size()));

	return RayLine(point,normal_out);
}
