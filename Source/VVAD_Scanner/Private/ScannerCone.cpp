// Fill out your copyright notice in the Description page of Project Settings.

#include "ScannerCone.h"

#define SUNFLOWER_PATTERN_ALPHA 2.f

UScannerCone::UScannerCone() {
	ConeAngle = 20.f;
}

void UScannerCone::UpdateAngle(float NewAngle) {
	ConeAngle = NewAngle;
	GenerateSunflowerPattern(SUNFLOWER_PATTERN_ALPHA);
}

void UScannerCone::UpdateDensity(float NewDensity) {
	PointDensity = NewDensity;
	GenerateSunflowerPattern(SUNFLOWER_PATTERN_ALPHA);
}

void UScannerCone::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int Num = CalculateNumTraces();
	if (Num > 0) {
		for (int i = 0; i < Num; i++) {
			FVector v;
			switch (ScanPattern) {
			case EConeScanPattern::Random:
				v = FMath::VRandCone(FVector::ForwardVector, FMath::DegreesToRadians(ConeAngle));
			break;
			case EConeScanPattern::Sunflower:
				v = SunflowerPattern[FMath::RandRange(0, SunflowerPattern.Num() - 1)];
				v.X = 1.f / tan(FMath::DegreesToRadians(ConeAngle));
			break;
			case EConeScanPattern::Circles:
			{
				float density = ConeAngle * PointDensity / 20;
				FVector2D v2d = FMath::RandPointInCircle(density).GetSafeNormal(0.000001);
				float dist = floor(FMath::FRandRange(0,density)) / density;
				v2d *= dist;

				v = FVector(1.f / tan(FMath::DegreesToRadians(ConeAngle)), v2d.X, v2d.Y);
			}
			break;
			case EConeScanPattern::Grid:
			{
				float density = ConeAngle * PointDensity / 20;
				FVector2D v2d = FMath::RandPointInCircle(density);
				v2d.X = floor(v2d.X) / density;
				v2d.Y = floor(v2d.Y) / density;
				v = FVector(1.f / tan(FMath::DegreesToRadians(ConeAngle)), v2d.X, v2d.Y);
			}
				break;
			default:
				break;
			}
			TraceLocalDirection(v);

		}
		
		SendPointsToGrid();
	}
}

void UScannerCone::GenerateSunflowerPattern(float alpha) {
	SunflowerPattern.Empty();
	SunflowerFointCount = ConeAngle * PointDensity;
	const float angle_stride = 360 * ((1 + sqrt(5)) / 2);
	const float b = round(alpha * sqrt(SunflowerFointCount)); //number of boundary points
	for (int k = 0; k < SunflowerFointCount; k++) {
		float r = ((k > (SunflowerFointCount - b)) ? 1.f : (sqrt(k - 0.5) / sqrt(SunflowerFointCount - (b + 1) / 2)));
		float theta = k * angle_stride;
		SunflowerPattern.Add(FVector(0, r * cos(theta), r * sin(theta)));
	}
}

void UScannerCone::BeginPlay() {
	Super::BeginPlay();
	GenerateSunflowerPattern(SUNFLOWER_PATTERN_ALPHA);

}
