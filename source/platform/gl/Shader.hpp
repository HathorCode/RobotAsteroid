/*
	A loader for simple.vert and simple.frag.
*/

#pragma once
#include "GLProcPtrs.hpp"
#include "Sprite.hpp"

#include <platform/Enum.hpp>

#include <glm/mat3x3.hpp>

namespace robitRabit {
	struct MainShader {
		GLuint glslProgram;
		GLuint vertexArray;
		GLint mdlViewMatrixLoc;		//World-view Uniform location
		GLuint vertexAttribArray;
		GLint textureSamplerLoc;	//Sampler Uniform location

		//GLSL shader attribute names, must appear in the same order they appear in shader programs.
		enum AttribNames {
			vertex,
			vertexTexCoord,
		};

		ShaderInitError Init(const uint32 vsTextSize, const GLchar* vsText, const uint32 fsTextSize, const GLchar* fsText) {
			//Compose shader data objects into arrays so we can use loops
			uint32 shaderSizes[2] = {vsTextSize, fsTextSize};
			const GLchar* shaderTexts[2] = {vsText, fsText};
			GLuint shaders[2];
			GLchar* shaderErrorMsgs[2] = {"Vertex shader failed to load:\n", "Pixel shader failed to load:\n"};
			shaders[0] = glCreateShader(GL_VERTEX_SHADER);
			shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);

			//Create the program ahead of time, so we don't create it twice in the loop
			this->glslProgram = glCreateProgram();

			//Do this for both shaders
			for (uint32 i = 0; i < 2; i++) {
				glShaderSource(shaders[i], 1, &shaderTexts[i], (GLint*)&shaderSizes[i]);
				glCompileShader(shaders[i]);

				//Check for compiler errors
				GLint compileSuccess;
				glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &compileSuccess);
				if (compileSuccess == GL_FALSE) {
					Str shaderErrorLog;
					glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, (GLint*)&shaderErrorLog.len);

					shaderErrorLog.str = (char*)alloc(shaderErrorLog.len);
					GLint fakeLength;
					glGetShaderInfoLog(shaders[i], shaderErrorLog.len, &fakeLength, shaderErrorLog.str);

					log.Write(shaderErrorMsgs[i]);
					shaderErrorLog.len--;       //Length includes null terminator; we don't want to print it
					log.Write(shaderErrorLog);
					log.Write("\n");
					//No need to cleanup, this is a fatal error
					return SIE_failedToCompile;
				}

				glAttachShader(this->glslProgram, shaders[i]);
			}

			//Link
			glBindAttribLocation(glslProgram, AttribNames::vertex, "vertex");
			glBindAttribLocation(glslProgram, AttribNames::vertexTexCoord, "vertexTexCoord");
			glLinkProgram(this->glslProgram);

			//Check for link errors
			GLint linkSuccess;
			glGetProgramiv(this->glslProgram, GL_LINK_STATUS, &linkSuccess);
			if (linkSuccess == GL_FALSE) {
				GLint infoLength = 0;
				glGetProgramiv(this->glslProgram, GL_INFO_LOG_LENGTH, &infoLength);

				char* errorLog = new char[infoLength];
				glGetProgramInfoLog(this->glslProgram, infoLength, &infoLength, errorLog);

				//TODO: log it

				//Exit with failure.
				delete[] errorLog;
				glDeleteProgram(this->glslProgram); //Don't leak the program
				return SIE_failedToLink;
			}

			//Remove shaders; we only care about the program
			glDetachShader(this->glslProgram, shaders[0]);
			glDetachShader(this->glslProgram, shaders[1]);
			glDeleteShader(shaders[0]);
			glDeleteShader(shaders[1]);

			//Set up shader variables
			glGenVertexArrays(1, &this->vertexArray);
			this->Use();

			//Uniforms
			mdlViewMatrixLoc = glGetUniformLocation(this->glslProgram, "mdlViewMatrix");
			textureSamplerLoc = glGetUniformLocation(this->glslProgram, "textureSampler");

			//Set up vertex attributes
			glVertexAttribPointer(AttribNames::vertexTexCoord,
			                      2,
			                      GL_FLOAT,
			                      GL_FALSE,
			                      0,
			                      0);
			return (robitRabit::ShaderInitError)0;
		}
		void Use() const {
			glUseProgram(this->glslProgram);
			glBindVertexArray(this->vertexArray);
			glEnableVertexAttribArray(AttribNames::vertexTexCoord);
		}
		void SetShaderUniforms(const glm::mat3 *mdlViewMatrix) const {
			glUniform1i(this->textureSamplerLoc, 0);
			glUniformMatrix3fv(this->mdlViewMatrixLoc, 1, GL_FALSE, (GLfloat*)mdlViewMatrix);
		}
		void Draw(const Sprite sprite, const glm::mat3 *mdlViewMatrix) {
			Use();

			//Set vertices pointer
			glEnableVertexAttribArray(AttribNames::vertex);
			glVertexAttribPointer(AttribNames::vertex,
			                      2,
			                      GL_FLOAT,
			                      GL_FALSE,
			                      0,
			                      sprite.bytesBufferOffset);

			//Set matrices
			SetShaderUniforms(mdlViewMatrix);

			sprite.texture.Bind();
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
	} mainShader;

}
