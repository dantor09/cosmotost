//Huaiyu Zhang
//Feb 24
#pragma once
#include "Box.h"
// #include "Global.h"
#include <GL/glx.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <new>
#include <sstream>
#include "fonts.h"
#include <string>
//#include "aparriott.h"
using namespace std;


class Entity; // forward declaration
class Blocky;  // forward declaration

//make a bullet need the pos of where this bullet creat
//bool(tb) is means this bullet from toster or not
//type is id of Bullet_type
void makeBullet(float x, float y,float z, bool tb, int type);
//make a bread need the pos of where this bread creat
//int Bread_t is the type of bread
//type is id of Bullet_type this bread carrying
void makeBread(float x, float y,float z, int Bread_t, int type);

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
    int starting_hp;
    int hp;
    int lives;
    int damage;
    float vertex[8];
    // CD is cool down
    //  when create item
    //need a number to tell how long we don't want that object been create again.
    int cd;

    Item();
    ~Item();

    // setters
    void setHP(int life);
    void setCD(int x);
    void setDamage(int x);
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
      int b_type;
      int score;
      float energy;
      float energy_recover;
      bool disable_keys;

      Toaster();
      ~Toaster();
      void posReset();
      // getters
      // unsigned char * get_tcolor();

      // other
      // draw toaster
      // void draw();
      void moveToster();
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
     void setBullet(float x, float y,float z, bool tb, int type);
     void moveBullet();
};

class Bread: public Item
{
  public:
  // the bullet type bread carrying
    int b_type;
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
//=======================================================================
class Gamerecord
{
public:
		int n = 0;
		char gamer[10];
  // type in name
    string str;
  //the highest score in file
    int highscore = 0;
  //the name in the file
    string reName;
	// true mean no record in it
	// false means file exist
		bool delt;
		Gamerecord();
    ~Gamerecord();
    void changeRecord(int);
    void getRecord();

};
