// D4E: 4th Dimension Visualization Engine
// by Jonny Bursa


#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <random>
//#include <cstdlib>

#include <vect.h>

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
//#include <SDL_mixer.h>
#include <SDL_ttf.h>
//#include <SDL_thread.h>

#include <glut.h>
#include <il.h>
#include <ilu.h>

#include <ft2build.h>
#include FT_FREETYPE_H


#include <Timer.h>
#include <SDL_customi.h>
#include <TTF_custom.h>
#include <SDL_GL_custom.h>
//#include <debug_custom.h>

using namespace std;

#include <vect4.h>
#include "Camera4.h"
#include "Shapes.h"

#include "Point.h"
#include "Object.h"

#include "Texture.h"
#include "Font.h"

//#include "Camera.h"

const int SCRN_W = (int)(960*1.3); //1920
const int SCRN_H = 720; //1080
const int FULL_W = 1920;
const int FULL_H = 1080;
const int SCRN_BPP = 32;

// SETTINGS BEGIN  -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
// main system settings configured before launch

const int FPS = 60;
bool fullscrn = true;
float MDIST3 = 2000; // max view distance in 3D
float MDIST4 = 40; // max view distance in 4D

// SETTINGS END    -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

// ----

// PSEUDO SETTINGS BEGIN  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
// SETTING variables configurable by input

bool stereo = false; // stereo mode
float dist = 600; // distance from viewing focus point, (acts as zoom)

float STED = 80; // stereo eye distance
float STID = 75; // stereo image distance

// primary plane of rotation
vect4 u(1,0,0,0);
vect4 v(0,1,0,0);
vect4 t(0,0,1,0); // points perpindicular to plane
float angle = 0;
int rotate = 0; // 0: no rotation. 1: positive rotation. -1: negative rotation
int rotate2 = 0;

float rotspeed = 0.01;

bool ortho = true; // toggle orthogonal plane rotation
bool shp = false; // toggle showing the rotation plane
bool shp2 = false; // toggle anti-axis (secondary rotation plane)
bool tog = false;

// PSEUDO SETTINGS END    = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

// ----

// SYSTEM VARIABLES     - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// other variables transparent to user

bool quit = false;
bool press = false;
vect scrn; // takes on current screen dimensions
int i = 0;

Camera4 cam(vect4(100,100,100,0), vect4(0,0,0,0));
Object pln(6000);
Object pln2(6000); // anti-axis!

// Timers
Timer tf;
Timer t_ESC;
Timer t_o;
Timer t_p;  // primary axis toggle timer
Timer t_p2; // anti-axis toggle timer
Timer t_ar; // arrow key timer (for all arrows)
Timer t_bs; // stereo toggle
Timer t_tog;// toggle object

// SYSTEM VARIABLES END - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


void input_manager(Uint8 *keys, int x, int y){

    if(keys[SDLK_ESCAPE]){
        if(t_ESC.getTime()>200){ // Escape key cooldown
            quit = true;
        }
        t_ESC.start();
    }

    if(keys[SDLK_1]){
        if(rotspeed >= 0.001){
            rotspeed -= 0.001;
        }
    }else if(keys[SDLK_2]){
        if(rotspeed < 0.1){
            rotspeed += 0.001;
        }
    }

    if(keys[SDLK_PERIOD]){
        STED += 2;
    }else if(keys[SDLK_COMMA]){
        STED -= 2;
    }

    if(keys[SDLK_9]){
        if(MDIST4 > 5){
            MDIST4 -= 1;
        }
    }else if(keys[SDLK_0]){
        if(MDIST4 < 1000){
            MDIST4 += 1;
        }
    }

    if(keys[SDLK_7]){
        if(MDIST3 > 500){
            MDIST3 -= 5;
        }
    }else if(keys[SDLK_8]){
        if(MDIST3 < 4000){
            MDIST3 += 5;
        }
    }
    // try putting "start" function inside if statement
    if(keys[SDLK_w]){
        if(ortho){
            if(t_ar.getTime() > 100){
                u.rotate(v, vect4(0,0,0,1), PI/4);
                t.rotate(v, vect4(0,0,0,1), PI/4);
                pln.rotate(v, vect4(0,0,0,1), PI/4);
                pln2.rotate(v, vect4(0,0,0,1), PI/4);
                t_ar.start();
            }
        }else{
            u.rotate(v, vect4(0,0,0,1), 0.005);
            t.rotate(v, vect4(0,0,0,1), 0.005);
            pln.rotate(v, vect4(0,0,0,1), 0.005);
            pln2.rotate(v, vect4(0,0,0,1), 0.005);
        }
    }else if(keys[SDLK_s]){
        if(ortho){
            if(t_ar.getTime() > 100){
                u.rotate(v, vect4(0,0,0,1), -PI/4);
                t.rotate(v, vect4(0,0,0,1), -PI/4);
                pln.rotate(v, vect4(0,0,0,1), -PI/4);
                pln2.rotate(v, vect4(0,0,0,1), -PI/4);
                t_ar.start();
            }
        }else{
            u.rotate(v, vect4(0,0,0,1), -0.005);
            t.rotate(v, vect4(0,0,0,1), -0.005);
            pln.rotate(v, vect4(0,0,0,1), -0.005);
            pln2.rotate(v, vect4(0,0,0,1), -0.005);
        }
    }

    if(keys[SDLK_a]){
        if(ortho){
            if(t_ar.getTime() > 100){
                u.rotate(vect4(0,0,1,0), vect4(0,0,0,1), PI/4);
                v.rotate(vect4(0,0,1,0), vect4(0,0,0,1), PI/4);
                t.rotate(vect4(0,0,1,0), vect4(0,0,0,1), PI/4);
                pln.rotate(vect4(0,0,1,0), vect4(0,0,0,1), PI/4);
                pln2.rotate(vect4(0,0,1,0), vect4(0,0,0,1), PI/4);
                t_ar.start();
            }
        }else{
            u.rotate(vect4(0,0,1,0), vect4(0,0,0,1), 0.005);
            v.rotate(vect4(0,0,1,0), vect4(0,0,0,1), 0.005);
            t.rotate(vect4(0,0,1,0), vect4(0,0,0,1), 0.005);
            pln.rotate(vect4(0,0,1,0), vect4(0,0,0,1), 0.005);
            pln2.rotate(vect4(0,0,1,0), vect4(0,0,0,1), 0.005);
        }
    }else if(keys[SDLK_d]){
        if(ortho){
            if(t_ar.getTime() > 200){
                u.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -PI/4);
                v.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -PI/4);
                t.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -PI/4);
                pln.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -PI/4);
                pln2.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -PI/4);
                t_ar.start();
            }
        }else{
            u.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -0.005);
            v.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -0.005);
            t.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -0.005);
            pln.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -0.005);
            pln2.rotate(vect4(0,0,1,0), vect4(0,0,0,1), -0.005);
        }
    }

    if(keys[SDLK_q]){
        if(ortho){
            if(t_ar.getTime() > 100){
                v.rotate(u, t, PI/4);
                pln.rotate(u, t, PI/4);
                pln2.rotate(u, t, PI/4);
                t_ar.start();
            }
        }else{
            v.rotate(u, t, 0.005);
            pln.rotate(u, t, 0.005);
            pln2.rotate(u, t, 0.005);
        }
    }else if(keys[SDLK_e]){
        if(ortho){
            if(t_ar.getTime() > 200){
                v.rotate(u, t, -PI/4);
                pln.rotate(u, t, -PI/4);
                pln2.rotate(u, t, -PI/4);
                t_ar.start();
            }
        }else{
            v.rotate(u, t, -0.005);
            pln.rotate(u, t, -0.005);
            pln2.rotate(u, t, -0.005);
        }
    }

    if(keys[SDLK_z]){
        rotate = 1;
    }else if(keys[SDLK_x]){
        rotate = -1;
    }else{
        rotate = 0;
    }

    if(keys[SDLK_c]){
        rotate2 = 1;
    }else if(keys[SDLK_v]){
        rotate2 = -1;
    }else{
        rotate2 = 0;
    }

    if(keys[SDLK_o]){
        if(t_o.getTime()>200){
            ortho = !ortho;
            t_o.start();
        }
    }

    if(keys[SDLK_p]){
        if(t_p.getTime()>100){
            shp = !shp;
            t_p.start();
        }
    }

    if(keys[SDLK_LEFTBRACKET]){
        if(t_p2.getTime()>100){
            shp2 = !shp2;
            t_p2.start();
        }
    }

    if(keys[SDLK_BACKSLASH]){
        if(t_bs.getTime()>100){
            if(stereo){
                stereo = false;
                dist = 600;
                t_bs.start();
            }else{
                stereo = true;
                dist = 800;
                t_bs.start();
            }
        }
    }

    if(keys[SDLK_TAB]){
        if(t_tog.getTime()>100){
            if(tog){
                tog = false;
                t_tog.start();
            }else{
                tog = true;
                t_tog.start();
            }
        }
    }

    /*
    if(keys[SDLK_r]){
        if(t_r.getTime()>500){
            u = vect4(1,0,0,0);
            v = vect4(0,1,0,0);
        }
    }
    */
    // more keys
}

int main(int argc, char *argv[]){

    int x, y; // placeholder for mouse position

    SDL_Event e;

    int ms = 0;
    int t42 = 1;
    int t0 = 1;
    double p = 1.0;

    // startup functions -------------------------------------------
    if(fullscrn){
        SDL_GL_BEGIN("D4E: 4th Dimension Visualization Engine", FULL_W, FULL_H, SCRN_BPP, SDL_OPENGL|SDL_FULLSCREEN);
        scrn.x = FULL_W;
        scrn.y = FULL_H;
    }else{
        SDL_GL_BEGIN("D4E: 4th Dimension Visualization Engine", SCRN_W, SCRN_H, SCRN_BPP);
        scrn.x = SCRN_W;
        scrn.y = SCRN_H;
    }

    SDL_Surface *icoi = SDL_LoadBMP("resources/cliftor.bmp");
    SDL_WM_SetIcon(icoi, NULL);

    IL_BEGIN();

    // variables ---------------------------------------------------

    SDL_Color white = {255, 255, 255};
    SDL_Color black = {0, 0, 0};
    SDL_Color col = {10, 255, 10};

    t_ESC.start();
    t_o.start();
    t_p.start();
    t_p2.start();
    t_ar.start();
    t_bs.start();
    t_tog.start();

    // Loads BEGIN 0=0=0=0=0=0=0=0=0=0=0=0=0=0=0

    pln.scale(60);
    pln2.scale(60);
    pln2.rotate(vect4(1,0,0,0), vect4(0,0,1,0), PI/2);
    pln2.rotate(vect4(0,1,0,0), vect4(0,0,0,1), PI/2);

    //Object obj1(wormhole, 50000, vect4(0,0,0,0), vect4(1,1,1,1));
    //obj1.scale(200);

    Object obj1(cliftor, 50000, vect4(0,0,0,0), vect4(1,1,1,1));
    obj1.scale(50);

    Object obj2(tricirc, 50000, vect4(0,0,0,0), vect4(1,1,1,1));
    obj2.scale(50);

    //obj1.rotate(vect4(1,0,0,0), vect4(0,1,0,0), PI/4);

    //Point p1(vect4(0,0,0,0));

    // Loads END   0=0=0=0=0=0=0=0=0=0=0=0=0=0=0

    // GAME LOOP BEGIN! -------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    while(!quit){
        //stringstream ss;

        tf.start();

        p = (double)t0 / 1000.0;   // how many seconds (conversion from miliseconds to seconds) last frame took
        p = 1 / p;             // inverse for FPS
        t42 = (int)p;            // turn to int

        // SDL EVENT MANAGER
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }else if(e.button.button == SDL_BUTTON_WHEELUP){
                dist -= 10;
            }else if(e.button.button == SDL_BUTTON_WHEELDOWN){
                dist += 10;
            }
            if(e.type == SDL_MOUSEBUTTONDOWN){
                if(e.button.button == SDL_BUTTON_LEFT){
                    press = true;
                }
            }else if(e.type == SDL_MOUSEBUTTONUP){
                if(e.button.button == SDL_BUTTON_LEFT){
                    press = false;
                }
            }
        }

        SDL_GetMouseState( &x, &y );
        input_manager(SDL_GetKeyState(NULL), x, y);

        if(rotate==1){
            obj1.rotate(u, v, rotspeed);
        }else if(rotate==-1){
            obj1.rotate(u, v, -rotspeed);
        }

        if(rotate2==1){
            obj1.rotate(t, scale4(cross4(u,v,t), -1), rotspeed);
        }else if(rotate2==-1){
            obj1.rotate(t, scale4(cross4(u,v,t), -1), -rotspeed);
        }

        //ss << "FPS: " << t42;

        // SCREEN UPDATE BEGIN

        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1,1,1);

        //courSoft.render(ss.str().c_str(), 0, 0, scrn, false);
        i = 0;
    do{
        if(stereo){
            cam.set(scrn, x, y, press, dist, MDIST3, STED, STID);
        }else{
            cam.set(scrn, x, y, press, dist, MDIST3);
        }

        // Draw the coordinate system
        glBegin(GL_LINES);
            glColor4f(0,1,0,0.15);
            glVertex3f(0,0,0);
            glVertex3f(100,0,0);
            glVertex3f(0,0,0);
            glVertex3f(0,100,0);
            glVertex3f(0,0,0);
            glVertex3f(0,0,100);
        glEnd();

        glBegin(GL_POINTS);

            if(tog){
                obj1.show(cam.getPos(), MDIST3, MDIST4);
            }else{
                obj2.show(cam.getPos(), MDIST3, MDIST4);
            }
            if(shp){
                pln.show(cam.getPos(), MDIST3, MDIST4);
                if(shp2){
                    pln2.show(cam.getPos(), MDIST3, MDIST4);
                }
            }

        /*
            //face in xy plane
            glColor4f(0.82, 0.41, 0.12, a);//this the color with which complete cube is drawn.
            glVertex3f(0, 0 ,0 );
            glVertex3f(50, 0, 0);
            glVertex3f(50, 50, 0);
            glVertex3f(0, 50, 0);

            //face in yz plane
            glColor4f(1, 0, 0, a);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 50);
            glVertex3f(0, 50, 0);
            glVertex3f(0, 50, 50);

            //face in zx plance
            glColor4f(0, 1, 0, a);
            glVertex3f(0, 0, 0);
            glVertex3f(0, 0, 50);
            glVertex3f(50, 0, 50);
            glVertex3f(50, 0, 0);

            //|| to xy plane.
            glColor4f(0, 0, 1, a);
            glVertex3f(0, 0, 50);
            glVertex3f(50, 0, 50);
            glVertex3f(50, 50, 50);
            glVertex3f(0, 50, 50);

            //|| to yz plane
            glColor4f(0.73, 0.58, 0.58, a);
            glVertex3f(50, 0 ,50 );
            glVertex3f(50, 50, 50);
            glVertex3f(50, 50, 0);
            glVertex3f(50, 0, 0);

            //|| to zx plane
            glColor4f(0.58, 0, 0.82, a);
            glVertex3f(0, 50, 0 );
            glVertex3f(0, 50, 50);
            glVertex3f(50, 50, 50);
            glVertex3f(50, 50, 0);

            */
        glEnd();

        rotate = false;


        i++;
    }while(i<2 && stereo);
        update();

        // SCREEN UPDATE END -------------------------------------------------------------|||||
        ms += t0;
        if((t0 = tf.getTime()) < 1000.0/FPS){
            SDL_Delay((1000.0/FPS) - t0);
        }
        t0 = tf.getTime();
    }

    // GAME LOOP END! ---------------->>>>>>>>>>

    SDL_Quit();
    return 0;
}
