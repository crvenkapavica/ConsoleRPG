#pragma once

#include "MapGenerator.h"
#include "../Characters/Character.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"
#include "../Characters/CharacterData.h"

using namespace std;

void MapGenerator::Initialize(const vector<weak_ptr<Character>>& player_characters) {

	_map = new char* [MAX_X];
	for (int i = 0; i < MAX_X; i++) _map[i] = new char[MAX_Y];

	_nodes = new path_node* [MAX_X];
	for (int i = 0; i < MAX_X; i++) _nodes[i] = new path_node[MAX_Y];

	_steps = new int* [MAX_X];
	for (int i = 0; i < MAX_X; i++) _steps[i] = new int[MAX_Y]();

	_distance = new int* [MAX_X];
	for (int i = 0; i < MAX_X; i++) _distance[i] = new int[MAX_Y]();

	InitBFS();
	InitPlayer(player_characters);
	InitDistanceBFS(_player_x, _player_y, 0);
	InitEnemies();
}

void MapGenerator::InitBFS() {

	srand(static_cast<unsigned int>(time(0)));

	InitilizeEmptyMap();

	//_step_limit = static_cast<int>(static_cast<float>(MAX_X * GetMapDensity()));

	_step_limit = 688;

	_moves = GetNumberOfMoves();
	_dir = GetDirection(START_X, START_Y);

	BFS(START_X, START_Y, 0);
}

void MapGenerator::BFS(int x, int y, int step) {

	if (x > MAX_X - 2 || x <= 1 || y > MAX_Y - 2 || y <= 1) return;
	if (_total_steps > _step_limit) return;
	if (_map[x][y] == PATH) return;

	GetRandomRectangle(x, y);

	if (_map[x][y] != PATH) ++_total_steps;
	_map[x][y] = PATH;
	DisableAdjacent(x, y);
	--_moves;
	if (_steps[x][y] > step || _steps[x][y] == 0) _steps[x][y] = step;

	if (step > 0) {
		int reverse_dir = GetReverseDirection();
		if (reverse_dir != -1) {
			_nodes[x][y][reverse_dir] = true;
		}
		else {
			_dir = GetRandTurnDirection(x, y);
			_moves = GetNumberOfMoves();
		}
	}
	_nodes[x][y][_dir] = true;

	if (!_moves) {
		_dir = GetVisitedNodeDirection(_nodes[x][y], x, y);
		_moves = GetNumberOfMoves();
	}

	for (int i = _dir; i < _dir + 3; i++) {
		int idx = i % 4;
		BFS(x + _dX[idx], y + _dY[idx], step + 1);
	}
}

void MapGenerator::InitilizeEmptyMap() {
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (_map[i][j] != PATH) 
				_map[i][j] = OBSTACLE;
}

void MapGenerator::DisableAdjacent(int x, int y) {

	if (_axis_sides == -1) return;

	if (_axis == 'x') {
		if (_axis_sides == 2 && _rnd_both) {
			DisableLocation(x + 1, y);
			DisableLocation(x - 1, y);
			_rnd_both--;
			
			_nodes[x][y].up = _nodes[x][y].down = true;
			_nodes[x + 1][y].up = true;
			_nodes[x - 1][y].down = true;
		}
		else if (_axis_sides == 1 && _rnd1) {
			DisableLocation(x + 1, y);
			_rnd1--;

			_nodes[x][y].down = true;
			_nodes[x + 1][y].up = true;
		}
		else if (_rnd2) {
			DisableLocation(x - 1, y);
			_rnd2--;

			_nodes[x][y].up = true;
			_nodes[x - 1][y].down = true;
		}
	}
	else {
		if (_axis_sides == 2 && _rnd_both) {
			DisableLocation(x, y + 1);
			DisableLocation(x, y - 1);
			_rnd_both--;

			_nodes[x][y].left = _nodes[x][y].right = true;
			_nodes[x][y + 1].left = true;
			_nodes[x][y - 1].right = true;
		}
		else if (_axis_sides == 1 && _rnd1) {
			DisableLocation(x, y - 1);
			_rnd1--;

			_nodes[x][y].left = true;
			_nodes[x][y - 1].right = true;
		}
		else if (_rnd2) {
			DisableLocation(x, y + 1);
			_rnd2--;
			
			_nodes[x][y].right = true;
			_nodes[x][y + 1].left = true;
		}
	}
}

void MapGenerator::DisableLocation(int x, int y) {
	if (_map[x][y] != PATH)
		_map[x][y] = OBSTACLE;
}

int MapGenerator::GetNumberOfMoves() {
	
	_width = GetCurrentMoveWidth();

	if (_total_steps < 50)  return rand() % 7 + 1;

	int rnd = rand() % 5;

	_rnd1 = rand() % 5 + 3;
	_rnd2 = rand() % 5 + 3;
	_rnd_both = rand() % 5 + 3;

	if (rnd == 0 || rnd == 1) {

		if (_dir == 0 || _dir == 1) {
			_axis = 'x';
		}
		else {
			_axis = 'y';
		}
		_axis_sides = 2;
	}
	else {
		_axis_sides = -1;
	}
	if (rnd == 1) {
		_axis_sides = rand() % 2;
	}

	return rand() % 7 + 1;
}

int MapGenerator::GetDirection(int x, int y) {
	_turn.push_back(make_pair(x, y));
	return rand() % 4;
}

int MapGenerator::GetReverseDirection() {
	switch (_dir) {
	case 0: return 1;
	case 1: return 0;
	case 2: return 3;
	case 3: return 2;
	default: return -1;
	}
}

int MapGenerator::GetVisitedNodeDirection(const path_node& visited_node, int x, int y) {

	_turn.push_back(make_pair(x, y));

	for (int i = 0; i < 4; i++) {
		if (!visited_node[i])
			return i;
	}

	return -1;
}

int MapGenerator::GetRandTurnDirection(OUT int& x, OUT int& y) {

	int dir = -1;

	while (dir == -1) {
		int rnd = rand() % _turn.size();
		x = _turn[rnd].first;
		y = _turn[rnd].second;

		path_node curr_node = _nodes[x][y];

		for (int i = 0; i < 4; i++) {
			if (!curr_node[i]) {
				dir = i;
				break;
			}
		}
	}

	return dir;
}

int MapGenerator::GetCurrentMoveWidth() {

	int rnd = rand() % 10 + 1;

	if (rnd == 10 || rnd == 9) return 3;
	if (rnd >= 6 || rnd <= 8) return 2;
	return 1;
}

float MapGenerator::GetMapDensity() {

	int rnd = rand() % 4 + 3;
	return rnd / 10.f;
}

void MapGenerator::GetRandomRectangle(int x, int y) {
	
	if (_map[x][y] == RECTANGLE) return;

	int rnd = rand() % 10;
	if (rnd == 1) {

		int a = rand() % 3 + 2;
		int b = rand() % 3 + 2;

		for (int i = 0; i < a; i++)
			for (int j = 0; j < b; j++) {
				if (x + i >= MAX_X - 1 || y + j >= MAX_Y - 1) break;
				_map[x + i][y + j] = RECTANGLE;
			}
	}
}

void MapGenerator::PrintDebugMap() {

	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
				cout << GetMapAnsi(_map[i][j]) << _map[i][j] << ANSI_COLOR_RESET;
		}
		cout << endl;
	}
	cout << ANSI_COLOR_RESET << endl;
}

void MapGenerator::PrintError() {
	for (int i = 0; i < _error.size(); i++)
		cout << _error[i] << endl;
}

void MapGenerator::MakeDebugMessage(int steps, string func) {

	string s = to_string(steps) + ":  ---" + func;
	_error.push_back(s);
}


void MapGenerator::GetPlayerStartPosition(int& x, int& y) {
	do {
		x = rand() % MAX_X;
		y = rand() & MAX_Y;
	} while (_map[x][y] != PATH);
}

void MapGenerator::InitPlayer(const vector<weak_ptr<Character>> player_characters) {

	for (auto& player : player_characters) {
		_player_characters.push_back(std::static_pointer_cast<PlayerCharacter>(player.lock()));
	}

	GetPlayerStartPosition(_player_x, _player_y);
	
	_border_x = _player_x - static_cast<int>(_player_characters[0].lock()->GetLightRadius()) + 1;
	if (_border_x < 2) _border_x = 2;
	_border_x_end = _player_x + static_cast<int>(_player_characters[0].lock()->GetLightRadius());
	if (_border_x_end > MAX_X - 2) _border_x_end = MAX_X - 2;
	_border_y = _player_y - static_cast<int>(_player_characters[0].lock()->GetLightRadius()) + 1;
	if (_border_y < 2) _border_y = 2;
	_border_y_end = _player_y + static_cast<int>(_player_characters[0].lock()->GetLightRadius());
	if (_border_y_end > MAX_Y - 2) _border_y_end = MAX_Y - 2;
	
	_map[_player_x][_player_y] = PLAYER;
}

void MapGenerator::ShowMap() {

	system("cls");

	if (_border_x == 0 && _border_x_end == 0) {
		ShowPosition();
	}
	else {
		_map[_player_x][_player_y] = PATH;
		DrawMap(_border_x, _border_x_end, _border_y, _border_y_end);
	}
}

void MapGenerator::DrawMap(int xs, int xe, int ys, int ye) {

	for (int i = xs; i < xe; i++) {
		for (int j = ys; j < ye; j++)
			cout << GetMapAnsi(_map[i][j]) << _map[i][j] << ANSI_COLOR_RESET;
		cout << endl;
	}
	cout << ANSI_COLOR_RESET;

	_map[_player_x][_player_y] = PLAYER; // for deleting for global map purposes

	GameplayStatics::DisplayMapMenu();
}

const char* MapGenerator::GetMapAnsi(char c) {

	switch (c) {
	case OBSTACLE:
		return ANSI_COLOR_CYAN;
	case PATH:;
	case RECTANGLE:
		return ANSI_COLOR_YELLOW;
	case PLAYER:
		return ANSI_COLOR_GREEN;
	case ENEMY:
		return ANSI_COLOR_RED;
	default:
		return ANSI_COLOR_RESET;
	}
}

void MapGenerator::ShowPosition() {

	system("cls");

	int radius = static_cast<int>(_player_characters[0].lock()->GetLightRadius());

	// TODO -=  NAPRAVOITI CHECK ZA OUT OF BOUNDS
	DrawMap(_player_x - radius + 1, _player_x + radius, _player_y - radius + 1, _player_y + radius);
	GameplayStatics::DisplayMapMenu();
}

void MapGenerator::ExtendMapBorders(int radius) {

	_border_x = _border_x - radius < _border_x ? _border_x - radius : _border_x;
	_border_x_end = _border_x_end + radius > _border_x_end ? _border_x_end + radius : _border_x;
	_border_y = _border_y - radius < _border_y ? _border_y - radius : _border_y;
	_border_y_end = _border_y_end + radius > _border_y_end ? _border_y_end + radius : _border_y;
}

void MapGenerator::HandleMovement() {

	cout << "Which direction do you want to move to ? " << endl;
	cout << "use W S A D keys to move " << endl;
	cout << ANSI_CLEAR_LINE << "-> ";

	string s;
	cin >> s;

	switch (s[0]) {
	case 'D':
	case 'd':
	case 80:
		Move(0);
		break;
	case 'A':
	case 'a':
	case 72:
		Move(1);
		break;
	case 'S':
	case 's':
	case 77:
		Move(2);
		break;
	case 'W':
	case 'w':
	case 75:
		Move(3);
		break;
	default:
		DisplayErrorMessage("Wrong Input");
		GameplayStatics::DisplayMapMenu();
		break;
	}
}

void MapGenerator::Move(int dir) {

	int radius = static_cast<int>(_player_characters[0].lock()->GetLightRadius());

	_map[_player_x][_player_y] = PATH;

	switch (dir) {
	case 0:
		++_player_y;
		if (_player_y + radius > _border_y_end)
			++_border_y_end;
		break;
	case 1:
		--_player_y;
		if (_player_y - radius < _border_y)
			--_border_y;
		break;
	case 2:
		++_player_x;
		if (_player_x + radius > _border_x_end)
			++_border_x_end;
		break;
	case 3:
		--_player_x;
		if (_player_x - radius < _border_x)
			--_border_x;
		break;
	}

	if (_map[_player_x][_player_y] == ENEMY) {

		ClearCharGrid();
		DrawPlayGrid();

		auto enemies = GetEnemies(_player_x, _player_y);
		GenerateCharacterGridPositions();
		AddCharactersToGrid();

		GameplayStatics::InitiateCombatMode(move(enemies));
	}

	_map[_player_x][_player_y] = PLAYER;

	ShowPosition();
}

void MapGenerator::DisplayErrorMessage(const string& message) {
	cout << ANSI_COLOR_RED << message << ANSI_COLOR_RESET << endl << endl;
}

void MapGenerator::InitDistanceBFS(int x, int y, int step) {
	BFS_Distance(x, y, step);
}

void MapGenerator::BFS_Distance(int x, int y, int step) {

	_distance[x][y] = step;

	for (int i = 0; i < 4; i++) {
		int newX = x + _dX[i];
		int newY = y + _dY[i];

		if (newX >= 2 && newX < MAX_X - 2 && newY >= 2 && newY < MAX_Y - 2 &&
			_map[newX][newY] == PATH && (_distance[newX][newY] == 0 || _distance[newX][newY] > step + 1)) {
			BFS_Distance(newX, newY, step + 1);
		}
	}
}

void MapGenerator::InitEnemies() {
	AddRandomMapEnemies();
}

void MapGenerator::AddRandomMapEnemies() {

	const int MAP_LEVEL = 1;

	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			int percent = static_cast<int>(_total_steps * 0.05);
			int rnd = GameplayStatics::GetRandInt(0, _total_steps);

			if (/*rnd <= percent && */_steps[i][j] > 0 && _map[i][j] != PLAYER) {
				_map[i][j] = ENEMY;

				int power_lvl_low = (_distance[i][j] + 2) * MAP_LEVEL;
				int power_lvl_high = power_lvl_low;
				power_lvl_low -= static_cast<int>(power_lvl_low * 0.3);
				power_lvl_high += static_cast<int>(power_lvl_high * 0.3);
				int power_lvl = GameplayStatics::GetRandInt(power_lvl_low, power_lvl_high);

				int rnd_enemies = 0;
				int total_power = 0;
				vector<int> char_classes;

				while (power_lvl) {
					int rndc = rand() % 6 + 50;
					if (power_lvl - CharDB::_data[static_cast<ECharacterClass>(rndc)]._power_lvl >= 0) {
						++rnd_enemies;
						char_classes.push_back(rndc);
						total_power += CharDB::_data[static_cast<ECharacterClass>(rndc)]._power_lvl;
						power_lvl -= CharDB::_data[static_cast<ECharacterClass>(rndc)]._power_lvl;
						if (rnd_enemies == 7) {
							++rnd_enemies;
							int t = power_lvl > 6 ? 6 : power_lvl;
							char_classes.push_back(50 + t - 1);
							total_power += CharDB::_data[static_cast<ECharacterClass>(rndc)]._power_lvl;
							power_lvl -= t;
							break;
						}
					}
				}

				vector<shared_ptr<Character>> enemies_vector;
				map<char, weak_ptr<Character>> enemies_map;

				for (int k = 0; k < rnd_enemies; k++) { 
					enemies_vector.push_back(make_shared<EnemyCharacter>(static_cast<ECharacterClass>(char_classes[k])));
					enemies_map['A' + k] = enemies_vector[k];
				}
				_enemy_map.push_back(move(enemies_vector));
				_enemy_map_xy.push_back(make_pair(i, j));	
				_enemy_name_map.push_back(enemies_map);
				_power_lvls.push_back(total_power);
				
				// restart static instance counter
				EnemyCharacter::_n = 0;
			}
		}
	}
}

vector<weak_ptr<Character>> MapGenerator::GetEnemies(int x, int y) {

	_enemy_index = 0;
	for (auto& xy : _enemy_map_xy) {
		if (xy.first == x && xy.second == y)
			break;
		++_enemy_index;
	}

	vector<weak_ptr<Character>> w_ptr;
	for (auto& enemy : _enemy_map.at(_enemy_index))
		w_ptr.push_back(weak_ptr<Character>(enemy));

	return w_ptr;
}

weak_ptr<Character> MapGenerator::GetCharacterFromAlias(char target) {
	if (_char_map.find(target) == _char_map.end()) return weak_ptr<Character>();
	int x = _char_map.at(target).first;
	int y = _char_map.at(target).second;
	return _char_grid[x][y]._here;
}

void MapGenerator::DisplayGrid() {
	for (int i = 0; i < GRID_X; i++) {
		for (int j = 0; j < GRID_Y; j++) {
			if (j < 17) cout << ANSI_COLOR_GREEN; else cout << ANSI_COLOR_PASTELLE_BROWN;
			if (j >= 64) cout << ANSI_COLOR_RED;
			cout << _grid[i][j];
		}
		cout << endl;
	}
}

void MapGenerator::DrawPlayGrid() {

	for (int i = 0; i < GRID_X; i++) {
		for (int j = 0; j < GRID_Y; j++) {		
			// this is the field where character placement will be
			if (i % 4 == 2 && j % 8 == 4) {
				_grid[i][j] = ' ';
			}
			else {
				if (i % 4 == 0) {
					if (j % 8 == 0) _grid[i][j] = '+';
					else _grid[i][j] = '-';
				}
				else {
					if (j % 8 == 0) _grid[i][j] = '|';
					else _grid[i][j] = ' ';
				}
			}
		}
	}
}

void MapGenerator::GenerateCharacterGridPositions() {

	for (auto& enemy : _enemy_name_map.at(_enemy_index)) {

		int first, second;
		do {
			int rnd = rand() % 10;
			first = _enemy_start_positions[rnd].first;
			second = _enemy_start_positions[rnd].second;
		} while (_char_grid[first][second]._here.lock().get());

		_char_grid[first][second]._here = enemy.second;
		_char_map[enemy.first] = make_pair(first, second);
	}

	//add player characters
	for (int i = 0; i < _player_characters.size(); i++) {
		_char_grid[i][0]._here = _player_characters[i];
		_char_map['0' + i] = make_pair(i, 0);
	}
}

void MapGenerator::AddCharactersToGrid() {

	for (int i = 0; i < CHAR_GRID_X; i++)
		for (int j = 0; j < CHAR_GRID_Y; j++) {
			int x = i * 4 + 2;
			int y = j * 8 + 4;

			if (_char_grid[i][j]._here.lock().get())
				_grid[x][y] = _char_grid[i][j]._here.lock()->GetAlias();
		}
}

bool MapGenerator::AddCharacterToCharGrid(const shared_ptr<Character>& instigator, weak_ptr<Character> summon) {

	int x = _char_map.at(instigator->GetAlias()).first;
	int y = _char_map.at(instigator->GetAlias()).second;

	int xx = 0, yy = 0;
	bool bHasSpawned = false;
	for (int i = 0; i < 8; i++) {
		xx = x + _dX8[i];
		yy = y + _dY8[i];

		if (xx >= 0 && xx < CHAR_GRID_X && yy >= 0 && yy < CHAR_GRID_Y) {
			if (!_char_grid[xx][yy]._here.lock().get()) {
				_char_grid[xx][yy]._here = summon;
				bHasSpawned = true;
				break;
			}
		}
	}
	
	if (bHasSpawned) {
		_char_map[_char_grid[xx][yy]._here.lock()->GetAlias()].first = xx;
		_char_map[_char_grid[xx][yy]._here.lock()->GetAlias()].second = yy;
	}

	_grid[xx * 4 + 2][yy * 8 + 4] = summon.lock()->GetAlias();

	UpdateCharGrid();

	return bHasSpawned;
}

void MapGenerator::UpdateCharGrid() {

	for (int i = 0; i < CHAR_GRID_X; ++i) 
		for (int j = 0; j < CHAR_GRID_Y; ++j) 
			for (int l = 0; l < 2; l++)
				for (int k = 0; k < 8; k++) {
				
					int x = i + _dX8[k];
					int y = j + _dY8[k];

					if (x >= 0 && x < CHAR_GRID_X && y >= 0 && y < CHAR_GRID_Y) {

						if (l && _char_grid[i][j]._neighbors[k].lock() && !_char_grid[x][y]._here.lock())
							_char_grid[x][y]._here = _char_grid[i][j]._neighbors[k];

						else _char_grid[i][j]._neighbors[k] = _char_grid[x][y]._here;
					}
					else _char_grid[i][j]._neighbors[k] = weak_ptr<Character>();
				}
}

void MapGenerator::ClearCharGrid() {
	for (int i = 0; i < CHAR_GRID_X; i++)
		for (int j = 0; j < CHAR_GRID_Y; j++) {
			_char_grid[i][j]._here = weak_ptr<Character>();
			for (auto& a : _char_grid[i][j]._neighbors)
				a = weak_ptr<Character>();
		}
}

void MapGenerator::MoveCharacterOnGrid(Character* character, EDirection direction) {

	int x = _char_map.at(character->GetAlias()).first;
	int y = _char_map.at(character->GetAlias()).second;

	int dir = static_cast<int>(direction);
	int xx = x + _dX8[dir];
	int yy = y + _dY8[dir];

	// Swap pointers and aliases from character source to character destination
	_char_grid[xx][yy]._here = _char_grid[x][y]._here;
	_char_grid[xx][yy]._here.lock()->SetAlias(_char_grid[x][y]._here.lock()->GetAlias());
	_char_grid[x][y]._here = weak_ptr<Character>();

#pragma warning(push)
#pragma warning(disable: 6011) // Suppressing warning C6011: dereferencing null pointer
	// Update map with alias
	_char_map[_char_grid[xx][yy]._here.lock()->GetAlias()].first = xx;
	_char_map[_char_grid[xx][yy]._here.lock()->GetAlias()].second = yy;
#pragma warning(pop)

	// Move on the real grid
	_grid[xx * 4 + 2][yy * 8 + 4] = character->GetAlias();
	_grid[x * 4 + 2][y * 8 + 4] = ' ';

	// Update _char_grid
	UpdateCharGrid();
}

vector<string> MapGenerator::GetCombatDirections(Character* character, OUT map<int, EDirection>& map) {

	int x = _char_map.at(character->GetAlias()).first;
	int y = _char_map.at(character->GetAlias()).second;
	vector<string> directions = { "NORTH", "NORTHEAST", "EAST", "SOUTHEAST", "SOUTH", "SOUTHWEST", "WEST", "NORTHWEST" };
	vector<string> v;

	for (int i = 0; i < 8; i++) {
		int xx = x + _dX8[i];
		int yy = y + _dY8[i];
		if (xx < 0 || xx >= CHAR_GRID_X || yy < 0 || yy >= CHAR_GRID_Y) continue;
		if (_char_grid[xx][yy]._here.lock().get()) continue;

		v.push_back(directions[i]);
		// Map values from InteractiveDisplay to real values
		map[static_cast<int>(v.size()) - 1] = static_cast<EDirection>(i);
	}
	return v;
}

vector<Character*> MapGenerator::GetCharactersInRange(Character* character) {
	vector<Character*> v;
	int x = _char_map.at(character->GetAlias()).first;
	int y = _char_map.at(character->GetAlias()).second;

	for (const auto& c : _char_grid[x][y]._neighbors)
		if (c.lock().get()) v.push_back(c.lock().get());

	return v;
}

const int MapGenerator::GetPowerLvl() const {
	return _power_lvls[_enemy_index];
}

int MapGenerator::GetEnemyIdx(char alias) {
	if (UPPER(alias) < 'A' || UPPER(alias) > 'Z') return -1;
	if (auto character = GetCharacterFromAlias(UPPER(alias)).lock()) {
		for (int i = 0; i < _enemy_map.at(_enemy_index).size(); i++) {
			if (character == _enemy_map.at(_enemy_index)[i])
				return i;
		}
	}
	return -1;
}

int MapGenerator::GetPlayerIdx(char alias) {
	auto character = GetCharacterFromAlias(alias);
	for (int i = 0; i < _player_characters.size(); i++) {
		if (std::static_pointer_cast<PlayerCharacter>(character.lock()) == _player_characters[i].lock())
			return i;
	}
	return -1;
}

void MapGenerator::KillEnemy(int idx) {
	if (Character* character = _enemy_map.at(_enemy_index)[idx].get()) {
		int x = _char_map.at(character->GetAlias()).first;
		int y = _char_map.at(character->GetAlias()).second;
		_char_grid[x][y]._here = weak_ptr<Character>();
		UpdateCharGrid();	// to make it more efficient we can just update the killed characters neighbours' neighbours
		_grid[x * 4 + 2][y * 8 + 4] = ' ';

		_char_map.erase(character->GetAlias());
		_enemy_map.at(_enemy_index)[idx].reset();
	}
}

void MapGenerator::KillEnemy(Character* character) {

	int x = _char_map.at(character->GetAlias()).first;
	int y = _char_map.at(character->GetAlias()).second;
	_char_grid[x][y]._here = weak_ptr<Character>();
	_grid[x * 4 + 2][y * 8 + 4] = ' ';

	_char_map.erase(character->GetAlias());

	for (auto& enemy : _enemy_map.at(_enemy_index))
		if (enemy.get() == character)
			enemy.reset();
}
