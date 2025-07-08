// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScanPointsCell.h"
#include "Math/Range.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

#include "ScanPointsGrid.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVVADscanner, Display, All);

UCLASS()
class VVAD_SCANNER_API AScanPointsGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Optimization", meta = (ToolTip = "Size of a cell(chunk) of a virtual grid. \nI do not recommend setting this value below 1000"))
	float CellSize;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Optimization", meta = (ToolTip = "Maximum points in one StaticMeshInstance component. \nAdding new points starting to get expensive with amount of existing instances. \nReduce this value if you experienceing lag when adding new points"))
	int MaxPointsPerHolder;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Optimization", meta = (ToolTip = "Maximum amount of StaticMeshInstance components in one cell. \nLess value - less max points, less computations"))
	int MaxHoldersPerCell;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Optimization", meta = (ToolTip = "When reached MaxHolders and MaxPoints, will start to relpace random old points with new ones"))
	bool bCanMoveOldPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Optimization", meta = (ToolTip = "Random offset added to each new point. Helps to reduce overdraw and z-fighting"))
	float PointRandomOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Optimization", meta = (ToolTip = "The distances where culling fade begins and ends"))
	FFloatRange CullDistance;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals")
	UStaticMesh* Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals")
	UMaterialInterface* Material;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Visuals")
	float PointScale;

	// Sets default values for this actor's properties
	AScanPointsGrid();

	UPROPERTY()
	TMap<FIntVector, AScanPointsCell*> Grid;

	UPROPERTY(BlueprintReadOnly, Category = "PointGrid | Debug")
	int PointsCountTotal = 0;
	UPROPERTY(BlueprintReadOnly, Category = "PointGrid | Debug")
	int HoldersCountTotal = 0;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "PointGrid")
	bool AddPoints(TArray<FVector> pts, TArray<float> ptData);

	UFUNCTION(BlueprintCallable, Category = "PointGrid")
	void SetMaterial(UMaterialInterface* NewMaterial);

	UFUNCTION(BlueprintCallable, Category = "PointGrid")
	void ClearAll();

private:

	const FVector PosToLocation(FIntVector pos) {
		return FVector(pos.X, pos.Y, pos.Z) * CellSize + FVector(CellSize / 2.);
	};

	const FIntVector LocationToPos(FVector location) {
		location = (location / CellSize);
		return FIntVector(floor(location.X), floor(location.Y), floor(location.Z));
	};

	UInstancedStaticMeshComponent* CreateInstanceHolder(FIntVector pos);

	AScanPointsCell* CreateCell(FIntVector pos);

	AScanPointsCell* GetGridCell(FIntVector pos);

};
