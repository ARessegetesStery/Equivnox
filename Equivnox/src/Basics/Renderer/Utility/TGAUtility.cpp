#include "eqxpch.h"

#include "tgaimage.h"

namespace EQX {
	/**
	 * Mix TGAColors
	 * 
	 * @param fore Newly added color
	 * @param back Originally existing color
	 * @param coeff The opacity of fore: 1 for completely opaque
	 * @return 
	 */
	TGAColor blendTGAColor(TGAColor fore, TGAColor back, double coeff)
	{
		return TGAColor((int)(fore.r * coeff + back.r * (1 - coeff) * 0.5),
			(int)(fore.g * coeff + back.g * (1 - coeff) * 0.5),
			(int)(fore.b * coeff + back.b * (1 - coeff) * 0.5),
			(int)(fore.a * coeff + back.a * (1 - coeff) * 0.5));
	}


}

