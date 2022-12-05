// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* AcceptableActor = GetAcceptableActor();

    if (AcceptableActor != nullptr)
    {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());
        if (Component != nullptr)
        {

            Component->SetSimulatePhysics(false);
            AcceptableActor->AttachToComponent(this,
                FAttachmentTransformRules::KeepWorldTransform,
                NAME_None);

        }
        UE_LOG(LogTemp, Display, TEXT("Acceptable Actor found: %s. Unlocking . . ."),
            *AcceptableActor->GetActorNameOrLabel());
        Mover->SetShouldMove(true);
    }

    else
    {
        Mover->SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}


AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        bool HasAcceptableTag = Actor->ActorHasTag(AcceptableActorTag);
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");

        if (HasAcceptableTag && !IsGrabbed)
        {
            return Actor;
        }
    }

    return nullptr;

}