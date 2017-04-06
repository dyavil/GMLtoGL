#include <stdio.h>
#include "objectToMesh.h"
#include <stdio.h>
#include "window.h"
#include "mesh.h"
#include "draw.h"
#include "orbiter.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <algorithm>    
#include <citygml/citygml.h>
#include <citygml/citymodel.h>
#include <citygml/cityobject.h>
#include <citygml/geometry.h>
#include <citygml/polygon.h>

// creation des objets openGL
Mesh mesh;
Mesh mesh2;
Mesh mesh3;
Mesh plane;
Mesh m_sphere;
Point cursor;
std::vector<Mesh> geometries;
Orbiter camera;
GLuint texture;
objectToMesh obm;
Transform t;
std::ofstream logfile;



void init_sphere()
{
    const int divBeta = 26;
    const int divAlpha = divBeta/2;
    int i,j;
    float beta, alpha, alpha2;

    m_sphere = Mesh(GL_TRIANGLE_STRIP);

    m_sphere.color( Color(1, 1, 1) );

    for(i=0;i<divAlpha;++i)
    {
        alpha = -0.5f*M_PI + float(i)*M_PI/divAlpha;
        alpha2 = -0.5f*M_PI + float(i+1)*M_PI/divAlpha;

        for(j=0;j<divBeta;++j)
        {
            beta = float(j)*2.f*M_PI/(divBeta-1);

            m_sphere.texcoord(beta/(2.0f*M_PI), 0.5f+alpha/M_PI);
            m_sphere.normal( Vector(cos(alpha)*cos(beta),  sin(alpha), cos(alpha)*sin(beta)) );
            m_sphere.vertex( Point(cos(alpha)*cos(beta),  sin(alpha), cos(alpha)*sin(beta)) );


            m_sphere.texcoord(beta/(2.0f*M_PI), 0.5f+alpha2/M_PI);
            m_sphere.normal( Vector(cos(alpha2)*cos(beta),  sin(alpha2), cos(alpha2)*sin(beta)) );
            m_sphere.vertex( Point(cos(alpha2)*cos(beta),  sin(alpha2), cos(alpha2)*sin(beta))   );
        }

        m_sphere.restart_strip();
    }
}

int init( )
{
    logfile.open ("GMLtoGL/log2.txt");
    ///home/dyavil/Documents/Master/TER/Part-2-Tunnel-Bridge-V3.gml
    ///home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_OBJET_REMARQUABLE_2012.gml
    ///home/dyavil/Downloads/CityGML_2.0_Test_Dataset_2012-04-23/Part-3-Railway-V2.gml
    ///home/dyavil/Downloads/citygml/examples/2.0/building/Building_LOD4-EPSG25832.gml
	citygml::ParserParams params;
	std::shared_ptr<const citygml::CityModel> city = citygml::load("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_BATI_2012.gml", params );
	cursor = Point();
    std::vector<std::string> thl= city->themes();
    std::string theme = "";

    for (unsigned int i = 0; i < thl.size(); ++i)
    {
        theme = thl[i];
    }
    init_sphere();
    

    const citygml::ConstCityObjects obj =  city->getRootCityObjects();
    obm.toMesh(obj, theme);
    
    obm.meshTo2D();
    mesh = obm.getMesh();
    Point pmin, pmax;
    mesh.bounds(pmin, pmax);
    obm.centerM(mesh, pmin, pmax);
    
    //obm.colorMeshTo2D();
    cursor = center(pmin, pmax);
    geometries = obm.getGeometriesMeshes();

    std::cout << geometries.size() <<std::endl;

    /*std::shared_ptr<const citygml::CityModel> city2 = citygml::load("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_WATER_2012.gml", params );
    
    std::vector<std::string> thl2= city2->themes();
    std::string theme2 = "";

    for (int i = 0; i < thl2.size(); ++i)
    {
        theme2 = thl2[i];
    }

    const citygml::ConstCityObjects obj2 =  city2->getRootCityObjects();
    mesh2 = obm.toMesh(obj2, theme2);
    obm.center(mesh2, pmin, pmax);
	
   
    std::shared_ptr<const citygml::CityModel> city3 = citygml::load("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_BATI_REMARQUABLE_2012.gml", params );
    
    std::vector<std::string> thl3= city3->themes();
    std::string theme3 = "";

    for (int i = 0; i < thl3.size(); ++i)
    {
        theme3 = thl3[i];
    }

    const citygml::ConstCityObjects obj3 =  city3->getRootCityObjects();
    mesh3 = obm.toMesh(obj3, theme3);
    obm.center(mesh3, pmin, pmax);*/



    mesh.bounds(pmin, pmax);
    obm.centerBase(m_sphere, pmin, pmax);
    Point spmin, spmax;
    m_sphere.bounds(spmin, spmax);
    cursor = center(spmin, spmax);
    std::cout << pmin << ", " << pmax << std::endl;
    std::cout << spmin << ", " << spmax << std::endl;
    logfile << pmin << ", " << std::endl;
    t = Scale(10, 10, 10);

    camera.lookat(pmin, pmax);


    //glClearDepth(1.f);
    //glClearColor(1, 0, 1, 0.5);
    glClearDepth(1.f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    return 0;   // renvoyer 0 ras, pas d'erreur, sinon renvoyer -1
}

// affichage
int draw( )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //draw(mesh, Identity(), Identity(), Identity());
    
    
    

    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
    
  
   // deplace la camera
    if(mb & SDL_BUTTON(1)) {             // le bouton gauche est enfonce
       // tourne autour de l'objet
        camera.rotation(mx, my);}
    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
           // approche / eloigne l'objet
        camera.move(mx);
    else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            // deplace le point de rotation
        camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

    if (key_state('z'))
    {
        t = t * Translation(Vector(0, 1, 0));
        cursor = t(Point(0, 0, 0));
        logfile << t(Point(0, 0, 0)) << ", " << std::endl;
    }
    if (key_state('d'))
    {
        t = t * Translation(Vector(1, 0, 0));
        cursor = t(Point(0, 0, 0));
    }

    if (key_state('s'))
    {
        t = t * Translation(Vector(0, -1, 0));
        cursor = t(Point(0, 0, 0));
    }
    if (key_state('q'))
    {
        t = t * Translation(Vector(-1, 0, 0));
        cursor = t(Point(0, 0, 0));
    }
    if(key_state('z') || key_state('d') || key_state('s') || key_state('q')) obm.computeShowned(cursor);
    draw(m_sphere,t,  camera);
    
    //draw(mesh, camera);
    for (unsigned int i = 0; i < obm.getShownedMeshes().size(); ++i)
    {
        draw(geometries[obm.getShownedMeshes()[i]], camera);
    }
    
    draw(mesh, camera);
    //draw(mesh3, camera);
    /*static float angle= 0;      // il faudrait declarer angle comme variable globale...
        if(key_state('j'))
            angle= angle + 3;
        if(key_state('k'))1
            angle= angle - 3;
        
        Transform T= RotationY( angle );
        draw(mesh2, T, Identity(), Identity());*/
    return 1;   // on continue, renvoyer 0 pour sortir de l'application
}

// destruction des objets openGL
int quit( )
{
    m_sphere.release();
    mesh.release();
    for (unsigned int i = 0; i < geometries.size(); ++i)
    {
        geometries[i].release();
    }
    /*for (unsigned int i = 0; i < obm.getShownedMeshes().size(); ++i)
    {
        obm.getShownedMeshes()[i].release();
    }*/
    logfile.close();
    return 0;   // ras, pas d'erreur
}


int main( int argc, char **argv )
{
    srand(time(NULL));
    // etape 1 : creer la fenetre
    Window window= create_window(1024, 640);
    if(window == NULL)
        return 1;       // erreur lors de la creation de la fenetre ou de l'init de sdl2

    // etape 2 : creer un contexte opengl pour pouvoir dessiner
    Context context= create_context(window);
    if(context == NULL)
        return 1;       // erreur lors de la creation du contexte opengl

    // etape 3 : creation des objets
    if(init() < 0)
    {
        printf("[error] init failed.\n");
        return 1;
    }

    // etape 4 : affichage de l'application, tant que la fenetre n'est pas fermee. ou que draw() ne renvoie pas 0
    run(window, draw);

    // etape 5 : nettoyage
    quit();
    release_context(context);
    release_window(window);
    return 0;
}

