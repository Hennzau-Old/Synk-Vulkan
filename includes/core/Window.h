#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

#include "core/utils/logs/Logger.h"

class Window
{
    public:

        /* static variables */



        /* structure */

        struct WindowCreateInfo
        {
            uint32_t              width     = 0;
            uint32_t              height    = 0;
            int                   resizable = GLFW_FALSE;

            std::string           title     = "noName";
        };

        /* functions */

        Window();
        ~Window();

        void                      clean();
        void                      setData(const WindowCreateInfo& createInfo);

        void                      setFramebufferResizedStatus(const bool& status);

        void                      update();

        static void               framebufferResizeCallback(GLFWwindow* window, int width, int height);
        static int                createWindow(Window* window, const WindowCreateInfo& createInfo);

        bool                      isClosed() const;

        inline uint32_t&          getWidth  () { return m_info.width; }
        inline uint32_t&          getHeight () { return m_info.height; }

        inline std::string&       getTitle  () { return m_info.title; }

        inline int                isResizable () { return m_info.resizable; }

        inline GLFWwindow*        getGLFWWindow       () { return m_window; }
        inline bool&              isFramebufferResized() { return m_framebufferResized; }

        WindowCreateInfo&         getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                        createWindow();

        /* variables */

        GLFWwindow*               m_window;
        bool                      m_framebufferResized;

        WindowCreateInfo          m_info;
};
