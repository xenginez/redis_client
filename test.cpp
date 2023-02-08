#include <iostream>

#include "redis_client.hpp"

#include "asio/asio/include/asio.hpp"

int main()
{
	asio::io_context io;
	std::string buf; buf.resize( 512 );
	asio::ip::tcp::socket socket( io );
	socket.connect( asio::ip::tcp::endpoint( asio::ip::address::from_string( "127.0.0.1" ), 6379 ) );
	redis::client c( [&]( std::string_view data ) { socket.write_some( asio::buffer( data.data(), data.size() ) ); } );

	c.ping( []( redis::value result ) { std::cout << result.get_string() << std::endl; } );
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );

	c.set( "abc", "123", []( redis::value result ) { std::cout << "set: " << result.get_string() << std::endl; });
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );

	c.get( "abc", []( redis::value result ) { std::cout << "get: " << result.get_string() << std::endl; });
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );

	c.del( "abc", []( redis::value result ) { std::cout << "del: " << result.get_int() << std::endl; });
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );

	c.hset( "hash", "abc", "123", []( redis::value result ) { std::cout << "hset: " << result.get_int() << std::endl; });
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );

	c.hget( "hash", "abc", []( redis::value result ) { std::cout << "hget: " << result.get_string() << std::endl; } );
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );

	c.hdel( "hash", "abc", []( redis::value result ) { std::cout << "hdel: " << result.get_int() << std::endl; } );
	c.input( buf.begin(), buf.begin() + socket.read_some( asio::buffer( buf ) ) );


	return 0;
}