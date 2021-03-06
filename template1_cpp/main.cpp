#include "common.h"
#include "Image.h"
#include "Player.h"
#include <GLFW/glfw3.h>
# define GLFW_DLL
# define GAME_OVER -2
# define GAME_TRAP -1
# define GAME_FIND_OUT 0


 std::vector<std::vector<int>> map;
// std::vector<int> vec;
int	counter_levels = 1;

struct		InputState
{
	bool	keys[1024]{};
	GLfloat lastX = 400, lastY = 300;
	bool	firstMouse = true;
	bool	captureMouse = true;
	bool	capturedMouseJustNow = false;
}static		Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_1:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case GLFW_KEY_2:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		if (action == GLFW_PRESS)
			Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
			Input.keys[key] = false;
	}
}

void Player::processPlayerMovement()
{
	if (Input.keys[GLFW_KEY_W]) {
		ProcessInput(MovementDir::DOWN);
	}
	else if (Input.keys[GLFW_KEY_S])
		ProcessInput(MovementDir::UP);
	if (Input.keys[GLFW_KEY_A])
		ProcessInput(MovementDir::LEFT);
	else if (Input.keys[GLFW_KEY_D])
		ProcessInput(MovementDir::RIGHT);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		Input.captureMouse = !Input.captureMouse;

	if (Input.captureMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Input.capturedMouseJustNow = true;
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (Input.firstMouse)
	{
		Input.lastX = float(xpos);
		Input.lastY = float(ypos);
		Input.firstMouse = false;
	}

	GLfloat xoffset = float(xpos) - Input.lastX;
	GLfloat yoffset = Input.lastY - float(ypos);

	Input.lastX = float(xpos);
	Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	// ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "Controls: "<< std::endl;
	std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
	std::cout << "W, A, S, D - movement  "<< std::endl;
	std::cout << "press ESC to exit" << std::endl;

	return 0;
}

int	GameLoop(GLFWwindow* window, Image &screenBuffer, Image &tmp,
				Player &player, std::string file_name, int counter_levels) {
	std::vector<int> vec;
	player.map_draw(file_name, screenBuffer, tmp, vec);
	while (!glfwWindowShouldClose(window))
	{
		int ret = 5;
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		player.processPlayerMovement();
		player.BlockWall();


		ret = player.Draw(screenBuffer, tmp, counter_levels);
		if (ret == GAME_TRAP) {
			glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
			glfwSwapBuffers(window);
			sleep(3);
			return (GAME_OVER);
		}
		else if (ret == GAME_FIND_OUT) {
			glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
			glfwSwapBuffers(window);
			sleep(1);
			glfwSwapBuffers(window);
			return (GAME_FIND_OUT);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
		glRasterPos2f(-1, 1);
		glPixelZoom(1, -1);
		glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return (2);
}


int main(int argc, char** argv)
{
	if(!glfwInit())
		return -1;
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ROGUELIKE GAME BY NINA", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback        (window, OnKeyboardPressed);
	glfwSetCursorPosCallback  (window, OnMouseMove);
	glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0)
		return -1;
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();
	Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
	Player player{starting_pos};
	Image img("../resources/tex.png");
	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
	Image tmp(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;

	int game = -10;
	if ((game = GameLoop(window, screenBuffer, tmp, player, "../map_level_1.txt", counter_levels) == GAME_FIND_OUT)) {
		++counter_levels;
	//	map.clear();
		if ((game = GameLoop(window, screenBuffer, tmp, player, "../map_level_2.txt", counter_levels)) == GAME_FIND_OUT) {
			screenBuffer.DrawPicForSeconds(Image("../resources/image.png"), 250, 250);
			glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
			glfwSwapBuffers(window);
			sleep(1);
			return (0);
		}
	}
	return (0);
}
