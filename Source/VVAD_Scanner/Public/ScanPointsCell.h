// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "ScanPointsCell.generated.h"

UCLASS()
class VVAD_SCANNER_API AScanPointsCell : public AActor {
	GENERATED_BODY()
public:
	AScanPointsCell();

	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> InstanceHolders;
	UPROPERTY()
	bool Loaded = true;

	UFUNCTION()
	void SetMaterial(UMaterialInterface* NewMaterial);
	UFUNCTION()
	void prepareForDestroy();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay (const EEndPlayReason::Type EndPlayReason)  override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
