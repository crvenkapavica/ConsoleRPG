#pragma once

#include "../RPGTypes.h"
#include "../GameplayStatics.h" 

class EnemyCharacter;
class PlayerCharacter;

using namespace std;

struct distance_node {
	int x;
	int y;
	int distance;
};

class MapGenerator {

public:
	MapGenerator() {}

	void Initialize(const vector<weak_ptr<PlayerCharacter>>& player_characters);

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

	void AddCharacterToCharGrid(Character* instigator, Character* summon);

	int GetEnemyIdx(char alias);

	int GetPlayerIdx(char alias);

	void KillEnemy(int idx);

	void KillEnemy(Character* character);

public:
	Character* GetCharacterFromAlias(char target);

	vector<string> GetCombatDirections(Character* character, OUT map<int, EDirection>& map);

	vector<Character*> GetCharactersInRange(Character* character);

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

	void InitPlayer(const vector<weak_ptr<PlayerCharacter>>& player_characters);

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

	vector<weak_ptr<EnemyCharacter>> GetEnemies(int x, int y);

	// GRID SPECIFIC
	// -------------------------------------------------------------------

	void DrawGrid();

	// Places each enemy on the [currently] random square on the play grid
	void GenerateCharacterGridPositions();

	// Adds characters to the play grid
	void AddCharactersToGrid();

	// Update internal char_grid's neighbours
	void UpdateCharGrid();

	void ClearCharacterGrid();

private:
	vector<EnemyCharacter*> _enemies;

	class vector<weak_ptr<PlayerCharacter>> _player_characters;

	char _map[MAX_X][MAX_Y];
	path_node _nodes[MAX_X][MAX_Y];  // TODO : use dynamic allocation later
	int _steps[MAX_X][MAX_Y] = { {0} };

	vector<string> _error;

	vector<pair<int, int>> _turn;

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

	int _distance[MAX_X][MAX_Y] = { {0} };

	vector<distance_node> _distances_nodes;

	vector<vector<shared_ptr<EnemyCharacter>>> _enemy_map; // this two vectors are aligned
	vector<pair<int, int>> _enemy_map_xy; // this two vectors are aligned
	vector<map<char, EnemyCharacter*>> _enemy_name_map; 

	int _enemy_index;

	// GRID SPECIFIC
	// ------------------------------------------------------------------
	char _grid[21][81];
	grid_node _char_grid[5][10];
	unordered_map<char, pair<int, int>> _char_map;

	vector<pair<int, int>> _enemy_start_positions = {
		{0, 9}, {0, 8},
		{1, 9}, {1, 8},
		{2, 9}, {2, 8},
		{3, 9}, {3, 8},
		{4, 9}, {4, 8}
	};

	int _dX8[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int _dY8[8] = {0, 1, 1, 1, 0, -1, -1, -1};
};
