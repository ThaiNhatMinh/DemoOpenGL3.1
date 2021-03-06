// DemoOpenGL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	glfwInit();														// Khơi tạo glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// Major của phiên bản OpenGL 3.1: 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);					// Minor của phiên bản OpenGL 3.1: 1
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);	// Xác định Profile của ứng dụng
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);						// Kích thước cửa sổ windows không thay đổi


	/* glfwCreateWindow()
		- 2 tham số đầu là kích thước của windows
		- Tham số thứ 3 là tiêu đề của của sổ Windows
		- 2 tham số còn lại không dùng nên để nullptr
	*/
	GLFWwindow* pWindows = glfwCreateWindow(800, 600, "Demo OpenGL", nullptr, nullptr);

	if (pWindows == nullptr)						// Kiểm tra khởi tạo thành công hay không
	{
		std::cout << "Failed to create Window.";	// Xuất lỗi và kết thúc chương trình
		return 0;
	}

	glfwMakeContextCurrent(pWindows);				// Tạo ngữ cảnh OpenGL cho cửa sổ hiện tại

	glewExperimental = GL_TRUE;			

	if (glewInit() != GLEW_OK)						// Khởi tạo glew và kiểm tra lỗi
	{
		std::cout << "Failed to init glew.";		// Xuất lỗi và kết thúc chương trình
		return 0;
	}

	glViewport(0, 0, 800, 600);						// Thiết lập vùng nhìn của OpenGL trùng với kích thức cửa sổ Window


	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,							// Đỉnh đầu tiên
		0.5f, -0.5f, 0.0f,							// Đỉnh thứ hai
		0.0f, 0.5f, 0.0f };							// Đỉnh thứ 3

	GLuint VBO;

	/* Khởi tạo buffer object
		- Tham  số đầu là số lượng buffer object mà ta muốn tạo
		- Tham số thứ 2 là địa chỉ đến biến chưa buffer object đó
	*/
	glGenBuffers(1, &VBO);							

	/* Kích hoạt buffer object để sử dụng
		- Tham số đầu tiên là loại dữ liệu mà ta muốn kích hoạt. 
		  Ở đây GL_ARRAY_BUFFER tương ứng là dữ liệu các đỉnh tam giác
		- Tham số thứ 2 là buffer object mà ta muốn kích hoạt
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		

	/* Nạp dữ liệu cho buffer object. Trước khi nạp thì buffer object phải được kích hoạt trước
		- Tham số đầu tiên là loại dữ liệu mà ta muốn kích hoạt.
		- Tham số thứ 2 là kích thước dữ liệu
		- Tham số thứ 3 là địa chỉ đến dữ liệu.
		- Tham số thứ 4 là loại hành động mà ta muốn khi dùng buffer object. 
		  GL_STATIC_DRAW nghĩa là buffer object chỉ dùng để vẽ. Không cập nhật dữ liệu hay chỉnh sửa gì.
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	const GLchar* vertexShaderData = "#version 140\n"		// OpenGL 3.1 hỗ trợ shader phiên bản 140 
		"in vec3 pos;\n"								// Dữ liệu đầu vào được khai báo bằng từ khóa "in", pos chứa đỉnh của tam giác
		"void main(){\n"								// chương trình chính
		"gl_Position = vec4(pos,1.0);\n"				// gl_Position là biến được GLSL định ngĩa sẵn để dùng. Chứa tọa độ trong NDC 
		"}";

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);			// Khởi tạo vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderData, NULL);	// Truyền chương trình cho shader
	glCompileShader(vertexShader);								// Biên dịch shader

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);	// Kiểm tra biên dịch

	if (!success)												// Nếu biên dịch thất bại
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);	// Lấy lỗi
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<infoLog << std::endl;
	}

	const GLchar* fragShaderData = "#version 140\n"		// OpenGL 3.1 hỗ trợ shader phiên bản 140 
		"out vec4 color;\n"								// Dữ liệu đầu ra được khai báo bằng từ khóa "out", color chứa màu sắc mà ta muốn xuất ra
		"void main(){\n"								// chương trình chính
		" color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"		// Ở ví dụ này thì màu sắc được cố định 
		"}";


	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);			// Khởi tạo fragment shader
	glShaderSource(fragmentShader, 1, &fragShaderData, NULL);	// Truyền chương trình cho shader
	glCompileShader(fragmentShader);								// Biên dịch shader


	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);	// Kiểm tra biên dịch

	if (!success)												// Nếu biên dịch thất bại
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);	// Lấy lỗi
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();							// Khởi tạo shader program
	glAttachShader(shaderProgram, vertexShader);				// Đưa Vertex shader vào shader program
	glAttachShader(shaderProgram, fragmentShader);				// Đưa Fragment shader vào shader program
	glLinkProgram(shaderProgram);								// Liên kết các shader lại

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	// kiểm tra lỗi

	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);	// Lấy thông tin lỗi dưới dạng text
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	// Mỗi khi muốn dùng shader ta gọi glUseProgram()
	glUseProgram(shaderProgram);

	/*
		Vòng lặp để giữ cho chương trình luôn chạy và chỉ kết thúc khi người dùng muốn tắt

	*/
	while (!glfwWindowShouldClose(pWindows))		// Kiểm tra cửa sổ windows có đóng hay không
	{
		glfwPollEvents();							// glfw xử lý sự kiện từ bàn phím và chuột

		glClearColor(0.5f, 1.0f, 0.5f, 1.0f);		// Đưa màu về mặc định
		glClear(GL_COLOR_BUFFER_BIT);				// Xóa bit màu

		// Kích hoạt buffer object
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Truyền dữ liệu
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Thiết lập cấu trúc dữ liệu
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid *)0);
		// Kích hoạt luồn dữ liệu
		glEnableVertexAttribArray(0);
		// Kích hoạt shader
		glUseProgram(shaderProgram);

		// Vẽ hình
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(pWindows);					// Tráo đổi vùng đệm front và back
	}

	glfwTerminate();								// Giải phóng bộ nhớ mà glfw đang giữ

	return 0;
}

