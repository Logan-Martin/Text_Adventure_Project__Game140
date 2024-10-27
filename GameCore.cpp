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
	printf("\nWelcome to The Haunted House!");
	printf("\nAs you enter, the door slams behind you. Find a way out!");
	printf("\n");

	playerState.WantsToLook = true;
	playerState.CurrentRoomIndex = 0;
	playerState.HasKey = false;

	// Index 0:
	RoomData room1 = CreateRoom("entrance", "There's a nice coat rack and a creaky wooden floor. Not much here.", false);
	AddExitToRoom(room1,"hallway",1,false);
	worldState.Rooms.push_back(room1);

	// Index 1:
	RoomData room2 = CreateRoom("hallway", "Some blurry pictures hang on the wall.", false);
	AddExitToRoom(room2, "entrance", 0,false);
	AddExitToRoom(room2, "kitchen", 2,false);
	worldState.Rooms.push_back(room2);

	// Index 2:
	RoomData room3 = CreateRoom("kitchen", "Dirty dishes are piled up, it looks like people haven't lived here in a while.", true);
	AddExitToRoom(room3, "hallway", 1,false);
	AddExitToRoom(room3, "livingroom", 3, true);
	worldState.Rooms.push_back(room3);

	// Index 3:
	RoomData room4 = CreateRoom("livingroom", "There's an old TV playing static, a faded green couch, and not much else.", false);
	AddExitToRoom(room4, "kitchen", 2, false);
	worldState.Rooms.push_back(room4);
}

void GetInput(PlayerState& playerState, const WorldState& worldState)
{
	playerState.WantsToLook = false;
	playerState.DesiredExit = "";
	playerState.DesiredPickUp = "";

	printf("What do you do? (Type 'help' for a list of commands.) \n");
	printf("> ");
	TextAdventureCommand command = ParseAdventureCommand();
	if (command.Verb == "quit")
	{
		playerState.WantsToExit = true;
	}
	else if (command.Verb == "help")
	{
		printf("Command List: look, quit, go [place], get [item]\n");
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
		printf("\n==================================== \n");
		printf("Location: %s \n", currentRoom.Name.c_str());
		printf("Description: %s \n\n", currentRoom.Description.c_str());

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
				printf("\n");
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