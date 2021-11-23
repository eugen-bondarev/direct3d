#include "sampler.h"

#include "../device/device.h"

#include "../GraphicsContext.h"

namespace VK
{
    Sampler::Sampler(VkFilter filter, const Device* device) : device{device ? *device : GetDevice()}
    {
        VkSamplerCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        create_info.magFilter = filter;
        create_info.minFilter = filter;
        create_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        create_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        create_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        create_info.anisotropyEnable = VK_TRUE;
        create_info.maxAnisotropy = this->device.properties.limits.maxSamplerAnisotropy;
        create_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

        create_info.unnormalizedCoordinates = VK_FALSE;

        create_info.compareEnable = VK_FALSE;
        create_info.compareOp = VK_COMPARE_OP_ALWAYS;
        create_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        create_info.mipLodBias = 0.0f;
        create_info.minLod = 0.0f;
        create_info.maxLod = 0.0f;

        VK_TRY(vkCreateSampler(this->device.GetVkDevice(), &create_info, nullptr, &vkSampler));
        
    }

    Sampler::~Sampler()
    {
        vkDestroySampler(device.GetVkDevice(), vkSampler, nullptr);
        
    }

    const VkSampler& Sampler::GetVkSampler() const
    {
        return vkSampler;
    }
}