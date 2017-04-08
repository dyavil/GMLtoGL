#include "objectToMesh.h"

void objectToMesh::toMesh(Mesh & mesh, const citygml::ConstCityObjects & obj, std::string them, std::vector<Mesh> & geometries){
	mesh = Mesh(GL_TRIANGLES);
	//mesh.color(Color(0.62, 0.60, 0.91));
	//mesh.color(Color(1, 0, 1));
	theme = them;
  	logfile.open ("GMLtoGL/log.txt");
  	float r = ((float) rand() / (RAND_MAX));
	float r1 = ((float) rand() / (RAND_MAX));
	float r2 = ((float) rand() / (RAND_MAX));
	for (unsigned int i = 0; i < obj.size(); ++i)
	{
		recursiveCall(mesh, obj[i], geometries, r, r1, r2, 0);
	}
	logfile.close();
	
	/*Point pmin, pmax;
    mesh.bounds(pmin, pmax);
    Point newcenter((pmin.x+pmax.x)/2, (pmin.y+pmax.y)/2, (pmin.z+pmax.z)/2);
    for (int i = 0; i < mesh.vertex_count(); ++i)
    {
        vec3 t = mesh.positions()[i];
        mesh.vertex(i, t.x-newcenter.x, t.y-newcenter.y, t.z-newcenter.z);
    }*/
}

void objectToMesh::centerM(Mesh & mesh, std::vector<Mesh> & geometries, Point pmax, Point pmin){
    Point newcenter((pmin.x+pmax.x)/2, (pmin.y+pmax.y)/2, (pmin.z+pmax.z)/2);
    for (int i = 0; i < mesh.vertex_count(); ++i)
    {
        vec3 t = mesh.positions()[i];
        mesh.vertex(i, t.x-newcenter.x, t.y-newcenter.y, t.z-newcenter.z);
    }
    for (unsigned int i = 0; i < geometries.size(); ++i)
    {
    	for (int j = 0; j < geometries[i].vertex_count(); ++j)
	    {
	        vec3 t = geometries[i].positions()[j];
	        geometries[i].vertex(j, t.x-newcenter.x, t.y-newcenter.y, t.z-newcenter.z);
	    }
    }
}


void objectToMesh::centerBase(Mesh & mesh, Point pmax, Point pmin){
    Point newcenter((pmin.x+pmax.x)/2, (pmin.y+pmax.y)/2, (pmin.z+pmax.z)/2);
    for (int i = 0; i < mesh.vertex_count(); ++i)
    {
        vec3 t = mesh.positions()[i];
        mesh.vertex(i, t.x-newcenter.x, t.y-newcenter.y, t.z-newcenter.z);
    }

}


void objectToMesh::recursiveCall(Mesh & mesh, const citygml::CityObject * obj, std::vector<Mesh> & geometries, float r, float r1, float r2, int deep)
{
	static int numb = 0;
	static int count = 0;
	count++;
	logfile << "Objet " << count << " a " << obj->getChildCityObjectsCount() << " enfants.\n";
	if (deep == 0)
	{
		r = ((float) rand() / (RAND_MAX));
		r1 = ((float) rand() / (RAND_MAX));
		r2 = ((float) rand() / (RAND_MAX));
	}
	for (unsigned int i = 0; i < obj->getChildCityObjectsCount(); ++i)
	{
		const citygml::CityObject *objc = &obj->getChildCityObject(i);
		recursiveCall(mesh, objc, geometries, r, r1, r2, deep+1);
	}
	numb += obj->getChildCityObjectsCount();
	logfile << "Objet " << count << " est composé de " << obj->getGeometriesCount() << " geometries.\n";
	
	for (unsigned int i = 0; i < obj->getGeometriesCount(); ++i)
	{
		
		citygml::Geometry gs = obj->getGeometry(i);
		recursiveGeometryCall(mesh, gs, geometries, r, r1, r2);
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


void objectToMesh::recursiveGeometryCall(Mesh & mesh, citygml::Geometry gs, std::vector<Mesh> & geometries, float r, float r1, float r2)
{
	static int count = 0;
	count++;
	logfile << "Geometry " << count << " est composée de " << gs.getGeometriesCount() << " gémotries enfant.\n";
	


	for (unsigned int i = 0; i < gs.getGeometriesCount(); ++i)
	{

		citygml::Geometry gsc = gs.getGeometry(i);
		recursiveGeometryCall(mesh, gsc, geometries, r, r1, r2);
	}
	logfile << "Geometry " << count << " est composée de " << gs.getPolygonsCount() << " polygon.\n";
	std::string s = "";
	
	Mesh temp;
	temp = Mesh(GL_TRIANGLES);
	temp.color(Color(r, r1, r2));
	for (unsigned int j = 0; j < gs.getPolygonsCount(); ++j)
		{
			std::shared_ptr<citygml::Polygon> pl = gs.getPolygon(j);
			/*std::vector<std::shared_ptr<citygml::LinearRing> > rings = pl->interiorRings();


			for (int i = 0; i < rings.size(); ++i)
			{
				std::cout <<  "taille : " << rings[i]->getVertices().size() << std::endl;
			}*/


			std::vector<TVec3d> tvec1 = pl->getVertices();
			std::vector<TVec2f> textvec = pl->getTexCoordsForTheme(theme, true);
			logfile << "Le polygon " << j << " de la geometry " << count << " est composé de " << tvec1.size() << " points.\n";
			int nbpoly = 0;
			float max = 0;
			float min = 2000000;
			float ymax = 0;
			float ymin = 6000000;
			float zmax = 0;
			float zmin = 2000000;
			
			//std::cout << tvec1.size() << "  " << textvec.size() << std::endl;
			for (unsigned int k = 0; k < tvec1.size(); ++k)
			{
				TVec3d vec1 = tvec1[k];
				float a, b, c;
				float aa, bb, cc;
				if (k < tvec1.size())
				{
					if(textvec.size() > k)mesh.texcoord(textvec[k].x, textvec[k].y);
					
					a = mesh.vertex(vec1.x-1700000, vec1.y-5200000, vec1.z);
					aa = temp.vertex(vec1.x-1700000, vec1.y-5200000, vec1.z);
					TVec3d vecnorm = vec1.normal();
					mesh.normal(vecnorm.x, vecnorm.y, vecnorm.z);
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
					if(textvec.size() > k+1) mesh.texcoord(textvec[k+1].x, textvec[k+1].y);
					b = mesh.vertex(vec1.x-1700000, vec1.y-5200000, vec1.z);
					bb = temp.vertex(vec1.x-1700000, vec1.y-5200000, vec1.z);
					TVec3d vecnorm = vec1.normal();
					mesh.normal(vecnorm.x, vecnorm.y, vecnorm.z);	
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
					if(textvec.size() > k+2)mesh.texcoord(textvec[k+2].x, textvec[k+2].y);
					c = mesh.vertex(vec1.x-1700000, vec1.y-5200000, vec1.z);
					cc = temp.vertex(vec1.x-1700000, vec1.y-5200000, vec1.z);
					TVec3d vecnorm = vec1.normal();
					mesh.normal(vecnorm.x, vecnorm.y, vecnorm.z);
					if (vec1[0] > max) max = vec1[0];
					if (vec1[0] < min) min = vec1[0];
					if (vec1[1] > ymax) ymax = vec1[1];
					if (vec1[1] < ymin) ymin = vec1[1];
					if (vec1[2] > zmax) zmax = vec1[2];
					if (vec1[2] < zmin) zmin = vec1[2];

					
					//mesh.triangle(b, c, a);
					mesh.triangle(a, b, c);
					temp.triangle(aa, bb, cc);
					nbpoly++;
					//logfile << "Triangle : " << a << "  " << "  " << b << "   " << c << "\n";
				}
			    //k++;
			    
			   
			}

			logfile << "xdiff : " << max-min << ", ydiff : " << ymax-ymin << ", zdiff : " << zmax-zmin << "\n";

		}
		geometries.push_back(temp);
}

