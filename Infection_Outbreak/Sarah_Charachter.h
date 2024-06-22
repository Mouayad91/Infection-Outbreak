#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sarah_Charachter.generated.h"

UCLASS()
class INFECTION_OUTBREAK_API ASarah_Charachter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASarah_Charachter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Backward/Forward input
	void MoveForward(float value);

	// left/right input
	void MoveRight(float value);

	/*
	* Called throw given input at spec rate to turn around
	* @param Rate
	*/
	void TurnAround(float Rate);

	/*
	* Called throw given input at spec rate to look up and down
	* @param Rate
	*/
	void LookUp(float Rate);

	/*func that fire weapons*/
	void ShootWeap();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



private:
	//Springarm =Camera Boom
	/*
	* Attach the Camera to the player character (BEHIND)
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	
	// Camera= Player Follow Camera
	/*Player character camera follow*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	
	// (Turn around rate) degree divided by second 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float TurnRate;

	// (Look up and down rate) degree divided by second 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float LookUpRate;


	/*SFX */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight , meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShootSfx;
	
	/*VFX */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ShootVFX;
	
	/*Shoot montage for the gun*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* ShootMontage;
	
public:

	// to return the Uspringarmcomponent type a puplic getter
	// FORCEINLINE less excution and save performance
	FORCEINLINE USpringArmComponent* getSpringArm() const {
		return SpringArm; 
	};

	//return camera subobject
	FORCEINLINE UCameraComponent* getCamera() const {

		return Camera;
	}

};
