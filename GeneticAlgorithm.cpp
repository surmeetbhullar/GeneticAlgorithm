//Genetic Algorithm
//Surmeet Bhullar


#include <iostream>
using namespace std; 
#include <iomanip>
#include <string>
#include <limits>
#include <algorithm>
#include <ctime>

//constants for each repeated number, it makes it much easier to find bugs 
const int NUMGENES = 16;
const int DIGIT = 5; 
const int MAPX = 10;
const int MAPY = 10;
const int BATTERIES = 40; 
const int ROBOTS = 200;
const int EMPTY = 0;
const int WALL = 1;
const int BATT = 2; 
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;

//class where all the robot components are present, px and py correspond to the actual position of the robot. power and fitness are defaulted to 5 and have the ability to be used but I later focused on just using turns. 
class robot 
{
  public: 
  int gene[NUMGENES][DIGIT];
  string map[MAPX][MAPY];
  int power = 5;
  int fitness = 5;
  int px;
  int py;
  int match = 0;
  int turns;
  int nsensor;
  int ssensor;
  int esensor;
  int wsensor;
  void setgene();
  void setbat();
  };

  //this sets all the genes of all 200 robots
void robot :: setgene()
{
  for (int i = 0; i < NUMGENES; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      gene[i][j] = rand() % 3;
    }
  }
  for (int k = 0; k < NUMGENES; k++)
  {
    gene[k][4] = rand() % 4;
  }
}

//this basically creates all the batteries and initializes all the places in the map to be empty spaces, and wherever there is a B already present it makes sure another B is not set on top. 
void robot :: setbat() 
{
  //makes all spots on the map empty spaces
  for (int i = 0; i < MAPX; i++)
  {
    for (int j = 0; j < MAPY; j++)
    {
      map[i][j] = " ";
    }
  }
  
  //creates 40 batteries with the letter B on the map
  for (int i = 0; i < BATTERIES; i++)
  {
    int x = rand() % 10;
    int y = rand() % 10;
    while (map[x][y] == "B")
    {
      x = rand() % 10;
      y = rand() % 10;
    }
    map[x][y] = "B";
  }
}

int main() 
{
  srand(time(NULL));

  //the for loop later on for assigning genes is 100 - 200, this is my little trick in order to create two childs using the genes of pairs starting from robot 0 and 1
  int mom = 0;
  int dad = 1;
  
  double average, sum;
  sum = 0;
  int avg[1000];
  robot bot[ROBOTS];
  

  //this for loop initializes all things that need to be initialized 
  for (int i = 0; i < ROBOTS; i++)
  {
    bot[i].setgene();
    bot[i].setbat();
    bot[i].px = rand() % 10;
    bot[i].py = rand() % 10;
    while (bot[i].map[bot[i].px][bot[i].py] == "B")
    {
      bot[i].px = rand() % 10; 
      bot[i].py = rand() % 10;
    }
    bot[i].map[bot[i].px][bot[i].py] = "R";
  }

  //the outside for loop corresponds to the number of generations that are ran
  for (int b = 0; b < 1000; b++)
  {
    //each generation has 200 robots 
    for (int x = 0; x < ROBOTS; x++)
    { 

      //each robot has 16 genes 
      for (int i = 0; i < NUMGENES; i++)
      {
        //this is a protection type of if statement, if the power is = 0 then it will break the loop if not it will run smoothly 
        if (bot[x].power != 0) 
        {
          //px and py are crucial for the robot, because they are used in order to make sure no out of bounds errors occur
          //there are three "if" or "else if" type statements corresponding to each sensor, this declares what is surrounding the robot
          if (bot[x].px - 1 == -1)
          { 
            //you'll see below the declaration of the sensor for walls specifically what I do is I see if one position above the current position the y would be -1, if so then that means that is an area we can not explore aka a wall
            bot[x].nsensor = WALL;
          }
          else if (bot[x].map[bot[x].px - 1][bot[x].py] == " ")
          {
            //same idea as above for this one but the wall if statement is always first because I can not risk any out of bounds errors or else it will lead to results I don't want 
            bot[x].nsensor = EMPTY;
          }
          else if (bot[x].map[bot[x].px - 1][bot[x].py] == "B")
          {
            bot[x].nsensor = BATT;
          }
          if (bot[x].px + 1 == 10)
          {
            bot[x].ssensor = WALL;
          }
          else if (bot[x].map[bot[x].px + 1][bot[x].py] == " ")
          {
            bot[x].ssensor = EMPTY;
          }
          else if (bot[x].map[bot[x].px + 1][bot[x].py] == "B")
          {
            bot[x].ssensor = BATT;
          }
          if (bot[x].py + 1 == 10)
          {
            bot[x].esensor = WALL;
          }
          else if (bot[x].map[bot[x].px][bot[x].py+1] == " ")
          {
            bot[x].esensor = EMPTY;
          }
          else if (bot[x].map[bot[x].px][bot[x].py+1] == "B")
          {
            bot[x].esensor = BATT;
          }
          if (bot[x].py - 1 == -1)
          {
            bot[x].wsensor = WALL;
          }
          else if (bot[x].map[bot[x].px][bot[x].py-1] == " ")
          {
            bot[x].wsensor = EMPTY;
          }
          else if (bot[x].map[bot[x].px][bot[x].py-1] == "B")
          {
            bot[x].wsensor = BATT;
          }
          //if there is even a single match between a sensor and corresponding gene digit then the corresponding gene movement is made
          //my gene code is like this: NESW the last digit represents the movement, but a Wall = 1, Battery = 2, Nothing = 0. For movement, North = 0, East = 1, South = 2, West = 3.
          if ((bot[x].gene[i][0] == bot[x].nsensor)||(bot[x].gene[i][1] == bot[x].esensor)||(bot[x].gene[i][2] == bot[x].ssensor)||(bot[x].gene[i][3] == bot[x].wsensor))
          {
          if (bot[x].gene[i][4] == NORTH)
          {
            if (bot[x].nsensor == WALL)
            {
              //because a movement is made power goes down 1 
              //because a movement is made turns increases by 1
              //since the robot hit a wall, the position does not change at all
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].nsensor == EMPTY)
            {
              //because we already know the north is empty the new position is made to be R and then the previous position is made to be an empty space
              bot[x].map[bot[x].px-1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              
              //this changes the actual position of the robot
              bot[x].px = bot[x].px - 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].nsensor == BATT)
            {
              //same idea as up above but now we are replacing what was a B with an empty space
              bot[x].map[bot[x].px-1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px - 1;
              //because the robot hit a battery the power increases by 5 instead of decreasing by 1
              bot[x].power = bot[x].power + 5;
              //because there is a movement turns is increased by 1
              bot[x].turns++;
            }
          } 
          else if (bot[x].gene[i][4] == EAST)
          {
            if (bot[x].esensor == WALL)
            {
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].esensor == EMPTY)
            {
              bot[x].map[bot[x].px][bot[x].py+1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py + 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].esensor == BATT)
            {
              bot[x].map[bot[x].px][bot[x].py+1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py + 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          } 
          else if (bot[x].gene[i][4] == SOUTH)
          {
            if (bot[x].ssensor == WALL)
            {
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].ssensor == EMPTY)
            {
              bot[x].map[bot[x].px+1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px + 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].ssensor == BATT)
            {
              bot[x].map[bot[x].px+1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px + 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          }
          else if (bot[x].gene[i][4] == WEST)
          {
            if (bot[x].wsensor == WALL)
            {
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].wsensor == EMPTY)
            {
              bot[x].map[bot[x].px][bot[x].py-1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py - 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].wsensor == BATT)
            {
              bot[x].map[bot[x].px][bot[x].py-1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py - 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          }
        }
        //this else statement is here for if there are no matches at all between the sensors and the gene codes. What this does is it randomizes a movement between 0 - 3 and the corresponding randomized number is then executed and changes are made to make up for the movement 
        else 
        {
          int random = rand() % 4;
          if (random == NORTH)
          {
            if (bot[x].nsensor == WALL)
            {
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].nsensor == EMPTY)
            {
              bot[x].map[bot[x].px-1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px - 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].nsensor == BATT)
            {
              bot[x].map[bot[x].px-1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px - 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          }
          else if (random == EAST)
          {
            if (bot[x].esensor == WALL)
            { 
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].esensor == EMPTY)
            {
              bot[x].map[bot[x].px][bot[x].py+1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py + 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].esensor == BATT)
            {
              bot[x].map[bot[x].px][bot[x].py+1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py + 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          }
          else if (random == SOUTH)
          {
            if (bot[x].ssensor == WALL)
            {
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].ssensor == EMPTY)
            {
              bot[x].map[bot[x].px+1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px + 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].ssensor == BATT)
            {
              bot[x].map[bot[x].px+1][bot[x].py] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].px = bot[x].px + 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          } 
          else if (random == WEST)
          {
            if (bot[x].wsensor == WALL)
            {
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].wsensor == EMPTY)
            {
              bot[x].map[bot[x].px][bot[x].py-1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py - 1;
              bot[x].power--;
              bot[x].turns++;
            }
            else if (bot[x].wsensor == BATT)
            {
              bot[x].map[bot[x].px][bot[x].py-1] = "R";
              bot[x].map[bot[x].px][bot[x].py] = " ";
              bot[x].py = bot[x].py - 1;
              bot[x].power = bot[x].power + 5;
              bot[x].turns++;
            }
          }
        }
      }
      //here the loop breaks if the power for the robot is = to 0
      else if (bot[x].power == 0)
      {
        break;
      }
      }
      //just in case there is any power left we go back to the preivous robot
      if (bot[x].power >> 0)
      {
        x--;
      }
      }
      //this for loop sums up the total turns for every robot of the generation
      for (int v = 0; v < ROBOTS; v++)
      {
        sum += bot[v].turns;
      }

      //the average is the sum of the turns of the robots divided by 200, this is what you see on your screen
      average = sum / ROBOTS;
      avg[b] = average;

      //my textbook expired so I found this neat little trick online regarding the sorting of the robot: https://www.cplusplus.com/reference/algorithm/sort/
      std::sort(bot, bot + ROBOTS,
      [](robot const & a, robot const & b) -> bool
         { return a.turns > b.turns; } );

      //this for loop swaps the genes of the robots, x is set to 100 and increases by 2 each time because I use x as the first child and x+1 as the second child 
      for (int x = 100; x < ROBOTS; x = x + 2)
      {
        //for the first child the first eight genes come from dad and for the second child the first eight genes come from mom 
        for (int l = 0; l < 8; l++)
        {
          for (int z = 0; z < 5; z++)
          {
            bot[x].gene[l][z] = bot[dad].gene[l][z];
            bot[x+1].gene[l][z] = bot[mom].gene[l][z];
          }
        }
        //for the first child the last eight genes come from mom and for the second child the last eight genes come from dad
        for (int y = 8; y < 16; y++)
        {
          for (int z = 0; z < 5; z++)
          {
            bot[x].gene[y][z] = bot[mom].gene[y][z];
            bot[x+1].gene[y][z] = bot[dad].gene[y][z];
          }
        }
        //mom and dad increase by two because we need a new set of parents 
        mom = mom + 2;
        dad = dad + 2;
      }
      //this is for the next generation, we need to make sure the parents always start from the top of the list 
      mom = 0;
      dad = 1;

      //this for loop is for mutations, there is a 5 percent chance of a mutation occuring in each newly inherited row of genes so we need to implement the chance of those mutations occuring
      for (int f = 100; f < ROBOTS; f++)
      {
        for (int z = 0; z < NUMGENES; z++)
        { 
          if (rand() % 100 < 5)
          {
            int s = rand() % 5;
            //because I have numbers 0-3 for the fifth digit of the gene code I have to create an if statement in order to make sure the right randomized number is assigned if a mutation even occurs 
            if (s == 4)
            {
              bot[f].gene[z][s] = rand() % 4;
            }
            else 
            {
              bot[f].gene[z][s] = rand() % 3;
            }
          }
        }
      } 

      //this makes sure new maps with new battery positions are created, it also creates new positions for each robot for the new map. Lastly, it resets power, fitness, and turns for each robot to the number they initially were. Sum and average are also set to 0.
      for (int i = 0; i < ROBOTS; i++)
      {
        bot[i].setbat();
        bot[i].power = 5;
        bot[i].fitness = 5;
        bot[i].turns = 0;
        bot[i].px = rand() % MAPX;
        bot[i].py = rand() % MAPY;
        while (bot[i].map[bot[i].px][bot[i].py] == "B")
        {
          bot[i].px = rand() % MAPX; 
          bot[i].py = rand() % MAPY;
        }
        bot[i].map[bot[i].px][bot[i].py] = "R";
      }
      sum = 0;
      average = 0;
    }
  cout << "Average Batteries Generation 1: " << avg[0] << endl;
  cout << "Average Batteries Generation 100: " << avg[99] << endl;
  cout << "Average Batteries Generation 200: " << avg[199] << endl;
  cout << "Average Batteries Generation 400: " << avg[399] << endl;
  cout << "Average Batteries Generation 600: " << avg[599] << endl;
  cout << "Average Batteries Generation 800: " << avg[799] << endl;
  cout << "Average Batteries Generation 1000: " << avg[999] << endl;
} 