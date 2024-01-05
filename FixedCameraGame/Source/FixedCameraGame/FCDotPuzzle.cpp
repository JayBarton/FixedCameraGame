// Fill out your copyright notice in the Description page of Project Settings.


#include "FCDotPuzzle.h"
#include "Kismet/GameplayStatics.h" 

AFCDotPuzzle::AFCDotPuzzle()
{
	slots.SetNum(numberOfRows);
	for (int i = 0; i < numberOfRows; i++)
	{
		//TODO see if we can't find a better way to do this rather than sizing all of these arrays individually
		if (i % 2 == 0)
		{
			slots[i].row.SetNum(numberOfRows);
			slots[i].rowDefault.SetNum(numberOfRows);
			slots[i].prop.SetNum(numberOfRows);
			slots[i].solution.SetNum(numberOfRows);
		}
		else
		{
			slots[i].row.SetNum(numberOfRows - 1);
			slots[i].rowDefault.SetNum(numberOfRows - 1);
			slots[i].prop.SetNum(numberOfRows - 1);
			slots[i].solution.SetNum(numberOfRows - 1);
		}
	}
}

void AFCDotPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCDotPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCDotPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCDotPuzzle::SelectSlot);

	PlayerInputComponent->BindAxis("LeftAndRight", this, &AFCDotPuzzle::AxisX);

}

void AFCDotPuzzle::Tick(float DeltaTime)
{
}

void AFCDotPuzzle::MoveRight()
{
	auto mat = Cast<UStaticMeshComponent>(slots[0].prop[currentSlot]->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	mat->SetRenderCustomDepth(false);
	currentSlot++;
	if (currentSlot >= numberOfSlots)
	{
		currentSlot = 0;
	}
	mat = Cast<UStaticMeshComponent>(slots[0].prop[currentSlot]->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	mat->SetRenderCustomDepth(true);
	
	UE_LOG(LogTemp, Warning, TEXT("right %i"), currentSlot);
}

void AFCDotPuzzle::MoveLeft()
{
	auto mat = Cast<UStaticMeshComponent>(slots[0].prop[currentSlot]->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	mat->SetRenderCustomDepth(false);
	currentSlot--;
	if (currentSlot < 0)
	{
		currentSlot = numberOfSlots - 1;
	}
	mat = Cast<UStaticMeshComponent>(slots[0].prop[currentSlot]->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	mat->SetRenderCustomDepth(true);

	UE_LOG(LogTemp, Warning, TEXT("left %i"), currentSlot);
}

void AFCDotPuzzle::SelectSlot()
{
	int selectedSlot = currentSlot;

	for (int i = 0; i < numberOfRows; i++)
	{
		//want to skip over the next row if going left from the left most slot or right from the right most slot
		if (selectedSlot < 0)
		{
			//in the case of less than 0, we are moving left from the left most, 
			//so want to "fall through" to 0 of the next row
			selectedSlot = 0;
		}
		else if(selectedSlot < slots[i].row.Num())
		{
			bool right = slots[i].row[selectedSlot];
			if (right)
			{
				slots[i].row[selectedSlot] = false;
				if (slots[i].row.Num() < numberOfRows)
				{
					selectedSlot++;
				}
			}
			else
			{
				slots[i].row[selectedSlot] = true;
				if (slots[i].row.Num() == numberOfRows)
				{
					selectedSlot--;
				}
			}
		}
	}
	UGameplayStatics::PlaySound2D(GetWorld(), selectSound);

	Toggle();
	CheckSolution();
}

void AFCDotPuzzle::AxisX(float value)
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

void AFCDotPuzzle::StartPuzzle()
{
	Super::StartPuzzle();
	currentSlot = 0;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	FVector startLocation = GetActorLocation();

	for (int i = 0; i < numberOfRows; i++)
	{
		slots[i].row = slots[i].rowDefault;

		float YStart = startLocation.Y;
		if (i % 2 != 0)
		{
			YStart -= 100;
		}

		for (int c = 0; c < slots[i].row.Num(); c++)
		{
			float Y = YStart - (c * 200);
			float Z = startLocation.Z - (i * 150);

			FVector location(startLocation.X, Y, Z);

			AActor* slot = GetWorld()->SpawnActor<AActor>(prop, location, FRotator::ZeroRotator, SpawnParams);

			slots[i].prop[c] = slot;
		}
	}
	auto mat = Cast<UStaticMeshComponent>(slots[0].prop[0]->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	mat->SetRenderCustomDepth(true);
	Toggle();
}

void AFCDotPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();
	for (int i = 0; i < numberOfRows; i++)
	{
		for (int c = 0; c < slots[i].prop.Num(); c++)
		{
			slots[i].prop[c]->Destroy();
		}
	}
}

void AFCDotPuzzle::CheckSolution()
{
	for (int i = 0; i < numberOfRows; i++)
	{
		if (slots[i].row != slots[i].solution)
		{
			UE_LOG(LogTemp, Warning, TEXT("that's wrong"));
			return;
		}
	}
	SolvePuzzle();
}

void AFCDotPuzzle::Toggle_Implementation()
{
}

void AFCDotPuzzle::SetUp_Implementation()
{
}

void AFCDotPuzzle::updateIndex_Implementation()
{
}
