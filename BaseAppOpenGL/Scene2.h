#pragma once
#include "SceneBaseClass.h"
#include "CTexture.h"
#include "CTimer.h"
#include "CTexto.h"
#include "CCamera.h"
#include "VertexData.h"
#include <glm/glm.hpp>
#include <vector>

#define CUBE_COLS 5
#define CUBE_ROWS 10

struct stCubeInfo {
	float fCubePos[3];
	GLubyte CubeColor[4];
};

class CScene2 : public CSceneBaseClass
{
public:
	CScene2();
	~CScene2(void);


	virtual void MouseMove(void);					// Tratamento de movimento do mouse
	virtual void KeyPressed(void);					// Tratamento de teclas pressionadas
	virtual void KeyDownPressed(WPARAM	wParam);	// Tratamento de teclas pressionadas
	virtual int DrawGLScene(void);					// Função que desenha a cena

	void Draw3DSGrid(float width, float length);
	void DrawAxis();
	void RenderCubes();
	void WipeCubes();

private:

	bool	keys[256];		// Array usado para rotinas do teclado
	bool	active;			// Window Active Flag Set To TRUE By Default
	bool	fullscreen;		// Exibir janela em modo fullscreem (TRUE) ou em janela (FALSE)


	CCamera* pCamera;	// Gerencia câmera OpenGL
	float fDeltaY;				// Rotação da câmera OpenGL no eixo Y
	float fDeltaX;				// Rotação da câmera OpenGL no eixo X

	CTexto* pTexto;	// Objeto que gerencia texto
	CTexture* pTextures;	// Objeto que gerencia texturas
	CTimer* pTimer;	// Objeto que gerencia o timer


	int		iFPS;			// FPS and FPS Counter
	int		iFrames;		// FPS and FPS Counter
	DWORD	ulLastFPS;		// FPS and FPS Counter
	char	szTitle[256];	// FPS and FPS Counter

	bool bGameReset;

	int iFullCollumns;
	int iCubeSpawnTime; // Ultra gambiarra
	int iCubeRows[5];
	bool bFullCollumns[5];
	std::vector<stCubeInfo> vCubes;

	bool bIsWireframe;	// Modos Wireframe/Solid
	bool bIsCameraFPS;	// Ativa modo de camera First Person Shooter (true) ou Third Person Shooter (false)
	bool bSnowmanRotReverse;

	float fRenderPosY;
	float fTimerPosY;

	float fRotY;
	float fSnowmanRotX;

	float fPosX;
	float fPosY;
	float fPosZ;

	unsigned char R;
	unsigned char G;
	unsigned char B;

};

