#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/utils/logs/Logger.h"

#include "core/Instance.h"
#include "core/Window.h"

class Surface
{
    public:

        /* static variables */



        /* structure */

        struct SurfaceCreateInfo
        {
            Instance*             instance  = nullptr;
            Window*               window    = nullptr;
        };

        /* functions */

        Surface();
        ~Surface();

        void                      clean();
        void                      setData(const SurfaceCreateInfo& createInfo);

        static int                createSurface(Surface* surface, const SurfaceCreateInfo& createInfo);

        inline VkSurfaceKHR&      getSurface() { return m_surface; }

        SurfaceCreateInfo&        getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                       createSurface();

        /* variables */

        VkSurfaceKHR              m_surface;

        SurfaceCreateInfo         m_info;
};
