#pragma once 

#include "Header/GlobalEnum.h"
#include "CollisionShape.h"
#include "CollisionStructTypes.generated.h"

USTRUCT(BlueprintType)
struct FJWCollisionShape
{
	GENERATED_BODY()

public:
	void MakeCollisionShape();
	

	UPROPERTY(EditAnywhere, Category = "Collision")
	EJWCollisionShapeType CollisionShapeTye;

	UPROPERTY(EditAnywhere, Category = "Extent")
	FVector ShapeExtent;

	FCollisionShape CollisionShape;

};


