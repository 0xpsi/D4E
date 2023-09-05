
class Object{
    private:
        vect4 pos;
        vect4 col;

        vector<Point*> pts;
        int num; // number of poinst in object

    public:
        Object(int, vect4 pos1 = vect4(0,0,0,0));
        Object(bool (*sh)(vect4), int, vect4, vect4 cl = vect4(1,1,1,1));

        void setPos(vect4);
        vect4 getPos();
        void movePos(vect4);

        void scale(float);

        void rotate(vect4, vect4, float, vect4 ro = vect4(0,0,0,0));

        void show(vect4, float, float);
};

Object::Object(int dens, vect4 pos1){
    pos = pos1;
    pts.resize(dens);
    num = dens;

    int i = 0;

    vect4 temp(0,0,0,0);
    while(i<num){
        temp.x = -2 + (float)rand()/((float)RAND_MAX/(4));
        temp.y = -2 + (float)rand()/((float)RAND_MAX/(4));
        pts[i] = new Point(temp);
        i++;
    }
}

Object::Object(bool (*sh)(vect4), int dens, vect4 pos1, vect4 cl){
    pos = pos1;
    pts.resize(dens);
    num = dens;

    int i = 0; // number added

    //srand(42);
    default_random_engine gen;
    uniform_real_distribution<float> sprd(-1,1);
    //printf("Object called! i<num? %s\n", (i<num)?"true":"false");
    vect4 temp(0,0,0,0);
    while(i<num){
        /*
        temp.x = -1 + (float)rand()/((float)RAND_MAX/(2));
        temp.y = -1 + (float)rand()/((float)RAND_MAX/(2));
        temp.z = -1 + (float)rand()/((float)RAND_MAX/(2));
        temp.w = -1 + (float)rand()/((float)RAND_MAX/(2));
        */
        temp.x = sprd(gen);
        temp.y = sprd(gen);
        temp.z = sprd(gen);
        temp.w = sprd(gen);
        //printf("temp.x produced: %f\n", temp.x);
        if(sh(temp)){
            //printf("vect4 accepted!\n");
            pts[i] = new Point(temp, cl);
            i++;
        }
    }
}

void Object::setPos(vect4 in){
    pos = in;
}

vect4 Object::getPos(){
    return pos;
}

void Object::movePos(vect4 in){
    pos += in;
}

void Object::scale(float s){
    for(int i=0; i<num; i++){
        (pts[i]->pos).x *= s;
        (pts[i]->pos).y *= s;
        (pts[i]->pos).z *= s;
        (pts[i]->pos).w *= s;
    }
}

// Rotate Object
// vect4 u & v: plane orientation parameters
// angle = duh
// vect4 ro = plane rotation offset
void Object::rotate(vect4 u, vect4 v, float angle, vect4 ro){
    for(int i = 0; i<num; i++){
        (pts[i]->pos).rotate(u, v, angle);
    }
    pos.rotate(u,v,angle);
}

// cam = camera position
// max3 = maximum 3d viewing distance
// max4 = maximum 4d viewing distance
void Object::show(vect4 cam, float max3, float max4){
    for(int i = 0; i<num; i++){
        pts[i]->show(cam, max3, max4, pos);
    }
}
