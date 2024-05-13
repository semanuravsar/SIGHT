//  The values info matrix represent the following;
//  mu1 is here == 1
//  mu2 is here == 2
//  mu3 is here == 3
//  BU is here == 4
//  This tile is scanned == 5
//  This tile has not been scanned == 0
//  The target is there == 6
//  mu is planning to go here == 7 // not used
//  obtacle here == 8

#include "DecisionUnArd_header.h";

// RFID includes
#include <SPI.h>
#include <MFRC522.h>
//RFID defines
#define RST_PIN 9
#define SS_PIN 10
#define TALK_PIN 8
#define ISRESET_PIN 3
MFRC522 rfid(SS_PIN, RST_PIN);

uint8_t isCardDetected = 0;

// Define positions
Position buPosition = {5, 5};
Position currentPosition = {2, 2};
// Position unknownTargetPosition = {1, 9};
Position targetPosition = {-1,-1};

// Define obstacles
Position obstacles[MAX_OBSTACLES];

int obstacleCount = 2;
int buLink = 0; 

// Define mu_info_matrix
int mu_info_matrix[GRID_SIZE][GRID_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 8, 8, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 4, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7}
};

// Set state to MOVING_TO_SEARCH_AREA
int state = MOVING_TO_SEARCH_AREA;

// Test function
// Test function
void test_move_decider_MU1() {

  

  // Call move_decider_MU1
  Position nextMoveResult = move_decider_MU1(buPosition, currentPosition, targetPosition, obstacles, obstacleCount, mu_info_matrix, state, buLink);

  
  if(nextMoveResult.x != 0 || nextMoveResult.y != 0){

    // Checking first the BU connecition
    if(((buPosition.x-currentPosition.x)^2 + (buPosition.y-currentPosition.y)^2)<10){
      buLink = 1;

      Serial.println("**************BU LINK IS ESTABLISHED");
    }
    else{
      buLink = 0;
      Serial.println("**************NO LINK WITH THE BU");
    }  

    // Updating the info matrix
    if(currentPosition.x == targetPosition.x && currentPosition.y == targetPosition.y){

    Serial.println("Target has been found.");
    state = TARGET_FOUND;
      // Updating the info matrix
    mu_info_matrix[currentPosition.x-1][currentPosition.y - 1] = 6;

    }
    else{
    
      mu_info_matrix[currentPosition.x-1][currentPosition.y - 1] = 5;

    }

    currentPosition.x = currentPosition.x + nextMoveResult.x;
    currentPosition.y = currentPosition.y + nextMoveResult.y;
    mu_info_matrix[currentPosition.x-1][currentPosition.y - 1] = 1;

    // If our state is searching and we have not more unscaenned tiles left
    if(state == CURRENTLY_SEARCHING && !unscannedTileLeft(mu_info_matrix)){
      state = TARGET_NOT_FOUND;
    }

  }

  // Output the result
  Serial.print("Move made: (");
  Serial.print(nextMoveResult.x);
  Serial.print(", ");
  Serial.print(nextMoveResult.y);
  Serial.println(")");

  // Current State
  Serial.print("Current State: ");
  Serial.println(state);

  // Current Position
  // Output the result
  Serial.print("Current Position: (");
  Serial.print(currentPosition.x);
  Serial.print(", ");
  Serial.print(currentPosition.y);
  Serial.println(")");

  // Print out the mu_info_matrix
  Serial.println("mu_info_matrix:");
  for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {
          Serial.print(mu_info_matrix[i][j]);
          Serial.print("\t");
      }
      Serial.println();
  }
}


// Setup function
void setup() {

  //RFID pin setups
  pinMode(ISRESET_PIN, OUTPUT);
  digitalWrite(ISRESET_PIN, HIGH);
  delay(2000);
  digitalWrite(ISRESET_PIN, LOW);
  pinMode(TALK_PIN, OUTPUT);
  digitalWrite(TALK_PIN, LOW);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();




  // Initialize serial communication
  Serial.begin(9600);

  obstacles[0].x = 2;
  obstacles[0].y = 5;
  obstacles[1].x = 2;
  obstacles[1].y = 6;

  // Test the move_decider_MU1 function
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  // Gets in the perimiter of the BU
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  Serial.println(buLink);
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  // Receives the info on the target from another MU
  state = MOVING_TO_TARGET;
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();
  test_move_decider_MU1();

}

// Loop function
void loop() {

// Checking if we are reaading a new card  
if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
  
  // Assign the coordinates
  // currentPosition.x = RFID_position_result().x;
  // currentPosition.y = RFID_position_result().y;  
  digitalWrite(TALK_PIN, HIGH);
  
  //***** THIS IS FOR SENDING THE DECIDED MOVE
  // Serial.write(coded_coordinates);
  // delay(1000);
  // digitalWrite(TALK_PIN, LOW);

}
}
  //




