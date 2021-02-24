// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCWeapon.generated.h"

class AFCPlayer;
class UAnimationAsset;
class UDamageType;

UCLASS()
class FIXEDCAMERAGAME_API AFCWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UAnimationAsset* fireAnimation;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

	//Not sure if I'm using this anymore...
	void Reload();

	void Hide();

	void Show();

	void FireCoolDown();

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkeletalMeshComponent* MeshComp;

	bool canFire;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	//	int32 currentAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammo")
	int32 maxAmmo;
	int32 ammoID;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
		float fireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	AFCPlayer* player;

};
