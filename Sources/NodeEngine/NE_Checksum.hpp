#ifndef NE_CHECKSUM_HPP
#define NE_CHECKSUM_HPP

#include <string>

namespace NE
{

class Checksum
{
public:
	Checksum ();
	~Checksum ();

	void	Add (char val);
	void	Add (wchar_t val);
	void	Add (int val);
	void	Add (double val);
	void	Add (const std::string& val);
	void	Add (const std::wstring& val);

	bool	operator== (const Checksum& rhs) const;
	bool	operator!= (const Checksum& rhs) const;

private:
	void	AddInteger (int val);

	int		checksum;
	int		counter;
};

}

#endif
