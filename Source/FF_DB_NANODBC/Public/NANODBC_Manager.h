// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "NANODBC_Manager.generated.h"

UCLASS()
class FF_DB_NANODBC_API ANANODBC_Manager : public AActor
{
	GENERATED_BODY()
	
protected:
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the game end or when destroyed.
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	
	// Sets default values for this actor's properties.
	ANANODBC_Manager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};