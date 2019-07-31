// PX2AISteeringWall2D.cpp

#include "PX2AISteeringWall2D.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Wall2D::Wall2D()
{}
//----------------------------------------------------------------------------
Wall2D::Wall2D(Vector2f A, Vector2f B) :m_vA(A), m_vB(B)
{
	CalculateNormal();
}
//----------------------------------------------------------------------------
Wall2D::Wall2D(Vector2f A, Vector2f B, Vector2f N) :m_vA(A), m_vB(B), m_vN(N)
{ }
//----------------------------------------------------------------------------
void Wall2D::Render(bool RenderNormals) const
{
	//gdi->Line(m_vA, m_vB);

	if (RenderNormals)
	{
		int MidX = (int)((m_vA.X() + m_vB.X()) / 2);
		int MidY = (int)((m_vA.Y() + m_vB.Y()) / 2);

		//gdi->Line(MidX, MidY, (int)(MidX + (m_vN.x * 5)), (int)(MidY + (m_vN.y * 5)));
	}
}
//----------------------------------------------------------------------------
void Wall2D::CalculateNormal()
{
	Vector2f temp = m_vB - m_vA;
	temp.Normalize();
	m_vN.X() = -temp.Y();
	m_vN.Y() = temp.X();
}
//----------------------------------------------------------------------------