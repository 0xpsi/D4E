
bool circle(vect4 in){
    float bound = 0.95;
    return (
        sqrt(in.z*in.z + in.w*in.w) <= 1 &&
        sqrt(in.z*in.z + in.w*in.w) >= bound &&
        abs(in.x) <= (1 - bound) && abs(in.y) <= (1 - bound)
    );
}

bool cube(vect4 in){
    float bound = 0.95;
    return (
        in.x > -bound && in.x < bound &&
        in.y > -bound && in.y < bound &&
        in.z > -bound && in.z < bound &&
        abs(in.w) <= 0.04
    );
}

bool cubeshell(vect4 in){
    return (
        !cube(in) &&
        abs(in.w) <= 0.04
    );
}

bool hcube(vect4 in){
    float bound = 0.97;
    return (
        in.x > -bound && in.x < bound &&
        in.y > -bound && in.y < bound &&
        in.z > -bound && in.z < bound &&
        in.w > -bound && in.w < bound
    );
}

bool hcubeshell(vect4 in){
    float bound = 0.95;
    return (
        !hcube(in)
    );
}

bool sphere(vect4 in){
    return (
       in.mag() <= 1 &&
       abs(in.w) <= 0.04
    );
}

bool wormhole(vect4 in){
    return (
       in.x*in.x + in.y*in.y + in.z*in.z - in.w*in.w >= 0.01 &&
       in.x*in.x + in.y*in.y + in.z*in.z - in.w*in.w <= 0.011
    );
}

bool hsphere(vect4 in){
    return (
       in.mag() <= 1
    );
}

bool sphereshell(vect4 in){
    float bound = 0.95;
    return (
        (in.mag() <= 1) &&
        (in.mag() >= bound) &&
        abs(in.w) <= (1-bound)
    );
}

bool hsphereshell(vect4 in){
    float bound = 0.95;
    return (
        (in.mag() <= 1) &&
        (in.mag() >= bound)
    );
}

bool tricirc(vect4 in){
    float bound = 0.99;
    float dx = (1 - bound)/2.f;
    return (
        ( ((in.y+1) >= sqrt(3)*(in.x+1) + 0.268 - (1.73)*dx &&
        (in.y+1) <= sqrt(3)*(in.x+1) + 0.268 + (1.73)*dx) ||
        ((in.y+1) >= 3.4641 - sqrt(3)*(in.x+1) + 0.268 - (1.73)*dx &&
        (in.y+1) <= 3.4641 - sqrt(3)*(in.x+1) + 0.268 + (1.73)*dx) ||
        ((in.y+1) >= 0.268 - dx &&
        (in.y+1) <= 0.268 + dx) ) &&
        sqrt(in.z*in.z + in.w*in.w) >= bound &&
        sqrt(in.z*in.z + in.w*in.w) <= 1
    );
}

bool trisquare(vect4 in){
    float bound = 0.99;
    float dx = (1 - bound)/2.f;
    return (
        ( ((in.y+1) >= sqrt(3)*(in.x+1) + 0.268 - (1.73)*dx &&
        (in.y+1) <= sqrt(3)*(in.x+1) + 0.268 + (1.73)*dx) ||
        ((in.y+1) >= 3.4641 - sqrt(3)*(in.x+1) + 0.268 - (1.73)*dx &&
        (in.y+1) <= 3.4641 - sqrt(3)*(in.x+1) + 0.268 + (1.73)*dx) ||
        ((in.y+1) >= 0.268 - dx &&
        (in.y+1) <= 0.268 + dx) ) &&
        !(in.z >= -bound && in.z <= bound && in.w <= bound && in.w >= -bound)
    );
}

bool tritri(vect4 in){
    float bound = 0.99;
    float dx = (1 - bound)/2.f;
    return (
        ( ((in.y+1) >= sqrt(3)*(in.x+1) + 0.268 - (1.73)*dx &&
        (in.y+1) <= sqrt(3)*(in.x+1) + 0.268 + (1.73)*dx) ||
        ((in.y+1) >= 3.4641 - sqrt(3)*(in.x+1) + 0.268 - (1.73)*dx &&
        (in.y+1) <= 3.4641 - sqrt(3)*(in.x+1) + 0.268 + (1.73)*dx) ||
        ((in.y+1) >= 0.268 - dx &&
        (in.y+1) <= 0.268 + dx) ) &&
        ( ((in.z+1) >= sqrt(3)*(in.w+1) + 0.268 - (1.73)*dx &&
        (in.z+1) <= sqrt(3)*(in.w+1) + 0.268 + (1.73)*dx) ||
        ((in.z+1) >= 3.4641 - sqrt(3)*(in.w+1) + 0.268 - (1.73)*dx &&
        (in.z+1) <= 3.4641 - sqrt(3)*(in.w+1) + 0.268 + (1.73)*dx) ||
        ((in.z+1) >= 0.268 - dx &&
        (in.z+1) <= 0.268 + dx) )
    );
}

bool cliftor(vect4 in){
    float bound = 0.99;
    return (
        sqrt(in.x*in.x + in.y*in.y) >= bound &&
        sqrt(in.x*in.x + in.y*in.y) <= 1 &&
        sqrt(in.z*in.z + in.w*in.w) >= bound &&
        sqrt(in.z*in.z + in.w*in.w) <= 1
    );
}

bool circrect(vect4 in){
    float bound = 0.95;
    return (
        !(in.x >= -bound && in.x <= bound && in.y <= bound && in.y >= -bound) &&
        sqrt(in.z*in.z + in.w*in.w) >= bound &&
        sqrt(in.z*in.z + in.w*in.w) <= 1
    );
}

bool rectrect(vect4 in){
    float bound = 0.991;
    return (
        !(in.x >= -bound && in.x <= bound && in.y <= bound && in.y >= -bound) &&
        !(in.z >= -bound && in.z <= bound && in.w <= bound && in.w >= -bound)
    );
}
