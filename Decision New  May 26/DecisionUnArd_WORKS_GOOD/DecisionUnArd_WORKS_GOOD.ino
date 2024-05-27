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
uint8_t active_s = 1;


// Define positions
Position buPosition = {5, 5};
Position currentPosition = {-1, -1};
Position previousPosition = {-1, -1};
// Position unknownTargetPosition = {1, 9};
Position targetPosition = {10,10};

// Next pos to be used in the comm protocol
Position MU1_NextPosition = {15, 15};
Position MU2_NextPosition = {10, 10};
Position MU3_NextPosition = {10, 10};

// States to be used in the comm protocol
uint8_t MU1_state = 0;
uint8_t MU2_state = 0;
uint8_t MU3_state = 0;
uint8_t ID = 1;


// Define obstacles
Position obstacles[MAX_OBSTACLES];
int obstacleCount = 2;

int buLink = 0; 
int ALERT_BU = 0;
// Define mu_info_matrix
int mu_info_matrix[GRID_SIZE][GRID_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 8, 0, 0, 0, 0},
    {0, 8, 0, 0, 0, 0, 0, 0, 0},
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

  // Serial.println("State when entering the move decider: ");
  // Serial.println(state);

  if(state ==  TARGET_FOUND && buLink && (currentPosition.x == targetPosition.x && currentPosition.y == targetPosition.y) ){
    state = TARGET_REACHED;
    mu_move = 5;
    // Serial.println("Girdi buraya");
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
  // Serial.println("Next Position will be: ");
  // Serial.println(currentPosition.x+nextMoveResult.x);
  // Serial.println(currentPosition.y+nextMoveResult.y);

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

    // AŞAĞI DOPĞRU GİDERKEN SAĞA DÖNCEĞİNE GERİ GİDİYO
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
  obstacles[0].x = 3;
  obstacles[0].y = 2;
  obstacles[1].x = 2;
  obstacles[1].y = 5;
  // obstacles[2].x = 3;
  // obstacles[2].y = 4;
}

void loop() {

MU1_state = state;

final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);

uint8_t mu_command = 13;
ALERT_BU = 0;


if(state < 3){
  if(targetPosition.x <10 && targetPosition.x > 0){
    state = MOVING_TO_TARGET;
  }
}


if(state == TARGET_REACHED){

  digitalWrite(TALK_PIN, HIGH);

  mu_command = 5; // search is over

  // Serial.println("LED PIN and TALK PIN high");
  Serial.write(mu_command);
  delay(500);
  digitalWrite(TALK_PIN, LOW);

}

else{
  // Checking if we are reading a new card  
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {

    // Assign the coordinates
    currentPosition = RFID_position_result();

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
      mu_command = 13;
    }
    else{

      buLink = bu_link_checker(currentPosition, buPosition);

      if(state == TARGET_NOT_FOUND && buLink){

        // Serial.println("Loopa giriyo.");

        digitalWrite(TALK_PIN, HIGH);

        mu_command = 13; // search is halted

        // Serial.println("LED PIN and TALK PIN high");
        Serial.write(mu_command);
        delay(500);
        digitalWrite(TALK_PIN, LOW);
        digitalWrite(LED_PIN, LOW);

        while(state == TARGET_NOT_FOUND){
          // NEW ADDITION
          //communicate
          final_communication(MU1_NextPosition, MU2_NextPosition, MU3_NextPosition, targetPosition, ID, MU1_state, MU2_state, MU3_state, buLink, active_s);
          if(targetPosition.x >0 && targetPosition.x<10){
            state = MOVING_TO_TARGET;
            mu_command = move_decider_wo_comm();
            
            digitalWrite(TALK_PIN, HIGH);
            Serial.write(mu_command);
            delay(500);
            digitalWrite(TALK_PIN, LOW);
            digitalWrite(LED_PIN, LOW);

            // Serial.print("Command after comm with BU: ");
            // Serial.println(mu_command);

            break;

          }
        }
      }
      else{
        // RFID intergrated, no comm
        mu_command = move_decider_wo_comm();

        if(ALERT_BU){
          mu_command = mu_command + 5;
        }

        Serial.write(mu_command);
        delay(500);
        digitalWrite(TALK_PIN, LOW);
        digitalWrite(LED_PIN, LOW);

        // Serial.print("Normal command: ");
        // Serial.println(mu_command);
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
  while(millis() - bu_data_send_start < 5000 ){
  for(int i = 0; i<8; i++){
    set_active_s(i);
    send_ping_message(MU1_NextPosition, targetPosition, ID, state);  }
  }
}

}