#include "Scene2.h"
#include <time.h>
#include <iostream>

CScene2::CScene2()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f, 0.1f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	// pTextures = new CTexture();
	// pTextures->CreateTextureMipMap(0, "../Scene1/CRATE.BMP");
	// pTextures->CreateTextureMipMap(1, "../Scene1/TriangleFaces.BMP");



	fRotY = 0.0f;
	fSnowmanRotX = 0.0f;

	fPosX = 0.0f;
	fPosY = 0.0f;
	fPosZ = 0.0f;

	/// SCENE 2 INIT
	
	iFullCollumns = 0;

	// Cria o timer especifico dos cubos
	iCubeSpawnTime = time(0);

	for (int i = 0; i < 5; i++) {
		bFullCollumns[i] = true;
		iCubeRows[i] = 0;
	}

	bGameReset = false;
}


CScene2::~CScene2(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}
}




int CScene2::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter

	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int iCurrTime = time(0); // Isso é pra basicamente fazer com que o if abaixo execute somente uma vez por segundo já que tudo isso é um loop constante

	if (iCurrTime != iCubeSpawnTime && !bGameReset) {
		iCubeSpawnTime = iCurrTime;

		srand(iCubeSpawnTime);
		int iPosSpawn = rand() % CUBE_COLS;
		
		// Checar as colunas que estão cheias...
		iFullCollumns = 0;
		for (int i = 0; i < CUBE_COLS; i++) {
			if (iCubeRows[i] >= CUBE_ROWS) {
				bFullCollumns[i] = true;
				iFullCollumns++;

			}
		}

		if (iFullCollumns >= CUBE_COLS) {
			bGameReset = true;
		}
		
		if (bFullCollumns[iPosSpawn] && !bGameReset) {

			int iAvailablePos = rand() % (CUBE_COLS - iFullCollumns); // Ele vai gerar um numero relativo as posições disponiveis, não absoluto, exemplo, se a posição 0, e 3 estiverem cheias, ele vai regerar um numero de 0-2, e vai ignorar os que estão cheios para inserir nos novos
			
			int iNewPos = 0;
			for (int i = 0; i < CUBE_COLS; i++) {
				if (!bFullCollumns[i]) { // Se a coluna não estiver cheia..
					if (iNewPos == iAvailablePos) {
						iPosSpawn = i; // e se a posição sorteada for correta então sera a a nova posição 
 					}
					else iNewPos++; // Se não bola pra frente...
				}
				
			}
		}

		if (!bGameReset) {

			int iColorSeed = iCubeSpawnTime;
			srand(iColorSeed);
			GLubyte R = rand() % 255;
			srand(iColorSeed);
			iColorSeed += rand();
			GLubyte G = rand() % 255;
			srand(iColorSeed);
			iColorSeed += rand();
			GLubyte B = rand() % 255;

			stCubeInfo stCube;
			stCube.fCubePos[0] = (float)iPosSpawn - 2;
			stCube.fCubePos[1] = (float)iCubeRows[iPosSpawn];
			stCube.fCubePos[2] = 0.0f;

			stCube.CubeColor[0] = R;
			stCube.CubeColor[1] = G;
			stCube.CubeColor[2] = B;

			vCubes.push_back(stCube);

			iCubeRows[iPosSpawn]++;
		}
	}
	
	// Renderizar todos os cubos

	for (int i = 0; i < vCubes.size(); i++) {
		glPushMatrix();

		stCubeInfo cube = vCubes.at(i);
		glTranslatef(cube.fCubePos[0], cube.fCubePos[1], cube.fCubePos[2]);
		glColor4ub(cube.CubeColor[0], cube.CubeColor[1], cube.CubeColor[2], 255);
		auxSolidCube(1.0);

		glPopMatrix();

	}
	
	if (!bGameReset) RenderCubes();
	else WipeCubes();

	fRotY += 1.0f;
	if (fRotY >= 360.0f)
		fRotY = 0.0f;






	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 150, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));

	glRasterPos2f(10.0f, 100.0f);
	pTexto->glPrint("fRot: %.1f ", fRotY);




	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene2::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene2::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	else if (GetKeyState('Q') & 0x80)
	{
		pCamera->moveGlob(0.0f, -pCamera->Up[1], 0.0f);
	}
	else if (GetKeyState('E') & 0x80)
	{
		pCamera->moveGlob(0.0f, pCamera->Up[1], 0.0f);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}


	if (GetKeyState(VK_LEFT) & 0x80)
		fPosX -= 0.1f;
	if (GetKeyState(VK_RIGHT) & 0x80)
		fPosX += 0.1f;
	if (GetKeyState(VK_UP) & 0x80)
		fPosZ -= 0.1f;
	if (GetKeyState(VK_DOWN) & 0x80)
		fPosZ += 0.1f;
	if (GetKeyState(VK_PRIOR) & 0x80)
		fPosY += 0.1f;
	if (GetKeyState(VK_NEXT) & 0x80)
		fPosY -= 0.1f;


}

void CScene2::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
		pTimer->Init();
		break;

	case VK_RETURN:
		break;

	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene2::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}



void CScene2::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}

void CScene2::RenderCubes() {
	for (int i = 0; i < vCubes.size(); i++) {
		glPushMatrix();

		stCubeInfo cube = vCubes.at(i);
		glTranslatef(cube.fCubePos[0], cube.fCubePos[1], cube.fCubePos[2]);
		glColor4ub(cube.CubeColor[0], cube.CubeColor[1], cube.CubeColor[2], 255);
		auxSolidCube(1.0);

		glPopMatrix();

	}
}

void CScene2::WipeCubes() {
	vCubes.clear();

	for (int i = 0; i < CUBE_COLS; i++) {
		iCubeRows[i] = 0;
		bFullCollumns[i] = false;
		iFullCollumns = 0;
		bGameReset = false;

	}
	
}