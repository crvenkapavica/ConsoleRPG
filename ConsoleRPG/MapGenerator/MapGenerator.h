#pragma once

#include "../RPGTypes.h"
#include "../GameplayStatics.h" 

class Character;
class EnemyCharacter;
class PlayerCharacter;

struct distance_node {
	int x;
	int y;
	int distance;
};

struct grid_node {
	std::weak_ptr<Character> _here;
	std::weak_ptr<Character> _neighbors[8];
};

struct path_node {

	path_node() : right(0), left(0), up(0), down(0) {}

	bool& operator[](const int& idx) {
		switch (idx) {
		case 0: return right;
		case 1: return left;
		case 2: return down;
		case 3: return up;
		default: return right;
		}
	}

	bool operator[](const int& idx) const {
		switch (idx) {
		case 0: return right == true;
		case 1: return left == true;
		case 2: return down == true;
		case 3: return up == true;
		default: return false;
		}
	}

	//corresponds to dX and dY directions
	bool right;
	bool left;
	bool down;
	bool up;
};


class MapGenerator {

public:
	MapGenerator() {}

	void Initialize(const std::vector<std::weak_ptr<Character>>& player_characters);

	// Shows the current player position on the map with radius of light_radius
	void ShowPosition();

	// Shows the portion of the map the player discovered
	void ShowMap();

	// Handle keypress for direction of movement
	void HandleMovement();

	// DEBUG
	/////////////////////////////////
	void PrintDebugMap();

	void PrintError();
	/////////////////////////////////

	void DisplayGrid();

	void MoveCharacterOnGrid(Character* character, EDirection direction);

	bool AddCharacterToCharGrid(const std::shared_ptr<Character>& instigator, std::weak_ptr<Character> summon);

	int GetEnemyIdx(char alias);

	int GetPlayerIdx(char alias);

	void KillEnemy(int idx);

	void KillEnemy(Character* character);

public:
	std::weak_ptr<Character> GetCharacterFromAlias(char target);

	std::vector<string> GetCombatDirections(Character* character, OUT std::map<int, EDirection>& map);

	std::vector<Character*> GetCharactersInRange(Character* character);

	const int GetPowerLvl() const;

	// testing
	//--------------
	void PrintDistance();

	//-------------------

private:
	// BFS that generates map
	void BFS(int x, int y, int step);

	void InitBFS();

	void InitilizeEmptyMap();

	int GetCurrentMoveWidth();

	float GetMapDensity();

	int GetNumberOfMoves();

	int GetDirection(int x, int y);

	int GetReverseDirection();

	int GetRandTurnDirection(OUT int& x, OUT int& y);

	int GetVisitedNodeDirection(const path_node& visited_node, int x, int y);

	void DisableAdjacent(int x, int y);

	void DisableLocation(int x, int y);

	void MakeDebugMessage(int steps, string func);

	// Makes a random rectangle segment
	void GetRandomRectangle(int x, int y);

	void InitEnemies();

	// Spawns enemies on the map and decides their power level
	void AddRandomMapEnemies();

	// PLAYER SPECIFIC
	// -------------------------------------------------------------------

	void GetPlayerStartPosition(int& x, int& y);

	void InitPlayer(const std::vector<std::weak_ptr<Character>> player_characters);

	void DisplayErrorMessage(const string& message);

	// Used to display the discovered portion of the map
	void ExtendMapBorders(int radius);

	void DrawMap(int xs, int xe, int ys, int ye);

	// Gets the color of a specific unit
	const char* GetMapAnsi(char c);

	// Calclate distance to each node
	void BFS_Distance(int x, int y, int step);

	void InitDistanceBFS(int x, int y, int step);

	// Move after input
	void Move(int dir);

	std::vector<std::weak_ptr<Character>> GetEnemies(int x, int y);

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
	std::vector<EnemyCharacter*> _enemies;

	class std::vector<std::weak_ptr<PlayerCharacter>> _player_characters;

	char** _map;

	path_node** _nodes;

	int** _steps;

	int** _distance;

	std::vector<string> _error;

	std::vector<std::pair<int, int>> _turn;

	int _step_limit;

	int _moves;
	int _dir;

	int _dX[4] = { 0, 0, 1, -1 };
	int _dY[4] = { 1, -1, 0, 0 };

	int _total_steps = 0;
	int _width = 0;

	char _axis;
	int _rnd1;
	int _rnd2;
	int _rnd_both;
	int _axis_sides;

	int _player_x;
	int _player_y;

	int _border_x = 0, _border_x_end = 0;
	int _border_y = 0, _border_y_end = 0;

	string _error_message;

	std::vector<distance_node> _distances_nodes;

	std::vector<std::vector<std::shared_ptr<Character>>> _enemy_map; // this two vectors are aligned
	std::vector<std::pair<int, int>> _enemy_map_xy;					 // this two vectors are aligned
	std::vector<std::map<char, std::weak_ptr<Character>>> _enemy_name_map;

	std::vector<std::vector<int>> _power_lvls;

	int _enemy_index;

	// GRID SPECIFIC
	// ------------------------------------------------------------------
	char _grid[21][81];
	grid_node _char_grid[5][10];
	std::unordered_map<char, std::pair<int, int>> _char_map;

	std::vector<std::pair<int, int>> _enemy_start_positions = {
		{0, 9}, {0, 8},
		{1, 9}, {1, 8},
		{2, 9}, {2, 8},
		{3, 9}, {3, 8},
		{4, 9}, {4, 8}
	};

	int _dX8[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int _dY8[8] = {0, 1, 1, 1, 0, -1, -1, -1};
};
