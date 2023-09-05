
class Camera4{
    private:
        bool sm; // Stereo Mode

        vect curs;
        vect4 pos;
        vect4 look;
    public:
        Camera4(vect4, vect4);

        vect4 getPos();

        void set(vect, int, int, bool, float, float);
        void set(vect, int, int, bool, float, float, float, float);
};

Camera4::Camera4(vect4 p, vect4 l){
    sm = false;
    curs.x = 0;
    curs.y = 0;
    pos = p;
    look = l;
}

vect4 Camera4::getPos(){
    return pos;
}

// Set Camera Parameters: NON-STEREO VERSION
// scrn vector used for aspect ratio
// x & y position for mouse for on the fly dx and dy calculations
// press: mouse pressed down or not?
// dist: distance from object
// MDIST3 used for camera FAR parameter
void Camera4::set(vect scrn, int x, int y, bool press, float dist, float MDIST3){

    if(press){
        float dx = x - curs.x;
        float dy = y - curs.y;

        float c = (float)cos(dx*0.005);
        float s = (float)sin(dx*0.005);

        pos.x = pos.x*c + pos.y*s;
        pos.y = pos.y*c - pos.x*s;
        pos.z += dy*0.5*(dist/500);
    }
    curs.x = x;
    curs.y = y;

    pos = scale4(pos, dist, true);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, scrn.x/scrn.y, 0.1, MDIST3);
    gluLookAt(pos.x, pos.y, pos.z, 0,0,0, 0,0,1);
}

// Set Camera Parameters: STEREO VERSION
// same parameters as non-stereo version + ...
// SED: Stereo eye seperation
// SID: Stereo image seperation
void Camera4::set(vect scrn, int x, int y, bool press, float dist, float MDIST3, float STED, float STID){
    vect temp;
    vect4 vsted(0,0,0,0); // offset for eyes
    vect4 vstid(0,0,0,0); // net offset for image projection
    vect posbase;

    if(press){
        float dx = x - curs.x;
        float dy = y - curs.y;

        float c = (float)cos(dx*0.005);
        float s = (float)sin(dx*0.005);

        pos.x = pos.x*c + pos.y*s;
        pos.y = pos.y*c - pos.x*s;
        pos.z += dy*(dist/500);
    }
    pos = scale4(pos, dist, true);
    curs.x = x;
    curs.y = y;
    posbase.x = pos.x;
    posbase.y = pos.y;

    if(!sm){ // new camera call
        temp = rotoscale(posbase, -PI/2, 1);
        sm = true;
    }else{ // second camera call
        temp = rotoscale(posbase, PI/2, 1);
        sm = false;
    }
    temp = scale(temp, STED, true);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, scrn.x/scrn.y, 0.1, MDIST3);
    gluLookAt(pos.x + (temp.x), pos.y + (temp.y), pos.z, temp.x, temp.y, 0, 0,0,1);

}
