/***************************************************************************
 * Copyright 1998-2018 by authors (see AUTHORS.txt)                        *
 *                                                                         *
 *   This file is part of LuxCoreRender.                                   *
 *                                                                         *
 * Licensed under the Apache License, Version 2.0 (the "License");         *
 * you may not use this file except in compliance with the License.        *
 * You may obtain a copy of the License at                                 *
 *                                                                         *
 *     http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                         *
 * Unless required by applicable law or agreed to in writing, software     *
 * distributed under the License is distributed on an "AS IS" BASIS,       *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 * See the License for the specific language governing permissions and     *
 * limitations under the License.                                          *
 ***************************************************************************/

#ifndef _SLG_WRINKLEDTEX_H
#define	_SLG_WRINKLEDTEX_H

#include "slg/textures/texture.h"

namespace slg {

//------------------------------------------------------------------------------
// Wrinkled texture
//------------------------------------------------------------------------------

class WrinkledTexture : public Texture {
public:
	WrinkledTexture(const TextureMapping3D *mp, const int octs, const float omg) :
		mapping(mp), octaves(octs), omega(omg) { }
	virtual ~WrinkledTexture() { delete mapping; }

	virtual TextureType GetType() const { return WRINKLED; }
	virtual float GetFloatValue(const HitPoint &hitPoint) const;
	virtual luxrays::Spectrum GetSpectrumValue(const HitPoint &hitPoint) const;
	virtual float Y() const { return .5f; }
	virtual float Filter() const { return .5f; }

	const TextureMapping3D *GetTextureMapping() const { return mapping; }
	int GetOctaves() const { return octaves; }
	float GetOmega() const { return omega; }

	virtual luxrays::Properties ToProperties(const ImageMapCache &imgMapCache, const bool useRealFileName) const;

private:
	const TextureMapping3D *mapping;
	const int octaves;
	const float omega;
};


}

#endif	/* _SLG_WRINKLEDTEX_H */
