// Fill out your copyright notice in the Description page of Project Settings.


#include "FCWeapon.h"
#include "FCPlayer.h"

// Sets default values
AFCWeapon::AFCWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AFCWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFCWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFCWeapon::Fire()
{
	/*if (currentAmmo > 0)
	{
		currentAmmo--;
		UE_LOG(LogTemp, Warning, TEXT("BANG"));
		UE_LOG(LogTemp, Warning, TEXT("%i"), currentAmmo);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("reloading"));

		player->Reload();
	}*/
	UE_LOG(LogTemp, Warning, TEXT("BANG"));
}

void AFCWeapon::Reload(int32 toAdd)
{
	//currentAmmo += toAdd;
}

void AFCWeapon::Hide()
{
	MeshComp->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	MeshComp->SetHiddenInGame(true);
}

void AFCWeapon::Show()
{
	if (player)
	{
		MeshComp->SetHiddenInGame(false);
		MeshComp->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, player->WeaponAttachPoint);
	}
}