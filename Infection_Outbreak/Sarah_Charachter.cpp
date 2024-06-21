

#include "Sarah_Charachter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


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


	/*Player camera et up*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// Attach camera to the end of the springarm
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Camera must follow springarm rotation so = flase
	Camera->bUsePawnControlRotation = false;

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

}

