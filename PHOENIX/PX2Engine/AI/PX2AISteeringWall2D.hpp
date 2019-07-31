// PX2AISteeringWall2D.hpp

#ifndef PX2AISTEERINGWALL2D_HPP
#define PX2AISTEERINGWALL2D_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Wall2D
	{
	public:
		Wall2D();
		Wall2D(Vector2f A, Vector2f B);
		Wall2D(Vector2f A, Vector2f B, Vector2f N);

		Vector2f From()const { return m_vA; }
		void SetFrom(Vector2f v) { m_vA = v; CalculateNormal(); }

		Vector2f To()const { return m_vB; }
		void SetTo(Vector2f v) { m_vB = v; CalculateNormal(); }

		Vector2f Normal()const { return m_vN; }
		void SetNormal(Vector2f n) { m_vN = n; }

		Vector2f Center()const { return (m_vA + m_vB) / 2.0; }

		void Render(bool RenderNormals) const;

	private:
		void CalculateNormal();

		Vector2f m_vA, m_vB, m_vN;
	};

}

#endif