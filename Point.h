
class Point{
    public:
        vect4 pos;
        vect4 col; // base color for point, (rgba)

        Point(vect4, vect4 cl = vect4(1,1,1,0));

        void set(vect4);

        // must be called within glBegin(GL_POINTS) and glEnd()
        void show(vect4, float, float, vect4 o = vect4(0,0,0,0));
};

Point::Point(vect4 in, vect4 cl){
    pos = in;
    col = cl;
}

void Point::set(vect4 in){
    pos = in;
}

// cam = camera position
// max3 = maximum 3d viewing distance
// max4 = maximum 4d viewing distance
// o = offset
void Point::show(vect4 cam, float max3, float max4, vect4 o){
    vect4 rpos = pos + o;
    float a = 1.f;
    float b = abs(rpos.w);
    float ht = 1.f; // half thickness
    vect4 temp = cam - rpos;
    vect4 ctem = col;
    if(b < max4){
        a = 1 - abs(rpos.w/max4);
        if(rpos.w > ht){
            ctem = vect4(a,1,a,1);
            //ctem.print("CTEM-GREEN: ","\n");
            //ctem.x = ctem.x*(1 - pos.w/max4);
            //ctem.y = ctem.y*(1 - pos.w/max4);
            //ctem.z = ctem.z + (1 - ctem.z)*(1 - pos.w/max4);
        }else if(rpos.w < -ht){
            ctem = vect4(1,a,a,1);
            //ctem.print("CTEM-RED: ","\n");
            //ctem.x = ctem.x + (1 - ctem.x)*(1 - pos.w/max4);
            //ctem.y = ctem.y*(1 - pos.w/max4);
            //ctem.z = ctem.z*(1 - pos.w/max4);
        }else{
            ctem = vect4(1,1,0,1);
        }
    }else if(b > max4 && b < 2*max4){
        if(rpos.w > 0){
            ctem = vect4(0,1,0,1);
        }else if(rpos.w < 0){
            ctem = vect4(1,0,0,1);
        }
    }else{
        ctem = vect4(0, 0, 0, 0);
    }
    glColor4f(ctem.x, ctem.y, ctem.z, (1 - temp.mag()/max3)*(1 - abs(rpos.w/max4)));
    //glColor4f(ctem.x, ctem.y, ctem.z, (1 - temp.mag()/max3));
    glVertex3f(rpos.x, rpos.y, rpos.z);

}
