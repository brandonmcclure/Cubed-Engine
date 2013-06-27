#ifndef VIEW_FRUSTRUM_H
#define VIEW_FRUSTRUM_H

class ViewFrustrum
{
public:
	void Update();

	void SetProjectionMatrix( D3DMATRIX projection );

	bool IsBoxInside( D3DVECTOR3 min, D3DVECTOR3 max );
	bool IsBoxInside( D3DVECTOR3 translation, D3DVECTOR3 min, D3DVECTOR3 max );
	bool IsSphereInside( D3DVECTOR3 translation, float radius );

private:
	D3DMATRIX m_projection;
	D3DPLANE m_planes[5];
};

#endif