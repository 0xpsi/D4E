
class Texture{
    private:
        GLuint id;

        GLuint texWidth;
        GLuint texHeight;

        GLuint imgWidth;
        GLuint imgHeight;

        GLuint *pixels;

    public:
        Texture();
        ~Texture();

        bool loadFile(char*);

        bool loadFileWithKey(char*, GLubyte, GLubyte, GLubyte, GLubyte a = 0);

        bool loadFileKeyReplace(char*, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte, GLubyte);

        void loadSurface(SDL_Surface*);

        bool loadTextureFromPixels32(GLuint*, GLuint, GLuint, GLuint, GLuint);
        bool loadTextureFromPixels32();
        bool loadPixelsFromFile(char*);

        void free();

        void render(GLfloat, GLfloat, float, float, SDL_Rect*);

        GLuint powerOfTwo(GLuint);

        GLuint getID();
        GLuint getImgWidth();
        GLuint getImgHeight();
        GLuint getTexWidth();
        GLuint getTexHeight();
};

Texture::Texture(){
    id = 0;
    pixels = NULL;
    texWidth = 0;
    texHeight = 0;
    imgWidth = 0;
    imgHeight = 0;
}

Texture::~Texture(){
    free();
}

// loads a texture from file
bool Texture::loadFile(char* path){
    // Texture loading success
    bool loaded = false;

    // Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);

    // Load image
    ILboolean success = ilLoadImage(path);

    // Image loaded successfully
    if(success == IL_TRUE){
        // Convert image to RGBA
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if(success == IL_TRUE){

            // initialize dimensions
            GLuint iw = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
            GLuint ih = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

            // calculate required texture dimensions
            //GLuint tw = powerOfTwo(iw);
            //GLuint th = powerOfTwo(ih);
            GLuint tw = iw;
            GLuint th = ih;
            //printf("WIDTH IS: %d\tNEW: %d\n", (int)ih, (int)tw);

            // Rescales image to powers of 2 dimensions - DOES NOT WORK
            /*
            if(iw != tw || ih != th){
                // place image at upper left
                iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

                // resize image
                iluEnlargeCanvas((int)tw, (int)th, 1);
            }
            */
            // Create texture from file pixels
            loaded = loadTextureFromPixels32((GLuint*)ilGetData(), iw, ih, iw, ih);
        }

        // Delete file from memory
        ilDeleteImages(1, &id);
    }
    // Report error
    if(!loaded){
        printf("Unable to load %s\n", path);
    }
    return loaded;
}

bool Texture::loadFileWithKey(char* path, GLubyte r, GLubyte g, GLubyte b, GLubyte a){
////debug_c("LFWK: begin");
    if(!loadPixelsFromFile(path)){
        return false;
    }
////debug_c("LFWK: pixels loaded");
    // go through pixels
    GLuint size = texWidth*texHeight;
////debug_c("LFWK: size calculated");
if(pixels == NULL){
    ////debug_c("LFWK: PIXELS ARE NULL!!!!");
}
    for(int i = 0; i<size; i++){
        // get pixel colors
        GLubyte *colors = (GLubyte*)&pixels[i];

        if(colors[0] == r && colors[1] == g && colors[2] == b){

            // make transparent
            colors[3] = a;
        }
    }
////debug_c("LFWK: finishing - about to \"load TextureFromPixels32\"");

    return loadTextureFromPixels32();
}

bool Texture::loadFileKeyReplace(char* path, GLubyte r, GLubyte g, GLubyte b, GLubyte r1, GLubyte g1, GLubyte b1, GLubyte r2, GLubyte g2, GLubyte b2){
    if(!loadPixelsFromFile(path)){
        return false;
    }

    // go through pixels
    GLuint size = texWidth*texHeight;

    for(int i = 0; i<size; i++){
        // get pixel colors
        GLubyte *colors = (GLubyte*)&pixels[i];

        if(colors[0] == r && colors[1] == g && colors[2] == b){

            // make transparent
            colors[3] = 0;
        }
        if(colors[0] == r1 && colors[1] == g1 && colors[2] == b1){
            colors[0] = r2;
            colors[1] = g2;
            colors[2] = b2;
        }
    }

    return loadTextureFromPixels32();
}

// loads a texture from a SDL surface
void Texture::loadSurface(SDL_Surface* surface){
    GLenum texture_format;

    // may be needed for this specific surface to texture conversion
    //glEnable(GL_TEXTURE_2D);

    // Set the color mode ---------------------------------------------------------------
    if((surface->w & (surface->w - 1)) != 0 ){
		printf("warning: image.bmp's width is not a power of 2\n");
	}
	if((surface->h & (surface->h - 1)) != 0 ){
		printf("warning: image.bmp's height is not a power of 2\n");
	}
    // get the number of channels in the SDL surface
    GLint nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4){    // contains alpha channel
        if(surface->format->Rmask == 0x000000ff){
            texture_format = GL_RGBA;
        }else{
            texture_format = GL_BGRA;
        }
    }else if (nOfColors == 3){
        if(surface->format->Rmask == 0x000000ff){
            texture_format = GL_RGB;
        }else{
            texture_format = GL_BGR;
        }
    }else{
        printf("warning: the image is not truecolor..  this will probably break\n");
        // this error should not go unhandled
    }

    /* Genarate texture */
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);

}

// support function for loadFile()
bool Texture::loadTextureFromPixels32(GLuint* pixels, GLuint iw, GLuint ih, GLuint tw, GLuint th){
    //Free texture if it exists
    free();

    //Get texture dimensions
    imgWidth = iw;
    imgHeight = ih;
    texWidth = tw;
    texHeight = th;

    //Generate texture ID
    glGenTextures(1, &id);

    //Bind texture ID
    glBindTexture(GL_TEXTURE_2D, id);

    //Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    //Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //Unbind texture
    glBindTexture(GL_TEXTURE_2D, NULL);

    //Check for error
    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error loading texture from %p pixels! %s\n", pixels, gluErrorString( error ) );
        return false;
    }
    return true;
}

// "overloaded" support function for key blitting
bool Texture::loadTextureFromPixels32(){
    bool success = true;

    if(id == 0 && pixels != NULL){
        // generate texture id
        glGenTextures(1, &id);

        // bind texture id
        glBindTexture(GL_TEXTURE_2D, id);

        // generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        // set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Unbind TExture
        glBindTexture(GL_TEXTURE_2D, NULL);

        GLenum error = glGetError();
        if(error != GL_NO_ERROR){
            fprintf(stderr, "Error loading texture from &p pixels! %s\n", pixels, gluErrorString(error));
            success = false;
        }else{
            // release the (kraken) ... i mean pixels
            delete[] pixels;
            pixels = NULL;
        }
    }else{
        fprintf(stderr, "Cannot load texture from current pixels! ");
        if(id != 0){
            fprintf(stderr, "A texture is already loaded!\n");
        }else if(pixels == NULL){
            fprintf(stderr, "No pixels to create texture from!\n);");
        }
        success = false;
    }
    return success;
}

// gets just the pixels from file
bool Texture::loadPixelsFromFile(char *path){
    //Deallocate texture data
//debug_c("LPFF 1");
    free();
//debug_c("LPFF 2");
    //Texture loading success
    bool pixelsLoaded = false;

    //Generate and set current image ID
    ILuint imgID = 0;
    ilGenImages(1, &imgID);
    ilBindImage(imgID);
//debug_c("LPFF 3");
    //Load image
    ILboolean success = ilLoadImage(path);
//debug_c("LPFF 4");
    //Image loaded successfully
    if(success == IL_TRUE){
    //debug_c("LPFF 5");
        //Convert image to RGBA
        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        //debug_c("LPFF 6");
        if(success == IL_TRUE){
        //debug_c("LPFF 7");
            // Initialize dimensions
            GLuint iw = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
            GLuint ih = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);
        //debug_c("LPFF 8");

            // Calculate required texture dimensions
            //GLuint tw = powerOfTwo(iw);
            //GLuint th = powerOfTwo(ih);
            GLuint tw = iw;
            GLuint th = ih;

            // Texture is the wrong size
            /*
            if(iw != tw || ih != th){
                // place image at upper left
                iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

                // resize image
                iluEnlargeCanvas((int)tw, (int)th, 1);
            }
            */
            // Allocate memory for texture data
            GLuint size = tw*th;
            pixels = new GLuint[size];
        //debug_c("LPFF 9");
            // Get image dimensions
            imgWidth = iw;
            imgHeight = ih;
            texWidth = tw;
            texHeight = th;

            // Copy pixels
            memcpy(pixels, ilGetData(), size*4);
            pixelsLoaded = true;
        //debug_c("LPFF 10");
        }

        //Delete file from memory
        ilDeleteImages(1, &imgID);
    //debug_c("LPFF 11");
    }

    //Report error
    if(!pixelsLoaded){
        printf("Unable to load %s\n", path);
    }

    return pixelsLoaded;
}

void Texture::render(GLfloat x, GLfloat y, float theta = 0.f, float s = 1.f, SDL_Rect *t_blit = NULL){
//void Texture::render(GLfloat x, GLfloat y){
    SDL_Rect blit;
    vect a;
    if(t_blit == NULL){
        //fprintf(stderr,"BLIT IS NULL!\n");
        blit.x = 0;
        blit.y = 0;
        blit.w = imgWidth;
        blit.h = imgHeight;
    }else{
        //fprintf(stderr,"BLIT IS NOT NULL!\n");
        blit.x = t_blit->x;
        blit.y = t_blit->y;
        blit.w = t_blit->w;
        blit.h = t_blit->h;
    }

    //If the texture exists
    if(id != 0){
        // clear color buffer
        // glClear(GL_COLOR_BUFFER_BIT);

        // translate model matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(x, y, 0.0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Bind the texture to which subsequent calls refer to
        glBindTexture(GL_TEXTURE_2D, id);

        glBegin(GL_QUADS);
            // top-left vertex
            a.x = -blit.w/2.0; a.y = -blit.h/2.0;
            a = rotoscale(a, theta, s);
            glTexCoord2f(((float)blit.x)/texWidth, ((float)blit.y)/texHeight);
            glVertex2f(a.x, a.y);

            // top-right vertex
            a.x = blit.w/2.0; a.y = -blit.h/2.0;
            a = rotoscale(a, theta, s);
            glTexCoord2f(((float)blit.x + blit.w)/texWidth, ((float)blit.y)/texHeight);
            glVertex2f(a.x, a.y);

            // bottom-right vertex
            a.x = blit.w/2.0; a.y = blit.h/2.0;
            a = rotoscale(a, theta, s);
            glTexCoord2f(((float)blit.x + blit.w)/texWidth, ((float)blit.y + blit.h)/texHeight);
            glVertex2f(a.x, a.y);

            // bottom-left vertex
            a.x = -blit.w/2.0; a.y = blit.h/2.0;
            a = rotoscale(a, theta, s);
            glTexCoord2f(((float)blit.x)/texWidth, ((float)blit.y + blit.h)/texHeight);
            glVertex2f(a.x, a.y);
        glEnd();
    }
}

void Texture::free(){
    //Delete texture
    if(id != 0){
        glDeleteTextures(1, &id);
        id = 0;
    }

    //Delete pixels
    if(pixels != NULL){
        delete[] pixels;
        pixels = NULL;
    }

    texWidth = 0;
    texHeight = 0;
    imgWidth = 0;
    imgHeight = 0;
}

GLuint Texture::powerOfTwo(GLuint num){
    if(num != 0){
        num--;
        num |= (num >> 1); //Or first 2 bits
        num |= (num >> 2); //Or next 2 bits
        num |= (num >> 4); //Or next 4 bits
        num |= (num >> 8); //Or next 8 bits
        num |= (num >> 16); //Or next 16 bits
        num++;
    }
    return num;
}

GLuint Texture::getID(){
    return id;
}

GLuint Texture::getImgWidth(){
    return imgWidth;
}

GLuint Texture::getImgHeight(){
    return imgHeight;
}

GLuint Texture::getTexWidth(){
    return texWidth;
}

GLuint Texture::getTexHeight(){
    return texHeight;
}
