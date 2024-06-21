// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Sarah_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class INFECTION_OUTBREAK_API USarah_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()


public:

	// Behave like Tick
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationSetting(float DeltaTime);


	// Behave like Begine play but for anim instance
	virtual void NativeInitializeAnimation() override;




private:

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class ASarah_Charachter* Sarah_Character;

	/* player character speed*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	/* to check if the player in the air or not*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInTheAir;
	
	/* to check if the player is moving or not*/
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	
};