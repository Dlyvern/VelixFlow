#ifndef SHADOW_SYSTEM_HPP
#define SHADOW_SYSTEM_HPP

#include <glm/glm.hpp>
#include "VelixFlow/Light.hpp"
#include <map>
#include <vector>
#include <unordered_map>
#include <memory>

#include "VelixFlow/DefaultMacros.hpp"


ELIX_NAMESPACE_BEGIN

class ShadowSystem
{
public:
	enum class ShadowQuality
	{
		LOW,
		MEDIUM,
		HIGH,
		ULTRA
	};

	struct Shadow
	{
		unsigned int fbo;
		unsigned int shadowMap;
		glm::mat4 lightMatrix;
	};

	const Shadow& getShadowData(lighting::Light* light) const;

	glm::mat4 getLightMatrix(lighting::Light* light) const;

	void cleanup();

	void updateLightMatrix(lighting::Light* light);

	void beginShadowPass(lighting::Light* light);
	void bindShadowPass(lighting::Light* light, unsigned int textureSlot);

	void endShadowPass();

	void init(const std::vector<std::shared_ptr<lighting::Light>>& lights, ShadowQuality shadowQuality = ShadowQuality::ULTRA);
private:
	Shadow createShadowFramebuffer();
	Shadow createShadowCubeFramebuffer();	   
	ShadowQuality m_quality;

	std::unordered_map<unsigned int, Shadow> m_shadowData;

	int getResolutionBasedOnQuality(ShadowQuality quality);
	glm::mat4 calculateSpotLightMatrix(lighting::Light* spotLight);
	glm::mat4 calculateDirectionalLightMatrix(lighting::Light* directionalLight); 
};

ELIX_NAMESPACE_END

#endif //SHADOW_SYSTEM_HPP
