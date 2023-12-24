// Fill out your copyright notice in the Description page of Project Settings.


#include "FCTowerPuzzle.h"
#include "Kismet/GameplayStatics.h" 

AFCTowerPuzzle::AFCTowerPuzzle()
{
	moveLocations.SetNum(3);
}

void AFCTowerPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCTowerPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCTowerPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCTowerPuzzle::SelectDisk);
	PlayerInputComponent->BindAxis("LeftAndRight", this, &AFCTowerPuzzle::AxisX);
}
void AFCTowerPuzzle::Tick(float DeltaTime)
{
	HandleAnimation(DeltaTime);
}

void AFCTowerPuzzle::HandleAnimation(float DeltaTime)
{
	if (isAnimating)
	{
		FVector P1;
		if (currentMoveLocation == 0)
		{
			//Just using the initial start location when moving to the first point
			P1 = diskStartLocation;
		}
		else
		{
			P1 = moveLocations[currentMoveLocation - 1];
		}
		FVector nextLocation = moveLocations[currentMoveLocation];
		movingDisk->SetActorLocation(FMath::Lerp(P1, nextLocation, t));
		t += DeltaTime * moveSpeed;
		if (movingDisk->GetActorLocation().Equals(nextLocation, 5.0f))
		{
			movingDisk->SetActorLocation(nextLocation);
			t = 0;
			currentMoveLocation++;
			if (currentMoveLocation >= 3)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), placeSound);
				isAnimating = false;
				EnableInput(Cast<APlayerController>(GetController()));
				CheckSolution();
			}
		}
	}
}

void AFCTowerPuzzle::MoveRight()
{
	currentRod++;
	if (currentRod >= numberOfRods)
	{
		currentRod = 0;
	}
	updateIndex();
}

void AFCTowerPuzzle::MoveLeft()
{
	currentRod--;
	if (currentRod <= -1)
	{
		currentRod = numberOfRods - 1;
	}
	updateIndex();
}

void AFCTowerPuzzle::SelectDisk()
{
	//initial selection
	if (selectedRod < 0)
	{
		if (towers[currentRod].disk.Num() > 0)
		{
			selectedRod = currentRod;
			int currentDisk = towers[selectedRod].disk.Top();
			auto mat = Cast<UStaticMeshComponent>(disks[currentDisk]->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			mat->SetRenderCustomDepth(true);
		}
	}
	//Can't place on the already selected rod
	else if (selectedRod != currentRod)
	{
		//Can't place larger disks on smaller disks. Also need to check if there are no disks on this rod
		if (towers[currentRod].disk.Num() == 0 || towers[selectedRod].disk.Last() > towers[currentRod].disk.Last())
		{
			MoveDisk();
		}
		else
		{
			int currentDisk = towers[selectedRod].disk.Top();
			auto mat = Cast<UStaticMeshComponent>(disks[currentDisk]->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			mat->SetRenderCustomDepth(false);
		}
		selectedRod = -1;
	}
}

void AFCTowerPuzzle::AxisX(float value)
{
	if (!moveAxis)
	{
		if (value == 1)
		{
			moveAxis = true;
			MoveRight();
		}
		else if (value == -1)
		{
			MoveLeft();
			moveAxis = true;
		}
	}
	if (value == 0)
	{
		moveAxis = false;
	}
	
}

void AFCTowerPuzzle::MoveDisk()
{
	//move disk
	int currentDisk = towers[selectedRod].disk.Pop();
	towers[currentRod].disk.Add(currentDisk);

	SetupAnimation(currentDisk);
}

void AFCTowerPuzzle::SetupAnimation(int currentDisk)
{
	movingDisk = disks[currentDisk];
	auto mat = Cast<UStaticMeshComponent>(movingDisk->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	mat->SetRenderCustomDepth(false);

	diskStartLocation = movingDisk->GetActorLocation();

	float Y = startLocation.Y - 100 * currentRod;
	float Z = startLocation.Z + 6.0f * (towers[currentRod].disk.Num() - 1);

	moveLocations[0] = FVector(diskStartLocation.X, diskStartLocation.Y, startLocation.Z + 70);
	moveLocations[1] = FVector(diskStartLocation.X, Y, startLocation.Z + 70);
	moveLocations[2] = FVector(diskStartLocation.X, Y, Z);

	isAnimating = true;
	currentMoveLocation = 0;
	UGameplayStatics::PlaySound2D(GetWorld(), moveSound);

	DisableInput(Cast<APlayerController>(GetController()));
}

void AFCTowerPuzzle::StartPuzzle()
{
	Super::StartPuzzle();

	currentRod = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	rods.SetNum(numberOfRods);
	for (int i = 0; i < numberOfRods; i++)
	{
		float Y = startLocation.Y - i * 100;
		FVector location(startLocation.X, Y, startLocation.Z);

		AActor* rod = GetWorld()->SpawnActor<AActor>(prop, location, FRotator::ZeroRotator, SpawnParams);
		rods[i] = rod;
	}
	towers.SetNum(numberOfRods);
	towers[0].disk.SetNum(numberOfDisks);
	disks.SetNum(numberOfDisks);
	for (int i = 0; i < numberOfDisks; i++)
	{
		towers[0].disk[i] = i;

		float Z = startLocation.Z + i * 6;
		FVector location(startLocation.X, startLocation.Y, Z);
		AActor* disk = GetWorld()->SpawnActor<AActor>(diskProp, location, FRotator::ZeroRotator, SpawnParams);
		float scale = 1.0f - 0.25f * i;
		disk->SetActorScale3D(FVector(scale, scale, disk->GetActorScale().Z));
		auto mat = Cast<UStaticMeshComponent>(disk->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		mat->SetMaterial(0, materials[i]);
		disks[i] = disk;
	}
	SetUp();
}

void AFCTowerPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();

	towers.Empty();

	for (int i = 0; i < numberOfDisks; i ++)
	{
		disks[i]->Destroy();
	}
	for (int i = 0; i < numberOfRods; i++)
	{
		rods[i]->Destroy();
	}
}

void AFCTowerPuzzle::CheckSolution()
{
	int sum = 0;
	//No point checking if the final rod doesn't have all the disks
	if (towers[numberOfRods - 1].disk.Num() == numberOfDisks)
	{
		for (int i = 0; i < numberOfDisks; i++)
		{
			if (towers[numberOfRods - 1].disk[i] == i)
			{
				sum++;
			}
		}
	}
	if (sum == numberOfDisks)
	{
		SolvePuzzle();
	}
}

void AFCTowerPuzzle::Toggle_Implementation()
{
}

void AFCTowerPuzzle::SetUp_Implementation()
{
}

void AFCTowerPuzzle::updateIndex_Implementation()
{
}
