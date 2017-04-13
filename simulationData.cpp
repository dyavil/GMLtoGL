#include "simulationData.h"


void simulationData::extractPlanes(Mesh & mesh){
	std::vector<std::pair<TriangleData, Vector>> wallPartsPos;
	std::vector<std::pair<TriangleData, Vector>> wallPartsMin;


	Vector normal;
	Vector normal2;
	bool set = false;
	bool set2 = false;
	for (int i = 0; i < mesh.triangle_count(); ++i)
	{
		TriangleData tmp = mesh.triangle(i);
		Point a(tmp.a);
		Point b(tmp.b);
		Point c(tmp.c);
		Vector ab = b-a;
		Vector ac = c-a;
		Vector n = normalize(cross(ac, ab));
		
		if (n.y > 0.80)
		{
			if(!set){
				normal = n;
				set = true;
			}
			else{
				normal = cross(normal, n);
			}
			wallPartsPos.push_back(std::make_pair(tmp, n));
		}
		else if (n.y < -0.80)
		{
			if(!set2){
				normal2 = n;
				set2 = true;
			}
			else{
				normal2 = cross(normal2, n);
			}
			wallPartsMin.push_back(std::make_pair(tmp, n));
		}
	}

	

	float moyY = 0;
	float min = 20000.0;
	for (unsigned int i = 0; i < wallPartsPos.size(); ++i)
	{
		//std::cout << wallPartsPos[i].second << std::endl;
		moyY += (wallPartsPos[i].first.a.y + wallPartsPos[i].first.b.y + wallPartsPos[i].first.c.y);
		if (wallPartsPos[i].first.a.y < min ) min = wallPartsPos[i].first.a.y;
		if (wallPartsPos[i].first.b.y < min ) min = wallPartsPos[i].first.b.y;
		if (wallPartsPos[i].first.c.y < min ) min = wallPartsPos[i].first.c.y;
	}
	moyY = moyY/(3*wallPartsPos.size());


	std::vector<std::vector<TriangleData>> rtest;

	Mesh temp2;
	temp2 = Mesh(GL_TRIANGLES);
	for (unsigned int i = 0; i < wallPartsPos.size(); ++i)
	{
		int a = temp2.vertex(wallPartsPos[i].first.a);
		int b = temp2.vertex(wallPartsPos[i].first.b);
		int c = temp2.vertex(wallPartsPos[i].first.c);
		temp2.triangle(a, b, c);
	}
	//if(temp2.triangle_count() > 0)tempMeshes.push_back(temp2);
	Point pmin2, pmax2;
	temp2.bounds(pmin2, pmax2);
	//planesMin.push_back(std::make_tuple(pmin2, pmax2, normal2));
	temp2.release();

	if(wallPartsPos.size() > 0) rtest = cutTriangleDataVec(wallPartsPos, 2);
	if(rtest.size() > 0) {
		//std::cout << "after call" << rtest.size() << std::endl;
		for (unsigned int i = 0; i < rtest.size(); ++i)
		{			
			Mesh temp;
			temp = Mesh(GL_TRIANGLES);
			float r = ((float) rand() / (RAND_MAX));
			float r1 = ((float) rand() / (RAND_MAX));
			float r2 = ((float) rand() / (RAND_MAX));
			temp.color(Color(r, r1, r2));
			for (unsigned int j = 0; j < rtest[i].size(); ++j)
			{
				//std::cout << wallPartsPos[i].second << std::endl;
				int a = temp.vertex(rtest[i][j].a);
				int b = temp.vertex(rtest[i][j].b);
				int c = temp.vertex(rtest[i][j].c);
				temp.triangle(a, b, c);
			}
			Point tmpmin, tmpmax;
			temp.bounds(tmpmin, tmpmax);
			if(temp.triangle_count() > 0 && distance(tmpmin, Point(tmpmax.x, tmpmin.y, tmpmax.z)) > 35 && distance(tmpmin, pmin2) < 20){
				planesPos.push_back(std::make_tuple(tmpmin, tmpmax, normal));
				tempMeshes.push_back(temp);
			}

		}
	}

	temp2 = Mesh(GL_TRIANGLES);
	for (unsigned int i = 0; i < wallPartsMin.size(); ++i)
	{
		int a = temp2.vertex(wallPartsMin[i].first.a);
		int b = temp2.vertex(wallPartsMin[i].first.b);
		int c = temp2.vertex(wallPartsMin[i].first.c);
		temp2.triangle(a, b, c);
	}
	//if(temp2.triangle_count() > 0)tempMeshes.push_back(temp2);
	temp2.bounds(pmin2, pmax2);

	std::vector<std::vector<TriangleData>> rtest2;
	if(wallPartsMin.size() > 0) rtest2 = cutTriangleDataVec(wallPartsMin, 2);
	if(rtest2.size() > 0) {
		//std::cout << "after call" << rtest2.size() << std::endl;
		for (unsigned int i = 0; i < rtest2.size(); ++i)
		{			
			Mesh temp;
			temp = Mesh(GL_TRIANGLES);
			float r = ((float) rand() / (RAND_MAX));
			float r1 = ((float) rand() / (RAND_MAX));
			float r2 = ((float) rand() / (RAND_MAX));
			temp.color(Color(r, r1, r2));
			for (unsigned int j = 0; j < rtest2[i].size(); ++j)
			{
				//std::cout << wallPartsMin[i].second << std::endl;
				int a = temp.vertex(rtest2[i][j].a);
				int b = temp.vertex(rtest2[i][j].b);
				int c = temp.vertex(rtest2[i][j].c);
				temp.triangle(a, b, c);
			}
			Point tmpmin, tmpmax;
			temp.bounds(tmpmin, tmpmax);
			if(temp.triangle_count() >0 && distance(tmpmin, Point(tmpmax.x, tmpmin.y, tmpmax.z)) > 35 && distance(tmpmax, pmax2) < 20){
				planesMin.push_back(std::make_tuple(tmpmin, tmpmax, normal2));
				tempMeshes.push_back(temp);
			}

		}
	}


	

}



std::vector<Mesh> simulationData::getPosPlanesMeshes()
{
	
	std::vector<Mesh> res;
	for (unsigned int i = 0; i < planesPos.size(); ++i)
	{
		Mesh tmp;
		tmp = Mesh(GL_TRIANGLES);
		tmp.color(Color(1, 0, 1));
		int a = tmp.vertex(Point((std::get<0>(planesPos[i])).x, (std::get<0>(planesPos[i])).y, (std::get<0>(planesPos[i])).z));
		int b = tmp.vertex(Point((std::get<1>(planesPos[i])).x, (std::get<1>(planesPos[i])).y, (std::get<1>(planesPos[i])).z));
		int c = tmp.vertex(Point((std::get<0>(planesPos[i])).x, (std::get<0>(planesPos[i])).y, (std::get<1>(planesPos[i])).z));
		int d = tmp.vertex(Point((std::get<1>(planesPos[i])).x, (std::get<1>(planesPos[i])).y, (std::get<0>(planesPos[i])).z));
		tmp.triangle(a, b, c);
		tmp.triangle(a, d, b);
		res.push_back(tmp);
	}
	return tempMeshes;
}

std::vector<Mesh> simulationData::getMinPlanesMeshes()
{
	
	std::vector<Mesh> res;
	for (unsigned int i = 0; i < planesMin.size(); ++i)
	{
		Mesh tmp;
		tmp = Mesh(GL_TRIANGLES);
		tmp.color(Color(1, 1, 0));
		int a = tmp.vertex(Point((std::get<0>(planesMin[i])).x, (std::get<0>(planesMin[i])).y, (std::get<0>(planesMin[i])).z));
		int b = tmp.vertex(Point((std::get<1>(planesMin[i])).x, (std::get<1>(planesMin[i])).y, (std::get<1>(planesMin[i])).z));
		int c = tmp.vertex(Point((std::get<0>(planesMin[i])).x, (std::get<0>(planesMin[i])).y, (std::get<1>(planesMin[i])).z));
		int d = tmp.vertex(Point((std::get<1>(planesMin[i])).x, (std::get<1>(planesMin[i])).y, (std::get<0>(planesMin[i])).z));
		tmp.triangle(d, c, b);
		tmp.triangle(d, a, c);
		res.push_back(tmp);
	}
	return res;
}


std::vector<std::vector<TriangleData>> simulationData::cutTriangleDataVec(std::vector<std::pair<TriangleData, Vector>> & in, int precision)
{
	std::vector<std::pair<TriangleData, bool>> togo;
	std::vector<std::vector<TriangleData>> res;
	for (unsigned int i = 0; i < in.size(); ++i)
	{
		togo.push_back(std::make_pair(in[i].first, false));
	}
	
	int j = 0;
	bool go = true;
	while(go)
	{
		std::vector<TriangleData> reselem;
		std::pair<TriangleData, bool> current = togo[j];
		Point a(current.first.a);
		Point b(current.first.b);
		Point c(current.first.c);
		std::vector<TriangleData> t1 = recur(a, togo, precision, j);
		std::vector<TriangleData> t2 = recur(b, togo, precision, j);
		std::vector<TriangleData> t3 = recur(c, togo, precision, j);

		for (unsigned int i = 0; i < t1.size(); ++i)
		{
			reselem.push_back(t1[i]);
		}
		for (unsigned int i = 0; i < t2.size(); ++i)
		{
			reselem.push_back(t2[i]);
		}
		for (unsigned int i = 0; i < t3.size(); ++i)
		{
			reselem.push_back(t3[i]);
		}

		unsigned int count = 0;
		togo[j].second = true;
		res.push_back(reselem);
		bool next = false;
		for (unsigned int i = 0; i < togo.size(); ++i)
		{
			if (togo[i].second)
			{
				count++;
			}else if(!next){
				next = true;
				j = i;
			}
			//std::cout << "after f " << togo[i].second <<std::endl;
		}
		if (count == togo.size()) go = false;
	}
	
	return res;
}


std::vector<TriangleData> simulationData::recur(Point cmp, std::vector<std::pair<TriangleData, bool>> & togo, int precision, int on)
{
	std::vector<TriangleData> res;
	for (unsigned int i = 0; i < togo.size(); ++i)
	{
		if (!togo[i].second && i != on)
		{

			if ((distance(cmp, Point(togo[i].first.a)) < precision) || (distance(cmp, Point(togo[i].first.b)) < precision) ||(distance(cmp, Point(togo[i].first.c)) < precision))
			{
				togo[i].second = true;
				res.push_back(togo[i].first);
				std::vector<TriangleData> t3 = recur(Point(togo[i].first.c), togo, precision, i);
				std::vector<TriangleData> t1 =recur(Point(togo[i].first.a), togo, precision, i);
				std::vector<TriangleData> t2 = recur(Point(togo[i].first.b), togo, precision, i);
				for (unsigned int j = 0; j < t1.size(); ++j)
				{
					res.push_back(t1[j]);
				}
				for (unsigned int j = 0; j < t2.size(); ++j)
				{
					res.push_back(t2[j]);
				}
				for (unsigned int j = 0; j < t3.size(); ++j)
				{
					res.push_back(t3[j]);
				}

			}
		}
	}	
	return res;
}

std::vector<Mesh> simulationData::splitMeshes(std::vector<Mesh> & m)
{
	std::vector<Mesh> res;
	for (unsigned int i = 0; i < m.size(); ++i)
	{
		std::vector<std::pair<TriangleData, Vector>> in;
		for (int j = 0; j < m[i].triangle_count(); ++j)
		{
			TriangleData tmp = m[i].triangle(j);
			in.push_back(std::make_pair(tmp, Vector(0, 0, 0)));
		}
		std::vector<std::vector<TriangleData>> restmp = cutTriangleDataVec(in, 5);
		if(restmp.size() > 0) {
			for (unsigned int k = 0; k < restmp.size(); ++k)
			{			
				Mesh temp;
				temp = Mesh(GL_TRIANGLES);
				float r = ((float) rand() / (RAND_MAX));
				float r1 = ((float) rand() / (RAND_MAX));
				float r2 = ((float) rand() / (RAND_MAX));
				temp.color(Color(r, r1, r2));
				for (unsigned int l = 0; l < restmp[k].size(); ++l)
				{
					int a = temp.vertex(restmp[k][l].a);
					int b = temp.vertex(restmp[k][l].b);
					int c = temp.vertex(restmp[k][l].c);
					temp.triangle(a, b, c);
				}
				Point tmpmin, tmpmax;
				temp.bounds(tmpmin, tmpmax);
				res.push_back(temp);

			}
		}
	}
	return res;
}