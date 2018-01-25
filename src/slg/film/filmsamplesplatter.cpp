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

#include "slg/film/filters/gaussian.h"
#include "slg/film/filmsamplesplatter.h"
#include "slg/film/sampleresult.h"

using namespace std;
using namespace luxrays;
using namespace slg;

//------------------------------------------------------------------------------
// FilmSampleSplatter
//------------------------------------------------------------------------------

FilmSampleSplatter::FilmSampleSplatter(const Filter *flt) : filter(flt) {
	if (filter) {
		const u_int size = Max<u_int>(4, Max(filter->xWidth, filter->yWidth) + 1);
		filterLUTs = new FilterLUTs(*filter, size);
	} else
		filterLUTs = NULL;
}

FilmSampleSplatter::~FilmSampleSplatter() {
	delete filterLUTs;
}

void FilmSampleSplatter::SplatSample(Film &film, const SampleResult &sampleResult, const float weight) const {
	const u_int *subRegion = film.GetSubRegion();

	if (!filter) {
		const int x = Floor2Int(sampleResult.filmX);
		const int y = Floor2Int(sampleResult.filmY);

		if ((x >= (int)subRegion[0]) && (x <= (int)subRegion[1]) && (y >= (int)subRegion[2]) && (y <= (int)subRegion[3])) {
			film.AddSample(x, y, sampleResult, weight);
		}
	} else {
		//----------------------------------------------------------------------
		// Add all data related information (not filtered)
		//----------------------------------------------------------------------

		if (film.HasDataChannel()) {
			const int x = Floor2Int(sampleResult.filmX);
			const int y = Floor2Int(sampleResult.filmY);

			if ((x >= (int)subRegion[0]) && (x <= (int)subRegion[1]) && (y >= (int)subRegion[2]) && (y <= (int)subRegion[3]))
				film.AddSampleResultData(x, y, sampleResult);
		}

		//----------------------------------------------------------------------
		// Add all color related information (filtered)
		//----------------------------------------------------------------------

		// Compute sample's raster extent
		const float dImageX = sampleResult.filmX - .5f;
		const float dImageY = sampleResult.filmY - .5f;
		const FilterLUT *filterLUT = filterLUTs->GetLUT(dImageX - floorf(sampleResult.filmX), dImageY - floorf(sampleResult.filmY));
		const float *lut = filterLUT->GetLUT();

		const int x0 = Floor2Int(dImageX - filter->xWidth * .5f + .5f);
		const int x1 = x0 + filterLUT->GetWidth();
		const int y0 = Floor2Int(dImageY - filter->yWidth * .5f + .5f);
		const int y1 = y0 + filterLUT->GetHeight();

		for (int iy = y0; iy < y1; ++iy) {
			if (iy < (int)subRegion[2]) {
				lut += filterLUT->GetWidth();
				continue;
			} else if(iy > (int)subRegion[3])
				break;

			for (int ix = x0; ix < x1; ++ix) {
				const float filterWeight = *lut++;

				if (ix < (int)subRegion[0])
					continue;
				else if (ix > (int)subRegion[1])
					break;

				const float filteredWeight = weight * filterWeight;
				film.AddSampleResultColor(ix, iy, sampleResult, filteredWeight);
			}
		}
	}
}
