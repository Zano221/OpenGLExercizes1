#include "Scene1.h"

CScene1::CScene1()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;
	bSnowmanRotReverse = false;

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

	//Setar as cordenadas de spawn das arvores

	srand(time(0));
	int iRandomTreeAmmount = (rand() % 20) + 20;

	for (int i = 0; i < iRandomTreeAmmount; i++) {

		srand(time(0) + i + 1);
		
		float fRandPosX = (float)(rand() % 40) - 20;
		float fRandPosZ = (float)(rand() % 40) - 20;

		sTreePos tree;
		tree.x = fRandPosX;
		tree.y = 0.0f;
		tree.z = fRandPosZ;

		tree.fScaleX = ((float)rand() / (float)RAND_MAX) + 1.0f;
		tree.fScaleZ = ((float)rand() / (float)RAND_MAX) + 1.0f;
		srand(rand());
		tree.fScaleY = ((float)rand() / (float)RAND_MAX) + 1.0f;

		vTrees.push_back(tree);
		
	}

	iTreeAmmount = vTrees.size();

}


CScene1::~CScene1(void)
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




int CScene1::DrawGLScene(void)	// Função que desenha a cena
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
	//Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do sistema cartesiano
	//DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glColor4ub(225, 200, 200, 255);

	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex3f(-20.0f, 0.0f, -20.0f);
	glVertex3f(20.0f, 0.0f, -20.0f);
	glVertex3f(20.0f, 0.0f, 20.0f);
	glVertex3f(-20.0f, 0.0f, 20.0f);
	glEnd();
	glPopMatrix();

	glColor4ub(255, 255, 255, 255);

	for (int i = 0; i < iTreeAmmount; i++) {
		
		srand(time(0)+i+1);
		glPushMatrix();
		
		sTreePos tree = vTrees.at(i);

		float fTreePosX = tree.x;
		float fTreePosZ = tree.z;

		float fTreeScaleX = tree.fScaleX;
		float fTreeScaleY = tree.fScaleY;
		float fTreeScaleZ = tree.fScaleZ;


		glTranslatef(fTreePosX, 0.0, fTreePosZ);
		glScalef(fTreeScaleX, fTreeScaleY, fTreeScaleX);
		DrawTree();
		
		glPopMatrix();
	}

	/*glPushMatrix();
	glTranslatef(10.0, 0.0, -5.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 0.0, -5.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 0.0, 3.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 0.0, 5.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 0.0, 3.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 0.0, 8.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3.0, 0.0, 5.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(9.0, 0.0, 0.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-5.0, 0.0, -5.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.0, 0.0, -5.0);
	DrawTree();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.0, 0.0, -1.0);
	DrawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0, 0.0, 3.0);
	DrawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0, 0.0, 7.0);
	DrawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0, 0.0, 5.0);
	DrawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0, 0.0, 2.0);
	DrawTree();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.0, 0.0, 9.0);
	DrawTree();
	glPopMatrix();*/

	glPushMatrix();
	glRotatef(fSnowmanRotX, 0.0f, 0.0f, 1.0f);
	DrawSnowman();
	glPopMatrix();

	if (!bSnowmanRotReverse) {
		fSnowmanRotX++;
		if (fSnowmanRotX > 60.0f) bSnowmanRotReverse = true;
	}
	else if (bSnowmanRotReverse) {
		fSnowmanRotX--;
		if (fSnowmanRotX < -60.0f) bSnowmanRotReverse = false;
	}



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




void CScene1::MouseMove(void) // Tratamento de movimento do mouse
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

void CScene1::KeyPressed(void) // Tratamento de teclas pressionadas
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

void CScene1::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
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
/*void CScene1::Draw3DSGrid(float width, float length)
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



void CScene1::DrawAxis()
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
}*/


void CScene1::DrawTree()
{
	// face frente
	glTranslatef(0.0f, 2.0f, 0.0f);
	glColor3b(69, 36, 9);
	auxSolidCylinder(0.2, 3);
	glColor3b(0, 50, 0);
	glTranslatef(0.0f, 1.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	auxSolidCone(2, 3);
	glTranslatef(0.0f, 0.0f, 1.5f);
	auxSolidCone(2, 3);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
}


void CScene1::DrawSnowman()
{
	glColor3b(125, 125, 125);
	glTranslatef(0.0f, 0.5f, 0.0f);
	auxSolidSphere(0.5);
	glTranslatef(0.0f, 0.7f, 0.0f);
	auxSolidSphere(0.4);

	//Peito + Botões
	glColor3b(125, 0, 0);
	glTranslatef(0.0f, 0.2f, 0.4f);
	auxSolidSphere(0.08);
	glTranslatef(0.0f, -0.25f, 0.0f);
	auxSolidSphere(0.08);
	glTranslatef(0.0f, 0, -0.4f);

	//Cabeça
	glColor3b(125, 125, 125);
	glTranslatef(0.0f, 0.6f, 0.0f);
	auxSolidSphere(0.3);

	//Olhos e nariz
	glColor3b(0, 0, 0);
	glTranslatef(-0.1f, 0.1f, 0.25f);
	auxSolidSphere(0.06);
	glTranslatef(0.2f, 0.0f, 0.0f);
	auxSolidSphere(0.06);
	glTranslatef(-0.1f, -0.1f, -0.25f);

	glColor3b(125, 125, 0);
	auxSolidCone(0.1, 0.6);

	//Chapeu

	glColor3b(0, 0, 0);
	glTranslatef(0.0f, -0.65f, 0.0f);
	auxSolidCylinder(0.2, 0.1);
	glTranslatef(0.0f, 0.4f, 0.0f);
	auxSolidCylinder(0.1, 0.4);

	
}