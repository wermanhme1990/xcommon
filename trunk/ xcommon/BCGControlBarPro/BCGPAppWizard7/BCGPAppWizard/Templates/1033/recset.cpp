// [!output ROWSET_IMPL] : implementation of the [!output ROWSET_CLASS] class
//

#include "stdafx.h"
#include "[!output APP_HEADER]"
#include "[!output ROWSET_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// [!output ROWSET_CLASS] implementation

[!if ODBC]
[!output ROWSET_CLASS_ODBC_IMPL]
[!endif]
