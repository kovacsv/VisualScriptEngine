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
			std::wstring ds = NE::StringToWString ([decSeparator cStringUsingEncoding:NSUTF8StringEncoding]).c_str ();
			result.SetDecimalSeparator (ds.c_str ()[0]);
			
			NSString* listSeparator = (NSString*) [[NSLocale currentLocale] objectForKey:NSLocaleGroupingSeparator];
			std::wstring ls = NE::StringToWString ([listSeparator cStringUsingEncoding:NSUTF8StringEncoding]).c_str ();
			result.SetListSeparator (ls.c_str ()[0]);
		} @catch (NSException*) {
			
		}
	}
	return result;
}

}
