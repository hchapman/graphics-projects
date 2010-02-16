#ifndef RE330_GLOBAL_H
#define RE330_GLOBAL_H

const float EPSILON = 0.000001;

#include <Qt/qglobal.h>

#ifdef RE330_LIB
# define RE330_EXPORT Q_DECL_EXPORT
#else
# define RE330_EXPORT Q_DECL_IMPORT
#endif

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs for no justifiable reason on all
// #includes of Singleton
//#   pragma warning( disable: 4661)

#endif // RE330_GLOBAL_H
