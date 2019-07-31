// PX2TexPackData.hpp

#ifndef PX2TEXPACKDATA_HPP
#define PX2TEXPACKDATA_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TexPackElement
	{
	public:
		TexPackElement ();
		~TexPackElement ();

		bool IsValid () const;

		// X  => sprite x pos in texture
		// Y  => sprite y pos in texture
		// W  => sprite width (may be trimmed)
		// H  => sprite height (may be trimmed)
		// OX => sprite's x-corner offset (only available if trimmed)
		// OY => sprite's y-corner offset (only available if trimmed)
		// OW => sprite's original width (only available if trimmed)
		// OH => sprite's original height (only available if trimmed)
		// Rolated => 'y' only set if sprite is rotated

		int X;
		int Y;
		int W;
		int H;
		int OX;
		int OY;
		int OW;
		int OH;
		bool Rolated;
		int TexWidth;
		int TexHeight;
		std::string ElementName;
		std::string ImagePathFull;
	};
	typedef Pointer0<TexPackElement> TexPackElementPtr;

	class PX2_ENGINE_ITEM TexPack
	{
	public:
		TexPack ();
		~TexPack ();

		bool IsValid () const;

		std::string ImageFullPath;
		std::string ImagePath;
		int Width;
		int Height;
		bool IsHasUpdate; // 是否有更新

		std::vector<TexPackElement> Elements;
	};
	typedef Pointer0<TexPack> TexPackPtr;

#include "PX2TexPackData.inl"

}

#endif