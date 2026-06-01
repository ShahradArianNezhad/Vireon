#include "engine/materialManager/material/texture/texture.hpp"
#include "glad/gl.h"



class FrameBuffer{
  unsigned int ID;

  public:
  FrameBuffer(){glGenFramebuffers(1,&ID);};
  FrameBuffer(const FrameBuffer &) = delete;
  FrameBuffer &operator=(const FrameBuffer &) = delete;
  FrameBuffer(FrameBuffer &&other) noexcept;
  ~FrameBuffer(){glDeleteFramebuffers(1,&ID);};

  void bind(){glBindFramebuffer(GL_FRAMEBUFFER,ID);}

  void checkComplete(){
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE) LOG_ERROR("Framebuffer incomplete = {}",status);
  }

  void attachTexture(Texture& tex){glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex.getID(),0);}
  static inline void bindScreen(){glBindFramebuffer(GL_FRAMEBUFFER,0);}
};
