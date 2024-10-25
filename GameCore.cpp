#include "GameCore.h"
#include "CommandParser.h"
#include <stdio.h>

RoomData CreateRoom(const std::string& inName, const std::string& inDesciption, bool inHasKey) {
	RoomData room = {};
	room.Name = inName;
	room.Description = inDesciption;
	room.HasKey = inHasKey;
	return room;
}

void AddExitToRoom(RoomData& roomToEdit, const std::string& exitName, int targetRoomIndex, bool isLocked) {
	RoomExitData exit = {};
	exit.Name = exitName;
	exit.TargetRoomIndex = targetRoomIndex;
	exit.Locked = isLocked;
	roomToEdit.Exits.push_back(exit);
}

void InitializeGame(PlayerState& playerState, WorldState& worldState)
{
	playerState.WantsToLook = true;
	playerState.CurrentRoomIndex = 2;
	playerState.HasKey = false;

	// Index 0:
	RoomData room1 = CreateRoom("Room1", "Room 1 Description", false);
	AddExitToRoom(room1,"room2",1,false);
	worldState.Rooms.push_back(room1);

	// Index 1:
	RoomData room2 = CreateRoom("Room2", "Room 2 Description", false);
	AddExitToRoom(room2, "room1", 0,false);
	AddExitToRoom(room2, "room3", 2,false);
	worldState.Rooms.push_back(room2);

	// Index 2:
	RoomData room3 = CreateRoom("Room3", "Room 3 Description", true);
	AddExitToRoom(room3, "room2", 1,true);
	worldState.Rooms.push_back(room3);
}

void GetInput(PlayerState& playerState, const WorldState& worldState)
{
	playerState.WantsToLook = false;
	playerState.DesiredExit = "";
	playerState.DesiredPickUp = "";

	printf("What do you do?\n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	if (command.Verb == "quit")
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "help")
	{
		printf("Command List: look, quit, go [place], get key, sleep\n");
	}
	else if (command.Verb == "look")
	{
		playerState.WantsToLook = true;
	}
	else if (command.Verb == "get")
	{
		playerState.DesiredPickUp = command.Parameter;
	}
	else if (command.Verb == "go")
	{
		playerState.DesiredExit = command.Parameter;
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

		if (currentRoom.HasKey) {
			printf("There is a key in this room! \n\n");
		}

		printf("Exits: \n");
		for (unsigned int i = 0; i < currentRoom.Exits.size(); ++i) {
			printf("%s\n", currentRoom.Exits[i].Name.c_str());
		}

		printf("\n");

		if (playerState.HasKey == true ) {
			printf("Inventory: \n");
			printf("Key \n");

		}

		//printf("Size of currentRoom Exit: %d \n ", currentRoom.Exits.size());

		printf("\n");
	};

}

void UpdateGame(PlayerState& playerState, WorldState& worldState)
{
	RoomData& currentRoom = worldState.Rooms[playerState.CurrentRoomIndex];
	if (playerState.DesiredExit != "") {

		bool foundExit = false;

		for (unsigned int i = 0; i < currentRoom.Exits.size(); ++i) {
			if (playerState.DesiredExit == currentRoom.Exits[i].Name) {
				foundExit = true;

				if (currentRoom.Exits[i].Locked == false) {
					playerState.CurrentRoomIndex = currentRoom.Exits[i].TargetRoomIndex;
					playerState.WantsToLook = true;
				}
				else {
					if (playerState.HasKey) {
						playerState.CurrentRoomIndex = currentRoom.Exits[i].TargetRoomIndex;
						playerState.WantsToLook = true;
					}
					else
					{
						printf("This door is locked. Please find a key. \n");
					}

				}
				
			}
		}

		if (foundExit == false) {
			printf("I could not find an exit called '%s' \n\n",playerState.DesiredExit.c_str());
		}

	}

	else if (playerState.DesiredPickUp != "") {
		if (playerState.DesiredPickUp == "key") {
			if (currentRoom.HasKey == true ) {
				printf("You picked up a key!");
				playerState.HasKey = true;
				currentRoom.HasKey = false;
			}
			else {
				printf("There is no key in this room.  \n");
			}
		}
		else
		{
			printf("I don't know what '%s' is.", playerState.DesiredPickUp.c_str());
		}

	}

}

void CleanupGame(PlayerState& playerState, WorldState& worldState)
{
	printf("Quitting...\n\n");
}