#ifndef DECISIONUNARD_H
#define DECISIONUNARD_H

// Define maximum dimensions for the grid
#define GRID_SIZE 9
#define MAX_OBSTACLES 15
#define MAX_PATH_LENGTH 50

// Here we define states
#define MOVING_TO_SEARCH_AREA 1// moves to its own search area to begin search
#define CURRENTLY_SEARCHING 2// is in its own area, and currently searching
#define TARGET_NOT_FOUND 3 // finished searching, did not find the target, will move to BU in this case
#define TARGET_FOUND 4 // found the target during my search, moving to BU
#define MOVING_TO_TARGET 5 // moving to target
#define TARGET_REACHED 6 // found the target

// Define the structure for a 2D position
struct Position {
  int x;
  int y;
};

// Function declarations
Position nextMove(Position buPosition, Position currentPosition, Position targetPosition, Position obstacles[], int obstacleCount);
Position move_to_BU(Position buPosition, Position currentPosition, Position obstacles[], int obstacleCount);
Position searcher(Position buPosition, Position currentPosition, Position obstacles[], int obstacleCount, int mu_info_matrix[GRID_SIZE][GRID_SIZE]);
Position move_to_target_MU1(Position buPosition, Position currentPosition, Position targetPosition, Position obstacles[], int obstacleCount, int& state);
Position move_decider_MU1(Position buPosition, Position currentPosition, Position targetPosition, Position obstacles[], int obstacleCount,int mu_info_matrix[GRID_SIZE][GRID_SIZE],int& state,int bu_link);
bool isObstacle(Position position, Position obstacles[], int obstacleCount);
bool unscannedTileLeft(const int matrix[GRID_SIZE][GRID_SIZE]);
Position RFID_position_result();
void clearSerialBuffer();

// bool isTarget = RFID_reader(Position& currentPosition);
// bool collision_risk = Ultrasonic_Collision_Check();


#endif
