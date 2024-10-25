#pragma once
#include <string>
#include <vector>

struct PlayerState
{
	int CurrentRoomIndex;
	std::string DesiredExit;

	bool WantsToExit;
	bool WantsToLook;
};

struct RoomExitData {
	std::string Name;
	int TargetRoomIndex;
};

struct RoomData {
	std::string Name;
	std::string Description;
	std::vector<RoomExitData> Exits;
};
	
struct WorldState
{
	std::vector<RoomData> Rooms;
};

void InitializeGame(PlayerState& playerState, WorldState& worldState);
void GetInput(PlayerState& playerState, const WorldState& worldState);
void RenderGame(const PlayerState& playerState, const WorldState& worldState);
void UpdateGame(PlayerState& playerState, WorldState& worldState);
void CleanupGame(PlayerState& playerState, WorldState& worldState);