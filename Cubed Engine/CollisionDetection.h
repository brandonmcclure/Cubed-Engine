#ifndef _COLLISION_DETECTION_H_
#define _COLLISION_DETECTION_H_

inline bool CheckCollision( Object *ob1, Object *ob2 )
{

	float ob1CX = ob1->GetMyMesh()->GetBoundingSphere()->centre.x;
	float ob1CY = ob1->GetMyMesh()->GetBoundingSphere()->centre.y;
	float ob1CZ = ob1->GetMyMesh()->GetBoundingSphere()->centre.z;
	float ob1R = ob1->GetMyMesh()->GetBoundingSphere()->radius;

	float ob2CX = ob2->GetMyMesh()->GetBoundingSphere()->centre.x;
	float ob2CY = ob2->GetMyMesh()->GetBoundingSphere()->centre.y;
	float ob2CZ = ob2->GetMyMesh()->GetBoundingSphere()->centre.z;
	float ob2R = ob2->GetMyMesh()->GetBoundingSphere()->radius;

	int i = 2^2;
	int b = 4;
	/*
	if ( ( ob1CX - ob2CX )^2 + ( ob1CY - ob2CY )^2 + ( ob1CZ - ob2CZ )^2 < ( ob1R + ob2R )^2 )
		return true;
	else*/
		return false;
}

#endif