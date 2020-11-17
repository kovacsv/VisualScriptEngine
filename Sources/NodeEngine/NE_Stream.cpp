#include "NE_Stream.hpp"

namespace NE
{

Stream::Stream () :
	status (Status::NoError)
{

}

Stream::~Stream ()
{

}

Stream::Status Stream::GetStatus () const
{
	return status;
}

InputStream::InputStream () :
	Stream ()
{

}

InputStream::~InputStream ()
{

}

OutputStream::OutputStream () :
	Stream ()
{

}

OutputStream::~OutputStream ()
{

}

}
