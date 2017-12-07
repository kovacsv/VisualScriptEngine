#include "SimpleTest.hpp"
#include "Serializable.hpp"
#include "SingleValues.hpp"
#include "MemoryStream.hpp"

#include <memory>

using namespace NE;

namespace MemoryStreamTest
{

TEST (TypeTest)
{
	MemoryOutputStream outputStream;
	ASSERT (outputStream.Write (true) == Stream::Status::NoError);
	ASSERT (outputStream.Write ((size_t) 1) == Stream::Status::NoError);
	ASSERT (outputStream.Write ((int) 2) == Stream::Status::NoError);
	ASSERT (outputStream.Write ((double) 3.0) == Stream::Status::NoError);
	ASSERT (outputStream.Write (std::string ("apple")) == Stream::Status::NoError);
	ASSERT (outputStream.Write (std::wstring (L"banana")) == Stream::Status::NoError);

	bool boolVal;
	size_t sizeVal;
	int intVal;
	double doubleVal;
	std::string stringVal;
	std::wstring wStringVal;

	MemoryInputStream inputStream (outputStream.GetBuffer ());
	ASSERT (inputStream.Read (boolVal) == Stream::Status::NoError);
	ASSERT (inputStream.Read (sizeVal) == Stream::Status::NoError);
	ASSERT (inputStream.Read (intVal) == Stream::Status::NoError);
	ASSERT (inputStream.Read (doubleVal) == Stream::Status::NoError);
	ASSERT (inputStream.Read (stringVal) == Stream::Status::NoError);
	ASSERT (inputStream.Read (wStringVal) == Stream::Status::NoError);

	ASSERT (boolVal == true);
	ASSERT (sizeVal == 1);
	ASSERT (intVal == 2);
	ASSERT (doubleVal == 3.0);
	ASSERT (stringVal == "apple");
	ASSERT (wStringVal == L"banana");
}

}
