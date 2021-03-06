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

#include "slg/lights/strategies/distributionlightstrategy.h"
#include "slg/scene/scene.h"

using namespace std;
using namespace luxrays;
using namespace slg;

//------------------------------------------------------------------------------
// DistributionLightStrategy
//------------------------------------------------------------------------------

LightSource *DistributionLightStrategy::SampleLights(const float u,
		const Point &p, const Normal &n,
		float *pdf) const {
	return SampleLights(u, pdf);
}

float DistributionLightStrategy::SampleLightPdf(const LightSource *light,
		const Point &p, const Normal &n) const {
	return lightsDistribution->Pdf(light->lightSceneIndex);
}

LightSource *DistributionLightStrategy::SampleLights(const float u,
		float *pdf) const {
	const u_int lightIndex = lightsDistribution->SampleDiscrete(u, pdf);
	assert ((lightIndex >= 0) && (lightIndex < scene->lightDefs.GetSize()));

	if (*pdf > 0.f)
		return scene->lightDefs.GetLightSources()[lightIndex];
	else
		return NULL;
}

Properties DistributionLightStrategy::ToProperties() const {
	return Properties() <<
			Property("lightstrategy.type")(LightStrategyType2String(GetType()));
}
