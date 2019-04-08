// Library Includes
#include <ctime>
#include <string>


// Local Includes
#include "d3dApp.h"
#include "d3dUtil.h"
#include "game_manager.h"
#include "mouse_input.h"
#include "keyboard_input.h"
#include "services.h"

class SnakeApp : public D3DApp
{
public:
	SnakeApp(HINSTANCE hInstance);
	~SnakeApp();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene(); 

	void ResizeWindow( const int _iWidth, const int _iHeight );

private:
	void buildFX();
	void buildVertexLayouts();
	void PopulateTextureArray();
 
private:
	ID3D10Effect* mFX;
	ID3D10EffectTechnique* mTech;
	ID3D10InputLayout* mVertexLayout;

	ID3D10EffectMatrixVariable* mfxWVPVar;

	D3DXMATRIX m_World;

	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_WVP;

	D3DXVECTOR3 m_EyePos;
	D3DXVECTOR3 m_LookAt;
	D3DXVECTOR3 m_Up;

	CMouseInput		m_MouseInput;
	CKeyboardInput	m_KeyboardInput;

	CGameManager	m_GameManager;

	Network			m_Network;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	srand( static_cast< unsigned int >( time( 0 ) ) );
	SnakeApp theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

SnakeApp::SnakeApp(HINSTANCE hInstance)
: D3DApp(hInstance), mFX(0), mTech(0), mfxWVPVar(0), mVertexLayout(0), 
	m_EyePos(0.0f, 0.0f, 0.0f), m_LookAt(0.0f, 0.0f, 0.0f), m_Up(0.0f, 0.0f, 0.0f )
{
	D3DXMatrixIdentity(&m_World);
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
	D3DXMatrixIdentity(&m_WVP); 
}

SnakeApp::~SnakeApp()
{
	if( md3dDevice )
		md3dDevice->ClearState();

	if( m_Network.IsServer() )
	{
		m_Network.Send( QUIT, "USERS" );
	}

	ReleaseCOM(mFX);
	ReleaseCOM(mVertexLayout);
}

void SnakeApp::initApp()
{
	D3DApp::initApp();
	
	m_EyePos	= D3DXVECTOR3( 0.0f, 0.0f, -50.0f );
	m_LookAt	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Up		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	mClearColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_LookAt, &m_Up);
		
	buildFX();
	buildVertexLayouts();	

	m_MouseInput.SetHWND( mhMainWnd );
	m_MouseInput.SetWindowSize( mClientWidth, mClientHeight );

	// Push devices onto the services
	CServices::ProvideService( SERVICETYPE_MOUSE, static_cast< void* >( &m_MouseInput ) );
	CServices::ProvideService( SERVICETYPE_KEYBOARD, static_cast< void* >( &m_KeyboardInput ) );
	CServices::ProvideService( SERVICETYPE_D3DDEVICE, static_cast< void* >( md3dDevice ) );
	CServices::ProvideService( SERVICETYPE_NETWORKDEVICE, static_cast< void* >( &m_Network ) );

	m_GameManager.Initialise( mClientWidth, mClientHeight );
}

void SnakeApp::onResize()
{
	D3DApp::onResize();

	m_MouseInput.SetWindowSize( mClientWidth, mClientHeight );
	D3DXMatrixOrthoLH( &m_Proj, static_cast< float > ( mClientWidth ), static_cast< float >( mClientHeight ), 1.0f, 1000.0f );
}

void SnakeApp::updateScene(float dt)
{
	m_MouseInput.Update();
	m_GameManager.Update( dt );
	m_KeyboardInput.UpdatePrevious();
}

void SnakeApp::drawScene()
{
	D3DApp::drawScene();

	md3dDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	md3dDevice->OMSetBlendState( 0, blendFactors, 0xffffffff);
    md3dDevice->IASetInputLayout(mVertexLayout);
	md3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
	
	// set constants
	m_WVP = m_World * m_View * m_Proj;
	mfxWVPVar->SetMatrix( ( float* )&m_WVP );

    D3D10_TECHNIQUE_DESC techDesc;
    mTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
        mTech->GetPassByIndex( p )->Apply(0);
		m_GameManager.Draw();
    }

	mSwapChain->Present(0, 0);
}

void SnakeApp::buildFX()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
  
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"assets/shaders/point_sprite_shader.fx", 0, 0, 
		"fx_4_0", shaderFlags, 0, md3dDevice, 0, 0, &mFX, &compilationErrors, 0);
	if(FAILED(hr))
	{
		if( compilationErrors )
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			ReleaseCOM(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	} 

	mTech = mFX->GetTechniqueByName( "TexTech" );	
	mfxWVPVar = mFX->GetVariableByName( "gWVP" )->AsMatrix();
}

void SnakeApp::buildVertexLayouts()
{
	// Create the vertex input layout.
	D3D10_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create the input layout
    D3D10_PASS_DESC PassDesc;
    mTech->GetPassByIndex(0)->GetDesc(&PassDesc);
    HR(md3dDevice->CreateInputLayout(vertexDesc, 4, PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize, &mVertexLayout));
} 


void SnakeApp::ResizeWindow( const int _iWidth, const int _iHeight )
{	
	mClientWidth = _iWidth;
	mClientHeight = _iHeight;
	
	SetWindowPos( mhMainWnd, HWND_TOP, 0, 0, mClientWidth, mClientHeight, SWP_NOMOVE );
}