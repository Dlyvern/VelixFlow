#ifndef UI_RENDER_HPP
#define UI_RENDER_HPP

#include "DefaultMacros.hpp"
#include "Render.hpp"

ELIX_NAMESPACE_BEGIN

class UIRender : public IRender
{
public:
    UIRender();
    window::ClearFlag getClearFlag() override;
    bool shouldExecute() const override;
    std::string getName() const override;
    void render(const elix::FrameData& frameData, Scene* scene = nullptr) override;
    int getPriority() const override;
    ~UIRender() override = default;

private:
    unsigned int m_VAO, m_VBO;
};

ELIX_NAMESPACE_END

#endif //UI_RENDER_HPP