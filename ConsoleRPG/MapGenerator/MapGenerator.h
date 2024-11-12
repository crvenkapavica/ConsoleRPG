#pragma once

#include "../GameplayStatics.h" 
#include "../RPGTypes.h"

class Character;
class EnemyCharacter;
class PlayerCharacter;

struct DistanceNode {
	int X;
	int Y;
	int Distance;
};

struct GridNode {
	std::weak_ptr<Character> Here;
	std::weak_ptr<Character> Neighbors[8];
};

struct PathNode {
	PathNode()
	: Right(false)
	, Left(false)
	, Down(false)
	, Up(false)
	{}

	bool& operator[](const int& Index) {
		switch (Index) {
		case 0: return Right;
		case 1: return Left;
		case 2: return Down;
		case 3: return Up;
		default: return Right;
		}
	}

	bool operator[](const int& Index) const {
		switch (Index) {
		case 0: return Right == true;
		case 1: return Left == true;
		case 2: return Down == true;
		case 3: return Up == true;
		default: return false;
		}
	}

	// Corresponds to dX and dY directions
	bool Right;
	bool Left;
	bool Down;
	bool Up;
};

// TODO :: MAKE STATIC FUNCTIONS 
class MapGenerator final {

public:
	MapGenerator() = default;
	MapGenerator(const MapGenerator&) = delete;
	MapGenerator(MapGenerator&&) = delete;
	~MapGenerator() = default;
	
	MapGenerator& operator=(const MapGenerator&) = delete;
	MapGenerator& operator=(MapGenerator&&) = delete;
	
	static MapGenerator& GetInstance();
	void Initialize(const std::vector<std::weak_ptr<Character>>& InPlayerCharacters);
	
	// Shows the current player position on the map with radius of light radius.
	void ShowPosition() const;
	// Shows the portion of the map the player discovered.
	void ShowMap() const;
	// Handle keypress for direction of movement.
	void HandleMovement();

	// DEBUG
	/////////////////////////////////
	void PrintDebugMap() const;
	void PrintError() const;
	/////////////////////////////////
	
	// Display play grid.
	void DisplayPlayGrid() const;
	// Move Character on char and play grids.
	void MoveCharacterOnGrid(const Character& InCharacter, const EDirection Direction);
	// Adds Character to char grid and returns true if successful.
	bool AddCharacterToCharGrid(const std::shared_ptr<Character>& Instigator, const std::weak_ptr<Character>& Summon);

	int GetEnemyIndex(const char Alias);
	int GetPlayerIndex(const char Alias);
	void KillEnemy(const int Index);
	void KillEnemy(const Character* InCharacter);

public:
	std::weak_ptr<Character> GetCharacterFromAlias(const char Target);
	// Get combat direction for InCharacter and save
	std::vector<std::string> GetCombatDirections(const Character* InCharacter, OUT std::map<int, EDirection>& InMap) const;
	std::vector<Character*> GetCharactersInRange(const Character* InCharacter);
	// Get monster power level at engaged tile.
	[[nodiscard]] inline int GetPowerLevel() const { return PowerLevels[EnemyIndex]; }

private:
	// BFS that generates map
	void BFS(int X, int Y, const int Step);
	void InitBFS();
	void InitializeEmptyMap() const;
	
	int GetCurrentMoveWidth();
	float GetMapDensity();
	int GetNumberOfMoves();
	int GetDirection(const int X, const int Y);
	[[nodiscard]] int GetReverseDirection() const;
	int GetRandomTurnDirection(OUT int& X, OUT int& Y) const;
	int GetVisitedNodeDirection(const PathNode& VisitedNode, const int X, const int Y);
	void DisableAdjacent(const int X, const int Y);
	void DisableLocation(const int X, const int Y) const;

	// Makes a random rectangle segment.
	void GetRandomRectangle(const int X, const int Y) const;
	// Initialize enemies on engaged tile.
	void InitEnemies();
	// Spawns enemies on the map and decides their power level
	void AddRandomMapEnemies();

	void MakeDebugMessage(const int InSteps, const std::string& Func);

	// PLAYER SPECIFIC
	// -------------------------------------------------------------------

	void InitPlayer(const std::vector<std::weak_ptr<Character>>& InPlayerCharacters);
	
	void GetPlayerStartPosition(int& X, int& Y) const;
	
	// Get Enemy Characters at the engaged tile.
	std::vector<std::weak_ptr<Character>> GetEnemies(int X, int Y);

	// Calculate distance to each node.
	void BFS_Distance(const int X, const int Y, const int Step);
	// Initialize "Step" distance starting from XY.
	void InitDistanceBFS(const int X, const int Y, const int Step);
	
	// Move after input.
	void Move(const int MoveDir);
	
	// Used to display the discovered portion of the map
	void ExtendMapBorders(const int Radius);
	// Draw discovered map.
	void DrawMap(int Xs, int Xe, int Ys, int Ye) const;
	// Gets the color of a specific unit.
	const char* GetMapAnsi(const char C) const;
	
	void DisplayErrorMessage(const std::string& Message);

	// GRID SPECIFIC
	// -------------------------------------------------------------------
	
	// Draws the GRID_X * GRID_Y play grid.
	void DrawPlayGrid();
	// Places each enemy on the [currently] random square on the play grid.
	void GenerateCharacterGridPositions();
	// Adds characters to the play grid.
	void AddCharactersToGrid();
	// Update internal char_grid's neighbours.
	void UpdateCharGrid();
	// Clear the whole Character grid.
	void ClearCharGrid();
	// -------------------------------------------------------------------
	
private:
	std::vector<EnemyCharacter*> Enemies;
	std::vector<std::weak_ptr<PlayerCharacter>> PlayerCharacters;

	char** Map;
	PathNode** Nodes;
	int** Steps;
	int** Distance;

	std::vector<int> Distances;
	std::vector<std::string> Error;
	std::vector<std::pair<int, int>> Turn;
	
	int Dir;
	int Moves;
	int StepLimit;
	
	int DX[4] = { 0, 0, 1, -1 };
	int DY[4] = { 1, -1, 0, 0 };

	int TotalSteps = 0;
	int Width = 0;

	char Axis;
	int Random1;
	int Random2;
	int RandomBoth;
	int AxisSides;

	int PlayerX;
	int PlayerY;
	int BorderX = 0, BorderXEnd = 0;
	int BorderY = 0, BorderYEnd = 0;
	int EnemyIndex;
	
	std::string ErrorMessage;
	std::vector<DistanceNode> DistancesNodes;
	std::vector<int> PowerLevels;
	std::vector<std::vector<std::shared_ptr<Character>>> EnemyMap;   // this two vectors are aligned
	std::vector<std::pair<int, int>> EnemyMapXY;					 // this two vectors are aligned
	std::vector<std::map<char, std::weak_ptr<Character>>> EnemyNameMap;
	
	// GRID SPECIFIC
	// ------------------------------------------------------------------
	char Grid[21][81];
	GridNode CharGrid[5][10];
	std::unordered_map<char, std::pair<int, int>> CharMap;

	std::vector<std::pair<int, int>> EnemyStartPositions = {
		{0, 9}, {0, 8},
		{1, 9}, {1, 8},
		{2, 9}, {2, 8},
		{3, 9}, {3, 8},
		{4, 9}, {4, 8}
	};

	int DX8[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int DY8[8] = {0, 1, 1, 1, 0, -1, -1, -1};
};
