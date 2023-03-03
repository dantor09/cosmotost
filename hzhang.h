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
using namespace std;

//make a bullet need the pos of where this bullet creat
//bool(tb) is means this bullet from toster or not
//type is id of Bullet_type
void make_Bullet(float x, float y,float z, bool tb, int type);
//make a bread need the pos of where this bread creat
//int Bread_t is the type of bread
//type is id of Bullet_type this bread carrying
void make_Bread(float x, float y,float z, int Bread_t, int type);

// item class is the parent class for all the object might have HP in game
// include toaster, bullets and breads
class Item: public Box
{
  public:
    //item_type show what kind of item it is
    int item_type;

    // Toaster is 0;

    // velocity
    float vel[3];

    //acceleration
    float acc[3];
    // all item should have hp
    // when two object touch, use HP - damage
    // by check HP we can tell if we need delete that object
    int HP;
    int damage;
    // CD is cool down
    //  when create item
    //need a number to tell how long we don't want that object been create again.
    int CD;

    Item();
    ~Item();

    // setters
    void set_HP(int life);
    void set_CD(int x);
    void set_damage(int x);
    void set_vel(float x, float y, float z);
    void set_acc(float x, float y, float z);

    // to check toaster HP, if <=0 then dead
    bool HP_check();
    // to check
    bool Collison(Item a);
    //change HP after Collison
    void HPdamage(Item a);
    // check if item reach the in the screen
    bool ScreenIn();
    //check if item out of screen
    bool ScreenOut();

    void draw();
};

class Toaster: public Item
{
  public:
      // the bullet type toaster carrying
      int b_type;
      int score;


      Toaster();
      ~Toaster();
      void posReset();
      // getters
      // unsigned char * get_tcolor();

      // other
      // draw toaster
      // void draw();
      void MoveToster();
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
     void set_Bullet(float x, float y,float z, bool tb, int type);
     void MoveBullet();
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
    void set_Bread(float x, float y,float z, int Bread_t, int type);
    void MoveBread();
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
    void ChangeR(int);
    void GetR();

};
