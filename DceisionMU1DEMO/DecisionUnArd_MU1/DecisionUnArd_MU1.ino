// TODO 1: The search should still continue if the RFID is missed,
// by remembering the last direction the MU was moving.
// TODO 2: BuLink should be updated in the comm section.


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
Position MU1_NextPosition = {15, 15};
Position MU2_NextPosition = {20, 20};
Position MU3_NextPosition = {20, 20};
uint8_t MU1_state = 0;
uint8_t MU2_state = 0;
uint8_t MU3_state = 0;
uint8_t ID = 1;

// COMM function variables
uint8_t loop_iteration_wo_ping_message;
uint8_t ping_time_out = 0;
uint8_t ping_timer = millis();
uint8_t active_s = 1;
bool waiting_for_target_info = 0;


// Define obstacles
Position obstacles[MAX_OBSTACLES];
int obstacleCount = 2;
int initial_obstacleCount = 2;

int buLink = 0; 
int ALERT_BU = 0;
// Define mu_info_matrix
int mu_info_matrix[GRID_SIZE][GRID_SIZE] = {
    {5, 5, 5, 5, 5, 5, 5, 5, 5},
    {5, 5, 5, 5, 5, 5, 5, 5, 5},
    {8, 8, 5, 5, 5, 5, 5, 5, 0},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 4, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7},
    {7, 7, 7, 7, 7, 7, 7, 7, 7}
};

// Set state to MOVING_TO_SEARCH_AREA
int state = MOVING_TO_SEARCH_AREA;

uint8_t move_decider_wo_comm() {

  uint8_t mu_move = 0;

  buLink = bu_link_checker(currentPosition, buPosition);

  if(state ==  TARGET_FOUND && buLink && (currentPosition.x == targetPosition.x && currentPosition.y == targetPosition.y) ){
    state = TARGET_REACHED;
    mu_move = 5;
    ALERT_BU = 1;

    return mu_move;
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
    waiting_for_target_info = 1;
  }

  if(state == MOVING_TO_TARGET && (currentPosition.x == targetPosition.x) && (currentPosition.y == targetPosition.y)){
    state = TARGET_REACHED;
    mu_move = 5;
    return mu_move;
  }

  if(state == MOVING_TO_SEARCH_AREA){
    if((currentPosition.x < 4) && (currentPosition.y > 0)){
      state = CURRENTLY_SEARCHING; 
    }
  }

  // Call move_decider_MU1
  Position nextMoveResult = move_decider_MU1(buPosition, currentPosition, targetPosition, obstacles, obstacleCount, mu_info_matrix, state, buLink);
  
  MU1_NextPosition.x = currentPosition.x + nextMoveResult.x;
  MU1_NextPosition.y = currentPosition.y + nextMoveResult.y;
  
 
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

    // Serial.println("First aimed Position x: " + String(currentPosition.x+nextMoveResult.x) + " y: " + String(currentPosition.y+nextMoveResult.y));


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

    // Serial.println("Next Position x: " + String(currentPosition.x+nextMoveResult.x) + " y: " + String(currentPosition.y+nextMoveResult.y));

    

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
  set_active_s(1);

  // Setting the obstacle positions
  obstacles[0].x = 3;
  obstacles[0].y = 1;
  obstacles[1].x = 3;
  obstacles[1].y = 2;
  // obstacles[2].x = 3;
  // obstacles[2].y = 4;
    
}



// void loop(){
//    MU1_state = state;


//   // Communication code 
//   final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);

// }



void loop() {

  
  obstacleCount = initial_obstacleCount;
  ALERT_BU = 0;

  uint8_t mu_command = 0;

  MU1_state = state;


  // Communication code 
  final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);
  bool movement_allowed = collision_avoidance_MU1(targetPosition, obstacles, obstacleCount, MU1_state, MU2_state, MU3_state, MU1_NextPosition, MU2_NextPosition, MU3_NextPosition);

  // bool movement_allowed = 1;

  // IF WE GET INFO ABOUT THE TARGET FROM OTHER 
  // UNITS STATE GETS UPDATED
  if(state<4){
    if(targetPosition.x<10 && targetPosition.x>0 ){
      state = MOVING_TO_TARGET;
    }
  }

  // Serial.println("MU2 Position: ");
  // Serial.println(MU2_NextPosition.x);
  // Serial.println(MU2_NextPosition.y);

  



  if(state == TARGET_REACHED){
    mu_command = 5; // search is over
    Serial.print('s');
    Serial.write(mu_command);
  }
  else{


    // Checking if we are reading a new card  
    if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

      
      // Assign the coordinates
      currentPosition = RFID_position_result();

      // Serial.println( "Read an RFID ");

      digitalWrite(LED_PIN, HIGH);
      digitalWrite(TALK_PIN, HIGH);
      // Serial.print('s');
      // Serial.write(5);
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
        // ,ğln("Failed to read the RFID card");
        mu_command = 0;
      }
      else{

        if(movement_allowed){
          
          mu_command = move_decider_wo_comm();

          // Serial.print("Mu command is: ");
          // Serial.println(mu_command);
          

          if(ALERT_BU){
          mu_command = mu_command + 5;
          }

          uint8_t coded_coordinates = mu_command*16 + mu_command;

          // Serial.write(coded_coordinates;
          // delay(500);
          digitalWrite(TALK_PIN, LOW);
          digitalWrite(LED_PIN, LOW);

          // Serial.print("Hey: ");
          // Serial.println(mu_command);
        }
        else{
          // we pause the decision unit for 3000ms
          mu_command = 0;
          uint8_t coded_coordinates = mu_command*16 + mu_command;

          // Serial.println("Enteres here");

          // Serial.write(coded_coordinates);
          // delay(500);
          digitalWrite(TALK_PIN, LOW);
          digitalWrite(LED_PIN, LOW);
          delay(2800);
        }
        
        if(MU2_state != TARGET_REACHED){
          MU2_NextPosition = {20, 20};
        }
        if(MU3_state != TARGET_REACHED){
          MU3_NextPosition = {20, 20};
        }     

      }
      // Serial.print("Current state: ");
      // Serial.println(state);

      // Serial.print("Curren pos: ");
      // Serial.print(currentPosition.x);
      // Serial.print(currentPosition.y);

      buLink = bu_link_checker(currentPosition, buPosition);
      if(waiting_for_target_info && buLink){
      
      Serial.print('s');
      Serial.write(10);

      while(waiting_for_target_info){
      // Communication code 
      final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);
      bool movement_allowed = collision_avoidance_MU1(targetPosition, obstacles, obstacleCount, MU1_state, MU2_state, MU3_state, MU1_NextPosition, MU2_NextPosition, MU3_NextPosition);

      if(targetPosition.x != -1){
        waiting_for_target_info = 0;
      }  
     
      }

      }

      else{
        Serial.print('s');
        Serial.write(mu_command); 
      }
       
    
    
    
    }
   
    rfid.PICC_HaltA(); // Halt PICC
    rfid.PCD_StopCrypto1(); // Stop encryption on PCD

  }

  // If target is found and BU connection is established
  if(ALERT_BU){
    unsigned long bu_data_send_start = millis();
    while(millis() - bu_data_send_start < 3000){
    for(int i = 0; i<8; i++){
      set_active_s(i);
      send_ping_message(MU1_NextPosition, targetPosition, ID, MU1_state);
    }
  }
  }

}