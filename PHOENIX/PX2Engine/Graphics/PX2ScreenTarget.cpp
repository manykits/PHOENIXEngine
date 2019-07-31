// PX2ScreenTarget.cpp

#include "PX2ScreenTarget.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Camera* ScreenTarget::CreateCamera ()
{
	// 将[-1,-1]^2 x [0,1]匹配到[0,1]^3
	Camera* camera = new0 Camera(false);
	camera->SetFrustum(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	camera->SetFrame(APoint::ORIGIN, AVector::UNIT_Z, AVector::UNIT_Y,
		AVector::UNIT_X);

	return camera;
}
//----------------------------------------------------------------------------
TriMesh* ScreenTarget::CreateRectangle (VertexFormat* vformat, int rtWidth,
										int rtHeight, float xmin, float xmax, float ymin, float ymax,
										float zValue)
{
	if (ValidFormat(vformat) && ValidSizes(rtWidth, rtHeight))
	{
		Float2 tc0, tc1, tc2, tc3;
		if (VertexShader::GetProfile() == VertexShader::VP_ARBVP1)
		{
			tc0 = Float2(0.0f, 0.0f);
			tc1 = Float2(1.0f, 0.0f);
			tc2 = Float2(1.0f, 1.0f);
			tc3 = Float2(0.0f, 1.0f);
		}
		else
		{
			float dx = 0.5f*(xmax - xmin)/(float)(rtWidth - 1);
			float dy = 0.5f*(ymax - ymin)/(float)(rtHeight - 1);
			xmin -= dx;
			xmax -= dx;
			ymin += dy;
			ymax += dy;
			tc0 = Float2(0.0f, 1.0f);
			tc1 = Float2(1.0f, 1.0f);
			tc2 = Float2(1.0f, 0.0f);
			tc3 = Float2(0.0f, 0.0f);
		}

		int vstride = vformat->GetStride();
		VertexBuffer* vbuffer = new0 VertexBuffer(4, vstride);
		VertexBufferAccessor vba(vformat, vbuffer);
		vba.Position<Float3>(0) = Float3(xmin, ymin, zValue);
		vba.Position<Float3>(1) = Float3(xmax, ymin, zValue);
		vba.Position<Float3>(2) = Float3(xmax, ymax, zValue);
		vba.Position<Float3>(3) = Float3(xmin, ymax, zValue);
		vba.TCoord<Float2>(0, 0) = tc0;
		vba.TCoord<Float2>(0, 1) = tc1;
		vba.TCoord<Float2>(0, 2) = tc2;
		vba.TCoord<Float2>(0, 3) = tc3;

		// 为square创建IndexBuffer
		IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
		int* indices = (int*)ibuffer->GetData();
		indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
		indices[3] = 0;  indices[4] = 2;  indices[5] = 3;

		return new0 TriMesh(vformat, vbuffer, ibuffer);
	}

	return 0;
}
//----------------------------------------------------------------------------
TriMesh *ScreenTarget::CreateRectangle (VertexFormat* vformat, 
	float xMin,	float xMax, float yMin, float yMax, float zValue)
{
	if (!ValidFormat(vformat))
		return 0;

	Float2 tc0, tc1, tc2, tc3;
	tc0 = Float2(0.0f, 0.0f);
	tc1 = Float2(1.0f, 0.0f);
	tc2 = Float2(1.0f, 1.0f);
	tc3 = Float2(0.0f, 1.0f);

	int vstride = vformat->GetStride();
	VertexBuffer* vbuffer = new0 VertexBuffer(4, vstride);
	VertexBufferAccessor vba(vformat, vbuffer);
	vba.Position<Float3>(0) = Float3(xMin, yMin, zValue);
	vba.Position<Float3>(1) = Float3(xMax, yMin, zValue);
	vba.Position<Float3>(2) = Float3(xMax, yMax, zValue);
	vba.Position<Float3>(3) = Float3(xMin, yMax, zValue);
	vba.TCoord<Float2>(0, 0) = tc0;
	vba.TCoord<Float2>(0, 1) = tc1;
	vba.TCoord<Float2>(0, 2) = tc2;
	vba.TCoord<Float2>(0, 3) = tc3;

	// 为square创建IndexBuffer
	IndexBuffer* ibuffer = new0 IndexBuffer(6, sizeof(int));
	int* indices = (int*)ibuffer->GetData();
	indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
	indices[3] = 0;  indices[4] = 2;  indices[5] = 3;

	return new0 TriMesh(vformat, vbuffer, ibuffer);
}
//----------------------------------------------------------------------------
bool ScreenTarget::CreatePositions (int rtWidth, int rtHeight, float xmin,
									float xmax, float ymin, float ymax, float zValue, Float3* positions)
{
	if (ValidSizes(rtWidth, rtHeight))
	{
		if (VertexShader::GetProfile() == VertexShader::VP_ARBVP1)
		{
			xmin = 0.0f;
			xmax = 1.0f;
			ymin = 0.0f;
			ymax = 1.0f;
		}
		else
		{
			float dx = 0.5f*(xmax - xmin)/(float)(rtWidth - 1);
			float dy = 0.5f*(ymax - ymin)/(float)(rtHeight - 1);
			xmin -= dx;
			xmax -= dx;
			ymin += dy;
			ymax += dy;
		}

		positions[0] = Float3(xmin, ymin, zValue);
		positions[1] = Float3(xmax, ymin, zValue);
		positions[2] = Float3(xmax, ymax, zValue);
		positions[3] = Float3(xmin, ymax, zValue);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ScreenTarget::CreateTCoords (Float2* tcoords)
{
	if (VertexShader::GetProfile() == VertexShader::VP_ARBVP1)
	{
		tcoords[0] = Float2(0.0f, 0.0f);
		tcoords[1] = Float2(1.0f, 0.0f);
		tcoords[2] = Float2(1.0f, 1.0f);
		tcoords[3] = Float2(0.0f, 1.0f);
	}
	else
	{
		tcoords[0] = Float2(0.0f, 1.0f);
		tcoords[1] = Float2(1.0f, 1.0f);
		tcoords[2] = Float2(1.0f, 0.0f);
		tcoords[3] = Float2(0.0f, 0.0f);
	}
}
//----------------------------------------------------------------------------
bool ScreenTarget::ValidSizes (int rtWidth, int rtHeight)
{
	if (rtWidth > 0 && rtHeight > 0)
	{
		return true;
	}

	assertion(false, "Invalid dimensions.\n");
	return false;
}
//----------------------------------------------------------------------------
bool ScreenTarget::ValidFormat (VertexFormat* vformat)
{
	int index = vformat->GetIndex(VertexFormat::AU_POSITION, 0);
	if (index < 0)
	{
		assertion(false, "Format must have positions.\n");
		return false;
	}

	if (vformat->GetAttributeType(index) != VertexFormat::AT_FLOAT3)
	{
		assertion(false, "Positions must be 3-tuples.\n");
		return false;
	}

	index = vformat->GetIndex(VertexFormat::AU_TEXCOORD, 0);
	if (index < 0)
	{
		assertion(false, "Format must have texture coordinates in unit 0.\n");
		return false;
	}

	if (vformat->GetAttributeType(index) != VertexFormat::AT_FLOAT2)
	{
		assertion(false, "Texture coordinates in unit 0 must be 2-tuples.\n");
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------