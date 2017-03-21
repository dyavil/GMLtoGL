#include "objectToMesh.h"

Mesh objectToMesh::toMesh(const citygml::ConstCityObjects & obj){
	Mesh mesh;
	mesh = Mesh(GL_TRIANGLES);
	//mesh.color(Color(1, 0, 1));
  	logfile.open ("GMLtoGL/log.txt");
	for (unsigned int i = 0; i < obj.size(); ++i)
	{
		recursiveCall(mesh, obj[i]);
	}
	logfile.close();
	return mesh;
}



void objectToMesh::recursiveCall(Mesh & mesh, const citygml::CityObject * obj)
{
	static int numb = 0;
	static int count = 0;
	count++;
	logfile << "Objet " << count << " a " << obj->getChildCityObjectsCount() << " enfants.\n";
	for (unsigned int i = 0; i < obj->getChildCityObjectsCount(); ++i)
	{
		const citygml::CityObject *objc = &obj->getChildCityObject(i);
		recursiveCall(mesh, objc);
	}
	numb += obj->getChildCityObjectsCount();
	logfile << "Objet " << count << " est composé de " << obj->getGeometriesCount() << " geometries.\n";
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
			//std::cout << "geoimp" << std::endl;
		}
	}
}


void objectToMesh::recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs)
{
	static int count = 0;
	count++;
	logfile << "Geometry " << count << " est composée de " << gs.getGeometriesCount() << " gémotries enfant.\n";
	for (unsigned int i = 0; i < gs.getGeometriesCount(); ++i)
	{
		citygml::Geometry gsc = gs.getGeometry(i);
		recursiveGeometryCall(mesh, gsc);
	}
	logfile << "Geometry " << count << " est composée de " << gs.getPolygonsCount() << " polygon.\n";
	for (unsigned int j = 0; j < gs.getPolygonsCount(); ++j)
		{
			std::shared_ptr<citygml::Polygon> pl = gs.getPolygon(j);
			/*std::vector<std::shared_ptr<citygml::LinearRing> > rings = pl->interiorRings();


			for (int i = 0; i < rings.size(); ++i)
			{
				std::cout <<  "taille : " << rings[i]->getVertices().size() << std::endl;
			}*/


			std::vector<TVec3d> tvec1 = pl->getVertices();
			logfile << "Le polygon " << j << " de la geometry " << count << " est composé de " << tvec1.size() << " points.\n";
			int nbpoly = 0;
			float max = 0;
			float min = 2000000;
			float ymax = 0;
			float ymin = 6000000;
			float zmax = 0;
			float zmin = 2000000;
			for (unsigned int k = 0; k < tvec1.size(); ++k)
			{
				TVec3d vec1 = tvec1[k];
				float a, b, c;
				if (k < tvec1.size())
				{
					a = mesh.vertex(vec1[0]-1700000, vec1[1]-5200000, vec1[2]);
					if (vec1[0] > max) max = vec1[0];
					if (vec1[0] < min) min = vec1[0];
					if (vec1[1] > ymax) ymax = vec1[1];
					if (vec1[1] < ymin) ymin = vec1[1];
					if (vec1[2] > zmax) zmax = vec1[2];
					if (vec1[2] < zmin) zmin = vec1[2];
					
				    vec1 = tvec1[k+1];
				}
				if (k+1 < tvec1.size())
				{
					b = mesh.vertex(vec1[0]-1700000, vec1[1]-5200000, vec1[2]);
					if (vec1[0] > max) max = vec1[0];
					if (vec1[0] < min) min = vec1[0];
					if (vec1[1] > ymax) ymax = vec1[1];
					if (vec1[1] < ymin) ymin = vec1[1];
					if (vec1[2] > zmax) zmax = vec1[2];
					if (vec1[2] < zmin) zmin = vec1[2];

				    vec1 = tvec1[k+2];
				}
				if (k+2 < tvec1.size())
				{
					c = mesh.vertex(vec1[0]-1700000, vec1[1]-5200000, vec1[2]);
					if (vec1[0] > max) max = vec1[0];
					if (vec1[0] < min) min = vec1[0];
					if (vec1[1] > ymax) ymax = vec1[1];
					if (vec1[1] < ymin) ymin = vec1[1];
					if (vec1[2] > zmax) zmax = vec1[2];
					if (vec1[2] < zmin) zmin = vec1[2];
					mesh.triangle(a, b, c);

					nbpoly++;
					//logfile << "Triangle : " << a << "  " << "  " << b << "   " << c << "\n";
				}
			    k++;
			    
			   
			}
			logfile << "xdiff : " << max-min << ", ydiff : " << ymax-ymin << ", zdiff : " << zmax-zmin << "\n";
			//std::cout << nbpoly << std::endl;
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


