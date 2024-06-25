#include "Sarah_Charachter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h" 
#include "Sound/SoundCue.h" 
#include "Engine/SkeletalMeshSocket.h" 
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"

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
    SpringArm->SocketOffset = FVector(0.f, 50.f, 50.f);

    /*Player camera set up*/
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // Attach camera to the end of the springarm
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    // Camera must follow springarm rotation so = flase
    Camera->bUsePawnControlRotation = false;

    //stop rotation on controller rotation just on YAW true
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // character movement configuration
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 520.f, 0.f);
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

        FVector SmokeEnd;
        bool bSmokeEnd = GetSmokeEndLocation(SocketTransform.GetLocation(), SmokeEnd);

        if (bSmokeEnd) {
            if (HitParticles) {
                UGameplayStatics::SpawnEmitterAtLocation(
                    GetWorld(),
                    HitParticles,
                    SmokeEnd
                );
            }

            UParticleSystemComponent* Smoke = UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),
                BulletSmoke,
                SocketTransform
            );

            if (Smoke) {
                Smoke->SetVectorParameter(FName("Target"), SmokeEnd);
            }
        }

        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

        if (AnimInstance && ShootMontage) {
            AnimInstance->Montage_Play(ShootMontage);
            AnimInstance->Montage_JumpToSection(FName("FireStart"));
        }
    }
}

bool ASarah_Charachter::GetSmokeEndLocation(const FVector& ShootSocketLocation, FVector& OutSmokeLocation)
{
    // Get Screen location
    FVector2D ViewportSize;

    if (GEngine && GEngine->GameViewport) {
        GEngine->GameViewport->GetViewportSize(ViewportSize);
    }

    // screen space location of CH
    FVector2D CHLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
    CHLocation.Y -= 50.f;

    FVector CHPosition;
    FVector CHDirection;

    // world direction and world position of the CH
    bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
        CHLocation, CHPosition, CHDirection);

    if (bScreenToWorld) {
        FHitResult TraceHit;
        const FVector Begins{ CHPosition };
        const FVector Ends{ CHPosition + CHDirection * 60000.f };

        OutSmokeLocation = Ends;

        // line trace by channel from the world location of the crosshair
        GetWorld()->LineTraceSingleByChannel(TraceHit,
            Begins,
            Ends,
            ECollisionChannel::ECC_Visibility);

        // did trace hit any object?
        if (TraceHit.bBlockingHit) {
            // Line trace by channel from the gun
            FHitResult GunTraceHit;
            const FVector GunTraceStart{ ShootSocketLocation };
            const FVector GunTraceEnd{ OutSmokeLocation };

            GetWorld()->LineTraceSingleByChannel(
                GunTraceHit,
                GunTraceStart,
                GunTraceEnd,
                ECollisionChannel::ECC_Visibility);

            if (GunTraceHit.bBlockingHit) {
                OutSmokeLocation = GunTraceHit.Location;
            }

            //OutSmokeLocation = TraceHit.Location;

            return true;
        }
    }

    return false;
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
