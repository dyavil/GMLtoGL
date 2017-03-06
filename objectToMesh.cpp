#include "objectToMesh.h"

Mesh objectToMesh::toMesh(const citygml::ConstCityObjects & obj){
	Mesh mesh;
	mesh = Mesh(GL_TRIANGLES);
	for (unsigned int i = 0; i < obj.size(); ++i)
	{
		recursiveCall(mesh, obj[i]);
	}
	return mesh;
}

void objectToMesh::recursiveCall(Mesh & mesh, const citygml::CityObject * obj)
{
	static int numb = 0;
	for (unsigned int i = 0; i < obj->getChildCityObjectsCount(); ++i)
	{
		const citygml::CityObject *objc = &obj->getChildCityObject(i);
		recursiveCall(mesh, objc);
	}
	numb += obj->getChildCityObjectsCount();
	for (unsigned int i = 0; i < obj->getGeometriesCount(); ++i)
	{
		citygml::Geometry gs = obj->getGeometry(i);
		recursiveGeometryCall(mesh, gs);
	}
	for (unsigned int i = 0; i < obj->getImplicitGeometryCount(); ++i)
	{
		citygml::ImplicitGeometry igs = obj->getImplicitGeometry(i);
		for (unsigned int j = 0; j < igs.getGeometriesCount(); ++j)
		{
			std::cout << "geoimp" << std::endl;
		}
	}
}


void objectToMesh::recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs)
{
	for (unsigned int i = 0; i < gs.getGeometriesCount(); ++i)
	{
		citygml::Geometry gsc = gs.getGeometry(i);
		recursiveGeometryCall(mesh, gsc);
	}
	for (unsigned int j = 0; j < gs.getPolygonsCount(); ++j)
		{
			std::shared_ptr<citygml::Polygon> pl = gs.getPolygon(j);
			/*std::vector<std::shared_ptr<citygml::LinearRing> > rings = pl->interiorRings();


			for (int i = 0; i < rings.size(); ++i)
			{
				std::cout <<  "taille : " << rings[i]->getVertices().size() << std::endl;
			}*/


			std::vector<TVec3d> tvec1 = pl->getVertices();
			for (unsigned int k = 0; k < tvec1.size(); ++k)
			{
				TVec3d vec1 = tvec1[k];
				unsigned int a, b, c;
				if (k < tvec1.size())
				{
					a = mesh.vertex(vec1[0], vec1[1], vec1[2]);
				    vec1 = tvec1[k+1];
				}
				if (k+1 < tvec1.size())
				{
					b = mesh.vertex(vec1[0], vec1[1], vec1[2]);
				    vec1 = tvec1[k+2];
				}
				if (k+2 < tvec1.size())
				{
					c = mesh.vertex(vec1[0], vec1[1], vec1[2]);
					mesh.triangle(a, b, c);
				}
			    
			    k++;
			    
			   
			}
			std::cout << tvec1.size() << std::endl;
		    /*TVec3d vec1 = tvec1[0];
		    unsigned int a = mesh.vertex(vec1[0], vec1[1], vec1[2]);
		    vec1 = tvec1[1];
		    unsigned int b = mesh.vertex(vec1[0], vec1[1], vec1[2]);
		    vec1 = tvec1[2];
		    unsigned int c = mesh.vertex(vec1[0], vec1[1], vec1[2]);
		    std::cout << tvec1.size() << "  " << tvec1[0][0] << "  " << tvec1[3][0] << std::endl;
		    mesh.triangle(a, b, c);*/
		}
}


