#include "MapGenerator.h"

#include <algorithm>
#include "../Characters/Character.h"
#include "../Characters/CharacterData.h"
#include "../Characters/EnemyCharacter.h"
#include "../Characters/PlayerCharacter.h"

#include "../GameplayStatics.h"

using namespace std;

void MapGenerator::Initialize(const std::vector<std::weak_ptr<Character>>& InPlayerCharacters) {

	Map = new char* [MAX_X];
	for (int i = 0; i < MAX_X; i++) Map[i] = new char[MAX_Y];

	Nodes = new PathNode* [MAX_X];
	for (int i = 0; i < MAX_X; i++) Nodes[i] = new PathNode[MAX_Y];

	Steps = new int* [MAX_X];
	for (int i = 0; i < MAX_X; i++) Steps[i] = new int[MAX_Y]();

	Distance = new int* [MAX_X];
	for (int i = 0; i < MAX_X; i++) Distance[i] = new int[MAX_Y]();

	InitBFS();
	InitPlayer(InPlayerCharacters);
	InitDistanceBFS(PlayerX, PlayerY, 0);
	InitEnemies();
}

void MapGenerator::InitBFS() {

	srand(static_cast<unsigned int>(time(0)));

	InitializeEmptyMap();

	//_step_limit = static_cast<int>(static_cast<float>(MAX_X * GetMapDensity()));

	StepLimit = 688;

	Moves = GetNumberOfMoves();
	Dir = GetDirection(START_X, START_Y);

	BFS(START_X, START_Y, 0);
}

void MapGenerator::BFS(int X, int Y, const int Step) {

	if (X > MAX_X - 2 || X <= 1 || Y > MAX_Y - 2 || Y <= 1) return;
	if (TotalSteps > StepLimit) return;
	if (Map[X][Y] == PATH) return;

	GetRandomRectangle(X, Y);

	if (Map[X][Y] != PATH) ++TotalSteps;
	Map[X][Y] = PATH;
	DisableAdjacent(X, Y);
	--Moves;
	if (Steps[X][Y] > Step || Steps[X][Y] == 0) Steps[X][Y] = Step;

	if (Step > 0) {
		if (const int ReverseDir = GetReverseDirection(); ReverseDir != -1) {
			Nodes[X][Y][ReverseDir] = true;
		}
		else {
			Dir = GetRandomTurnDirection(X, Y);
			Moves = GetNumberOfMoves();
		}
	}
	Nodes[X][Y][Dir] = true;

	if (!Moves) {
		Dir = GetVisitedNodeDirection(Nodes[X][Y], X, Y);
		Moves = GetNumberOfMoves();
	}

	for (int i = Dir; i < Dir + 3; i++) {
		const int idx = i % 4;
		BFS(X + DX[idx], Y + DY[idx], Step + 1);
	}
}

void MapGenerator::InitializeEmptyMap() const {
	for (int i = 0; i < MAX_X; i++)
		for (int j = 0; j < MAX_Y; j++)
			if (Map[i][j] != PATH)
				Map[i][j] = OBSTACLE;
}

void MapGenerator::DisableAdjacent(const int X, const int Y) {

	if (AxisSides == -1) return;

	if (Axis == 'x') {
		if (AxisSides == 2 && RandomBoth) {
			DisableLocation(X + 1, Y);
			DisableLocation(X - 1, Y);
			RandomBoth--;
			
			Nodes[X][Y].Up = Nodes[X][Y].Down = true;
			Nodes[X + 1][Y].Up = true;
			Nodes[X - 1][Y].Down = true;
		}
		else if (AxisSides == 1 && Random1) {
			DisableLocation(X + 1, Y);
			Random1--;

			Nodes[X][Y].Down = true;
			Nodes[X + 1][Y].Up = true;
		}
		else if (Random2) {
			DisableLocation(X - 1, Y);
			Random2--;

			Nodes[X][Y].Up = true;
			Nodes[X - 1][Y].Down = true;
		}
	}
	else {
		if (AxisSides == 2 && RandomBoth) {
			DisableLocation(X, Y + 1);
			DisableLocation(X, Y - 1);
			RandomBoth--;

			Nodes[X][Y].Left = Nodes[X][Y].Right = true;
			Nodes[X][Y + 1].Left = true;
			Nodes[X][Y - 1].Right = true;
		}
		else if (AxisSides == 1 && Random1) {
			DisableLocation(X, Y - 1);
			Random1--;

			Nodes[X][Y].Left = true;
			Nodes[X][Y - 1].Right = true;
		}
		else if (Random2) {
			DisableLocation(X, Y + 1);
			Random2--;
			
			Nodes[X][Y].Right = true;
			Nodes[X][Y + 1].Left = true;
		}
	}
}

void MapGenerator::DisableLocation(const int X, const int Y) const {
	if (Map[X][Y] != PATH)
		Map[X][Y] = OBSTACLE;
}

int MapGenerator::GetNumberOfMoves() {
	
	Width = GetCurrentMoveWidth();

	if (TotalSteps < 50)  return rand() % 7 + 1;

	const int Random = rand() % 5;

	Random1 = rand() % 5 + 3;
	Random2 = rand() % 5 + 3;
	RandomBoth = rand() % 5 + 3;

	if (Random == 0 || Random == 1) {

		if (Dir == 0 || Dir == 1) {
			Axis = 'x';
		}
		else {
			Axis = 'y';
		}
		AxisSides = 2;
	}
	else {
		AxisSides = -1;
	}
	if (Random == 1) {
		AxisSides = rand() % 2;
	}

	return rand() % 7 + 1;
}

int MapGenerator::GetDirection(int X, int Y) {
	Turn.emplace_back(X, Y);
	return rand() % 4;
}

int MapGenerator::GetReverseDirection() const {
	switch (Dir) {
		case 0: return 1;
		case 1: return 0;
		case 2: return 3;
		case 3: return 2;
		default: return -1;
	}
}

int MapGenerator::GetVisitedNodeDirection(const PathNode& VisitedNode, int X, int Y) {
	Turn.emplace_back(X, Y);
	for (int i = 0; i < 4; i++)
		if (!VisitedNode[i]) return i;
	return -1;
}

int MapGenerator::GetRandomTurnDirection(OUT int& X, OUT int& Y) const {
	int RandomDir = -1;
	while (RandomDir == -1) {
		const int Random = rand() % Turn.size();
		X = Turn[Random].first;
		Y = Turn[Random].second;

		PathNode CurrNode = Nodes[X][Y];

		for (int i = 0; i < 4; i++)
			if (!CurrNode[i]) { RandomDir = i; break; }
	}
	return RandomDir;
}

int MapGenerator::GetCurrentMoveWidth() {

	const int Random = rand() % 10 + 1;

	if (Random == 10 || Random == 9) return 3;
	if (Random >= 6 && Random <= 8) return 2;
	return 1;
}

float MapGenerator::GetMapDensity() {
	const int Random = rand() % 4 + 3;
	return static_cast<float>(Random) / 10.f;
}

void MapGenerator::GetRandomRectangle(const int X, const int Y) const {
	
	if (Map[X][Y] == RECTANGLE) return;

	if (const int Random = rand() % 10; Random == 1) {

		const int a = rand() % 3 + 2;
		const int b = rand() % 3 + 2;

		for (int i = 0; i < a; i++)
			for (int j = 0; j < b; j++) {
				if (X + i >= MAX_X - 1 || Y + j >= MAX_Y - 1) break;
				Map[X + i][Y + j] = RECTANGLE;
			}
	}
}

void MapGenerator::PrintDebugMap() const {

	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
				cout << GetMapAnsi(Map[i][j]) << Map[i][j] << ANSI_COLOR_RESET;
		}
		cout << '\n';
	}
	cout << ANSI_COLOR_RESET << '\n';
}

void MapGenerator::PrintError() const {
	for (const auto& e : Error)
		std::cout << e << '\n';
}

void MapGenerator::MakeDebugMessage(const int InSteps, const string& Func) {
	const string s = to_string(InSteps) + ":  ---" + Func;
	Error.push_back(s);
}


void MapGenerator::GetPlayerStartPosition(int& X, int& Y) const {
	do {
		X = rand() % MAX_X;
		Y = rand() & MAX_Y;
	} while (Map[X][Y] != PATH);
}

void MapGenerator::InitPlayer(const vector<weak_ptr<Character>>& InPlayerCharacters) {

	for (const auto& Player : InPlayerCharacters)
		PlayerCharacters.push_back(std::static_pointer_cast<PlayerCharacter>(Player.lock()));

	GetPlayerStartPosition(PlayerX, PlayerY);
	
	BorderX = PlayerX - static_cast<int>(PlayerCharacters[0].lock()->GetLightRadius()) + 1;
	BorderX = std::max(BorderX, 2);
	BorderXEnd = PlayerX + static_cast<int>(PlayerCharacters[0].lock()->GetLightRadius());
	BorderXEnd = std::min(BorderXEnd, MAX_X - 2);
	BorderY = PlayerY - static_cast<int>(PlayerCharacters[0].lock()->GetLightRadius()) + 1;
	BorderY = std::max(BorderY, 2);
	BorderYEnd = PlayerY + static_cast<int>(PlayerCharacters[0].lock()->GetLightRadius());
	BorderYEnd = std::min(BorderYEnd, MAX_Y - 2);

	Map[PlayerX][PlayerY] = PLAYER;
}

void MapGenerator::ShowMap() {

	system("cls");

	if (BorderX == 0 && BorderXEnd == 0) {
		ShowPosition();
	}
	else {
		Map[PlayerX][PlayerY] = PATH;
		DrawMap(BorderX, BorderXEnd, BorderY, BorderYEnd);
	}
}

void MapGenerator::DrawMap(int Xs, int Xe, int Ys, int Ye) {

	for (int i = Xs; i < Xe; i++) {
		for (int j = Ys; j < Ye; j++)
			cout << GetMapAnsi(Map[i][j]) << Map[i][j] << ANSI_COLOR_RESET;
		cout << '\n';
	}
	cout << ANSI_COLOR_RESET;

	Map[PlayerX][PlayerY] = PLAYER; // for deleting for global map purposes

	GameplayStatics::DisplayMapMenu();
}

const char* MapGenerator::GetMapAnsi(const char C) {

	[[fallthrough]]
	switch (C) {
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

	const int Radius = static_cast<int>(PlayerCharacters[0].lock()->GetLightRadius());

	// TODO -=  NAPRAVOITI CHECK ZA OUT OF BOUNDS
	DrawMap(PlayerX - Radius + 1, PlayerX + Radius, PlayerY - Radius + 1, PlayerY + Radius);
	GameplayStatics::DisplayMapMenu();
}

void MapGenerator::ExtendMapBorders(const int Radius) {

	BorderX = BorderX - Radius < BorderX ? BorderX - Radius : BorderX;
	BorderXEnd = BorderXEnd + Radius > BorderXEnd ? BorderXEnd + Radius : BorderX;
	BorderY = BorderY - Radius < BorderY ? BorderY - Radius : BorderY;
	BorderYEnd = BorderYEnd + Radius > BorderYEnd ? BorderYEnd + Radius : BorderY;
}

void MapGenerator::HandleMovement() {

	cout << "Which direction do you want to move to ? " << '\n';
	cout << "use W S A D keys to move " << '\n';
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

void MapGenerator::Move(const int MoveDir) {

	const int Radius = static_cast<int>(PlayerCharacters[0].lock()->GetLightRadius());

	Map[PlayerX][PlayerY] = PATH;

	switch (MoveDir) {
		case 0:
			++PlayerY;
			if (PlayerY + Radius > BorderYEnd)
				++BorderYEnd;
			break;
		case 1:
			--PlayerY;
			if (PlayerY - Radius < BorderY)
				--BorderY;
			break;
		case 2:
			++PlayerX;
			if (PlayerX + Radius > BorderXEnd)
				++BorderXEnd;
			break;
		case 3:
			--PlayerX;
			if (PlayerX - Radius < BorderX)
				--BorderX;
			break;
		default:
			break;
	}

	if (Map[PlayerX][PlayerY] == ENEMY) {

		ClearCharGrid();
		DrawPlayGrid();

		auto enemies = GetEnemies(PlayerX, PlayerY);
		GenerateCharacterGridPositions();
		AddCharactersToGrid();

		GameplayStatics::InitiateCombatMode(std::move(enemies));
	}

	Map[PlayerX][PlayerY] = PLAYER;

	ShowPosition();
}

void MapGenerator::DisplayErrorMessage(const string& Message) {
	cout << ANSI_COLOR_RED << Message << ANSI_COLOR_RESET << '\n' << '\n';
}

void MapGenerator::InitDistanceBFS(const int X, const int Y, const int Step) {
	BFS_Distance(X, Y, Step);
}

void MapGenerator::BFS_Distance(const int X, const int Y, const int Step) {

	Distance[X][Y] = Step;

	for (int i = 0; i < 4; i++) {
		const int NewX = X + DX[i];
		const int NewY = Y + DY[i];

		if (NewX >= 2 && NewX < MAX_X - 2 && NewY >= 2 && NewY < MAX_Y - 2 &&
			Map[NewX][NewY] == PATH && (Distance[NewX][NewY] == 0 || Distance[NewX][NewY] > Step + 1)) {
			BFS_Distance(NewX, NewY, Step + 1);
		}
	}
}

void MapGenerator::InitEnemies() {
	AddRandomMapEnemies();
}

void MapGenerator::AddRandomMapEnemies() {

	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			int percent = static_cast<int>(TotalSteps * 0.05);
			int Random = GameplayStatics::GetRandInt(0, TotalSteps);

			if (/*Random <= percent && */Steps[i][j] > 0 && Map[i][j] != PLAYER) {
				constexpr int MAP_LEVEL = 1;
				Map[i][j] = ENEMY;

				int PowerLevelLow = (Distance[i][j] + 2) * MAP_LEVEL;
				int PowerLevelHigh = PowerLevelLow;
				PowerLevelLow -= static_cast<int>(PowerLevelLow * 0.3);
				PowerLevelHigh += static_cast<int>(PowerLevelHigh * 0.3);
				int PowerLevel = GameplayStatics::GetRandInt(PowerLevelLow, PowerLevelHigh);

				int RandomEnemies = 0;
				int TotalPower = 0;
				vector<int> CharClasses;

				while (PowerLevel) {
					if (int RandomClass = rand() % 6 + 50; PowerLevel - CharDb::Data[static_cast<ECharacterClass>(RandomClass)].PowerLevel >= 0) {
						++RandomEnemies;
						CharClasses.push_back(RandomClass);
						TotalPower += CharDb::Data[static_cast<ECharacterClass>(RandomClass)].PowerLevel;
						PowerLevel -= CharDb::Data[static_cast<ECharacterClass>(RandomClass)].PowerLevel;
						if (RandomEnemies == 7) {
							++RandomEnemies;
							const int TempPowerLevel = PowerLevel > 6 ? 6 : PowerLevel;
							CharClasses.push_back(50 + TempPowerLevel - 1);
							TotalPower += CharDb::Data[static_cast<ECharacterClass>(RandomClass)].PowerLevel;
							PowerLevel -= TempPowerLevel;
							break;
						}
					}
				}

				vector<shared_ptr<Character>> EnemiesVector;
				map<char, weak_ptr<Character>> EnemiesMap;

				for (int k = 0; k < RandomEnemies; k++) { 
					EnemiesVector.push_back(make_shared<EnemyCharacter>(static_cast<ECharacterClass>(CharClasses[k])));
					EnemiesMap['A' + k] = EnemiesVector[k];
				}
				EnemyMap.push_back(std::move(EnemiesVector));
				EnemyMapXY.emplace_back(i, j);	
				EnemyNameMap.push_back(EnemiesMap);
				PowerLevels.push_back(TotalPower);
				
				// restart static instance counter
				EnemyCharacter::_n = 0;
			}
		}
	}
}

vector<weak_ptr<Character>> MapGenerator::GetEnemies(const int X, const int Y) {
	EnemyIndex = 0;
	for (auto& [first, second] : EnemyMapXY) {
		if (first == X && second == Y) break;
		++EnemyIndex;
	}

	vector<weak_ptr<Character>> WPtr;
	for (auto& enemy : EnemyMap.at(EnemyIndex))
		WPtr.push_back(weak_ptr<Character>(enemy));

	return WPtr;
}

weak_ptr<Character> MapGenerator::GetCharacterFromAlias(const char Target) {
	if (!CharMap.contains(Target)) return {};
	const int x = CharMap.at(Target).first;
	const int y = CharMap.at(Target).second;
	return CharGrid[x][y].Here;
}

void MapGenerator::DisplayGrid() const {
	for (const auto& i : Grid) {
		for (int j = 0; j < GRID_Y; j++) {
			if (j < 17) cout << ANSI_COLOR_GREEN; else cout << ANSI_COLOR_PASTELLE_BROWN;
			if (j >= 64) cout << ANSI_COLOR_RED;
			cout << i[j];
		}
		cout << '\n';
	}
}

void MapGenerator::DrawPlayGrid() {

	for (int i = 0; i < GRID_X; i++) {
		for (int j = 0; j < GRID_Y; j++) {		
			// this is the field where character placement will be
			if (i % 4 == 2 && j % 8 == 4) {
				Grid[i][j] = ' ';
			}
			else {
				if (i % 4 == 0) {
					if (j % 8 == 0) Grid[i][j] = '+';
					else Grid[i][j] = '-';
				}
				else {
					if (j % 8 == 0) Grid[i][j] = '|';
					else Grid[i][j] = ' ';
				}
			}
		}
	}
}

void MapGenerator::GenerateCharacterGridPositions() {

	for (auto& [grid1, grid2] : EnemyNameMap.at(EnemyIndex)) {

		int First, Second;
		do {
			const int Random = rand() % 10;
			First = EnemyStartPositions[Random].first;
			Second = EnemyStartPositions[Random].second;
		} while (CharGrid[First][Second].Here.lock().get());

		CharGrid[First][Second].Here = grid2;
		CharMap[grid1] = make_pair(First, Second);
	}

	//add player characters
	for (int i = 0; i < PlayerCharacters.size(); i++) {
		CharGrid[i][0].Here = PlayerCharacters[i];
		CharMap['0' + i] = make_pair(i, 0);
	}
}

void MapGenerator::AddCharactersToGrid() {

	for (int i = 0; i < CHAR_GRID_X; i++)
		for (int j = 0; j < CHAR_GRID_Y; j++) {
			const int x = i * 4 + 2;
			const int y = j * 8 + 4;

			if (CharGrid[i][j].Here.lock())
				Grid[x][y] = CharGrid[i][j].Here.lock()->GetAlias();
		}
}

bool MapGenerator::AddCharacterToCharGrid(const std::shared_ptr<Character>& Instigator, const std::weak_ptr<Character>& Summon) {

	const int x = CharMap.at(Instigator->GetAlias()).first;
	const int y = CharMap.at(Instigator->GetAlias()).second;

	int xx = 0, yy = 0;
	bool bHasSpawned = false;
	for (int i = 0; i < 8; i++) {
		xx = x + DX8[i];
		yy = y + DY8[i];

		if (xx >= 0 && xx < CHAR_GRID_X && yy >= 0 && yy < CHAR_GRID_Y) {
			if (!CharGrid[xx][yy].Here.lock()) {
				CharGrid[xx][yy].Here = Summon;
				bHasSpawned = true;
				break;
			}
		}
	}
	
	if (bHasSpawned) {
		CharMap[CharGrid[xx][yy].Here.lock()->GetAlias()].first = xx;
		CharMap[CharGrid[xx][yy].Here.lock()->GetAlias()].second = yy;
	}

	Grid[xx * 4 + 2][yy * 8 + 4] = Summon.lock()->GetAlias();

	UpdateCharGrid();

	return bHasSpawned;
}

void MapGenerator::UpdateCharGrid() {

	for (int i = 0; i < CHAR_GRID_X; ++i) 
		for (int j = 0; j < CHAR_GRID_Y; ++j) 
			for (int l = 0; l < 2; l++)
				for (int k = 0; k < 8; k++) {

					const int x = i + DX8[k];
					const int y = j + DY8[k];

					if (x >= 0 && x < CHAR_GRID_X && y >= 0 && y < CHAR_GRID_Y) {

						if (l && CharGrid[i][j].Neighbors[k].lock() && !CharGrid[x][y].Here.lock())
							CharGrid[x][y].Here = CharGrid[i][j].Neighbors[k];

						else CharGrid[i][j].Neighbors[k] = CharGrid[x][y].Here;
					}
					else CharGrid[i][j].Neighbors[k] = weak_ptr<Character>();
				}
}

void MapGenerator::ClearCharGrid() {
	for (auto& GridNodes : CharGrid)
		for (auto& [Here, Neighbors] : GridNodes) {
			Here = {};
			for (auto& Neighbor : Neighbors) Neighbor = {};
		}
}

void MapGenerator::MoveCharacterOnGrid(const Character& InCharacter, const EDirection Direction) {

	const int x = CharMap.at(InCharacter.GetAlias()).first;
	const int y = CharMap.at(InCharacter.GetAlias()).second;

	const int dir = static_cast<int>(Direction);
	const int xx = x + DX8[dir];
	const int yy = y + DY8[dir];

	// Swap pointers and aliases from character source to character destination
	CharGrid[xx][yy].Here = CharGrid[x][y].Here;
	CharGrid[xx][yy].Here.lock()->SetAlias(CharGrid[x][y].Here.lock()->GetAlias());
	CharGrid[x][y].Here = {};

#pragma warning(push)
#pragma warning(disable: 6011) // Suppressing warning C6011: dereferencing null pointer
	// Update map with alias
	CharMap[CharGrid[xx][yy].Here.lock()->GetAlias()].first = xx;
	CharMap[CharGrid[xx][yy].Here.lock()->GetAlias()].second = yy;
#pragma warning(pop)

	// Move on the real grid
	Grid[xx * 4 + 2][yy * 8 + 4] = InCharacter.GetAlias();
	Grid[x * 4 + 2][y * 8 + 4] = ' ';

	// Update _char_grid
	UpdateCharGrid();
}

vector<string> MapGenerator::GetCombatDirections(const Character* InCharacter, OUT map<int, EDirection>& InMap) const {
	const int x = CharMap.at(InCharacter->GetAlias()).first;
	const int y = CharMap.at(InCharacter->GetAlias()).second;
	const vector<string> Directions = { "NORTH", "NORTHEAST", "EAST", "SOUTHEAST", "SOUTH", "SOUTHWEST", "WEST", "NORTHWEST" };
	vector<string> v;

	for (int i = 0; i < 8; i++) {
		const int xx = x + DX8[i];
		const int yy = y + DY8[i];
		if (xx < 0 || xx >= CHAR_GRID_X || yy < 0 || yy >= CHAR_GRID_Y) continue;
		if (CharGrid[xx][yy].Here.lock()) continue;

		v.push_back(Directions[i]);

		// Map values from InteractiveDisplay to real values
		InMap[static_cast<int>(v.size()) - 1] = static_cast<EDirection>(i);
	}
	return v;
}

vector<Character*> MapGenerator::GetCharactersInRange(const Character* InCharacter) {
	vector<Character*> v;
	const int x = CharMap.at(InCharacter->GetAlias()).first;
	const int y = CharMap.at(InCharacter->GetAlias()).second;

	for (const auto& c : CharGrid[x][y].Neighbors)
		if (c.lock()) v.push_back(c.lock().get());

	return v;
}

int MapGenerator::GetEnemyIdx(const char Alias) {
	if (UPPER(Alias) < 'A' || UPPER(Alias) > 'Z') return -1;
	
	if (const auto character = GetCharacterFromAlias(UPPER(Alias)).lock())
		for (int i = 0; i < static_cast<int>(EnemyMap.at(EnemyIndex).size()); i++)
			if (character == EnemyMap.at(EnemyIndex)[i]) return i;
	
	return -1;
}

int MapGenerator::GetPlayerIdx(const char Alias) {
	const auto Character = GetCharacterFromAlias(Alias);
	for (int i = 0; i < PlayerCharacters.size(); i++)
		if (std::static_pointer_cast<PlayerCharacter>(Character.lock()) == PlayerCharacters[i].lock()) return i;
	
	return -1;
}

void MapGenerator::KillEnemy(const int Idx) {
	if (const Character* Character = EnemyMap.at(EnemyIndex)[Idx].get()) {
		const int x = CharMap.at(Character->GetAlias()).first;
		const int y = CharMap.at(Character->GetAlias()).second;
		CharGrid[x][y].Here = {};
		UpdateCharGrid();	// to make it more efficient we can just update the killed characters neighbours' neighbours
		Grid[x * 4 + 2][y * 8 + 4] = ' ';

		CharMap.erase(Character->GetAlias());
		EnemyMap.at(EnemyIndex)[Idx].reset();
	}
}

void MapGenerator::KillEnemy(const Character* InCharacter) {
	const int x = CharMap.at(InCharacter->GetAlias()).first;
	const int y = CharMap.at(InCharacter->GetAlias()).second;
	CharGrid[x][y].Here = {};
	Grid[x * 4 + 2][y * 8 + 4] = ' ';

	CharMap.erase(InCharacter->GetAlias());

	for (auto& enemy : EnemyMap.at(EnemyIndex))
		if (enemy.get() == InCharacter) enemy.reset();
}
