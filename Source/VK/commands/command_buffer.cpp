#include "command_buffer.h"

#include "../device/device.h"
#include "../memory/buffer.h"

namespace VK
{
    CommandBuffer::CommandBuffer(CommandPool *command_pool) : commandPool{command_pool}
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = command_pool->GetVkCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        VK_TRY(vkAllocateCommandBuffers(Global::device->GetVkDevice(), &allocInfo, &vkCommandBuffer));

        
    }

    CommandBuffer::~CommandBuffer()
    {
        Free();

        
    }

    void CommandBuffer::Begin(VkCommandBufferUsageFlags flags) const
    {
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = flags;			   // Optional
        begin_info.pInheritanceInfo = nullptr; // Optional

        VK_TRY(vkBeginCommandBuffer(vkCommandBuffer, &begin_info));
    }

    void CommandBuffer::End() const
    {
        VK_TRY(vkEndCommandBuffer(vkCommandBuffer));
    }

    void CommandBuffer::SubmitToQueue(const VkQueue &queue, VkSemaphore *wait_semaphore, const VkSemaphore *signal_semaphore, VkFence fence) const
    {
        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &vkCommandBuffer;

        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        if (wait_semaphore)
        {
            submit_info.waitSemaphoreCount = 1;
            submit_info.pWaitSemaphores = wait_semaphore;
            submit_info.pWaitDstStageMask = &wait_stage;
        }

        if (signal_semaphore)
        {
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores = signal_semaphore;
        }

        VK_TRY(vkQueueSubmit(queue, 1, &submit_info, fence));
    }

    void CommandBuffer::BeginRenderPass(RenderPass *render_pass, Framebuffer *framebuffer, const std::array<float, 4>& color) const
    {
        VkRenderPassBeginInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        submit_info.renderPass = render_pass->GetVkRenderPass();
        submit_info.framebuffer = framebuffer->GetVkFramebuffer();
        submit_info.renderArea.extent = {static_cast<uint32_t>(framebuffer->GetSize().x), static_cast<uint32_t>(framebuffer->GetSize().y)};

        VkClearValue clear_color;
        memcpy(&clear_color, color.data(), sizeof(VkClearValue));

        std::array<VkClearValue, 2> clearValues{clear_color, { 1.0f, 0.0f }};
        
        submit_info.clearValueCount = static_cast<uint32_t>(clearValues.size());
        submit_info.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(vkCommandBuffer, &submit_info, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::EndRenderPass() const
    {
        vkCmdEndRenderPass(vkCommandBuffer);
    }

    void CommandBuffer::BindPipeline(const Pipeline *pipeline) const
    {
        vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetVkPipeline());
    }

    void CommandBuffer::BindVertexBuffers(const std::vector<Buffer *> &buffers, const std::vector<VkDeviceSize> &offsets) const
    {
        VK_ASSERT(buffers.size() == offsets.size());

        std::vector<VkBuffer> vkBuffers(buffers.size());
        for (int i = 0; i < buffers.size(); i++)
        {
            vkBuffers[i] = buffers[i]->GetVkBuffer();
        }
        vkCmdBindVertexBuffers(
            vkCommandBuffer, 0,
            static_cast<uint32_t>(vkBuffers.size()),
            vkBuffers.data(),
            offsets.data());
    }

    void CommandBuffer::BindIndexBuffer(Buffer *index_buffer, VkIndexType index_type) const
    {
        vkCmdBindIndexBuffer(vkCommandBuffer, index_buffer->GetVkBuffer(), 0, index_type);
    }

    void CommandBuffer::BindDescriptorSets(const Pipeline *pipeline, uint32_t amount_of_descriptor_sets, VkDescriptorSet *descriptor_sets, uint32_t amount_of_offsets, uint32_t* offsets) const
    {
        vkCmdBindDescriptorSets(
            vkCommandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipeline->GetVkPipelineLayout(), 0,
            amount_of_descriptor_sets, descriptor_sets, amount_of_offsets, offsets);
    }

    void CommandBuffer::Free() const
    {
        vkFreeCommandBuffers(Global::device->GetVkDevice(), commandPool->GetVkCommandPool(), 1, &vkCommandBuffer);
    }

    VkCommandBuffer &CommandBuffer::GetVkCommandBuffer()
    {
        return vkCommandBuffer;
    }
}