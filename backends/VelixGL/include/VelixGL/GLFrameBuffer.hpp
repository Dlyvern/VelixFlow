#ifndef FRAME_BUFFER_HPP
#define FRAME_BUFFER_HPP

#include <set>
#include <vector>
#include <cstdint>

#include "VelixFlow/DefaultMacros.hpp"

ELIX_NAMESPACE_BEGIN

class GLFrameBuffer
{
public:
    enum class InternalFormat
    {
        DEPTH24,
        DEPTH24_STENCIL8,
    };

    enum class Attachment
    {
        DEPTH,
        DEPTH_STENCIL,
    };

    void create(int width, int height, InternalFormat format, bool texture = false);

    unsigned int getTexture(int index) const;

    void addTextureId(unsigned int id);

    void addAttachment(Attachment attachment);

    int getWidth() const;

    int getHeight() const;

    void resize(int newWidth, int newHeight);

    void bind() const;

    static void unbind();

    ~GLFrameBuffer();

private:
    std::vector<unsigned int> m_textureIds;

    int m_width{0};
    int m_height{0};

    InternalFormat m_internalFormat;
    std::set<Attachment> m_attachments;

    unsigned int m_fbo{0};
    unsigned int m_rbo{0};
};

ELIX_NAMESPACE_END

#endif //FRAME_BUFFER_HPP
