#include "Sarah_AnimInstance.h"
#include "Sarah_Charachter.h"
#include "GameFramework/CharacterMovementComponent.h"



void USarah_AnimInstance::UpdateAnimationSetting(float DeltaTime)
{
	if (Sarah_Character == nullptr) {

		Sarah_Character = Cast<ASarah_Charachter>(TryGetPawnOwner());


	}

	if (Sarah_Character) {

		// get player speed from velocity
		FVector Velocity{ Sarah_Character->GetVelocity() };

		Velocity.Z = 0;
		Speed = Velocity.Size();

		// is player character in the air?
		bIsInTheAir = Sarah_Character->GetCharacterMovement()->IsFalling();

		// is player character moving?
		if (Sarah_Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) {

			bIsAccelerating = true;
		}
		else {
			bIsAccelerating = false;
		}
	}

}

void USarah_AnimInstance::NativeInitializeAnimation()
{

	Sarah_Character = Cast<ASarah_Charachter>(TryGetPawnOwner());

}