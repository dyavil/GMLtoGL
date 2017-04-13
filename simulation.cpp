#include "simulation.h"

void simulation::init(std::string f){
	gmlfiles.push_back(f);
	globals.push_back(Mesh());
	citygml::ParserParams params;
	std::cout << "\nLoading and parsing file...\n" <<std::endl;
	std::shared_ptr<const citygml::CityModel> city = citygml::load(f, params );

    std::vector<std::string> thl= city->themes();
    std::string theme = "";

    for (unsigned int i = 0; i < thl.size(); ++i)
    {
        theme = thl[i];
    }
    
    std::cout << "Generating meshes...\n" << std::endl;
    const citygml::ConstCityObjects obj =  city->getRootCityObjects();
    obm.toMesh(globals[0], obj, theme, geometries);

    Point pmin, pmax;
    globals[0].bounds(pmin, pmax);
    globalmin = pmin;
    globalmax = pmax;
    obm.centerM(globals[0], geometries, pmin, pmax);
    globals[0].bounds(pmin, pmax);
    
    simulationData dat;
    

    
    
    std::cout << "splitting geometries...\n" << geometries.size() << std::endl;
    geometries = dat.splitMeshes(geometries);
    for (unsigned int i = 0; i < geometries.size(); ++i)
    {
    	Point tpmin, tpmax;
    	geometries[i].bounds(tpmin, tpmax);

    	boxes.push_back(std::make_pair(tpmin, tpmax));
    }
    std::cout << "splitting ended..."<< geometries.size() <<"\n\nExtracting planes...\n" << std::endl;

    for (unsigned int i = 0; i < geometries.size(); ++i)
    {
    	dat.extractPlanes(geometries[i]);
    	//if (m.triangle_count() > 0) walls.push_back(m);
    }
    walls = dat.getPosPlanesMeshes();
    std::vector<Mesh> walltemp = dat.getMinPlanesMeshes();
    for (unsigned int i = 0; i < walltemp.size(); ++i)
    {
    	walls.push_back(walltemp[i]);
    }
    std::cout << "Done\n" << std::endl;

    camera.lookat(pmin, pmax);
    meshTo2D(0);
    init_user();
    Point spmin, spmax;
	user.bounds(spmin, spmax);
	cursor = center(spmin, spmax);
	
	float fl = computeShowned(cursor);
	userMovement = Identity();
	userMovement = userMovement*Translation(Vector(0, 0, ground));
	std::cout << "Init ended\n" << std::endl;
}

void simulation::addFile(std::string f){
	gmlfiles.push_back(f);
	citygml::ParserParams params;
	std::cout << "\nAdding file\nLoading and parsing file...\n" <<std::endl;
	std::shared_ptr<const citygml::CityModel> city = citygml::load(f, params );

    std::vector<std::string> thl= city->themes();
    std::string theme = "";
    unsigned int prev = geometries.size();

    for (unsigned int i = 0; i < thl.size(); ++i)
    {
        theme = thl[i];
    }
    
    std::cout << "Generating meshes...\n" << std::endl;
    globals.push_back(Mesh());
    const citygml::ConstCityObjects obj =  city->getRootCityObjects();
    obm.toMesh(globals[globals.size()-1], obj, theme, geometries);
    obm.centerBase(globals[globals.size()-1], globalmin, globalmax);
    meshTo2D(globals.size()-1);
    for (unsigned int i = prev; i < geometries.size(); ++i)
    {
    	obm.centerBase(geometries[i], globalmin, globalmax);
    	Point tpmin, tpmax;
    	geometries[i].bounds(tpmin, tpmax);
    	boxes.push_back(std::make_pair(tpmin, tpmax));
    }
}


float simulation::distanceM(Point & a, Point & bpmin, Point & bpmax){
	
	Point p = center(bpmin, bpmax);
	ground = p.z+220;
	float d1 = distance(a, bpmin);
	float d2 = distance(a, bpmax);
	//float d3 = distance(a, p);
	float d4 = distance(a, Point(bpmax.x, bpmin.y, p.z));
	float d5 = distance(a, Point(bpmin.x, bpmax.y, p.z));

	float r1 = std::min(d1, d2);
	//float r2 = std::min(r1, d3);
	float r3 = std::min(r1, d4);
	float rb = std::min(r3, d5);

	float distTop = 2000.0;
	float distBottom = 2000.0;
	if (a.x <= bpmax.x && a.x >= bpmin.x)
	{
		//arrte top et bottom
		distTop = distance(a, Point(a.x, bpmax.y, a.z));
		distBottom = distance(a, Point(a.x, bpmin.y, a.z));
	}
	float distLeft = 2000.0;
	float distRight = 2000.0;
	if (a.y <= bpmax.y && a.y >= bpmin.y)
	{
		//arrte left et right
		distLeft = distance(a, Point(bpmin.x, a.y, a.z));
		distRight = distance(a, Point(bpmax.x, a.y, a.z));
	}
	float tt1 = std::min(distRight, distLeft);
	float tt2 = std::min(tt1, distTop);
	float tt3 = std::min(tt2, distBottom);
	float tt4 = std::min(tt3, rb);
	return tt4;
}



void simulation::meshTo2D(int id){
	for (int i = 0; i < globals[id].index_count(); ++i)
	{
		int pos = globals[id].indices()[i];
		vec3 temp = globals[id].positions()[pos];	
		globals[id].vertex(globals[id].indices()[i], temp.x, temp.y, 150.0);	
	}
}

void simulation::geometriesTo2D(){
	for (unsigned int j = 0; j < geometries.size(); ++j)
	{
		for (int i = 0; i < geometries[j].index_count(); ++i)
		{
			int pos = geometries[j].indices()[i];
			vec3 temp = geometries[j].positions()[pos];	
			geometries[j].vertex(geometries[j].indices()[i], temp.x, temp.y, 0.0);	
		}
	}
	
}


bool simulation::inBox(Point & a, Point & bpmin, Point & bpmax){
	if (a.x <= bpmax.x && a.x >= bpmin.x){
		if (a.y <= bpmax.y && a.y >= bpmin.y){
			return true;
		}
	}
	return false;
}

float simulation::computeShowned(Point p){
	const int maxdist = 100;
	showned.clear();
	float temp = 0;
	int nb = 0;
	for (unsigned int i = 0; i < geometries.size(); ++i)
	{		
		if ((distanceM(p, boxes[i].first, boxes[i].second) < maxdist) || inBox(p, boxes[i].first, boxes[i].second))
		{
			showned.push_back(i);
			nb++;
			temp += boxes[i].first.z+220;

		}
	}
	if(nb>0) return temp/nb;
	return 220;
}


void simulation::run(int action, int mx, int my){

	if (key_state('z'))
	{
		
	    userMovement = userMovement * Translation(Vector(0, 5, 0));
	    cursor = userMovement(Point(0, 0, 0));
	    
	}
	if (key_state('d'))
	{
	    userMovement = userMovement * Translation(Vector(5, 0, 0));
	    cursor = userMovement(Point(0, 0, 0));
	    
	}

	if (key_state('s'))
	{
	    userMovement = userMovement * Translation(Vector(0, -5, 0));
	    cursor = userMovement(Point(0, 0, 0));
	}
	if (key_state('q'))
	{
	    userMovement = userMovement * Translation(Vector(-5, 0, 0));
	    cursor = userMovement(Point(0, 0, 0));
	}
	if(key_state('z') || key_state('d') || key_state('s') || key_state('q')){
		ground = computeShowned(cursor);

		Point t = Point(cursor.x, cursor.y, ground);
		//std::cout << t-cursor << cursor << std::endl;
		//std::cout << userMovement << std::endl;
		userMovement = userMovement*Translation((t-cursor));
		cursor.z = ground;
	}
	draw(user, userMovement*Scale(15, 15, 15),  camera);


	for (unsigned int i = 0; i < showned.size(); ++i)
    {
        draw(geometries[showned[i]], Translation(Vector(0, 0, 220)), camera);
    }
    for (unsigned int i = 0; i < walls.size(); ++i)
    {
        draw(walls[i],Translation(Vector(0, 0, 220)), camera);
    }
    
    for (unsigned int i = 0; i < globals.size(); ++i)
    {
    	draw(globals[i], camera);
    }
    
}

void simulation::init_user()
{
    const int divBeta = 26;
    const int divAlpha = divBeta/2;
    int i,j;
    float beta, alpha, alpha2;

    user = Mesh(GL_TRIANGLE_STRIP);

    user.color( Color(0.1, 0.8, 0.1) );

    for(i=0;i<divAlpha;++i)
    {
        alpha = -0.5f*M_PI + float(i)*M_PI/divAlpha;
        alpha2 = -0.5f*M_PI + float(i+1)*M_PI/divAlpha;

        for(j=0;j<divBeta;++j)
        {
            beta = float(j)*2.f*M_PI/(divBeta-1);

            user.texcoord(beta/(2.0f*M_PI), 0.5f+alpha/M_PI);
            user.normal( Vector(cos(alpha)*cos(beta),  sin(alpha), cos(alpha)*sin(beta)) );
            user.vertex( Point(cos(alpha)*cos(beta),  sin(alpha), cos(alpha)*sin(beta)) );


            user.texcoord(beta/(2.0f*M_PI), 0.5f+alpha2/M_PI);
            user.normal( Vector(cos(alpha2)*cos(beta),  sin(alpha2), cos(alpha2)*sin(beta)) );
            user.vertex( Point(cos(alpha2)*cos(beta),  sin(alpha2), cos(alpha2)*sin(beta))   );
        }

        user.restart_strip();
    }
}

void simulation::quit(){
	user.release();
    globals[0].release();
    for (unsigned int i = 0; i < geometries.size(); ++i)
    {
        geometries[i].release();
    }
}