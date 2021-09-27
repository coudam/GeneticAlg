// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::Spawn()
{
	isEaten = false;
	float rand_r = std::rand() % 4000;
	float rand_f = std::rand() * 6.282f;
	position = FVector2D(cos(rand_f) * rand_r, sin(rand_f) * rand_r);
	SetActorLocation(FVector(position, 20.f));
	SetActorHiddenInGame(false);
}


void AFood::Eat()
{
	//isEaten = true;
	//SetActorHiddenInGame(true);
	Spawn();
}