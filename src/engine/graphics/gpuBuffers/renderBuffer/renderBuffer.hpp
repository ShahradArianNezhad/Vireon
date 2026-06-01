#include "glad/gl.h"
#include <cstddef>



class RenderBuffer{
  unsigned int ID;

  public:
  RenderBuffer(){glGenRenderbuffers(1,&ID);};
  RenderBuffer(const RenderBuffer &) = delete;
  RenderBuffer &operator=(const RenderBuffer &) = delete;
  RenderBuffer(RenderBuffer &&other) noexcept;
  ~RenderBuffer(){glDeleteRenderbuffers(1,&ID);};


  void bind(){glBindRenderbuffer(GL_RENDERBUFFER, ID);}
  void unbind(){glBindRenderbuffer(GL_RENDERBUFFER, 0);}
  void useOnFrameBuffer(){glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ID);}
  void setRenderBufferSize(size_t w,size_t h){glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w,h);}
};
