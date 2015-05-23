/*
	Initializes and loads opengl
*/
#pragma once
#include "Win.hpp"

#include <engine/renderer/Renderer.hpp>

#include <platform/gl/GLProcPtrs.hpp>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

#pragma warning(push)
#pragma warning(disable:4706)

//OpenGL procedure pointers
PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC   wglCreateContextAttribsARB;

namespace robitRabit {
	struct OGL {
		HGLRC handle;
		
		void Init() {
			//TODO: separate pixel format enumeraiton and do a better job of it
			//Start with a basic format for old ogl
			PIXELFORMATDESCRIPTOR pxFormatDesc = {
				sizeof(PIXELFORMATDESCRIPTOR),                  //size
				1,                                              //version
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL         //flags
				| PFD_DOUBLEBUFFER | PFD_GENERIC_ACCELERATED,
				PFD_TYPE_RGBA,                                  //pixel type
				32,                                             //pixel bits
				8,                                              //red bits
				0,                                              //red shift
				8,                                              //green bits
				0,                                              //green shift
				8,                                              //blue bits
				0,                                              //blue shift
				8,                                              //alpha bits
				0,                                              //alpha shift
				0,                                              //accum bits
				0,                                              //accum red bits
				0,                                              //accum green bits
				0,                                              //accum blue bits
				0,                                              //accum alpha bits
				0,                                              //depth bits
				0,                                              //stencil bits
				0,                                              //auxilary buffers
				0,                                              //layer type
				0,                                              //reserved
				0,                                              //layer mask
				0,                                              //visible mask
				0,                                              //damage mask
			};
			
			int32 pxForm = ChoosePixelFormat(win.deviceContext, &pxFormatDesc);
			if (pxForm != -1) {
				SetPixelFormat(win.deviceContext, pxForm, &pxFormatDesc);

				//Create a simple old rendering context
				handle = wglCreateContext(win.deviceContext);
				assert(handle != nullptr);                //There is no reason this should ever fail
				wglMakeCurrent(win.deviceContext, handle);

				//Try to create a modern context with 
				if (LoadAdvancedContextGLProcs()) {
					//Now get a proper modern context
					const int iAttribIList[] = {
						WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
						WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
						WGL_SWAP_METHOD_ARB, WGL_SWAP_EXCHANGE_ARB,
						WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
						WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
						WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
						WGL_COLOR_BITS_ARB, 24,
						WGL_RED_BITS_ARB, 8,
						WGL_RED_SHIFT_ARB, 0,
						WGL_GREEN_BITS_ARB, 8,
						WGL_GREEN_SHIFT_ARB, 0,
						WGL_BLUE_BITS_ARB, 8,
						WGL_BLUE_SHIFT_ARB, 0,
						WGL_ALPHA_BITS_ARB, 0,
						WGL_ALPHA_SHIFT_ARB, 0,
						WGL_DEPTH_BITS_ARB, 16,
						WGL_STENCIL_BITS_ARB, 0,
						WGL_AUX_BUFFERS_ARB, 0,
						0
					};
					int32 pixelFormat;
					UINT numPixelFormatsReturned;
					if (wglChoosePixelFormatARB(win.deviceContext,
					                            iAttribIList,
					                            nullptr,
					                            1,
					                            &pixelFormat,
					                            &numPixelFormatsReturned)
						&& numPixelFormatsReturned > 0) {
						//Destroy legacy objects
						wglMakeCurrent(nullptr, nullptr);
						wglDeleteContext(handle);
						win.Cleanup();

						//Setup new pixel format
						BOOL worked = win.Init();
						assert(worked); //Shouldn't fail when it succeeded with the same settings
						worked = SetPixelFormat(win.deviceContext,
						                        pixelFormat,
						                        &pxFormatDesc);  //This has no effect for us, so we can use the legacy one
						assert(worked);

						//Create extended context
						int glAdvContextAttribList[] = {
							WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
							WGL_CONTEXT_MINOR_VERSION_ARB, 2,
							//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, implied by default value
#                           ifdef _DEBUG
								WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#                           endif
							0
						};
						if (nullptr != (handle = wglCreateContextAttribsARB(win.deviceContext, nullptr, glAdvContextAttribList))) {
							worked = wglMakeCurrent(win.deviceContext, handle);
							assert(worked);
						} else {
							log.Write("[OpenGL] At least version 3.2 of OpenGL is required to run ToastCat.\n"
							          "Please update your graphics drivers\n");
						}
					} else {
						log.Write("[OpenGL] At least version 3.2 of OpenGL is required to run ToastCat.\n"
						          "Please update your graphics drivers\n");
						log.Write(GetLastError());
						ExitProgram();
						/*TODO: better pixel format loading should retry until minimum features
						cannot be retrieved, and should warn the user of unavailable formats,
						and the reason they are unavailable.*/
					}
				}

				//Load OpenGL procedures
				if (LoadGLProcs()) {
					renderer.Init();
				} else { MessageBox(nullptr,
				                    "This program requires at least OpenGL version 3.2 which was not found on this computer. Please Update your GPU drivers.",
				                    "OpenGL Version Not Supported",
				                    MB_OK);
					log.Write("Fatal error, OpenGL is out of date.");
					ExitProgram();
					log.Write("OpenGL functions failed to load, glewGetErrorString reports:\n\t");
				}
			} else {
				log.Write("Failed to create a pixel format, GetLastError reports: ");
				char lastError[11];
				sprintf_s<11>(lastError, "%d", GetLastError());
				log.Write(lastError);
				log.Write("\nTry updating your graphics drivers!");
				ExitProgram();
			}
		}

		static bool LoadAdvancedContextGLProcs() {
			if(!(wglChoosePixelFormatARB=(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB"))){return false;}
			if(!(wglCreateContextAttribsARB=(PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB"))){return false;}
			return true;
		}
		static bool LoadGLProcs() {
			/*Use below regular expression to generate from above
			  \(.+?\)\s-+\(.+\);
			  if(!(\2=(\1)wglGetProcAddress("\2"))){return false;}
			*/
			if(!(glTexImage3D=(PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D"))){return false;}
			if(!(glBindAttribLocation=(PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation"))){return false;}
			if(!(glCreateShader=(PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader"))){return false;}
			if(!(glCreateProgram=(PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram"))){return false;}
			if(!(glShaderSource=(PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource"))){return false;}
			if(!(glCompileShader=(PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader"))){return false;}
			if(!(glGetShaderiv=(PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv"))){return false;}
			if(!(glGetShaderInfoLog=(PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog"))){return false;}
			if(!(glAttachShader=(PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader"))){return false;}
			if(!(glLinkProgram=(PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram"))){return false;}
			if(!(glGetProgramiv=(PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv"))){return false;}
			if(!(glGetProgramInfoLog=(PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"))){return false;}
			if(!(glDeleteProgram=(PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram"))){return false;}
			if(!(glDetachShader=(PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader"))){return false;}
			if(!(glDeleteShader=(PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader"))){return false;}
			if(!(glGenVertexArrays=(PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays"))){return false;}
			if(!(glGetUniformLocation=(PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation"))){return false;}
			if(!(glEnableVertexAttribArray=(PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray"))){return false;}
			if(!(glVertexAttribPointer=(PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer"))){return false;}
			if(!(glDeleteVertexArrays=(PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays"))){return false;}
			if(!(glUseProgram=(PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram"))){return false;}
			if(!(glBindVertexArray=(PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray"))){return false;}
			if(!(glUniform1i=(PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i"))){return false;}
			if(!(glUniformMatrix3fv=(PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress("glUniformMatrix3fv"))){return false;}
			if(!(glGenBuffers=(PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers"))){return false;}
			if(!(glDeleteBuffers=(PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers"))){return false;}
			if(!(glBufferData=(PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData"))){return false;}
			if(!(glBufferSubData=(PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData"))){return false;}
			if(!(glBindBuffer=(PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer"))){return false;}
			if(!(glBlendEquation=(PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation"))){return false;}
			if(!(glActiveTexture=(PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture"))){return false;}
			return true;
		}
	} gl;   //The directX graphics manager is also called gl.
}

#pragma warning(pop)
