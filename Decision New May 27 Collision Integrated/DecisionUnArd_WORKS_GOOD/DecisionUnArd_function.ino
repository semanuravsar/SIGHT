// Custom map structure to hold visited nodes and their parents
struct CustomMap {
  Position keys[GRID_SIZE * GRID_SIZE]; // Store positions (keys)
  Position values[GRID_SIZE * GRID_SIZE]; // Store parent positions (values)
  int size; // Size of the map

  // Constructor
  CustomMap() {
    size = 0;
  }

  // Function to check if the map contains a key
  bool contains(Position key) {
    for (int i = 0; i < size; i++) {
      if (keys[i].x == key.x && keys[i].y == key.y) {
        return true;
      }
    }
    return false;
  }

  // Function to insert a key-value pair into the map
  void insert(Position key, Position value) {
    keys[size] = key;
    values[size] = value;
    size++;
  }

  // Function to retrieve the parent of a key
  Position getParent(Position key) {
    for (int i = 0; i < size; i++) {
      if (keys[i].x == key.x && keys[i].y == key.y) {
        return values[i];
      }
    }
    // If key not found, return {0, 0}
    return {0, 0};
  }
};

// Function to check if a position is an obstacle
bool isObstacle(Position position, Position obstacles[], int obstacleCount) {
  for (int i = 0; i < obstacleCount; i++) {
    if (position.x == obstacles[i].x && position.y == obstacles[i].y) {
      return true;
    }
  }
  return false;
}

// Function to find the next move
Position nextMove(Position buPosition, Position currentPosition, Position aimedPosition, Position obstacles[], int obstacleCount) {
  // Initialize queue for BFS
  Position queue[GRID_SIZE * GRID_SIZE];
  CustomMap visited;

  // Add starting position to queue
  int rear = 0;
  queue[rear++] = currentPosition;

  // Perform BFS
  while (rear > 0) {
    // Dequeue
    Position current = queue[0];
    for (int i = 0; i < rear - 1; i++) {
      queue[i] = queue[i + 1];
    }
    rear--;

    // Check if current position is the target
    if (current.x == aimedPosition.x && current.y == aimedPosition.y) {
      // Reconstruct path
      Position currentPath = current;
      Position path[MAX_PATH_LENGTH];
      int pathLength = 0;
      while (!(currentPath.x == currentPosition.x && currentPath.y == currentPosition.y)) {
        path[pathLength++] = currentPath;
        currentPath = visited.getParent(currentPath);

        // For debugging purposes
        // Serial.print("\n Path in reverse  ");
        // Serial.print(currentPath.x);
        // Serial.print(",");
        // Serial.print(currentPath.y);

      }

      // Return the next move
      return {path[pathLength-1].x - currentPosition.x, path[pathLength-1].y - currentPosition.y};
    
  }

    // Generate neighbors (up, down, left, right)
    Position neighbors[] = {
      {current.x - 1, current.y},
      {current.x + 1, current.y},
      {current.x, current.y - 1},
      {current.x, current.y + 1}
    };

    // Add valid neighbors to queue
    for (int i = 0; i < 4; i++) {
      Position neighbor = neighbors[i];
      // Check if the neighbor is within the grid bounds
      if (neighbor.x >= 1 && neighbor.x <= GRID_SIZE && neighbor.y >= 1 && neighbor.y <= GRID_SIZE) {
        if (!isObstacle(neighbor, obstacles, obstacleCount) && !visited.contains(neighbor)) {
          queue[rear++] = neighbor;
          visited.insert(neighbor, current);
        }
      }
    }
  }
  //******************************************************************
  //***ADD CHECK FOR AIMING AT AN MU & MAKE THEM MOVE TO BU INSTEAD***
  //*********THIS WILL BE ADDED AFTER MOVE_TO_BU IS FINISHED**********  
  //******************************************************************
  


  // No path found
  return {0, 0};
}

//******************************************************************
//************************SEARCHER FUNCTION*************************
//******************************************************************

// GRID_SIZE is a global variable, no need to have it as input.

Position searcher(Position buPosition, Position currentPosition, Position obstacles[], int obstacleCount, int mu_info_matrix[GRID_SIZE][GRID_SIZE]) {
    

  for(int i = 0; i<obstacleCount; i++){
    mu_info_matrix[obstacles[i].x-1][obstacles[i].y-1] = 8;
  }
 

  Position aimedPosition; // Will be input to the next_move_function
  int distance_matrix[GRID_SIZE][GRID_SIZE];

  for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {

        // Calculating the distances (Note that the value we multiply may change if it isn't sufficient)
        distance_matrix[i][j] = abs(currentPosition.x - i - 1) + abs(currentPosition.y - j - 1) + 10*mu_info_matrix[i][j];
      }
    }
    
    int min_value = distance_matrix[0][0]; // Assuming the first element is the min value
    int min_row = 0;      // Initialize the index of the minimum value
    int min_column = 0;

    for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {

        if(distance_matrix[i][j]<min_value){
          
          min_value = distance_matrix[i][j];
          min_row = i;
          min_column = j;

        }      
      }
    }

    aimedPosition.x = min_row + 1; // Since our positions start from 1 but indexes start from 0 we add a 1.
    aimedPosition.y = min_column + 1;

    
    Position nextMoveResult = nextMove(buPosition, currentPosition, aimedPosition, obstacles, obstacleCount);
    

  return nextMoveResult;
}

//******************************************************************
//***********************MOVE TO BU FUNCTION************************
//******************************************************************

Position move_to_BU(Position buPosition, Position currentPosition, Position obstacles[], int obstacleCount){

 Position nextMoveResult = nextMove(buPosition, currentPosition, buPosition, obstacles, obstacleCount);

 return nextMoveResult;
}

//******************************************************************
//*********************MOVE TO TARGET_1 FUNCTION********************
//******************************************************************

// State of the MU should be adjusted in the route planner function, not in the indvidual movement functions.
// Also after getting the next position info, we should check if current+move == target, and change state accordingly.
// Of course depending on the MU the position we check will differ. 
// You might wanna take the state out of this and adjust it after the result

Position move_to_target_MU1(Position buPosition, Position currentPosition, Position targetPosition, Position obstacles[], int obstacleCount, int& state){

  Position aimedPosition = targetPosition;

  // If the target is in any of the corners, the position might be most probably blocked 
  // by other MUs so instead of moving direclt on to the target, just go the the diagonal position.
  if(aimedPosition.x == 1 && aimedPosition.y == 1){
    aimedPosition.x = 2;
    aimedPosition.y = 2;
  }
  if(aimedPosition.x == 1 && aimedPosition.y == 9){
    aimedPosition.x = 2;
    aimedPosition.y = 8;
  }

  if(aimedPosition.x == 9 && aimedPosition.y == 1){
    aimedPosition.x = 8;
    aimedPosition.y = 2;
  }

  if(aimedPosition.x == 9 && aimedPosition.y == 9){
    aimedPosition.x = 8;
    aimedPosition.y = 8;
  }

  Position nextMoveResult = nextMove(buPosition, currentPosition, aimedPosition, obstacles, obstacleCount);

  return nextMoveResult;

  }

  Position move_decider_MU1(Position buPosition, Position currentPosition, Position targetPosition, Position obstacles[], int obstacleCount,int mu_info_matrix[GRID_SIZE][GRID_SIZE],int& state, int bu_link){

  Position nextMoveResult;


  if(state == MOVING_TO_SEARCH_AREA){

    // Adding the BU to the obstacles
    obstacles[obstacleCount] = buPosition;
    obstacleCount = obstacleCount + 1;

    nextMoveResult = searcher(buPosition, currentPosition, obstacles, obstacleCount,mu_info_matrix);

  }

  // Somewhere I should check (either in the main or at the end of here) if while I am in the state of searching, whether
  // my info matrix has ay zeros left or not. If I have no zeros left, then my state should change to TARGET_NOT_FOUND
  else if(state == CURRENTLY_SEARCHING){

    
    // Adding the BU to the obstacles
    obstacles[obstacleCount] = buPosition;
    obstacles[obstacleCount + 1].x = 4;
    // We add the tile in the other MUs regions that we might move towards into our obstacles to prevent trespassing.
    obstacles[obstacleCount + 1].y = currentPosition.y;
    obstacleCount = obstacleCount + 2;

    nextMoveResult = searcher(buPosition, currentPosition, obstacles, obstacleCount, mu_info_matrix);

  }

  // If target is NOT found move to the BU
  else if(state == TARGET_NOT_FOUND){

    // If we have a connection with the BU stay in place
    if(bu_link == 1){
    
    nextMoveResult;
    nextMoveResult.x = 0;
    nextMoveResult.y = 0;
      
    }
    // Move to the BU
    else{
      nextMoveResult = move_to_BU(buPosition, currentPosition, obstacles, obstacleCount);
    }
  }

  else if(state == TARGET_FOUND){
    
    // The job of the finding MU is to move to the BU to inform it. Again having a connection is enough.
    // If we have a connection with move to the target
    if(bu_link == 1){
    
    state = MOVING_TO_TARGET;

    // Adding the BU to the obstacles
    obstacles[obstacleCount] = buPosition;
    obstacleCount = obstacleCount + 1;

    nextMoveResult = move_to_target_MU1(buPosition, currentPosition, targetPosition, obstacles, obstacleCount, state);
    }
    // Move to the BU
    else{
      nextMoveResult = move_to_BU(buPosition, currentPosition, obstacles, obstacleCount);
    }

  }

  else if(state == MOVING_TO_TARGET){
    // Adding the BU to the obstacles
    obstacles[obstacleCount] = buPosition;
    obstacleCount = obstacleCount + 1;

    nextMoveResult = move_to_target_MU1(buPosition, currentPosition, targetPosition, obstacles, obstacleCount, state);
  }

  // This may be redundant.
  else if(state == TARGET_REACHED){

    nextMoveResult;
    nextMoveResult.x = 0;
    nextMoveResult.y = 0;

  }

  return nextMoveResult;
  // State should be adjusted after RFID reading. It either stay the same or we switch to TARGET_FOUND

}

// Function to check if all elements in the matrix are non-zero
bool unscannedTileLeft(const int matrix[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (matrix[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}

Position RFID_position_result(){

  Position position_result;
  // If we had failure to read then return -1,-1 to incdicate mistake
  position_result.x = -1;
  position_result.y = -1; 

  byte block = 14;
  byte buffer[18];
  byte size = sizeof(buffer);
  MFRC522::MIFARE_Key key;

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  if (rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &rfid.uid) != MFRC522::STATUS_OK) {
    // Serial.println("Authentication failed");
    return position_result;
  }

  if (rfid.MIFARE_Read(block, buffer, &size) != MFRC522::STATUS_OK) {
    // Serial.println("Read failed");
    return position_result;
  } else {
    position_result.x = buffer[0];
    position_result.y = buffer[1];
    
    return position_result;  // Return 0 if coordinates don't match any predefined pairs
  }

}

int bu_link_checker(Position currentPosition, Position buPosition){
// Checking first the BU connecition
      
      int buLink_check;

      int distance = ((buPosition.x-currentPosition.x)*(buPosition.x-currentPosition.x) + (buPosition.y-currentPosition.y)*(buPosition.y-currentPosition.y));

      // Serial.println("dsiatcne is: " + String(distance));

      if(((buPosition.x-currentPosition.x)*(buPosition.x-currentPosition.x) + (buPosition.y-currentPosition.y)*(buPosition.y-currentPosition.y))<10){
        buLink_check = 1;

        // Serial.println("BU L");
      }
      else{
        buLink_check = 0;
        // Serial.println("No BU L");
      }  

      return buLink_check;
}

bool collision_avoidance_MU1(Position targetPosition, Position obstacles[], int& obstacleCount, int MU1_state, int MU2_state, int MU3_state, Position MU1_NextPosition, Position MU2_NextPosition,Position MU3_NextPosition){
  
  int distance_to_MU2 = abs(MU2_NextPosition.x - MU1_NextPosition.x) + abs(MU2_NextPosition.y - MU1_NextPosition.y);
  int distance_to_MU3 = abs(MU3_NextPosition.x - MU1_NextPosition.x) + abs(MU3_NextPosition.y - MU1_NextPosition.y);
  bool movement_allowed = 0;
  bool movement_allowed_wrt_2 = 1;
  bool movement_allowed_wrt_3 = 1;

  if(MU1_state == 1 && MU1_NextPosition.x == 3){
    MU1_state = 2;
  }

  if(MU2_state == 1 && MU2_NextPosition.x == 4){
    MU2_state = 2;
  }

  
  if(MU3_state == 1 && MU3_NextPosition.x == 7){
    MU3_state = 2;
  }
  

  if(MU1_state == TARGET_REACHED){
    movement_allowed_wrt_2 = 0;
    movement_allowed_wrt_3 = 0;
  }
  else{
    // CHECK COLLISION W/ MU2
    if(distance_to_MU2 <= 2){

      if(MU2_state == TARGET_REACHED){
        
        movement_allowed_wrt_2 = 1;
        obstacles[obstacleCount].x = MU2_NextPosition.x;
        obstacles[obstacleCount].y = MU2_NextPosition.y;  
        obstacleCount++;
      }
      else{
        // MU1 is in its own search area
        if(0<MU1_NextPosition.x && MU1_NextPosition.x<4){
          if(MU1_state == CURRENTLY_SEARCHING){
            if(MU2_state == CURRENTLY_SEARCHING){
              
              movement_allowed_wrt_2 = 1;
              obstacles[obstacleCount].x = MU2_NextPosition.x;
              obstacles[obstacleCount].y = MU2_NextPosition.y;  
              obstacleCount++;

            }
            else{
              if(MU2_state>2){

                movement_allowed_wrt_2 = 1;
                obstacles[obstacleCount].x = MU2_NextPosition.x;
                obstacles[obstacleCount].y = MU2_NextPosition.y;  
                obstacleCount++;

              }
              else{movement_allowed_wrt_2 = 0;
                // Serial.println("Check 1 ");
}
            }
          }
          else{
            movement_allowed_wrt_2 = 1;
            obstacles[obstacleCount].x = MU2_NextPosition.x;
            obstacles[obstacleCount].y = MU2_NextPosition.y;  
            obstacleCount++;
          }
        }
        else{
          if(MU2_state>2){
            movement_allowed_wrt_2 = 1;
            obstacles[obstacleCount].x = MU2_NextPosition.x;
            obstacles[obstacleCount].y = MU2_NextPosition.y;  
            obstacleCount++;
          }
          else{
            if(MU1_state>2){
              movement_allowed_wrt_2 = 0;
              // Serial.println("Check 2 ");
            }
            else{
              movement_allowed_wrt_2 = 1;
              obstacles[obstacleCount].x = MU2_NextPosition.x;
              obstacles[obstacleCount].y = MU2_NextPosition.y;  
              obstacleCount++;
            }
          }
        }
      }
      
      // If MU2 is at the target's position, we wait for it to clear our position first
      if(MU2_NextPosition.x == targetPosition.x && MU2_NextPosition.y == targetPosition.y){
        movement_allowed_wrt_2 = 0;
        // Serial.println("Check 3 ");
      }
    } 
    else{
      
      movement_allowed_wrt_2 = 1;

      // OBSTACLE POSITION UPDATE
      if(MU2_state == TARGET_REACHED){
        movement_allowed_wrt_2 = 1;
        obstacles[obstacleCount].x = MU2_NextPosition.x;
        obstacles[obstacleCount].y = MU2_NextPosition.y;  
        obstacleCount++;

      }
    }

    
    // CHECK COLLISION W/ MU3
    if(distance_to_MU3 <= 2){
      
      if(MU3_state == TARGET_REACHED){
        
        movement_allowed_wrt_3 = 1;
        obstacles[obstacleCount].x = MU3_NextPosition.x;
        obstacles[obstacleCount].y = MU3_NextPosition.y;  
        obstacleCount++;
      }
      else{
        if(0<MU1_NextPosition.x && MU1_NextPosition.x<4 && MU1_state == CURRENTLY_SEARCHING){
          if(MU3_state>2){
            movement_allowed_wrt_3 = 1;
            obstacles[obstacleCount].x = MU3_NextPosition.x;
            obstacles[obstacleCount].y = MU3_NextPosition.y;  
            obstacleCount++;
          }
          else{movement_allowed_wrt_3 = 1;}
        }
        else{
          if(MU1_state>2){
            if(MU3_state>2){
              movement_allowed_wrt_3 = 1;
              obstacles[obstacleCount].x = MU3_NextPosition.x;
              obstacles[obstacleCount].y = MU3_NextPosition.y;  
              obstacleCount++;
            }
            else{movement_allowed_wrt_3 = 1;}
          }
          else{
            movement_allowed_wrt_3 = 1;
            obstacles[obstacleCount].x = MU3_NextPosition.x;
            obstacles[obstacleCount].y = MU3_NextPosition.y;  
            obstacleCount++;
          }
        }
      }

      // If MU3 is at the target's position, we wait for it to clear our position first
      if(MU3_NextPosition.x == targetPosition.x && MU3_NextPosition.y == targetPosition.y){
        movement_allowed_wrt_3 = 0;
      }
    }
    
    else{
      movement_allowed_wrt_3 = 1;
      // OBSTACLE POSITION UPDATE
      if(MU3_state == TARGET_REACHED){
        movement_allowed_wrt_3 = 1;
        obstacles[obstacleCount].x = MU3_NextPosition.x;
        obstacles[obstacleCount].y = MU3_NextPosition.y;  
        obstacleCount++;
      }

    }
  }

  movement_allowed_wrt_3 = 1;

  // Serial.print("Move allowed wrt 2: ");
  // Serial.println(movement_allowed_wrt_2);
  // Serial.println("MU1 next: ");
  // Serial.print(MU1_NextPosition.x);
  // Serial.println(MU1_NextPosition.y);
  // Serial.println("MU2: ");
  // Serial.println(MU2_NextPosition.x);
  // Serial.println(MU2_NextPosition.y);
  // Serial.println("MU1 state: ");
  // Serial.println(MU1_state);
  // Serial.println("MU2 state: ");
  // Serial.println(MU2_state);
  
  return movement_allowed = movement_allowed_wrt_2 && movement_allowed_wrt_3;
  // MIGHT ADD THE CASE WHERE I ADD THEM AS OBSTACLE POSTIONS ANYWAYS.


}

