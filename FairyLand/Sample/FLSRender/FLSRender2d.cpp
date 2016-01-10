#include "FairyLandLib.h"
#include <Windows.h>
using namespace FL;

MainWindow			g_MainWin;
IRenderDevice*		g_pDevice = _null;
ITextureFactory*	g_pTexFactory = _null;
IFontTexture*		g_pFontTexture = _null;
ITexture*			g_pTexture1 = _null;

IResourceCache*		g_pResCache = _null;
IResourceManager*	g_pResMgr   = _null;

_bool OnInitializeCallback( )
{		
	g_pResCache = CreateResourceCache( );
	FL_ASSERT( g_pResCache != _null )

	g_pResMgr = CreateResourceManager( );
	FL_ASSERT( g_pResMgr != _null )
		g_pResMgr ->InsertSearchPath( L"..\\Sample\\PSSampleRender\\Resource");
	g_pResCache->SetCacheTimeout( 1000 );

	GetResourceManagerPointer( )->OpenPEResArchive( L"", L"" );
	g_pResMgr->OpenFileArchive( L".", L"" );
	g_pResMgr->OpenFileArchive( L"Resource", L"" );
	g_pResMgr->CreateMemArchive( L"FLTemp", L"FLTemp" );

	FL_ASSERT(CreateGraphicsManager()!=NULL);

	GetGraphicsManagerPointer()->CreateRenderDevice( MainWindow::GetMainWindow( ), IRenderDevice::_DEVICE_HARDWARE_NOVSYNC );
	g_pDevice = GetRenderDevicePointer();
	FL_ASSERT( g_pDevice != _null )

	g_pTexFactory = GetTextureFactoryPointer();
	FL_ASSERT( g_pTexFactory != _null )

	g_pFontTexture		=	g_pTexFactory->CreateFontTexture( L"ËÎÌו", 12, 0, _false, _false, _false );
	g_pTexture1			=   g_pTexFactory->CreateTexture( L"1440tj1.jpg" );

	g_pFontTexture->SetFontColor( Color::cYellow );
	g_pDevice->SetCamera( Camera( Vector3( 100.0f, 100.0f, 100.0f ), Vector3::cOrigin, Vector3::cZAxis ) );



	return _true;
}

_void OnFinalizeCallback( )
{
	g_pTexFactory->ReleaseTexture(g_pTexture1);
	g_pTexFactory->ReleaseFontTexture( g_pFontTexture );
}

_bool OnMessageCallback( _dword msgid, _dword wparam, _dword lparam )
{
	

	return _true;
}

_bool OnIdleCallback( _dword elapse )
{
	if ( elapse == 0 )
		return _true;
	g_pResMgr->Update( elapse );
	g_pResCache->Update( elapse );
	if ( g_pDevice->IsDeviceLost( ) == _true )
	{
		g_pDevice->ResetDevice( );
		return _true;
	}
	Rect clientrect = g_MainWin.GetClientRect( );
	g_pDevice->ResizeDevice( clientrect.Width( ), clientrect.Height( ) );
	g_pDevice->SetProjectionTransform( Matrix4::CreatePerspectiveFovLH( Math::cPi / 4.0f,
		(_float) clientrect.Width( ) / (_float) clientrect.Height( ), 0.1f, 5000.0f ) );

	g_pDevice->SetClearColor( Color::cDarkBlue );
	g_pDevice->ClearBuffer( );

	//g_pDevice->Render( PrimitiveAxis( Vector3::cOrigin, 100.0f ) );
	Vector2 pos1(0,0);
	Vector2 pos2(800,600);
	//Vector2 pos2(g_pTexture1->GetSurfaceWidth(0),g_pTexture1->GetSurfaceHeight(0));
	Vector2 tex1(0,0);
	Vector2 tex2(1,1);
	FL::OverlayRectangleImage img( pos1, pos2, tex1, tex2, Color::cWhite, g_pTexture1 );
	g_pDevice->Render(img);
	g_pDevice->Present( );
	return _true;
}

//----------------------------------------------------------------------------
// WinMain Implementation
//----------------------------------------------------------------------------

_long __stdcall WinMain( HINSTANCE instance, HINSTANCE, _chara* cmdline, _long )
{
	Trace::SetTraceTarget( Trace::_TARGET_TRACEFILE );
	g_MainWin.SetInitializeCallback( OnInitializeCallback );
	g_MainWin.SetFinalizeCallback( OnFinalizeCallback );
	g_MainWin.SetMessageCallback( OnMessageCallback );
	g_MainWin.SetIdleCallback( OnIdleCallback );
	if ( g_MainWin.Create( L"FairyLand Render Demo", Point( 800, 600 ) ) == _true )
	{
		g_MainWin.Show( );
		g_MainWin.Run( _false );
	}
	g_MainWin.Close( );
	FreeAllFairyLandSingleton( );
	return 0;
}
