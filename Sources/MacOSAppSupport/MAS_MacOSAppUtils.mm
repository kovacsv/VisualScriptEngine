#include "MAS_MacOSAppUtils.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

#include "NE_StringUtils.hpp"

namespace MAS
{
	
NE::BasicStringSettings GetStringSettingsFromSystem ()
{
	NE::BasicStringSettings result = NE::GetDefaultStringSettings ();
	@autoreleasepool {
		@try {
			NSString* decSeparator = (NSString*) [[NSLocale currentLocale] objectForKey:NSLocaleDecimalSeparator];
			std::wstring decimalSeparatorStr = NE::StringToWString ([decSeparator cStringUsingEncoding:NSUTF8StringEncoding]);
			wchar_t decimalSeparatorChar = decimalSeparatorStr[0];
			result.SetDecimalSeparator (decimalSeparatorChar);

			wchar_t listSeparatorChar = L',';
			if (listSeparatorChar == decimalSeparatorChar) {
				listSeparatorChar = L';';
			}
			result.SetListSeparator (listSeparatorChar);
		} @catch (NSException*) {
			
		}
	}
	return result;
}

}
