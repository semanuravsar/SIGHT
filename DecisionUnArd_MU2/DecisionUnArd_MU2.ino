//  The values info matrix represent the following;
//  mu1 is here == 1
//  mu2 is here == 2
//  mu3 is here == 3
//  BU is here == 4
//  This tile is scanned == 5
//  This tile has not been scanned == 0
//  The target is there == 6
//  mu is planning to go here == 7
//  obtacle here == 8

#include "DecisionUnArd_header.h";
#include "IR_module_header.h";

// RFID Includes
#include <SPI.h>
#include <MFRC522.h>
//RFID defines
#define RST_PIN 3
#define SS_PIN 10
#define TALK_PIN 2
MFRC522 rfid(SS_PIN, RST_PIN);

int LED_PIN = 5;


// Define positions
Position buPosition = {5, 5};
Position currentPosition = {-1, -1};
Position previousPosition = {-1, -1};
// Position unknownTargetPosition = {1, 9};
Position targetPosition = {-1,-1};
Position MU1_NextPosition = {20, 20};
Position MU2_NextPosition = {15, 15};
Position MU3_NextPosition = {20, 20};
uint8_t MU1_state = 0;
uint8_t MU2_state = 0;
uint8_t MU3_state = 0;
uint8_t ID = 2;

// COMM function variables
uint8_t loop_iteration_wo_ping_message = 0;
uint8_t ping_time_out = 0;
uint8_t ping_timer = millis();
uint8_t active_s = 1;


// Define obstacles
Position obstacles[MAX_OBSTACLES];
int obstacleCount = 2;
int initial_obstacleCount = 2;

int buLink = 0; 
int ALERT_BU = 0;
// Define mu_info_matrix
int mu_info_matrix[GRID_SIZE][GRID_SIZE] = {
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {8, 7, 7, 7, 8, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 4, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7}
};

// Set state to MOVING_TO_SEARCH_AREA
int state = MOVING_TO_SEARCH_AREA;

uint8_t move_decider_wo_comm() {

  Position aimedTargetPosition = {20, 20};

  if(state > 3){
    aimedTargetPosition = aimedTargetPositionCalculator_MU2(targetPosition);
  }

  uint8_t mu_move = 0;

  buLink = bu_link_checker(currentPosition, buPosition);

  // Serial.println("State when entering the move decider: ");
  // Serial.println(state);

  if(state ==  TARGET_FOUND && buLink && (currentPosition.x == aimedTargetPosition.x && currentPosition.y == aimedTargetPosition.y) ){
    state = TARGET_REACHED;
    mu_move = 5;
    // Serial.println("Girdi buraya");
    return mu_move;
    ALERT_BU = 1;
  }

  if(state == TARGET_FOUND && buLink){
    state = MOVING_TO_TARGET;
    ALERT_BU = 1;
  }

  // Updating the info matrix
  if(currentPosition.x == targetPosition.x && currentPosition.y == targetPosition.y){mu_info_matrix[currentPosition.x-1][currentPosition.y-1] = 6;}
  else{mu_info_matrix[currentPosition.x-1][currentPosition.y-1] = 5;}

  // If our state is searching and we have no more unscanned tiles left
  if(state == CURRENTLY_SEARCHING && !unscannedTileLeft(mu_info_matrix)){
    state = TARGET_NOT_FOUND;
  }

  if(state == MOVING_TO_TARGET && (currentPosition.x == aimedTargetPosition.x) && (currentPosition.y == aimedTargetPosition.y)){
    state = TARGET_REACHED;
    mu_move = 5;
    return mu_move;
  }

  if(state == MOVING_TO_SEARCH_AREA){
    if((currentPosition.x < 7) && (currentPosition.x > 3)){
      state = CURRENTLY_SEARCHING; 
    }
  }

  // Call move_decider_MU2
  Position nextMoveResult = move_decider_MU2(buPosition, currentPosition, targetPosition, obstacles, obstacleCount, mu_info_matrix, state, buLink);
  MU2_NextPosition.x = currentPosition.x + nextMoveResult.x;
  MU2_NextPosition.y = currentPosition.y + nextMoveResult.y;

  // 0 == STOP
  // 1 == FORWARDS
  // 2 == TURN RIGHT
  // 3 == TURN LEFT
  // 4 == TURN AROUND
  // 5 == TERMINATE SEARCH

  // If we are just starting the search
  if(previousPosition.x == -1){
 
    if(nextMoveResult.x == 1 && nextMoveResult.y == 0){mu_move = 1;}
    else if(nextMoveResult.x == -1 && nextMoveResult.y == 0){mu_move = 4;}
    else if(nextMoveResult.x == 0 && nextMoveResult.y == 1){mu_move = 3;}
    else if(nextMoveResult.x == 0 && nextMoveResult.y == -1){mu_move = 2;}
    else{mu_move = 0;}

  }
  else{
    Position orientation;
    orientation.x = currentPosition.x - previousPosition.x;
    orientation.y = currentPosition.y - previousPosition.y;

    //ORIENTATION = RIGHT
    if(orientation.x == 0 && orientation.y == 1){
      if(nextMoveResult.x == 1 && nextMoveResult.y == 0){mu_move = 2;}
      else if(nextMoveResult.x == -1 && nextMoveResult.y == 0){mu_move = 3;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == 1){mu_move = 1;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == -1){mu_move = 4;}
      else{mu_move = 0;}
    }
    //ORIENTATION = LEFT
    else if(orientation.x == 0 && orientation.y == -1){
      if(nextMoveResult.x == 1 && nextMoveResult.y == 0){mu_move = 3;}
      else if(nextMoveResult.x == -1 && nextMoveResult.y == 0){mu_move = 2;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == 1){mu_move = 4;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == -1){mu_move = 1;}
      else{mu_move = 0;}
    }
    //ORIENTATION = FORWARD (DEFAULT)
    else if(orientation.x == 1 && orientation.y == 0){
      if(nextMoveResult.x == 1 && nextMoveResult.y == 0){mu_move = 1;}
      else if(nextMoveResult.x == -1 && nextMoveResult.y == 0){mu_move = 4;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == 1){mu_move = 3;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == -1){mu_move = 2;}
      else{mu_move = 0;}
    }

    //ORIENTATION = BACKWARDS
    else if(orientation.x == -1 && orientation.y == 0){
      if(nextMoveResult.x == 1 && nextMoveResult.y == 0){mu_move = 4;}
      else if(nextMoveResult.x == -1 && nextMoveResult.y == 0){mu_move = 1;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == 1){mu_move = 2;}
      else if(nextMoveResult.x == 0 && nextMoveResult.y == -1){mu_move = 3;}
      else{mu_move = 0;}
    }
    else{mu_move = 0;}
    
    // Serial.println("First aimed Position x: " + String(currentPosition.x+nextMoveResult.x) + " y: " + String(currentPosition.y+nextMoveResult.y));

  }

  // Keeping the previous position
  previousPosition = currentPosition;

  return mu_move;

}


// Setup function
void setup() {

  //RFID pin setups
  pinMode(TALK_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(TALK_PIN, LOW);
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  // COMM pin setups
  initialize_IR_module();
  pinMode(SHIFT_REG_INPUT, OUTPUT);
  pinMode(SHIFT_REG_CLK_PIN, OUTPUT);
  digitalWrite(SHIFT_REG_CLK_PIN, HIGH);
  initialize_IR_module();
  set_active_s(2);

  // Setting the obstacle positions
  obstacles[0].x = 2;
  obstacles[0].y = 1;
  obstacles[1].x = 2;
  obstacles[1].y = 5;
  // obstacles[2].x = 3;
  // obstacles[2].y = 4;
    
}

void loop() {

obstacleCount = initial_obstacleCount;
ALERT_BU = 0;

uint8_t mu_command = 0;

MU2_state = state;
// Communication code 
final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);
bool movement_allowed = collision_avoidance_MU2(targetPosition, obstacles, obstacleCount, MU1_state, MU2_state, MU3_state, MU1_NextPosition, MU2_NextPosition, MU3_NextPosition);

// Serial.print("Move allowed: ");
// Serial.println(movement_allowed);

// movement_allowed = 1;


// IF WE GET INFO ABOUT THE TARGET FROM OTHER 
// UNITS STATE GETS UPDATED
if(state<4){
  if(targetPosition.x>0 && targetPosition.x<10){
    state = MOVING_TO_TARGET;
  }
}

if(state == TARGET_REACHED){
  mu_command = 5; // search is over
}

else{

  // Checking if we are reading a new card  
  if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {


    // Assign the coordinates
    currentPosition = RFID_position_result();

    // Serial.println("cuurent x: ");
    // Serial.print(currentPosition.x);
    // Serial.print(currentPosition.y);

    // Serial.println("LED should turn on");

    digitalWrite(LED_PIN, HIGH);
    digitalWrite(TALK_PIN, HIGH);
      
      
    // If current position is greater than 9, it means that that position is 
    // the target. By subtracting 9 we get the currentPosition info as well.
    if(currentPosition.x>9){
      if(state < 4){
        state = TARGET_FOUND;
      }
      currentPosition.x = currentPosition.x - 9;
      currentPosition.y = currentPosition.y - 9;
      targetPosition.x = currentPosition.x;
      targetPosition.y = currentPosition.y;
    }


    if(currentPosition.x == -1){
      // Serial.println("Failed to read the RFID card");
      mu_command = 0;
    }
    else{
      
      // ADD AN EXTRA COMMUNICATION PERIOD HERE FOR CHECKING THE SURROUNDINGS
      // ADD THE CORRESPONDING DELAY TO LINE 74 OF THE MOTION UNIT CODE
      // unsigned long communication_check_start = millis();
      
      // while(millis()-communication_check_start < 2000){
      //   // Communication code 
      //   final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);
      //   bool movement_allowed = collision_avoidance_MU2(targetPosition, obstacles, obstacleCount, MU1_state, MU2_state, MU3_state, MU1_NextPosition, MU2_NextPosition, MU3_NextPosition);

      // }

        // movement_allowed = 1;

      if(movement_allowed){



        mu_command = move_decider_wo_comm();

        // Serial.print("mu is commaned: ");
        // Serial.println(mu_command);

        if(ALERT_BU){
        mu_command = mu_command + 5;
        }

        uint8_t coded_coordinates = mu_command*16 + mu_command;

        // Serial.println("LED PIN and TALK PIN high");
        Serial.write(coded_coordinates);
        delay(500);
        digitalWrite(TALK_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
      }
      else{
        // we pause the decision unit for 3000ms
        mu_command = 0;
        uint8_t coded_coordinates = mu_command*16 + mu_command;

        // Serial.print("Command: ");
        // Serial.println(mu_command);
        // Serial.print("state is: ");
        // Serial.println(state);

        // Serial.println("LED PIN and TALK PIN high");
        Serial.write(coded_coordinates);
        delay(500);
        digitalWrite(TALK_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        delay(3000);


      }
      
      if(MU1_state != TARGET_REACHED){
        Position MU1_NextPosition = {20, 20};
      }
      if(MU3_state != TARGET_REACHED){
        Position MU3_NextPosition = {20, 20};
      }     

    }
  }


  //Serial.println("Place the card to the reader...");
  rfid.PICC_HaltA(); // Halt PICC
  rfid.PCD_StopCrypto1(); // Stop encryption on PCD

}

// If target is found and BU connection is established
if(ALERT_BU){
  unsigned long bu_data_send_start = millis();
  while(millis() - bu_data_send_start < 3000){
  for(int i = 0; i<8; i++){
    set_active_s(i);
    send_ping_message(MU2_NextPosition, targetPosition, ID, MU2_state);
  }
 }
}

}