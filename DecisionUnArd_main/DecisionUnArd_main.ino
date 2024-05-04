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

Position targetPosition = {-1, -1};

// Should think about where to update these. Maybe just update the 5th and 
int mu_info_matrix[GRID_SIZE][GRID_SIZE];

void setup() {
  
  // Variables: (might declare these globally)
  // state
  // mu_info_matrix
  // obstacles
  // obstacleCount
  // currentPosition
  // buPosition
  // targetPosition --> Position targetPosition = {-1, -1};
  // collision_risk (depending on the o/p, we will communicate with our surroundings.)
  // move_allowed (depending on collision_risk this will either be direclty 1, or will be calculated in the collision_avoidance function.)

  // I should initialize the obstacles at each turn. Check if it gets updated in the functions.
  Position obstacles[MAX_OBSTACLES] = {{4, 5}, {7, 4}, {3, 6}};
  int obstacleCount = 3;
  
  // I will assume my info on these to be zero before each move
  bool isTarget;
  bool movement_allowed; // collision_avoidance o/p
  int MU2_state = 0;
  int MU3_state = 0;
  
  // I initialized these as 20 so that the differences in the collision_avoidance is large 
  Position MU2_position = {20, 20};
  Position MU3_position = {20, 20};
  Position nextMove; // This is not actually a position but just the next move.

  if (state != TARGET_REACHED){

  // Pass the adress of the currenPosition, it gets updated in the function, no need to have it as o/p
  isTarget = RFID_reader(&currentPosition);
  
  // Info matrix and target_info are updated.
  if(isTarget){
    targetPosition = currentPosition;
    mu_info_matrix[currentPosition.x - 1][currentPosition.y-1] = 6;

    // If during these states we have encountered the target, we change our state to TARGET_FOUND.
    if(state == CURRENTLY_SEARCHING || state == TARGET_NOT_FOUND || state == MOVING_TO_SEARCH_AREA){
      state == TARGET_FOUND;
    }
  }
  else{
    mu_info_matrix[currentPosition.x - 1][currentPosition.y-1] = 5;
  }

  
  // Share info on the target and your own position and state info.
  if(state == TARGET_FOUND || state == MOVING_TO_TARGET){
    // No return info is needed for the target_info except when informing the BU
    transmit_target_info(state, CurrentPosition, targetPosition);
  }
  else{
    transmit_general_info(state, CurrentPosition);
  }
  
  // Before making any move, communicate with the surroundings to see if there's any new info, 
  // and get the position & state info of the MUs in you perimiter
  [bu_link, bu_target_info, MU2_state, MU3_state, MU2_position, MU3_position] = listen_to_units();  

  // checking collision_avoidance to see if movement is allowed.
  movement_allowed = collision_avoidance(target_position, state, currentPosition, MU2_state, MU3_state, MU2_position, MU3_position, &obstacles, &obstacleCount);

  // If you are allowed to move
  if(movement_allowed){
    nextMove = move_decider_MU1(buPosition, currentPosition, targetPosition, obstacles, obstacleCount, mu_info_matrix);
  }
  // If you are not allowed to move just STAY
  else{
    nextMove.x = 0;
    nextMove.y = 0;
  }


  send_command_to_motion_unit(nextMove);
  }
  else{
    nextMove.x = 0;
    nextMove.y = 0;
  }

 }

void loop() {
  // Nothing to do here for this example
}