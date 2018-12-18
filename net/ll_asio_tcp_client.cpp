// ll_asio_tcp_client.cpp
//
// Copyright (c) Liu Xuan Le, 2012-2016.
//

#include "ll_asio_tcp_client.h"

ll::asio::TTcpClient::TTcpClient( TAsio * owner ): THandle( owner )
{
}

ll::asio::TTcpClient::~TTcpClient()
{
	if ( z_h )
		z_h->destroy();
}

bool ll::asio::TTcpClient::create( const CBOBJ_ERROR & cb_error )
{
	if ( z_h )
		z_h->destroy();
	z_h = ll::New( z_owner, this );
	z_handle = z_h.ptr();
	if ( !z_handle )
		return false;
	return z_h->create( z_h, cb_error );
}
void ll::asio::TTcpClient::destroy()
{
	if ( z_h )
		z_h->destroy();
	z_h = NULL;
	z_handle = NULL;
}

