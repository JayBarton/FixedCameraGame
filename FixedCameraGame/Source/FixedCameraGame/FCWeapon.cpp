// Fill out your copyright notice in the Description page of Project Settings.


#include "FCWeapon.h"
#include "FCPlayer.h"
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystem.h" 
#include "FCEnemy.h"

#include "DrawDebugHelpers.h" 

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
	
	if (player)
	{

		FVector shotStart = player->GetActorLocation();
		FVector shotDirection = player->GetActorForwardVector();

		float HalfRad = FMath::DegreesToRadians(0.0f);
		//float HalfRad = FMath::DegreesToRadians(BulletSpread);
		shotDirection = FMath::VRandCone(shotDirection, HalfRad, HalfRad);

		FVector TraceEnd = shotStart + (shotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(player);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TracerEndpoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		FHitResult Hit;
		//if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
		if (GetWorld()->LineTraceSingleByChannel(Hit, shotStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			//Blocking hit, process damage
			AActor* HitActor = Hit.GetActor();

			//	SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			int32 damage = 1;

			if (auto enemy = Cast<AFCEnemy>(HitActor))
			{
				enemy->TakeDamage(damage, Hit);
			}
			TracerEndpoint = Hit.ImpactPoint;
		}

		DrawDebugLine(GetWorld(), shotStart, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
	MeshComp->PlayAnimation(fireAnimation, false);
	canFire = false;
	FTimerHandle FireTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AFCWeapon::FireCoolDown, fireRate, false);

}

void AFCWeapon::Reload()
{

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
		canFire = true;
		MeshComp->SetHiddenInGame(false);
		MeshComp->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, player->WeaponAttachPoint);
	}
}

void AFCWeapon::FireCoolDown()
{
	canFire = true;
}
