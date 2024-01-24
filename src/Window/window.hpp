#pragma once

namespace Project {

class Window {
public:
    Window();
    ~Window();

    void keyEvent(int key, int scancode, int action, int mods);
};

} // namespace Project
