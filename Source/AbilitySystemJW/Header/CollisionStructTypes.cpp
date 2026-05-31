#include "Header/CollisionStructTypes.h"

void FJWCollisionShape::MakeCollisionShape()
{
	switch (CollisionShapeTye)
	{
	case EJWCollisionShapeType::Sphere:
	{
		CollisionShape = FCollisionShape::MakeSphere(ShapeExtent.X);
		break;
	}
	case EJWCollisionShapeType::Box:
	{
		CollisionShape = FCollisionShape::MakeBox(FVector(ShapeExtent.X, ShapeExtent.Y, ShapeExtent.Z));
		break;
	}
	case EJWCollisionShapeType::Capsule:
	{
		CollisionShape = FCollisionShape::MakeCapsule(ShapeExtent.X, ShapeExtent.Y);
		break;
	}
	default:
	{
		break;
	}

	}
}
