#include "Sarah_AnimInstance.h"
#include "Sarah_Charachter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h" 


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

		FRotator AimingRotation = 
			Sarah_Character->GetBaseAimRotation();
		
		FRotator MovementRotation = 
			UKismetMathLibrary::MakeRotFromX(Sarah_Character->GetVelocity());
		


		MovementOffset = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation, AimingRotation
		).Yaw;
		

		if (Sarah_Character->GetVelocity().Size() > 0.f) {

			YawLastOffset = MovementOffset;

		}

		//FString RotateMessage = FString::Printf(TEXT("Base Aim Rotation : %f") ,AimingRotation.Yaw);
		//FString RotateMessages = FString::Printf(TEXT("MovementRoation : %f"), MovementRotation.Yaw);

		
		
		/*if (GEngine) {

			GEngine->AddOnScreenDebugMessage(1, 0.f ,FColor::Yellow, RotateMessages);

		}
		*/
		


	}

}

void USarah_AnimInstance::NativeInitializeAnimation()
{

	Sarah_Character = Cast<ASarah_Charachter>(TryGetPawnOwner());

}