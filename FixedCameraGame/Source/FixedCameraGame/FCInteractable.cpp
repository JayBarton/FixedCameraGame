// Fill out your copyright notice in the Description page of Project Settings.


#include "FCInteractable.h"
#include "Components/BoxComponent.h"
// Sets default values
AFCInteractable::AFCInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetCollisionProfileName(TEXT("InteractBox"));
	/*BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);*/
	BoxComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFCInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFCInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFCInteractable::Action_Implementation()
{
	return;
}


