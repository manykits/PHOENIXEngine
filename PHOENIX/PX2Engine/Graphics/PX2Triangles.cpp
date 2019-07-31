// PX2Triangles.cpp

#include "PX2Triangles.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Renderable, Triangles);
PX2_IMPLEMENT_STREAM(Triangles);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Triangles);
PX2_IMPLEMENT_DEFAULT_NAMES(Renderable, Triangles);
PX2_IMPLEMENT_DEFAULT_STREAM(Renderable, Triangles);

//----------------------------------------------------------------------------
Triangles::Triangles (PrimitiveType type)
:
Renderable(type)
{
}
//----------------------------------------------------------------------------
Triangles::Triangles (PrimitiveType type, VertexFormat* vformat,
					  VertexBuffer* vbuffer, IndexBuffer* ibuffer)
					  :
Renderable(type, vformat, vbuffer, ibuffer)
{
}
//----------------------------------------------------------------------------
Triangles::~Triangles ()
{
}
//----------------------------------------------------------------------------
bool Triangles::GetModelTriangle (int i, APoint* modelTriangle) const
{
	int v0, v1, v2;
	if (GetTriangle(i, v0, v1, v2))
	{
		VertexBufferAccessor vba(mVFormat, mVBuffer);
		modelTriangle[0] = vba.Position<Float3>(v0);
		modelTriangle[1] = vba.Position<Float3>(v1);
		modelTriangle[2] = vba.Position<Float3>(v2);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
bool Triangles::GetWorldTriangle (int i, APoint* worldTriangle) const
{
	APoint modelTriangle[3];
	if (GetModelTriangle(i, modelTriangle))
	{
		worldTriangle[0] = WorldTransform*modelTriangle[0];
		worldTriangle[1] = WorldTransform*modelTriangle[1];
		worldTriangle[2] = WorldTransform*modelTriangle[2];
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
Float3 Triangles::GetPosition (int v) const
{
	int index = mVFormat->GetIndex(VertexFormat::AU_POSITION);
	if (index >= 0)
	{
		char* positions = mVBuffer->GetData() + mVFormat->GetOffset(index);
		int stride = mVFormat->GetStride();
		return *(Float3*)(positions + v*stride);
	}

	assertion(false, "GetPosition failed.\n");
	return Float3(0.0f, 0.0f, 0.0f);
}
//----------------------------------------------------------------------------
void Triangles::UpdateModelSpace (UpdateType type)
{
	UpdateModelBound();
	if (type == GU_MODEL_BOUND_ONLY)
	{
		return;
	}

	VertexBufferAccessor vba(this);
	if (vba.HasNormal())
	{
		UpdateModelNormals(vba);
	}

	if (type != GU_NORMALS)
	{
		if (vba.HasTangent() || vba.HasBinormal())
		{
			if (type == GU_USE_GEOMETRY)
			{
				UpdateModelTangentsUseGeometry(vba);
			}
			else
			{
				UpdateModelTangentsUseTCoords(vba);
			}
		}
	}

	Renderer::UpdateAll(mVBuffer);
}
//----------------------------------------------------------------------------
void Triangles::UpdateModelNormals (VertexBufferAccessor& vba)
{
	// 根据包含顶点的三角形法线的平均值，计算法线。
	const int numVertices = vba.GetNumVertices();
	int i;
	for (i = 0; i < numVertices; ++i)
	{
		vba.Normal<Float3>(i) = Float3(0.0f, 0.0f, 0.0f);
	}

	const int numTriangles = GetNumTriangles();
	for (i = 0; i < numTriangles; ++i)
	{
		// 获得三角形顶点索引
		int v0, v1, v2;
		if (!GetTriangle(i, v0, v1, v2))
		{
			continue;
		}

		// 获得顶点位置
		APoint pos0 = vba.Position<Float3>(v0);
		APoint pos1 = vba.Position<Float3>(v1);
		APoint pos2 = vba.Position<Float3>(v2);

		// 计算面法线
		AVector triEdge1 = pos1 - pos0;
		AVector triEdge2 = pos2 - pos0;
		AVector triNormal = triEdge1.Cross(triEdge2);

		// 顶点法线为所有面法线之和
		vba.Normal<AVector>(v0) += triNormal;
		vba.Normal<AVector>(v1) += triNormal;
		vba.Normal<AVector>(v2) += triNormal;
	}

	// 将顶点法线单位化
	for (i = 0; i < numVertices; ++i)
	{
		vba.Normal<AVector>(i).Normalize();
	}
}
//----------------------------------------------------------------------------
void Triangles::UpdateModelTangentsUseGeometry (VertexBufferAccessor& vba)
{
	const int numVertices = vba.GetNumVertices();
	HMatrix* dNormal = new1<HMatrix>(numVertices);
	HMatrix* wwTrn = new1<HMatrix>(numVertices);
	HMatrix* dwTrn = new1<HMatrix>(numVertices);
	memset(wwTrn, 0, numVertices*sizeof(HMatrix));
	memset(dwTrn, 0, numVertices*sizeof(HMatrix));

	const int numTriangles = GetNumTriangles();
	int i, row, col;
	for (i = 0; i < numTriangles; ++i)
	{
		// 获得三角形索引
		int v[3];
		if (!GetTriangle(i, v[0], v[1], v[2]))
		{
			continue;
		}

		for (int j = 0; j < 3; j++)
		{
			// 获得顶点的位置和法线
			int v0 = v[j];
			int v1 = v[(j + 1) % 3];
			int v2 = v[(j + 2) % 3];
			APoint pos0 = vba.Position<Float3>(v0);
			APoint pos1 = vba.Position<Float3>(v1);
			APoint pos2 = vba.Position<Float3>(v2);
			AVector nor0 = vba.Normal<Float3>(v0);
			AVector nor1 = vba.Normal<Float3>(v1);
			AVector nor2 = vba.Normal<Float3>(v2);

			// Compute the edge from pos0 to pos1, project it to the tangent
			// plane of the vertex, and compute the difference of adjacent
			// normals.
			AVector edge = pos1 - pos0;
			AVector proj = edge - edge.Dot(nor0)*nor0;
			AVector diff = nor1 - nor0;
			for (row = 0; row < 3; ++row)
			{
				for (col = 0; col < 3; ++col)
				{
					wwTrn[v0][row][col] += proj[row]*proj[col];
					dwTrn[v0][row][col] += diff[row]*proj[col];
				}
			}

			// Compute the edge from pos0 to pos2, project it to the tangent
			// plane of the vertex, and compute the difference of adjacent
			// normals.
			edge = pos2 - pos0;
			proj = edge - edge.Dot(nor0)*nor0;
			diff = nor2 - nor0;
			for (row = 0; row < 3; ++row)
			{
				for (col = 0; col < 3; ++col)
				{
					wwTrn[v0][row][col] += proj[row]*proj[col];
					dwTrn[v0][row][col] += diff[row]*proj[col];
				}
			}
		}
	}

	// Add N*N^T to W*W^T for numerical stability.  In theory 0*0^T is added
	// to D*W^T, but of course no update is needed in the implementation.
	// Compute the matrix of normal derivatives.
	for (i = 0; i < numVertices; ++i)
	{
		AVector nor = vba.Normal<Float3>(i);
		for (row = 0; row < 3; ++row)
		{
			for (col = 0; col < 3; ++col)
			{
				wwTrn[i][row][col] =
					0.5f*wwTrn[i][row][col] + nor[row]*nor[col];
				dwTrn[i][row][col] *= 0.5f;
			}
		}

		wwTrn[i].SetColumn(3, APoint::ORIGIN);
		dNormal[i] = dwTrn[i]*wwTrn[i].Inverse();
	}

	delete1(wwTrn);
	delete1(dwTrn);

	// If N is a unit-length normal at a vertex, let U and V be unit-length
	// tangents so that {U, V, N} is an orthonormal set.  Define the matrix
	// J = [U | V], a 3-by-2 matrix whose columns are U and V.  Define J^T
	// to be the transpose of J, a 2-by-3 matrix.  Let dN/dX denote the
	// matrix of first-order derivatives of the normal vector field.  The
	// shape matrix is
	//   S = (J^T * J)^{-1} * J^T * dN/dX * J = J^T * dN/dX * J
	// where the superscript of -1 denotes the inverse.  (The formula allows
	// for J built from non-perpendicular vectors.) The matrix S is 2-by-2.
	// The principal curvatures are the eigenvalues of S.  If k is a principal
	// curvature and W is the 2-by-1 eigenvector corresponding to it, then
	// S*W = k*W (by definition).  The corresponding 3-by-1 tangent vector at
	// the vertex is called the principal direction for k, and is J*W.  The
	// principal direction for the minimum principal curvature is stored as
	// the mesh tangent.  The principal direction for the maximum principal
	// curvature is stored as the mesh bitangent.
	for (i = 0; i < numVertices; ++i)
	{
		// Compute U and V given N.
		AVector norvec = vba.Normal<Float3>(i);
		AVector uvec, vvec;
		AVector::GenerateComplementBasis(uvec, vvec, norvec);

		// Compute S = J^T * dN/dX * J.  In theory S is symmetric, but
		// because we have estimated dN/dX, we must slightly adjust our
		// calculations to make sure S is symmetric.
		float s01 = uvec.Dot(dNormal[i]*vvec);
		float s10 = vvec.Dot(dNormal[i]*uvec);
		float sAvr = 0.5f*(s01 + s10);
		float smat[2][2] =
		{
			{ uvec.Dot(dNormal[i]*uvec), sAvr },
			{ sAvr, vvec.Dot(dNormal[i]*vvec) }
		};

		// Compute the eigenvalues of S (min and max curvatures).
		float trace = smat[0][0] + smat[1][1];
		float det = smat[0][0]*smat[1][1] - smat[0][1]*smat[1][0];
		float discr = trace*trace - 4.0f*det;
		float rootDiscr = Mathf::Sqrt(Mathf::FAbs(discr));
		float minCurvature = 0.5f*(trace - rootDiscr);
		// float maxCurvature = 0.5f*(trace + rootDiscr);

		// Compute the eigenvectors of S.
		AVector evec0(smat[0][1], minCurvature - smat[0][0], 0.0f);
		AVector evec1(minCurvature - smat[1][1], smat[1][0], 0.0f);
		AVector tanvec, binvec;
		if (evec0.SquaredLength() >= evec1.SquaredLength())
		{
			evec0.Normalize();
			tanvec = evec0.X()*uvec + evec0.Y()*vvec;
			binvec = norvec.Cross(tanvec);
		}
		else
		{
			evec1.Normalize();
			tanvec = evec1.X()*uvec + evec1.Y()*vvec;
			binvec = norvec.Cross(tanvec);
		}

		if (vba.HasTangent())
		{
			vba.Tangent<Float3>(i) = tanvec;
		}

		if (vba.HasBinormal())
		{
			vba.Binormal<Float3>(i) = binvec;
		}
	}

	delete1(dNormal);
}
//----------------------------------------------------------------------------
void Triangles::UpdateModelTangentsUseTCoords (VertexBufferAccessor& vba)
{
	// 每个顶点可能被方位多次，每个顶点只要计算一次tangent坐标就可以了。所以使
	// 用0向量被用作为一个表示该顶点是否被计算过的标志。
	const int numVertices = vba.GetNumVertices();
	bool hasTangent = vba.HasTangent();
	Float3 zero(0.0f, 0.0f, 0.0f);
	int i;
	if (hasTangent)
	{
		for (i = 0; i < numVertices; ++i)
		{
			vba.Tangent<Float3>(i) = zero;
		}
	}
	else
	{
		for (i = 0; i < numVertices; ++i)
		{
			vba.Binormal<Float3>(i) = zero;
		}
	}

	const int numTriangles = GetNumTriangles();
	for (i = 0; i < numTriangles; i++)
	{
		// 获得三角形的顶点的 positions, normals, tangents,和 texture
		// coordinates。
		int v0, v1, v2;
		if (!GetTriangle(i, v0, v1, v2))
		{
			continue;
		}

		APoint locPosition[3] =
		{
			vba.Position<Float3>(v0),
			vba.Position<Float3>(v1),
			vba.Position<Float3>(v2)
		};

		AVector locNormal[3] =
		{
			vba.Normal<Float3>(v0),
			vba.Normal<Float3>(v1),
			vba.Normal<Float3>(v2)
		};

		AVector locTangent[3] =
		{
			(hasTangent ? vba.Tangent<Float3>(v0) : vba.Binormal<Float3>(v0)),
			(hasTangent ? vba.Tangent<Float3>(v1) : vba.Binormal<Float3>(v1)),
			(hasTangent ? vba.Tangent<Float3>(v2) : vba.Binormal<Float3>(v2))
		};

		Float2 locTCoord[3] =
		{
			vba.TCoord<Float2>(0, v0),
			vba.TCoord<Float2>(0, v1),
			vba.TCoord<Float2>(0, v2)
		};

		for (int curr = 0; curr < 3; ++curr)
		{
			Float3 currLocTangent = (Float3)locTangent[curr];
			if (currLocTangent != zero)
			{
				// 顶点tangent已经被计算过了
				continue;
			}

			// Compute the tangent space at the vertex.
			AVector norvec = locNormal[curr];
			int prev = ((curr + 2) % 3);
			int next = ((curr + 1) % 3);
			AVector tanvec = ComputeTangent(
				locPosition[curr], locTCoord[curr],
				locPosition[next], locTCoord[next],
				locPosition[prev], locTCoord[prev]);

			// Project T into the tangent plane by projecting out the surface
			// normal N, and then making it unit length.
			tanvec -= norvec.Dot(tanvec)*norvec;
			tanvec.Normalize();

			// Compute the bitangent B, another tangent perpendicular to T.
			AVector binvec = norvec.UnitCross(tanvec);

			if (vba.HasTangent())
			{
				locTangent[curr] = tanvec;
				if (vba.HasBinormal())
				{
					vba.Binormal<Float3>(curr) = binvec;
				}
			}
			else
			{
				vba.Binormal<Float3>(curr) = tanvec;
			}
		}
	}
}
//----------------------------------------------------------------------------
AVector Triangles::ComputeTangent (
								   const APoint& position0, const Float2& tcoord0,
								   const APoint& position1, const Float2& tcoord1,
								   const APoint& position2, const Float2& tcoord2)
{
	// 计算顶点方位偏差。
	AVector diffP1P0 = position1 - position0;
	AVector diffP2P0 = position2 - position0;

	if (Mathf::FAbs(diffP1P0.Length()) < Mathf::ZERO_TOLERANCE
		||  Mathf::FAbs(diffP2P0.Length()) < Mathf::ZERO_TOLERANCE)
	{
		// 三角形太小了，将其蜕化。
		return AVector::ZERO;
	}

	// 计算纹理坐标偏差。
	float diffU1U0 = tcoord1[0] - tcoord0[0];
	float diffV1V0 = tcoord1[1] - tcoord0[1];
	if (Mathf::FAbs(diffV1V0) < Mathf::ZERO_TOLERANCE)
	{
		// The triangle effectively has no variation in the v texture
		// coordinate.
		if (Mathf::FAbs(diffU1U0) < Mathf::ZERO_TOLERANCE)
		{
			// 三角形UV一样，将三角形当做退化三角形。
			return AVector::ZERO;
		}

		// The variation is effectively all in u, so set the tangent vector
		// to be T = dP/du.
		return diffP1P0/diffU1U0;
	}

	// Compute the change in texture coordinates at the vertex P0 in the
	// direction of edge P2-P0.
	float diffU2U0 = tcoord2[0] - tcoord0[0];
	float diffV2V0 = tcoord2[1] - tcoord0[1];
	float det = diffV1V0*diffU2U0 - diffV2V0*diffU1U0;
	if (Mathf::FAbs(det) < Mathf::ZERO_TOLERANCE)
	{
		// 三角形顶点共线
		return AVector::ZERO;
	}

	// 三角形不共线，所有使用计算公式dP/du = (dv1*dP2-dv2*dP1)/(dv1*du2-dv2*du1)
	// 计算。
	return (diffV1V0*diffP2P0 - diffV2V0*diffP1P0)/det;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Triangles::RegistProperties ()
{
	Renderable::RegistProperties();

	AddPropertyClass("Triangles");

	AddProperty("NumVertexes", Object::PT_INT, GetNumVertices(), false);
	AddProperty("NumTriangles", Object::PT_INT, GetNumTriangles(), false);
}
//----------------------------------------------------------------------------
void Triangles::OnPropertyChanged (const PropertyObject &obj)
{
	Renderable::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------