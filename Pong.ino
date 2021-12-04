#include <Adafruit_CircuitPlayground.h>
#include <time.h>
#include <stdbool.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Set up CPX board with buttons
  CircuitPlayground.begin();
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);

  //Set LED brightness as 5
  CircuitPlayground.setBrightness(5);

  //Generate a random seed for determining who gets to start
  randomSeed(analogRead(0));
}


void startup() {
  //Create ring of lights
  for (int i = 0; i <= 9; i++) {
    CircuitPlayground.setPixelColor(i,255,0,0);
    delay(250);
  }
  //Turn off lights
  CircuitPlayground.clearPixels();
}


//Signal the player who starts
void signalStart(unsigned int player) {

  //Player 2
  if (player == 2) {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 5; j++) {
        //Flash Start Side
        CircuitPlayground.setPixelColor(j,255,69,0);
      }
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
    }
  }
  //Player 1
  else {
    for (int i = 0; i < 3; i++) {
      for (int j = 5; j < 10; j++) {
        //Flash Start Side
        CircuitPlayground.setPixelColor(j,255,69,0); //Orange
      }
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
    }
  }
}


//Light up Ball position
void ballLight(unsigned int ball) {
  //ball 1-3 corresponds to lights 5-9, ball 4-6 corresponds to lights 0-4
  //Algorithm corrects these light values and gives a 1 light gap between them
  
  if (ball <= 3) {
    CircuitPlayground.setPixelColor(2*(ball-1)+5,255,255,255);
  }
  else {
    CircuitPlayground.setPixelColor(2*(ball-4),255,255,255);
  }
}


//Signal the side of whoever missed
void missSignal(unsigned int player) {
  //Player 1 Miss
  if (player == 2) {
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        //Flash Loser Side
        CircuitPlayground.setPixelColor(j,255,0,0);
      }
    delay(250);
    CircuitPlayground.clearPixels();
    delay(250);
    }
  }
  //Player 2 Miss
  else if (player == 1) {
    for (int i = 0; i < 5; i++) {
      for (int j = 5; j < 10; j++) {
        //Flash Loser Side
        CircuitPlayground.setPixelColor(j,255,0,0);
      }
    delay(250);
    CircuitPlayground.clearPixels();
    delay(250);
    } 
  }
}


//Check if there is a winner
bool checkWin (unsigned int P1score, unsigned int P2score) {
  bool win = false;

  //If either player has 5 or more points
  if (P1score >= 3 || P2score >= 3) {
    //Winner
    win = true;
  }
  //Return boolean
  return win;
}


//Show who won
void winnerDisplay (unsigned int P1score, unsigned int P2score) {
  //If Player 1 wins
  if (P1score >= 3) {
    for (int i = 0; i <= 4; i++) {
      CircuitPlayground.setPixelColor(i,0,255,0);
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
    }
  }
  //If Player 2 wins
  else if (P2score >= 3) {
    for (int i = 5; i <= 9; i++) {
      CircuitPlayground.setPixelColor(i,0,255,0);
      delay(500);
      CircuitPlayground.clearPixels();
      delay(500);
    }
  }
}


//Display Score of both sides
void displayScore (unsigned int P1score, unsigned int P2score) {
  //Display P1score
  for (int i = 0; i < P1score; i++) {
    CircuitPlayground.setPixelColor(i,255,0,0);
  }

  //Display P2score
  for (int i = 5; i < 5+P2score; i++) {
    CircuitPlayground.setPixelColor(i,255,0,0);
  }

  //Hold the score
  delay(2500);
  CircuitPlayground.clearPixels();
}


//Handles the first hit
unsigned int firstHit(unsigned int player) {
  unsigned int hit = 0;

  //Player 1
  if (player == 1) {
    if (digitalRead(A1) == LOW) {
      hit = 1;
    }
    else if (digitalRead(A2) == LOW) {
      hit = 2;
    }
    else if (digitalRead(A3) == LOW) {
      hit = 3;
    }
  }
  //Player 2
  else if (player == 2) {
    if (digitalRead(A4) == LOW) {
      hit = 4;
    }
    else if (digitalRead(A5) == LOW) {
      hit = 5;
    }
    else if (digitalRead(A6) == LOW) {
      hit = 6;
    }
  }

  //Return the new ball location
  return hit;
}



//Handles the remainder of hits
unsigned int hit(unsigned int player, unsigned int ball) {
  unsigned int hitpos = 0;

  //Light up ball position in white
  ballLight(ball);
  
  //Player 2
  if (player == 2) {
    
    //Position 1
    if (ball == 1) {
      while (digitalRead(A1) == HIGH) {
        //Wait for correct input
      }
      hitpos = 4; //Hit across
      //Curve ball
      if (digitalRead(A2) == LOW) {
        hitpos++;
      }
    }

    //Position 2
    else if (ball == 2) {
      while (digitalRead(A2) == HIGH) {
        //Wait for correct input
      }
      hitpos = 5; //Hit across
      //Curve ball
      if (digitalRead(A1) == LOW) {
        hitpos--;
      }
      else if (digitalRead(A3) == LOW) {
        hitpos++;
      }
    }

    //Position 3
    else if (ball == 3) {
      while (digitalRead(A3) == HIGH) {
        //Wait for correct input
      }
      hitpos = 6; //Hit across
      //Curve ball
      if (digitalRead(A2) == LOW) {
        hitpos--;
      }      
    }
  }

  //Player 1
  else if (player == 1) {
    //Position 1
    if (ball == 4) {
      while (digitalRead(A4) == HIGH) {
        //Wait for correct input
      }
      hitpos = 1; //Hit across
      //Curve ball
      if (digitalRead(A5) == LOW) {
        hitpos++;
      }
    }

    //Position 2
    else if (ball == 5) {
      while (digitalRead(A5) == HIGH) {
        //Wait for correct input
      }
      hitpos = 2; //Hit across
      //Curve ball
      if (digitalRead(A4) == LOW) {
        hitpos--;
      }
      else if (digitalRead(A6) == LOW) {
        hitpos++;
      }
    }

    //Position 3
    else if (ball == 6) {
      while (digitalRead(A6) == HIGH) {
        //Wait for correct input
      }
      hitpos = 3; //Hit across
      //Curve ball
      if (digitalRead(A5) == LOW) {
        hitpos--;
      }      
    }
  }

  CircuitPlayground.clearPixels(); //Remove the ball light
  return hitpos; //Return new ball location
}


void loop() {

  //Initialize variables
  unsigned long startTime, endTime;
  unsigned int rally = 0; //Ensure fair time reduction for each side
  double tdiff = 0;
  float timePong = 2000; //2 ms. Reset margin of error
  unsigned int start = 0;
  bool win = false, miss = false;
  unsigned int P1score = 0, P2score = 0, ball = 0, player = 2;


  //Determine who starts
  start = random(1,3);
  player = start;

  //Begin Game
  while (win == false) {

  //Reset variables each loop
  win = false;
  miss = false;
  timePong = 2000;
  rally = 0;
  

  //Run the startup program
  startup();
  signalStart(player);
    
  //Begin the round via First Hit
  if (start == 1) {
    while (digitalRead(A1) == HIGH && digitalRead(A2) == HIGH && digitalRead(A3) == HIGH) {
      //Wait until button is pressed
    }
    ball = firstHit(start);
    player = 2;
  }
  else {
    while (digitalRead(A4) == HIGH && digitalRead(A5) == HIGH && digitalRead(A6) == HIGH) {
      //Wait until button is pressed
    }
    ball = firstHit(start);
    player = 1;
  }


    //Continue the game
    while (miss == false) {
      //Start the clock
      startTime = millis();

      //Hit the ball
      ball = hit(player, ball);

      //Switching player
      if (player == 2) {
        player = 1;
      }
      else {
        player = 2;
      }

      //End the clock
      endTime = millis();

      //Check time difference in ms
      tdiff = (endTime - startTime);  

      //If miss, then add score to the winner and exit loop
      if (tdiff > timePong) {
        if (player == 2) {
          P1score++;
          start = 2; //Player 2 starts next turn
          missSignal(start);
        }
        else if (player == 1) {
          P2score++;
          start = 1; //Player 1 starts next turn
          missSignal(start);
        }
        miss = true; //Exit loop
      }

      //Reducing the time between each rally
      if (rally == 2) {
        rally = 0;
        if (timePong > 250) {
          timePong = timePong - 250;
        }
        else {
          timePong = timePong/2;
        }
      }
      else {
        rally++;
      }

      
    }

    //Display the current score
    displayScore(P2score,P1score);

    //Check if winner
    win = checkWin(P1score,P2score);
  }

  //Show who won
  winnerDisplay(P2score,P1score);
  
}
