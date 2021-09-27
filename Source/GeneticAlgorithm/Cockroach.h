// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cockroach.generated.h"

UCLASS()
class GENETICALGORITHM_API ACockroach : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACockroach();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
