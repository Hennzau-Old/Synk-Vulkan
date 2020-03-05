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

        void                      update();

        static int                createWindow(Window* window, const WindowCreateInfo& createInfo);

        bool                      isClosed() const;

        inline uint32_t&          getWidth  () { return m_info.width; }
        inline uint32_t&          getHeight () { return m_info.height; }

        inline std::string&       getTitle  () { return m_info.title; }

        inline int                isResizable () { return m_info.resizable; }

        inline GLFWwindow*        getGLFWWindow() { return m_window; }

        WindowCreateInfo&         getInfo();

        /* variables */

    private:

        /* structure */

        /* functions */

        int                        createWindow();

        /* variables */

        GLFWwindow*               m_window;

        WindowCreateInfo          m_info;
};
