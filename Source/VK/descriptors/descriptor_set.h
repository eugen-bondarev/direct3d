#pragma once

#include "../Common.h"

#include "descriptor_pool.h"

#include "../image/image_view.h"
#include "../image/sampler.h"

namespace Engine
{
	namespace Vk
	{
		class DescriptorSet
		{
		public:
			DescriptorSet(DescriptorPool* descriptor_pool, const std::vector<VkDescriptorSetLayout>& layouts);
			~DescriptorSet();

			void Update(const std::vector<VkWriteDescriptorSet>& write_descriptor_sets);

			VkDescriptorSet& GetVkDescriptorSet();

		private:
			VkDescriptorSet vkDescriptorSet;

			DescriptorSet(const DescriptorSet&) = delete;
			DescriptorSet& operator=(const DescriptorSet&) = delete;
		};

		VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorBufferInfo* descriptor_buffer_info);
		VkWriteDescriptorSet CreateWriteDescriptorSet(DescriptorSet* descriptor_set, uint32_t binding, VkDescriptorType descriptor_type, const VkDescriptorImageInfo* descriptor_image_info);
	}
}