//Huaiyu Zhang
//Feb 24
#pragma once
#include "Box.h"
// #include "Global.h"
#include <GL/glx.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <new>
#include <sstream>
#include "fonts.h"
#include <string>
#include <vector>

//#include "aparriott.h"
using namespace std;


class Entity; // forward declaration
class Blocky;  // forward declaration
struct HighScore; // forward declaration

//make a bullet need the pos of where this bullet creat
//type is id of Bullet_type
void makeBullet(float x, float y,float z, int bullet_type);
//make a bread need the pos of where this bread creat
//int Bread_t is the type of bread
//type is id of Bullet_type this bread carrying
void makeBread(float x, float y,float z, int Bread_t, int type);
void makeSpear(float x, float y,float z, int type);

float crossX(float x0,float y0,float xa,float ya,float xb,float yb);
float maxRadius(float *arr,int n);
float minRadius(float *arr,int n);
float maxTan(float *arr,int n);
float minTan(float *arr,int n);
bool pointIn(float x0, float y0, float *arr, int n);


// item class is the parent class for all the object might have HP in game
// include toaster, bullets and breads
class Item: public Box
{
  public:
    GLuint * tex;
    Box * plive;
    //item_type show what kind of item it is
    int item_type;
    bool trace = false;

    // Toaster is 0;

    // velocity
    float vel[3];

    //acceleration
    float acc[3];
    // all item should have hp
    // when two object touch, use HP - damage
    // by check HP we can tell if we need delete that object
    float starting_hp;
    float hp;
    int lives;
    float damage;
    float vertex[8];
    // CD is cool down
    //  when create item
    //need a number to tell how long we don't want that object been create again.
    int cd;

    Item();
    ~Item();

    // setters
    void setHP(float life);
    void setCD(int x);
    void setDamage(float x);
    void setVel(float x, float y, float z);
    void setAcc(float x, float y, float z);
    void setVertex();
    // for trace Bullet
    void setTrace(Item tos);
    // to check toaster HP, if <=0 then dead
    bool hpCheck();
    // to check
    bool collision(Item a);
    //change HP after Collison
    void hpDamage(float);
    void hpDamage(Item a);
    void hpDamage(Entity & e);  // defined in mkausch.cpp for entity collision
    void hpDamage(Blocky & b); // defined in mkausch.cpp for BF collision
    // check if item reach the in the screen
    bool screenIn();
    //check if item out of screen
    bool screenOut();
    void draw();
    void draw(Item tos);
    Item & operator = (const Item &);

};

class Toaster: public Item
{
  public:
      // the bullet type toaster carrying
      int bullet_type_prime;
      int bullet_type_minor;
      int score;
      float energy;
      float energy_recover;
      bool disable_keys;
      bool laserOn;
      float distance;
      float laser_damage[4] = {0.1,0.25,0.5,1};
      float laser_h[4] = {1,1.5,2,2.5};

      Toaster();
      ~Toaster();
      void posReset();
      bool laserCollision(Item a);
      bool laserCollision(Entity a);
      void setDistance(float val);
      void laserDamage(Item &a);
      void laserDamage(Entity &a);

      // getters
      // unsigned char * get_tcolor();

      // other
      // draw toaster
      // void draw();
      void moveToster();
      void bulletReload();
      void tdraw();
      // string PrintScore();

} ;

class Bullet: public Item
{
  public:
     Bullet();
     ~Bullet();


     // tb to check if from Toaster or Bread
     // type is id of what type of bullet
     // different bullet may have different damage hp or velocitys
     void setBullet(float x, float y,float z, int type);
     void moveBullet();
};

class Bread: public Item
{
  public:
  // the bullet type bread carrying
    int bullet_type_prime;
  // how many point(score) bread carrying
    int point;


    Bread();
    ~Bread();
    // Bread_t is the id of what type of Bread
    // type is id of what type of bullet this Bread carrying
    // different bullet may have different damage hp or velocitys
    void setBread(float x, float y,float z, int Bread_t, int type);
    void moveBread();
};
class Spear: public Item
{
  public:
    Spear();
    ~Spear();
    void setSpear(float x, float y,float z, int spear_type);
    void moveSpear();

};

class Donut
{
  public:
  // Boss
    float out_radius;
    float inner_radius;
    float deamage_radius;
    float hp;
    int cd;     // CD between Donets weapon
    int count_down; // CD count_down
    bool up_down; // 0 up, 1 down
    bool weapon; // if weapon is true then donut attecking
                 // if weapon is false then countdown cd 
    int weapon_id;
    bool shelled; // have shelled or not
    float pos[3];
    float vel[3];
    int donut_count;
    int weapon_outer_count;
    int weapon_inner_count;
   
  //========================================
    Donut();
    ~Donut();
    void moveDonut();
    void setCD();
    bool hpCheck();
    bool collision(Item);
    void hpDemageDonut(Item);
    void draw();
    void atteckMove(int num);
};
//=======================================================================

