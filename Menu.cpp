#include "Menu.h"
#include "Global.h"

#define BSIZE 5

// Constructors

Menu::Menu(unsigned int _n_texts, float w, float h, float _x, float _y)
    : n_texts{_n_texts}, pos{_x, _y, 0}
        /*w{_w}, h{_h}, 
        bw{_w+BSIZE}, bh{_h+BSIZE},*/ 
        
{
    // dynamially allocate boxes/rects for text display
    mainbox.set_dim(w, h);
    boarder.set_dim(w+BSIZE, h+BSIZE);
    
    try {
        t_boxs = new Box[n_texts];
        texts = new Rect[n_texts];

        float spacing = (2*h)/(n_texts+1);

        for(size_t i = 0; i < n_texts; i++) {
            t_boxs[i].pos[0] = mainbox.pos[0];
        }

    } catch( std::bad_alloc& ba ) {
        // if one was allocated and not the other than delete the one that 
        if (texts) delete [] texts;
        if (t_boxs) delete [] t_boxs;
        // print to screen for now until we have logging set up
        std::cerr << "Error allocating rectangles in Menu call\n"
                << ba.what() << '\n';
        texts = nullptr;
        t_boxs = nullptr;
    }
}

Menu::~Menu()
{
    if (texts)
        delete [] texts;
    
    if (t_boxs)
        delete [] t_boxs;
        
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
    temp << "menu.w: " << w << std::endl
        << "menu.h: " << h << std::endl
        << "menu.bw: " << bw << std::endl
        << "menu.bh: " << bh << std::endl
        << "menu.pos[x]: " << pos[0] << std::endl
        << "menu.pos[y]: " << pos[1] << std::endl;

    return temp.str();
}