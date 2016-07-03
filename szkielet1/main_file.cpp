#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <thread>
#include <future>
#include <ctime>
#include <fstream>
#include <iterator>
#include <assert.h>
#include <string>
#include "constants.h"
#include "allmodels.h"
#include "resource.h"
#include "lodepng.h"
#include "myCube.h"


Models::Cube cube;
Models::Sphere sphere(0.5f, 36, 36);
Models::Sphere meal_sphere(0.1f, 36, 36);

float translate_speed1 = 0;
float translate_speed2 = 0;// [radiany/s]
float rotate_speed1 = 0;
float rotate_speed2 = 0;
float scale_speed = 0;

bool flagW = false;
bool flagA = false;
bool flagS = false;
bool flagD = false;

int posx = 2;
int posz = 1;

int enemyA_position_x = 18;
int enemyA_position_z = 16;
int enemyB_position_x = 1;
int enemyB_position_z = 18;
int enemyC_position_x = 16;
int enemyC_position_z = 18;

int meal_count = 150;

int time_begin;

GLuint my_textures[2];
unsigned width0, height0, width1, height1; // Zmienne do wczytania wymiarów tekstury
std::vector<unsigned char> myimage0;	// Wektor do wczytania obrazka
std::vector<unsigned char> myimage1;	// Wektor do wczytania obrazka


int tab[20][20] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0 },
	{ 0,1,1,1,1,1,1,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,1,0 },
	{ 0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,0 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0 },
	{ 0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};



bool meal_tab[20][20]
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0 },
	{ 0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0 },
	{ 0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,1,0,0,1,0 },
	{ 0,1,0,0,1,0,0,1,0,0,0,0,1,0,1,0,0,0,1,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,1,0 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0 },
	{ 0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0 },
	{ 0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0 },
	{ 0,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
};

void CreateMealTab()
{
	for (int i = 0; i < 20;++i)
	{
		for (int j = 0; j < 20;++j)
		{
			if (tab[i][j] == 1)
				meal_tab[i][j] = true;
			else
				meal_tab[i][j] = false;
		}
	}
}

void Movement()
{
	while (1)
	{
		Sleep(60);
		if (meal_tab[posx][posz] == true)
		{
			--meal_count;
			PlaySound(TEXT("pacman_chomp.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_NOSTOP);
		}
		meal_tab[posx][posz] = false;
		if (flagW)
		{
			if (tab[posx][posz + 1] == 1)
				posz += 1;
		}
		if (flagA)
		{
			if (tab[posx + 1][posz] == 1)
				posx += 1;
		}
		if (flagS)
		{
			if (tab[posx][posz - 1])
				posz -= 1;
		}
		if (flagD)
		{
			if (tab[posx - 1][posz])
				posx -= 1;
		}
	}
}

int level;

void EnemyMovement()
{
	static int moveA = rand() % 4;
	static int moveB = rand() % 4;
	static int moveC = rand() % 4;
	while (1)
	{
		if (level == 1)
			Sleep(170);
		else if (level == 2)
			Sleep(120);
		else if (level == 3)
			Sleep(70);
		else
			assert(false);

		if (moveA == 0)
		{
			if (tab[enemyA_position_x][enemyA_position_z + 1] == 1)
				enemyA_position_z += 1;
			else
			{
				int numbers[7] = { 0, 0, 1, 1, 2,3,3 };
				moveA = numbers[rand() % 7];
			}
		}
		else if (moveA == 1)
		{

			if (tab[enemyA_position_x + 1][enemyA_position_z] == 1)
				enemyA_position_x += 1;
			else
			{
				int numbers[7] = { 0, 0, 1, 1, 2,2,3 };
				moveA = numbers[rand() % 7];
			}
		}
		else if (moveA == 2)
		{
			if (tab[enemyA_position_x][enemyA_position_z - 1] == 1)
				enemyA_position_z -= 1;
			else
			{
				int numbers[7] = { 0, 1, 1, 2,2,3,3 };
				moveA = numbers[rand() % 7];
			}
		}
		else if (moveA == 3)
		{
			if (tab[enemyA_position_x - 1][enemyA_position_z] == 1)
				enemyA_position_x -= 1;
			else
			{
				int numbers[7] = { 0,0, 1, 2,2,3,3 };
				moveA = numbers[rand() % 7];
			}
		}

		if (moveB == 0)
		{
			if (tab[enemyB_position_x][enemyB_position_z + 1] == 1)
				enemyB_position_z += 1;
			else
			{
				int numbers[7] = { 0, 0, 1, 1, 2,3,3 };
				moveB = numbers[rand() % 7];
			}
		}
		else if (moveB == 1)
		{

			if (tab[enemyB_position_x + 1][enemyB_position_z] == 1)
				enemyB_position_x += 1;
			else
			{
				int numbers[7] = { 0, 0, 1, 1, 2,2,3 };
				moveB = numbers[rand() % 7];
			}
		}
		else if (moveB == 2)
		{
			if (tab[enemyB_position_x][enemyB_position_z - 1] == 1)
				enemyB_position_z -= 1;
			else
			{
				int numbers[7] = { 0, 1, 1, 2,2,3,3 };
				moveB = numbers[rand() % 7];
			}
		}
		else if (moveB == 3)
		{
			if (tab[enemyB_position_x - 1][enemyB_position_z] == 1)
				enemyB_position_x -= 1;
			else
			{
				int numbers[7] = { 0,0, 1, 2,2,3,3 };
				moveB = numbers[rand() % 7];
			}
		}

		if (moveC == 0)
		{
			if (tab[enemyC_position_x][enemyC_position_z + 1] == 1)
				enemyC_position_z += 1;
			else
			{
				int numbers[7] = { 0, 0, 1, 1, 2,3,3 };
				moveC = numbers[rand() % 7];
			}
		}
		else if (moveC == 1)
		{

			if (tab[enemyC_position_x + 1][enemyC_position_z] == 1)
				enemyC_position_x += 1;
			else
			{
				int numbers[7] = { 0, 0, 1, 1, 2,2,3 };
				moveC = numbers[rand() % 7];
			}
		}
		else if (moveC == 2)
		{
			if (tab[enemyC_position_x][enemyC_position_z - 1] == 1)
				enemyC_position_z -= 1;
			else
			{
				int numbers[7] = { 0, 1, 1, 2,2,3,3 };
				moveC = numbers[rand() % 7];
			}
		}
		else if (moveC == 3)
		{
			if (tab[enemyC_position_x - 1][enemyC_position_z] == 1)
				enemyC_position_x -= 1;
			else
			{
				int numbers[7] = { 0,0, 1, 2,2,3,3 };
				moveC = numbers[rand() % 7];
			}
		}
	}
}

void CheckCollisionAndWin(bool &exit)
{
	while (1)
	{
		if ((enemyA_position_x == posx && enemyA_position_z == posz)
			|| enemyB_position_x == posx && enemyB_position_z == posz
			|| enemyC_position_x == posx && enemyC_position_z==posz)
		{
			PlaySound(TEXT("pacman_death.wav"), nullptr, SND_ASYNC | SND_FILENAME);
			MessageBox(nullptr, TEXT("Przegrales"), TEXT("You lose"), MB_OK);
			exit = true;
			return;
		}
		if (!meal_count)
		{
			PlaySound(TEXT("epic_win.wav"), nullptr, SND_ASYNC | SND_FILENAME);

			std::fstream file("high_scores.txt", std::fstream::in | std::fstream::out);
			assert(file.good());

			file.seekp(0, file.end);
			double win_time = (double)(clock() - time_begin) / CLOCKS_PER_SEC;
			file << win_time << std::endl;

			file.seekg(0, file.beg);
			const std::vector<double> vec(std::istream_iterator<double>(file), {});
			const auto max_element_it = std::min_element(vec.begin(), vec.end());
			assert(max_element_it != vec.end());
			file.close();
			
			std::wstring text = TEXT("Wygrales!\nTwoj czas ") + std::to_wstring(win_time)+TEXT("\nNajlepszy czas ")+std::to_wstring(*max_element_it);
			MessageBox(nullptr, text.c_str(), TEXT("You win"), MB_OK);
			
			exit = true;
			return;
		}
	}
}

void DrawMealSphere(glm::mat4 M, glm::mat4 V, int x, int y, int z)
{
	M = glm::translate(M, glm::vec3(x, y, z));
	glLoadMatrixf(glm::value_ptr(V*M));
	meal_sphere.drawSolid();
}

void DrawSphere(glm::mat4 M, glm::mat4 V, int x, int y, int z)
{
	M = glm::translate(M, glm::vec3(x, y, z));
	glLoadMatrixf(glm::value_ptr(V*M));
	sphere.drawSolid();
}

void DrawEnemy(glm::mat4 M, glm::mat4 V, int x, int y, int z)
{
	M = glm::translate(M, glm::vec3(x, y, z));
	M = glm::scale(M, glm::vec3(0.5, 0.5, 0.5));
	glLoadMatrixf(glm::value_ptr(V*M));
	cube.drawSolid();
}

void DrawCube(glm::mat4 M, glm::mat4 V, int x, int y, int z)
{
	M = glm::translate(M, glm::vec3(x, y, z));
	M = glm::scale(M, glm::vec3(0.5, 0.5, 0.5));
	glLoadMatrixf(glm::value_ptr(V*M));
	cube.drawSolid();
}

void DrawWireCube(glm::mat4 M, glm::mat4 V, int x, int y, int z)
{
	M = glm::translate(M, glm::vec3(x, y, z));
	M = glm::scale(M, glm::vec3(0.5, 0.5, 0.5));
	glLoadMatrixf(glm::value_ptr(V*M));
	cube.drawWire();
}

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_LEFT) translate_speed1 = -3.14f;
		if (key == GLFW_KEY_RIGHT) translate_speed1 = 3.14f;
		if (key == GLFW_KEY_UP) translate_speed2 = -3.14f;
		if (key == GLFW_KEY_DOWN) translate_speed2 = 3.14f;
		if (key == GLFW_KEY_RIGHT && (mods & GLFW_MOD_SHIFT)) rotate_speed1 = -2.8f;
		if (key == GLFW_KEY_LEFT && (mods & GLFW_MOD_SHIFT)) rotate_speed1 = 2.8f;
		if (key == GLFW_KEY_UP && (mods & GLFW_MOD_SHIFT)) rotate_speed2 = -2.8f;
		if (key == GLFW_KEY_DOWN && (mods & GLFW_MOD_SHIFT)) rotate_speed2 = 2.8f;
		if (key == GLFW_KEY_W) flagW = true;
		if (key == GLFW_KEY_A) flagA = true;
		if (key == GLFW_KEY_S) flagS = true;
		if (key == GLFW_KEY_D) flagD = true;
		if (key == GLFW_KEY_KP_SUBTRACT) scale_speed = -0.3f;
		if (key == GLFW_KEY_KP_ADD) scale_speed = 0.3f;
	}

	if (action == GLFW_RELEASE)
	{
		translate_speed1 = 0;
		translate_speed2 = 0;
		rotate_speed1 = 0;
		rotate_speed2 = 0;
		flagW = false;
		flagA = false;
		flagS = false;
		flagD = false;
		scale_speed = 0;
	}
}

//Procedura obs³ugi b³êdów
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

GLfloat fLargest;

//Procedura inicjuj¹ca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który nale¿y wykonaæ raz, na pocz¹tku programu************
	glClearColor(0, 0, 0, 1); //Czyœæ ekran na czarno
	glEnable(GL_DEPTH_TEST); //W³¹cz u¿ywanie Z-Bufora
	glEnable(GL_COLOR_MATERIAL); //glColor3d ma modyfikowaæ w³asnoœci materia³u
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	glfwSetKeyCallback(window, key_callback);

	unsigned error = lodepng::decode(myimage0, width0, height0, "bricks.png");
	unsigned error2 = lodepng::decode(myimage1, width1, height1, "tile.png");

	if (!glewIsExtensionSupported("GL_EXT_texture_filter_anisotropic"))
		printf_s("Nie ma filtrowania anizotropowego");
	else
		printf_s("Jest filtrowanie anizotropowe");

	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

	glGenTextures(2, my_textures);

	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glBindTexture(GL_TEXTURE_2D, my_textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width0, height0, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)myimage0.data());

	glBindTexture(GL_TEXTURE_2D, my_textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)myimage1.data());
}

void DrawTexturedCube(glm::mat4 M, glm::mat4 V, double x, double y, double z, GLuint texture)
{
	M = glm::translate(M, glm::vec3(x, y, z));
	M = glm::scale(M, glm::vec3(0.5, 0.5, 0.5));
	glLoadMatrixf(glm::value_ptr(V*M));

	glBindTexture(GL_TEXTURE_2D, texture); //Wybierz teksturê


	glEnableClientState(GL_VERTEX_ARRAY); //W³¹cz uzywanie tablicy wspó³rzêdnych wierzcho³ków
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); //W³¹cz u¿ywanie tablicy wspó³rzêdnych teksturowania
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, myCubeVertices); //Zdefiniuj tablicê, która jest Ÿród³em wspó³rzêdnych wierzcho³ków
	glNormalPointer(GL_FLOAT, 0, geomNormals);
	glTexCoordPointer(2, GL_FLOAT, 0, myCubeTexCoords); //Zdefiniuj tablicê, która jest Ÿród³em wspó³rzêdnych teksturowania
	glDrawArrays(GL_QUADS, 0, myCubeVertexCount); //Narysuj obiekt
	glDisableClientState(GL_VERTEX_ARRAY); //Wy³¹cz uzywanie tablicy wspó³rzêdnych wierzcho³ków
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); //Wy³¹cz u¿ywanie tablicy wspó³rzêdnych teksturowania
	glDisableClientState(GL_NORMAL_ARRAY);
}

void DrawBoard(glm::mat4 M,glm::mat4 V)
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			if (tab[i][j] == 0)
			{
				glColor3d(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				DrawTexturedCube(M, V, i, 0, j, my_textures[0]);
				glDisable(GL_TEXTURE_2D);

				glColor3d(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				DrawTexturedCube(M, V, i, 1, j,my_textures[0]);
				glDisable(GL_TEXTURE_2D);
			}
			else
			{
				glColor3d(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				DrawTexturedCube(M, V, i, 0, j,my_textures[1]);
				glDisable(GL_TEXTURE_2D);

				if (meal_tab[i][j])
				{
					glColor3d(1, 1, 0);
					DrawMealSphere(M, V, i, 1, j);
				}
			}
			glColor3d(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			DrawTexturedCube(M, V, i, -1, j,my_textures[0]);
			glDisable(GL_TEXTURE_2D);
		}
	}
}

//Procedura rysuj¹ca zawartoœæ sceny
void drawScene(GLFWwindow* window, float translate_x, float translate_z, float an1, float an2,float sca) {
	//************Tutaj umieszczaj kod rysuj¹cy obraz******************l

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wykonaj czyszczenie bufora kolorów

	glm::mat4 V = glm::lookAt( //Wylicz macierz widoku
		glm::vec3(0.0f, 20.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 P = glm::perspective(50 * PI / 180, 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania
																	  //Za³aduj macierz rzutowania do OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));

	//PrzejdŸ w tryb pracy z macierz¹ Model-Widok
	glMatrixMode(GL_MODELVIEW);

	//Wylicz macierz obrotu o k¹t angle wokó³ osi (0,1,1)
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::rotate(M, an2, glm::vec3(1, 0, 0));
	M = glm::rotate(M, an1, glm::vec3(0, 0, 1));
	M = glm::translate(M, glm::vec3(translate_x - 10, 0, translate_z - 10));
	M = glm::scale(M, glm::vec3(sca, sca, sca));

	glLoadMatrixf(glm::value_ptr(V*M)); //Za³aduj wyliczon¹ macierz do OpenGL

	DrawBoard(M, V);

	glColor3d(1.0, 1.0, 0.0);
	DrawSphere(M, V, posx, 1, posz);

	glColor3d(0.0, 1.0, 0.0);
	DrawEnemy(M, V, enemyA_position_x, 1, enemyA_position_z);

	glColor3d(1.0, 0.0, 0.0);
	DrawEnemy(M, V, enemyB_position_x, 1, enemyB_position_z);

	glColor3d(0.0, 1.0, 1.0);
	DrawEnemy(M, V, enemyC_position_x, 1, enemyC_position_z);


	//Przerzuæ tylny bufor na przedni
	glfwSwapBuffers(window);

}

bool play;

BOOL CALLBACK Level(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_COMMAND:
	{
		// reakcja na przyciski
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			printf_s("Latwy\n");
			level = 1;
			EndDialog(hwnd, IDOK);
			break;
		}
		case IDCANCEL:
		{
			printf_s("Sredni\n");
			level = 2;
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		case IDHELP:
		{
			printf_s("Trudny\n");
			level = 3;
			EndDialog(hwnd, IDHELP);
			break;
		}
		}
	}
	break;

	default: return FALSE;
	}

	return TRUE;
}

BOOL CALLBACK Menu(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_COMMAND:
	{
		// reakcja na przyciski
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			printf_s("Nowa gra\n");
			play = true;
			EndDialog(hwnd, IDOK);
			DialogBox(GetModuleHandle(NULL),
				MAKEINTRESOURCE(IDD_LEVEL), 0, Level);
			break;
		}
		case IDHELP:
		{
			printf_s("Wyjscie\n");
			play = false;
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		case IDCANCEL:
		{
			printf_s("Tablica wynikow\n");
			std::fstream file("high_scores.txt", std::fstream::in | std::fstream::out);
			assert(file.good());

			std::vector<double> vec(std::istream_iterator<double>(file), {});
			std::sort(vec.begin(), vec.end());

			std::string s = "";

			for (unsigned int i = 0; i < vec.size(); ++i)
			{
				s += std::to_string(vec[i]);
				s += "\n";
			}
			printf_s(s.c_str());
			MessageBoxA(0, s.c_str(), "Tablica wynikow", MB_OK);
			file.close();

		}
		}
	}
	break;

	default: return FALSE;
	}

	return TRUE;
}


int main(void)
{
	bool exit_game = false;

	GLFWwindow* window; //WskaŸnik na obiekt reprezentuj¹cy okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1366, 768, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL. 

	if (!window) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		exit(EXIT_FAILURE);
	}
	PlaySound(TEXT("pacman_beginning.wav"), nullptr, SND_ASYNC | SND_LOOP);

	DialogBox(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_MENU), 0, Menu);

	if (!play)
		return 0;

	PlaySound(nullptr, nullptr, SND_ASYNC | SND_LOOP);

	std::thread t1(CheckCollisionAndWin, std::ref(exit_game));
	t1.detach();
	std::thread t3(EnemyMovement);
	t3.detach();
	std::thread t4(Movement);
	t4.detach();

	initOpenGLProgram(window); //Operacje inicjuj¹ce

	time_begin = clock();
	float translate_x = 2;
	float translate_z = 3;
	float an1 = 0;
	float an2 = 0;
	float sca = 0.9f;
	glfwSetTime(0); //Wyzeruj licznik czasu

					//G³ówna pêtla
	while (!glfwWindowShouldClose(window)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		translate_x += translate_speed1*static_cast<float>(glfwGetTime()); //Zwiêksz k¹t o prêdkoœæ k¹tow¹ razy czas jaki up³yn¹³ od poprzedniej klatki
		translate_z += translate_speed2*static_cast<float>(glfwGetTime());
		an1 += rotate_speed1*static_cast<float>(glfwGetTime());
		an2 += rotate_speed2*static_cast<float>(glfwGetTime());
		sca += scale_speed*static_cast<float>(glfwGetTime());
		if (exit_game) break;
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, translate_x, translate_z, an1, an2,sca); //Wykonaj procedurê rysuj¹c¹
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}

	glfwDestroyWindow(window); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}