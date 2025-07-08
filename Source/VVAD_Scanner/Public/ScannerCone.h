// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScannerBase.h"
#include "ScannerCone.generated.h"

UENUM()
enum class EConeScanPattern {
	Random,
	Sunflower,
	Circles,
	Grid
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VVAD_SCANNER_API UScannerCone : public UScannerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Cone")
	float ConeAngle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Cone")
	EConeScanPattern ScanPattern;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Cone")
	float PointDensity = 20.f;
	
public:
	UScannerCone();

	UFUNCTION(BlueprintCallable, Category = "Scanner | Cone")
	void UpdateAngle(float NewAngle);
	UFUNCTION(BlueprintCallable, Category = "Scanner | Cone")
	void UpdateDensity(float NewDensity);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void GenerateSunflowerPattern(float alpha);
	
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FVector> SunflowerPattern;
	UPROPERTY()
	int SunflowerFointCount = 200;

};
