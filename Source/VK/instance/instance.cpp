#include "instance.h"

#include "validation.h"

namespace VK
{
    namespace Global
    {
        Instance *instance;

        Instance::Instance()
        {
            if (Validation::enableValidationLayers && !Validation::CheckValidationSupport())
            {
                throw EXCEPTION_WHAT("Validation layers required but not supported.");
            }

            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Hello Triangle";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            auto glfwExtensions = Validation::GetRequestedExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
            createInfo.ppEnabledExtensionNames = glfwExtensions.data();

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
            if (Validation::enableValidationLayers)
            {
                createInfo.enabledLayerCount = static_cast<uint32_t>(Validation::validationLayers.size());
                createInfo.ppEnabledLayerNames = Validation::validationLayers.data();

                Validation::PopulateDebugMessengerCreateInfo(debugCreateInfo);
                createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
            }
            else
            {
                createInfo.enabledLayerCount = 0;
            }

            VK_TRY(vkCreateInstance(&createInfo, nullptr, &vkInstance));

            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
            std::vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

            LINE_OUT("Available extensions:");
            for (const auto &ext : extensions)
            {
                LINE_OUT(ext.extensionName);
            }

            Validation::SetupDebugMessenger(vkInstance);

            
        }

        Instance::~Instance()
        {
            Validation::Destroy(vkInstance);

            vkDestroyInstance(vkInstance, nullptr);

            
        }

        VkInstance Instance::GetVkInstance() const
        {
            return vkInstance;
        }
    }
}