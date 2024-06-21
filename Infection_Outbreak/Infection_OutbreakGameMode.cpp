
#include "Infection_OutbreakGameMode.h"
#include "Infection_OutbreakCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInfection_OutbreakGameMode::AInfection_OutbreakGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
