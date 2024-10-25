#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>

RoomData CreateRoom(const std::string& inName, const std::string& inDesciption) {
	RoomData room = {};
	room.Name = inName;
	room.Description = inDesciption;

	return room;
}

void AddExitToRoom(RoomData& roomToEdit, const std::string& exitName, int targetRoomIndex) {
	RoomExitData exit = {};
	exit.Name = exitName;
	exit.TargetRoomIndex = targetRoomIndex;
	roomToEdit.Exits.push_back(exit);
}

void InitializeGame(PlayerState& playerState, WorldState& worldState)
{
	playerState.WantsToLook = true;
	playerState.CurrentRoomIndex = 2;

	// Index 0:
	RoomData room1 = CreateRoom("Room1", "Room 1 Description");
	AddExitToRoom(room1,"room2",1);
	worldState.Rooms.push_back(room1);

	// Index 1:
	RoomData room2 = CreateRoom("Room2", "Room 2 Description");
	AddExitToRoom(room2, "room1", 0);
	AddExitToRoom(room2, "room3", 2);
	worldState.Rooms.push_back(room2);

	// Index 2:
	RoomData room3 = CreateRoom("Room3", "Room 3 Description");
	AddExitToRoom(room3, "room2", 1);
	worldState.Rooms.push_back(room3);
}

void GetInput(PlayerState& playerState, const WorldState& worldState)
{
	playerState.WantsToLook = false;

	printf("What do you do?\n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	if (command.Verb == "quit")
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "help")
	{
		printf("Command List: look, quit, go [place], get keys, sleep\n");
	}
	else if (command.Verb == "look")
	{
		playerState.WantsToLook = true;
	}
	else
	{
		printf("I don't understand\n");
	}
	printf("\n");

}

void RenderGame(const PlayerState& playerState, const WorldState& worldState)
{

	if (playerState.WantsToLook == true) {
		RoomData currentRoom = worldState.Rooms[playerState.CurrentRoomIndex];
		printf("==================================== \n");
		printf("Location: %s \n", currentRoom.Name.c_str());
		printf("%s \n\n", currentRoom.Description.c_str());
		printf("Exits: \n");

		for (unsigned int i = 0; i < currentRoom.Exits.size(); ++i) {
			printf("%s\n", currentRoom.Exits[i].Name.c_str());
		}

		//printf("Size of currentRoom Exit: %d \n ", currentRoom.Exits.size());

		printf("\n");
	};

}

void UpdateGame(PlayerState& playerState, WorldState& worldState)
{

}

void CleanupGame(PlayerState& playerState, WorldState& worldState)
{
	printf("Quitting...\n\n");
}