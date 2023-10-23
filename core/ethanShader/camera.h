#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "transformations.h"

namespace ethanShader {
	
	struct Camera {
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		//ew::Mat4 ViewMatrix(); //World->View
		//ew::Mat4 ProjectionMatrix(); //View->Clip

		ew::Mat4 ViewMatrix()
		{
			return ethanShader::LookAt(position, target, ew::Vec3(0, 1, 0));
		};
		
		ew::Mat4 ProjectionMatrix()
		{
			switch (orthographic)
			{
			case true:
				return ethanShader::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
				break;
			case false:
				return ethanShader::Perspective(fov, aspectRatio, nearPlane, farPlane);
				break;
			}
		};
	};

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = 0, pitch = 0; //Degrees
		float mouseSensitivity = 0.005f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 2.5f; //How fast to move with arrow keys (M/S)
	};

}
