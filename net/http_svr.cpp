// http_svr.cpp
//

#include "http_svr.h"

THttpSvr::THttpSvr()
	: z_asio( NULL )
{
}

THttpSvr::~THttpSvr()
{
}

bool THttpSvr::Init()
{
	z_filePath = "d:";
	z_svrPort = 80;
	return true;
}

void THttpSvr::UnInit()
{
	z_tcpsvr = NULL;
}

bool THttpSvr::Start( ll::asio::TAsio * asio )
{
	z_asio = asio;
	z_tcpsvr = ll::New( asio );
	z_tcpsvr->create( ll::asio::CBOBJ_ERROR( z_cb_error_s, this ) );
	sockaddr_in addr;
	if ( !ll::asio::comm::ip_addr( addr, "0.0.0.0", z_svrPort ) )
	{
		LLTRACE < "failed to start http server, ip_addr\n";
		return false;
	}
	if ( !z_tcpsvr->bind( addr ) )
	{
		LLTRACE < "failed to start http server, bind, err: " <
			z_tcpsvr->getError() < ", errdesp: " < z_tcpsvr->getErrDesp() < "\n";
		return false;
	}
	if ( !z_tcpsvr->listen( 5, ll::asio::CBOBJ( z_cb_connection_s, this ) ) )
	{
		LLTRACE < "failed to start http server, listen, err: " <
			z_tcpsvr->getError() < ", errdesp: " < z_tcpsvr->getErrDesp() < "\n";
		return false;
	}
	return true;
}

void THttpSvr::z_remove_http( THttpServer * http )
{
	std::map< THttpServer *, THttpServerPtr >::iterator pos = z_https.find( http );
	if ( pos == z_https.end() )
		return;
	LLTRACE < "remove http connect: " < *http;
	z_https.erase( pos );
}

void THttpSvr::z_cb_error( ll::asio::THandle * h, int err )
{
	// 不知道监听端口的错误会出在哪里，暂时不处理
}
void THttpSvr::z_cb_connection( ll::asio::THandle * h )
{
	THttpServerPtr http = ll::New( this, z_asio );
	if ( http->accept( z_tcpsvr.ptr() ) )
		z_https.insert( std::pair< THttpServer *, THttpServerPtr >( http.ptr(), http ) );
}
void THttpSvr::z_cb_error_s( void * userdata, ll::asio::THandle * h, int err )
{
	THttpSvr * pThis = ( THttpSvr * )userdata;
	pThis->z_cb_error( h, err );
}
void THttpSvr::z_cb_connection_s( void * userdata, ll::asio::THandle * h )
{
	THttpSvr * pThis = ( THttpSvr * )userdata;
	pThis->z_cb_connection( h );
}

cout << "success!!!" << endl;
