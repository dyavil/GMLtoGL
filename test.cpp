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
Orbiter camera;
GLuint texture;

int init( )
{
    objectToMesh obm;
    ///home/dyavil/Documents/Master/TER/Part-2-Tunnel-Bridge-V3.gml
    ///home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_OBJET_REMARQUABLE_2012.gml
    ///home/dyavil/Downloads/CityGML_2.0_Test_Dataset_2012-04-23/Part-3-Railway-V2.gml
    ///home/dyavil/Downloads/citygml/examples/2.0/building/Building_LOD4-EPSG25832.gml
	citygml::ParserParams params;
	std::shared_ptr<const citygml::CityModel> city = citygml::load("/home/dyavil/Documents/TER/LYON_1ER_2012/LYON_1ER_BATI_2012.gml", params );
	std::cout << "load" << std::endl;
    std::vector<std::string> thl= city->themes();
    std::string theme = "";

    for (int i = 0; i < thl.size(); ++i)
    {
        theme = thl[i];
        std::cout << "in" << std::endl;
    }

    const citygml::ConstCityObjects obj =  city->getRootCityObjects();
    
    std::cout << "object" << std::endl;
	mesh = obm.toMesh(obj, theme);
    mesh2.color(Color(1, 0, 1));

	
   
    

    mesh2 = Mesh(GL_TRIANGLES);
    Point center((142320+142340+142500)/3, (-24356-24250-24240)/3, 0);
    unsigned int a = mesh2.vertex(142320, -24356, 0);
    unsigned int b = mesh2.vertex(142340, -24250, 0);
    unsigned int c = mesh2.vertex(142500, -24240, 0);
    //unsigned int d = mesh2.vertex(-0.5, 0, 0);

    /*unsigned int e = mesh2.vertex(-0.5, -0.5, -0.5);
    unsigned int f = mesh2.vertex(0, -0.5, -0.5);
    unsigned int g = mesh2.vertex(0, 0, -0.5);
    unsigned int h = mesh2.vertex(-0.5, 0, -0.5);*/

    
    mesh2.triangle(a, c, b);
    mesh2.triangle(a, b, c);
    mesh2.triangle(c, a, b);
    /*mesh2.triangle(e, h, g);

    mesh2.triangle(e, a, d);
    mesh2.triangle(e, d, h);

    mesh2.triangle(b, f, g);
    mesh2.triangle(b, g, c);

    mesh2.triangle(a, b, c);
    mesh2.triangle(a, c, d);*/

    Point pmin, pmax;
    mesh.bounds(pmin, pmax);
    std::cout << pmin.x << ", " << pmax.x << std::endl;
    std::cout << pmin.y << ", " << pmax.y << std::endl;
    //camera.lookat(Point(-13.f, -5.f, -9.f), Point(13.f, 5.f, 9.f));
    //camera.lookat(Point(-201535.f, -201542.f, -201542.f), Point(201542.f, 201542.f, 201542.f));
    camera.lookat(pmin, pmax);
    std::string s = "/home/dyavil/Documents/TER/LYON_1ER_2012//LYON_1ER_BATI_Appearance/LYON_1ER_00001_Wall.jpg";
    texture= read_texture(0, s.c_str());

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
    //draw(mesh, camera);
    //draw(mesh2, Identity(), Identity(), Identity());

    int mx, my;
    unsigned int mb= SDL_GetRelativeMouseState(&mx, &my);
  
   // deplace la camera
    if(mb & SDL_BUTTON(1))              // le bouton gauche est enfonce
       // tourne autour de l'objet
        camera.rotation(mx, my);
    else if(mb & SDL_BUTTON(3))         // le bouton droit est enfonce
           // approche / eloigne l'objet
        camera.move(mx);
    else if(mb & SDL_BUTTON(2))         // le bouton du milieu est enfonce
            // deplace le point de rotation
        camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height());

    /*DrawParam param;
    param.camera(camera).texture(texture);
    param.draw(mesh);*/
    draw(mesh, camera);
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
    mesh.release();
    return 0;   // ras, pas d'erreur
}


int main( int argc, char **argv )
{
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

