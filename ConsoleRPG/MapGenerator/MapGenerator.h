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

	bool& operator[](const int& Idx) {
		switch (Idx) {
		case 0: return Right;
		case 1: return Left;
		case 2: return Down;
		case 3: return Up;
		default: return Right;
		}
	}

	bool operator[](const int& Idx) const {
		switch (Idx) {
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


class MapGenerator {

public:
	MapGenerator() = delete;

	void Initialize(const std::vector<std::weak_ptr<Character>>& InPlayerCharacters);

	// Shows the current player position on the map with radius of light_radius
	void ShowPosition();

	// Shows the portion of the map the player discovered
	void ShowMap();

	// Handle keypress for direction of movement
	void HandleMovement();

	// DEBUG
	/////////////////////////////////
	void PrintDebugMap() const;

	void PrintError() const;
	/////////////////////////////////

	void DisplayGrid() const;

	void MoveCharacterOnGrid(const Character& InCharacter, const EDirection Direction);

	bool AddCharacterToCharGrid(const std::shared_ptr<Character>& Instigator, const std::weak_ptr<Character>& Summon);

	int GetEnemyIdx(char Alias);

	int GetPlayerIdx(char Alias);

	void KillEnemy(int Idx);

	void KillEnemy(const Character* InCharacter);

public:
	std::weak_ptr<Character> GetCharacterFromAlias(char Target);

	std::vector<string> GetCombatDirections(const Character* InCharacter, OUT std::map<int, EDirection>& InMap) const;

	std::vector<Character*> GetCharactersInRange(const Character* InCharacter);

	// inline?
	[[nodiscard]] inline int GetPowerLvl() const { return PowerLevels[EnemyIndex]; }

private:
	// BFS that generates map
	void BFS(int X, int Y, int Step);

	void InitBFS();

	void InitializeEmptyMap() const;

	static int GetCurrentMoveWidth();

	static float GetMapDensity();

	int GetNumberOfMoves();

	int GetDirection(int X, int Y);

	int GetReverseDirection() const;

	int GetRandomTurnDirection(OUT int& X, OUT int& Y) const;

	int GetVisitedNodeDirection(const PathNode& VisitedNode, int X, int Y);

	void DisableAdjacent(int X, int Y);

	void DisableLocation(int X, int Y) const;

	void MakeDebugMessage(int InSteps, const string& Func);

	// Makes a random rectangle segment
	void GetRandomRectangle(int X, int Y) const;

	void InitEnemies();

	// Spawns enemies on the map and decides their power level
	void AddRandomMapEnemies();

	// PLAYER SPECIFIC
	// -------------------------------------------------------------------

	void GetPlayerStartPosition(int& X, int& Y) const;

	void InitPlayer(const std::vector<std::weak_ptr<Character>>& InPlayerCharacters);

	static void DisplayErrorMessage(const string& Message);

	// Used to display the discovered portion of the map
	void ExtendMapBorders(int Radius);

	void DrawMap(int Xs, int Xe, int Ys, int Ye);

	// Gets the color of a specific unit
	static const char* GetMapAnsi(char C);

	// Calculate distance to each node
	void BFS_Distance(int X, int Y, int Step);

	void InitDistanceBFS(int X, int Y, int Step);

	// Move after input
	void Move(int MoveDir);

	std::vector<std::weak_ptr<Character>> GetEnemies(int X, int Y);

	// GRID SPECIFIC
	// -------------------------------------------------------------------

	void DrawPlayGrid();

	// Places each enemy on the [currently] random square on the play grid
	void GenerateCharacterGridPositions();

	// Adds characters to the play grid
	void AddCharactersToGrid();

	// Update internal char_grid's neighbours
	void UpdateCharGrid();

	void ClearCharGrid();

private:
	std::vector<EnemyCharacter*> Enemies;

	class std::vector<std::weak_ptr<PlayerCharacter>> PlayerCharacters;

	char** Map;

	PathNode** Nodes;

	int** Steps;

	int** Distance;

	std::vector<int> Distances;

	std::vector<string> Error;

	std::vector<std::pair<int, int>> Turn;

	int StepLimit;

	int Moves;
	int Dir;

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

	string ErrorMessage;

	std::vector<DistanceNode> DistancesNodes;

	std::vector<std::vector<std::shared_ptr<Character>>> EnemyMap; // this two vectors are aligned
	std::vector<std::pair<int, int>> EnemyMapXY;					 // this two vectors are aligned
	std::vector<std::map<char, std::weak_ptr<Character>>> EnemyNameMap;

	std::vector<int> PowerLevels;

	int EnemyIndex;

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
