#include "Sarah_Charachter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h" 
#include "Sound/SoundCue.h" 
#include "Engine/SkeletalMeshSocket.h" 
#include "DrawDebugHelpers.h"

// Sets default values (Constructor)
ASarah_Charachter::ASarah_Charachter() :

	TurnRate(45.f),
	LookUpRate(45.f)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* SpringArm set up*/
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	// camera follow distance
	SpringArm->TargetArmLength = 350.f;
	//Rotate springarm whenever controller moved
	SpringArm->bUsePawnControlRotation = true;


	/*Player camera set up*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Attach camera to the end of the springarm
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Camera must follow springarm rotation so = flase
	Camera->bUsePawnControlRotation = false;


	//stop rotation on controller rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// character movement configuration
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 520.f,0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.3f;
}



// Called when the game starts or when spawned
void ASarah_Charachter::BeginPlay()
{
	Super::BeginPlay();



}

void ASarah_Charachter::MoveForward(float value)
{
	if ((Controller != nullptr) && value != 0.0f)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, value);
	}
}

void ASarah_Charachter::MoveRight(float value)
{
	if ((Controller != nullptr) && value != 0.0f)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };
		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, value);
	}
}

void ASarah_Charachter::TurnAround(float Rate)
{
	
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());



}

void ASarah_Charachter::LookUp(float Rate)
{

	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());


}

void ASarah_Charachter::ShootWeap()
{

	//UE_LOG(LogTemp, Warning, TEXT("Is firing "));

	if (ShootSfx) {

		UGameplayStatics::PlaySound2D(this, ShootSfx);
	}

	const USkeletalMeshSocket* Socket_VFX = GetMesh()->GetSocketByName("Shoot_VFX");

	if (Socket_VFX) {

		const FTransform SocketTransform = Socket_VFX->GetSocketTransform(GetMesh());

		if (ShootSfx) {

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShootVFX, SocketTransform);
		}


		FHitResult ShootHit;
		const FVector begins{ SocketTransform.GetLocation() };
		const FQuat Rotation{ SocketTransform.GetRotation() };
		const FVector RotationAxis{ Rotation.GetAxisX() };
		const FVector TraceEnd{ begins + RotationAxis * 40'000.f };

		GetWorld()->LineTraceSingleByChannel(ShootHit,begins, TraceEnd,ECollisionChannel::ECC_Visibility);

		if (ShootHit.bBlockingHit) {

			DrawDebugLine(GetWorld(), begins, TraceEnd, FColor::Red, false,3.f);

			DrawDebugPoint(GetWorld(), ShootHit.Location, 5.f,FColor::Yellow, false,3.f);

			if (HitParticles) {

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ShootHit.Location);


			}
		}


		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && ShootMontage) {

		
			AnimInstance->Montage_Play(ShootMontage);
			AnimInstance->Montage_JumpToSection(FName("FireStart"));
		}

	}

}

// Called every frame
void ASarah_Charachter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASarah_Charachter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASarah_Charachter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASarah_Charachter::MoveRight);

	PlayerInputComponent->BindAxis("TurnAround", this, &ASarah_Charachter::TurnAround);
	PlayerInputComponent->BindAxis("LookUp", this, &ASarah_Charachter::LookUp);

	// use inherited function for Lookup and down and turn around
	PlayerInputComponent->BindAxis("TurnOnMouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpDownOnMouse", this, &APawn::AddControllerPitchInput);

	// Jump Input
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	
	PlayerInputComponent->BindAction("ShootBtn", IE_Pressed, this, &ASarah_Charachter::ShootWeap);

}

