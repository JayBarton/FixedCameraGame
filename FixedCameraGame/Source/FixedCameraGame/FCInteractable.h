// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FCInteractableInterface.h"
#include "FCInteractable.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class FIXEDCAMERAGAME_API AFCInteractable : public AActor, public IFCInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFCInteractable(); 

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool active = true;

	//Not ideal, but should work for now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool puzzle = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
		void Action();  // prototype declaration
	virtual void Action_Implementation() override;  // actual implementation of our interact method

};
