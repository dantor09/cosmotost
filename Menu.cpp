#include <iostream>
#include <string>
#include "Menu.h"
#include "Global.h"

#define BSIZE 5
#define PADDING 20

// Constructors

Menu::Menu(unsigned int _n_texts, 
            float w, float h, 
            float x, float y,
            std::string* _words)
    : n_texts{_n_texts}, pos{x, y, 0}
{
    // dynamially allocate boxes/rects for text display
    mainbox.set_dim(w, h);
    mainbox.set_pos(pos[0], pos[1], pos[2]);
    mainbox.set_color(47, 61, 64);
    boarder.set_dim(w+BSIZE, h+BSIZE);
    boarder.set_pos(pos[0], pos[1], pos[2]);
    boarder.set_color(69, 85, 89);
    
    try {
        t_boxs = new Box[n_texts];
        texts = new Rect[n_texts];
        words = new std::string[n_texts];
        

        float spacing = (2*h)/(n_texts+1);
        std::cout << "spacing: " << spacing << std::endl;

        for(int i = 0; i < n_texts; i++) {
            t_boxs[i].w = mainbox.w - PADDING;
            t_boxs[i].h = (spacing/2.0) - BSIZE;
            t_boxs[i].pos[0] = mainbox.pos[0];
            t_boxs[i].pos[1] = (pos[1]+mainbox.h)-((i+1)*spacing);
            t_boxs[i].set_color(61, 90, 115);
            std::cout << "t_box[" << i << "].pos[1]: " 
                        << t_boxs[i].pos[1] << std::endl;
            
            words[i] = _words[i];

            // Leaving this here for the next poor soul that
            // comes accross this issue:

            // apparently you need to set their position every single time
            // you print them or they fall off the screen for some stupid reason
            // so this code will now be found below in the draw() function

            // texts[i].bot = mainbox.pos[1]-(i*20);
            // texts[i].left = t_boxs[i].pos[0];
            // texts[i].center = 1;
        }

    } catch (std::bad_alloc& ba) {
        // if one was allocated and not the other than delete the one that 
        if (texts) delete [] texts;
        if (t_boxs) delete [] t_boxs;
        if (words) delete [] words;
        // print to screen for now until we have logging set up
        std::cerr << "Error allocating rectangles in Menu call\n"
                << ba.what() << '\n';
        texts = nullptr;
        t_boxs = nullptr;
        words = nullptr;
    }
}

Menu::~Menu()
{
    if (texts)
        delete [] texts;
    
    if (t_boxs)
        delete [] t_boxs;

    if (words)
        delete [] words;
        
}

void Menu::draw()
{
    // draw boarder

    glColor3ubv(boarder.color);
    
    glPushMatrix();
    glTranslatef(boarder.pos[0], boarder.pos[1], boarder.pos[2]);
    glBegin(GL_QUADS);
        glVertex2f(-boarder.w, -boarder.h);
        glVertex2f(-boarder.w,  boarder.h);
        glVertex2f( boarder.w,  boarder.h);
        glVertex2f( boarder.w, -boarder.h);
    glEnd();
    glPopMatrix();

    // draw mainbox

    glColor3ubv(mainbox.color);
    
    glPushMatrix();
    glTranslatef(mainbox.pos[0], mainbox.pos[1], mainbox.pos[2]);
    glBegin(GL_QUADS);
        glVertex2f(-mainbox.w, -mainbox.h);
        glVertex2f(-mainbox.w,  mainbox.h);
        glVertex2f( mainbox.w,  mainbox.h);
        glVertex2f( mainbox.w, -mainbox.h);
    glEnd();
    glPopMatrix();

    // draw all t_boxes

    for(size_t i = 0; i < n_texts; i++) {
        glColor3ubv(t_boxs[i].color);
    
        glPushMatrix();
        glTranslatef(t_boxs[i].pos[0], t_boxs[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
            glVertex2f(-t_boxs[i].w, -t_boxs[i].h);
            glVertex2f(-t_boxs[i].w,  t_boxs[i].h);
            glVertex2f( t_boxs[i].w,  t_boxs[i].h);
            glVertex2f( t_boxs[i].w, -t_boxs[i].h);
        glEnd();
        glPopMatrix();

    }

    // draw texts - need to pass in texts still; this is only for testing
    
    // for(int i = 0; i < n_texts; i++) {
    //     ggprint8b((texts+i), 16, 0x00FFFFFF, "c0sm0t0asT");
    //     // std::cout << "tests[" << i << "].pos[1]: " << texts[i].pos[1] << std::endl;
    // }


    for(int i = 0; i < n_texts; i++) {
        texts[i].bot = t_boxs[i].pos[1] - 5;
        texts[i].left = t_boxs[i].pos[0];
        texts[i].center = 1;


        // r[i].bot = t_boxs[i].pos[1] - 5;
        // r[i].left = t_boxs[i].pos[0];
        // //g.r[i].center = box[i].pos[0];

        // r[i].center = 1;

        ggprint8b(texts+i, 16, 0x00ffffff, words[i].c_str());
    }
    


    
}


void Menu::set_bcolor(int r, int g, int b) 
{
    bcolor[0] = (char)r; 
    bcolor[1] = (char)g; 
    bcolor[2] = (char)b;
}

void Menu::set_color(int r, int g, int b) 
{
    color[0] = (char)r; 
    color[1] = (char)g; 
    color[2] = (char)b;
}

void Menu::set_pos(float x, float y, float z) 
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

std::string Menu::get_info()
{
    std::ostringstream temp;
    temp <<             std::endl;

    return temp.str();
}

/*
                Color Scheme

.feeling-blue-1-hex { color: #3D5A73; }
.feeling-blue-2-hex { color: #2F3D40; }
.feeling-blue-3-hex { color: #455559; }
.feeling-blue-4-hex { color: #28403D; }
.feeling-blue-5-hex { color: #182625; }

.feeling-blue-1-rgba { color: rgba(61, 89, 114, 1); }
.feeling-blue-2-rgba { color: rgba(47, 61, 63, 1); }
.feeling-blue-3-rgba { color: rgba(68, 84, 89, 1); }
.feeling-blue-4-rgba { color: rgba(40, 63, 61, 1); }
.feeling-blue-5-rgba { color: rgba(24, 38, 36, 1); }

.feeling-blue-1-hsla { color: hsla(207, 30, 34, 1); }
.feeling-blue-2-hsla { color: hsla(190, 15, 21, 1); }
.feeling-blue-3-hsla { color: hsla(192, 12, 30, 1); }
.feeling-blue-4-hsla { color: hsla(172, 23, 20, 1); }
.feeling-blue-5-hsla { color: hsla(175, 22, 12, 1); }
*/