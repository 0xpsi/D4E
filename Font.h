
class Font{
    private:
        Texture tex;
        SDL_Rect charset[100];
        float lwidth;  // individual letter width
        float lheight; // individual letter height
        //GLubyte r,g,b; // color of the text

    public:
        Font(GLubyte, GLubyte, GLubyte);
        ~Font();
        void render(const char*, double, double, vect, bool);
};

Font::Font(GLubyte a, GLubyte b, GLubyte c){
    lwidth = 16;
    lheight = 26;

    //fprintf(stderr, "Begin Font Debug:\n");
    tex.loadFileKeyReplace("resources/font_courier.png", 255, 255, 255, 0, 0, 0, a, b, c);
    int t = 0;
    for(int y = 0; y<10; y++){
        for(int x = 0; x<10; x++){
            t = 10*y + x;
            charset[t].x = x*lwidth;
            charset[t].y = y*lheight;
            charset[t].w = lwidth;
            charset[t].h = lheight;
            //fprintf(stderr, "charset[%d].x = %d\n", t, charset[t].x);
            //fprintf(stderr, "charset[%d].y = %d\n---\n", t, charset[t].y);
        }
    }
}

Font::~Font(){
    tex.free();
}

void Font::render(const char *text, double x, double y, vect scrn, bool center){
    const char *cp = text; int t = 0; int m = -1;
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, scrn.x, scrn.y, 0.0, 1.0, -1.0);


    while(*cp != '\0'){
        m++;
        cp++;
    }
    cp = text;
    while(*cp != '\0'){
        if(center){
            tex.render(x + lwidth*t - (m*lwidth)/2.0, y, 0, 1, &charset[(*cp)-32]);
        }else{
            tex.render(x + lwidth*t + lwidth/2.0, y + lheight/2.0, 0, 1, &charset[(*cp)-32]);
        }
        cp++;
        t++;
    }

    glPopMatrix();
}
