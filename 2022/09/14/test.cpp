#include <gtest/gtest.h>
#include "escape.h"

template< typename Type >
static std::string escape( const std::string& str ) {
    std::vector<char> out( 2*str.size() );
    size_t nb = Type::escape( str.data(), str.size(), out.data() );
    return std::string( out.data(), nb );
}

template< typename Type >
void runBasicTests() {
  EXPECT_EQ( escape<Type>( "abc" ), "abc" );
  EXPECT_EQ( escape<Type>( "\"" ), "\\\"" );
  EXPECT_EQ( escape<Type>( "abc\\" ), "abc\\\\" );
  EXPECT_EQ( escape<Type>( "\\abc" ), "\\\\abc" );
  EXPECT_EQ( escape<Type>( "abc\\\"abc" ), "abc\\\\\\\"abc" );
}

TEST( Original, Basic ) {
  runBasicTests<Original>();
}

TEST( Optimized, Basic ) {
  runBasicTests<Optimized>();
}



