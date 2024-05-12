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

Position aimedPosition; // Will be input to the next_move_function
int distance_matrix[GRID_SIZE][GRID_SIZE];

for (int i = 0; i < GRID_SIZE; i++) {
    for (int j = 0; j < GRID_SIZE; j++) {

      // Calculating the distances (Note that the value we multiply may change if it isn't sufficient)
      distance_matrix[i][j] = abs(currentPosition.x - i) + abs(currentPosition.y - j) + 10*mu_info_matrix[i][j];
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

if((currentPosition.x + nextMoveResult.x == targetPosition.x) && (currentPosition.y + nextMoveResult.y == targetPosition.y)){
  state = TARGET_REACHED;
}

return nextMoveResult;

}

Position move_decider_MU1(Position buPosition, Position currentPosition, Position targetPosition, Position obstacles[], int obstacleCount,int mu_info_matrix[GRID_SIZE][GRID_SIZE],int& state, int bu_link){

Position nextMoveResult;


if(state == MOVING_TO_SEARCH_AREA){

  // Adding the BU to the obstacles
  obstacles[obstacleCount] = buPosition;
  obstacleCount = obstacleCount + 1;

  nextMoveResult = searcher(buPosition, currentPosition, obstacles, obstacleCount,mu_info_matrix);

  if((currentPosition.x + nextMoveResult.x < 4) && (currentPosition.x + nextMoveResult.x > 0)){
    // If when this move is made, we reach the target, the state update will be done by the RFID output. 
    // But for now we just know that we are now in our search area.
    state = CURRENTLY_SEARCHING; 
  }

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


