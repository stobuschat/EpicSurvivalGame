#include "AI/SurvG_ZombieCharacter.h"

#include "AI/SBotWaypoint.h"
#include "AI/SurvG_ZombieAIController.h"
#include "Player/SBaseCharacter.h"
#include "Player/SCharacter.h"
#include "Player/SPlayerState.h"
/* AI Include */
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
#include "Sound/SoundCue.h"
#include "SurvivalGame/SurvivalGame.h"

// Sets default values
ASurvG_ZombieCharacter::ASurvG_ZombieCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    /* Note: We assign the Controller class in the Blueprint extension of this class
        Because the zombie AIController is a blueprint in content and it's better to avoid content references in code.  */
    /*AIControllerClass = ASurvG_ZombieAIController::StaticClass();*/

    /* Our sensing component to detect players by visibility and noise checks. */
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
    PawnSensingComp->SetPeripheralVisionAngle(60.0f);
    PawnSensingComp->SightRadius = 2000;
    PawnSensingComp->HearingThreshold = 600;
    PawnSensingComp->LOSHearingThreshold = 1200;

    /* Ignore this channel or it will absorb the trace impacts instead of the skeletal mesh */
    GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
    GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
    GetCapsuleComponent()->SetCapsuleRadius(42.0f);

    /* These values are matched up to the CapsuleComponent above and are used to find navigation paths */
    GetMovementComponent()->NavAgentProps.AgentRadius = 42;
    GetMovementComponent()->NavAgentProps.AgentHeight = 192;

    AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieLoopedSoundComp"));
    AudioLoopComp->bAutoActivate = false;
    AudioLoopComp->bAutoDestroy = false;
    AudioLoopComp->SetupAttachment(RootComponent);

    Health = 100;
    MeleeDamage = 24.0f;
    SprintingSpeedModifier = 3.0f;

    /* By default we will not let the AI patrol, we can override this value per-instance. */
    BotType = EBotBehaviorType::Passive;
    SenseTimeOut = 2.5f;

    /* Note: Visual Setup is done in the AI/ZombieCharacter Blueprint file */
}

void ASurvG_ZombieCharacter::BeginPlay()
{
    Super::BeginPlay();

    /* This is the earliest moment we can bind our delegates to the component */
    if (PawnSensingComp)
    {
        PawnSensingComp->OnSeePawn.AddDynamic(this, &ASurvG_ZombieCharacter::OnSeePlayer);
        PawnSensingComp->OnHearNoise.AddDynamic(this, &ASurvG_ZombieCharacter::OnHearNoise);
    }

    BroadcastUpdateAudioLoop(bSensedTarget);

    /* Assign a basic name to identify the bots in the HUD. */
    ASPlayerState* PS = Cast<ASPlayerState>(GetPlayerState());
    if (PS)
    {
        PS->SetPlayerName("Bot");
        PS->SetIsABot(true);
    }
}

void ASurvG_ZombieCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    /* Check if the last time we sensed a player is beyond the time out value to prevent bot from endlessly following a player. */
    if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut &&
        (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
    {
        ASurvG_ZombieAIController* AIController = Cast<ASurvG_ZombieAIController>(GetController());
        if (AIController)
        {
            bSensedTarget = false;
            /* Reset */
            AIController->SetTargetEnemy(nullptr);

            /* Stop playing the hunting sound */
            BroadcastUpdateAudioLoop(false);
        }
    }
}

void ASurvG_ZombieCharacter::OnSeePlayer(APawn* Pawn)
{
    if (!IsAlive())
    {
        return;
    }

    if (!bSensedTarget)
    {
        BroadcastUpdateAudioLoop(true);
    }

    /* Keep track of the time the player was last sensed in order to clear the target */
    LastSeenTime = GetWorld()->GetTimeSeconds();
    bSensedTarget = true;

    ASurvG_ZombieAIController* AIController = Cast<ASurvG_ZombieAIController>(GetController());
    ASBaseCharacter* SensedPawn = Cast<ASBaseCharacter>(Pawn);
    if (AIController && SensedPawn->IsAlive())
    {
        AIController->SetTargetEnemy(SensedPawn);
    }
}

void ASurvG_ZombieCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
    if (!IsAlive())
    {
        return;
    }

    if (!bSensedTarget)
    {
        BroadcastUpdateAudioLoop(true);
    }

    bSensedTarget = true;
    LastHeardTime = GetWorld()->GetTimeSeconds();

    ASurvG_ZombieAIController* AIController = Cast<ASurvG_ZombieAIController>(GetController());
    if (AIController)
    {
        AIController->SetTargetEnemy(PawnInstigator);
    }
}

void ASurvG_ZombieCharacter::PerformMeleeStrike(AActor* HitActor)
{
    if (HitActor && HitActor != this && IsAlive())
    {
        ACharacter* OtherPawn = Cast<ACharacter>(HitActor);
        if (OtherPawn)
        {
            ASPlayerState* MyPS = Cast<ASPlayerState>(GetPlayerState());
            ASPlayerState* OtherPS = Cast<ASPlayerState>(OtherPawn->GetPlayerState());

            if (MyPS && OtherPS)
            {
                if (MyPS->GetTeamNumber() == OtherPS->GetTeamNumber())
                {
                    /* Do not attack other zombies. */
                    return;
                }

                /* Set to prevent a zombie to attack multiple times in a very short time */
                LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

                FPointDamageEvent DmgEvent;
                DmgEvent.DamageTypeClass = PunchDamageType;
                DmgEvent.Damage = MeleeDamage;

                HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);
            }
        }
    }
}

UAudioComponent* ASurvG_ZombieCharacter::PlayCharacterSound(USoundCue* CueToPlay)
{
    if (CueToPlay)
    {
        return UGameplayStatics::SpawnSoundAttached(
            CueToPlay, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
    }

    return nullptr;
}

void ASurvG_ZombieCharacter::PlayHit(
    float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{
    Super::PlayHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, bKilled);

    /* Stop playing the hunting sound */
    if (AudioLoopComp && bKilled)
    {
        AudioLoopComp->Stop();
    }
}

void ASurvG_ZombieCharacter::SimulateMeleeStrike_Implementation()
{
    PlayAnimMontage(MeleeAnimMontage);
    PlayCharacterSound(SoundAttackMelee);
}

bool ASurvG_ZombieCharacter::IsSprinting() const
{
    /* Allow a zombie to sprint when he has seen a player */
    return bSensedTarget && !GetVelocity().IsZero();
}

void ASurvG_ZombieCharacter::BroadcastUpdateAudioLoop_Implementation(bool bNewSensedTarget)
{
    /* Start playing the hunting sound and the "noticed player" sound if the state is about to change */
    if (bNewSensedTarget && !bSensedTarget)
    {
        PlayCharacterSound(SoundPlayerNoticed);

        AudioLoopComp->SetSound(SoundHunting);
        AudioLoopComp->Play();
    }
    else
    {
        if (BotType == EBotBehaviorType::Patrolling)
        {
            AudioLoopComp->SetSound(SoundWandering);
            AudioLoopComp->Play();
        }
        else
        {
            AudioLoopComp->SetSound(SoundIdle);
            AudioLoopComp->Play();
        }
    }
}
