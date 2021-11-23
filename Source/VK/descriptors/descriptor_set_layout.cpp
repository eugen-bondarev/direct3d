#include "descriptor_set_layout.h"

#include "../device/device.h"

#include "../GraphicsContext.h"

namespace VK
{		
    VkDescriptorSetLayoutBinding CreateBinding(uint32_t index, VkDescriptorType type, VkShaderStageFlags stage_flags)
    {
        VkDescriptorSetLayoutBinding binding{};		
        binding.binding = index;
        binding.descriptorType = type;
        binding.descriptorCount = 1;
        binding.stageFlags = stage_flags;
        binding.pImmutableSamplers = nullptr; // Optional

        return binding;
    }

    DescriptorSetLayout::DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& bindings, const Global::Device* device) : device{device ? *device : GetDevice()}
    {
        VkDescriptorSetLayoutCreateInfo layout_info{};
        layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layout_info.bindingCount = static_cast<uint32_t>(bindings.size());
        layout_info.pBindings = bindings.data();

        VK_TRY(vkCreateDescriptorSetLayout(this->device.GetVkDevice(), &layout_info, nullptr, &vkDescriptorSetLayout));
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        vkDestroyDescriptorSetLayout(device.GetVkDevice(), vkDescriptorSetLayout, nullptr);
    }

    VkDescriptorSetLayout& DescriptorSetLayout::GetVkDescriptorSetLayout()
    {
        return vkDescriptorSetLayout;
    }
}