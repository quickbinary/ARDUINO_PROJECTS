// Game of Life for Arduino UNO R4 WiFi 
// with On board 12x8 LED Matrix

#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

// grid dimensions
#define MAX_Y 8
#define MAX_X 12
// custom starting grid patterns
#define MAX_C 18
// time to wait between turns
#define TURN_DELAY 800
// how many turns per game before starting a new game
#define TURNS_MAX 60
// how many turns to wait if there are no changes before starting a new game
#define NO_CHANGES_RESET 4

int TURNS = 0;       
int NO_CHANGES = 0;  

// initialise the LED matrix

byte allOn[MAX_Y][MAX_X] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};


// game state. 0 is dead cell, 1 is live cell
byte grid[MAX_Y][MAX_X] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

// custom starting grid patterns
boolean cgrids[MAX_C][MAX_Y][MAX_X] = {
    {
        {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0},
    },
    {
        {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    },
    {
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
        {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    },


     {
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
   },

     {
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
      { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
      { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 },
      { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
   },

     {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0}, 
      {0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
      {0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    }
   
};


void setup() {

  Serial.begin(115200);
  Serial.println("Conways Game of life");
  Serial.println("For Arduino UNO R4 WiFi");
  Serial.println("Roni Bandini, 2023, Argentina");
  
  matrix.begin();
  
  randomSeed(analogRead(0));
  
  reset_grid();
  display_grid();  
  debug_grid();
}

void loop() {
  
  delay(TURN_DELAY);

  play_gol();

  TURNS++;
  
  // for when the game enters a static stable state
  if (NO_CHANGES > NO_CHANGES_RESET) {
    Serial.println("No recent changes...");
    reset_grid();
  }
  // for when the game cycles between a few stable states
  if (TURNS > TURNS_MAX) {
    Serial.println("Game cycle detected...");
    reset_grid();
  }

  display_grid();
}

/////////////////////////////////////////////////////////////////////

void play_gol() {
   
  boolean new_grid[MAX_Y][MAX_X] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };

  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      
      int neighboughs = count_neighboughs(y, x);
      
      if (grid[y][x] == 1) {
        if ((neighboughs == 2) || (neighboughs == 3)) {
          //Serial.println("+");
          new_grid[y][x] = 1;
        } else {
           //Serial.println("-");
          new_grid[y][x] = 0;
        }
      } else {
        if (neighboughs == 3) {
          //Serial.println("+");
          new_grid[y][x] = 1;
        } else {          
          new_grid[y][x] = 0;
        }
      }
    }
  }

  // update to new matrix while count changes
  int changes = 0;
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      if (new_grid[y][x] != grid[y][x]) {
        changes++;
      }
      grid[y][x] = new_grid[y][x];
    }
  }

  // update global counter when no changes occured
  if (changes == 0) {
    NO_CHANGES++;
  }
}

/////////////////////////////////////////////////////////


int count_neighboughs(int y, int x) {
  int count = 0;

  // -- Row above us ---
  if (y > 0) {
    // above left
    if (x > 0) {
      count += grid[y - 1][x - 1];
    }
    // above
    count += grid[y - 1][x];
    // above right
    if ((x + 1) < 8) {
      count += grid[y - 1][x + 1];
    }
  }

  // -- Same row -------
  // left
  if (x > 0) {
    count += grid[y][x - 1];
  }
  // right
  if ((x + 1) < 8) {
    count += grid[y][x + 1];
  }

  // -- Row below us ---
  if ((y + 1) < 8) {
    // below left
    if (x > 0) {
      count += grid[y + 1][x - 1];
    }
    // below
    count += grid[y + 1][x];
    // below right
    if ((x + 1) < 8) {
      count += grid[y + 1][x + 1];
    }
  }

  return count;
}

/////////////////////////////////////////////////////////////////////
void reset_grid() {

  Serial.println("Reset grid");

  // all leds on
  matrix.renderBitmap(allOn, 8, 12);
  delay(1000);
  
  NO_CHANGES  = 0;
  TURNS       = 0;

  int grid_type           = random(0, 2);
  int custom_grid_choice  = random(0, MAX_C - 1);

  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      if (grid_type == 0) {
        // custom starting grid pattern        
        grid[y][x] = cgrids[custom_grid_choice][y][x];
      } else {
        // random starting grid pattern        
        if (random(0, MAX_X) <= 1) {
          grid[y][x] = 1;
        }
      }
      
    }
  }

  debug_grid();
  
}

/////////////////////////////////////////////////////////////////////

void display_grid() {
  matrix.renderBitmap(grid, MAX_Y, MAX_X);
}


/////////////////////////////////////////////////////////////////////
void debug_grid() {
  for (int y = 0; y < MAX_Y; y++) {
    Serial.print("y(");
    Serial.print(y);
    Serial.print("): ");

    for (int x = 0; x < MAX_X; x++) {
      Serial.print(grid[y][x]);
      Serial.print(", ");
    }

    Serial.println("");
  }
  Serial.println("");
}