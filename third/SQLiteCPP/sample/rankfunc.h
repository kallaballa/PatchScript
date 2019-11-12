#ifndef RANKFUNC_H
#define RANKFUNC_H

#include <SQLiteCPP/sqlite3.h>

#ifdef __cplusplus
extern "C" {
#endif

void rankfunc(sqlite3_context *pCtx, int nVal, sqlite3_value **apVal);

#ifdef __cplusplus
}
#endif

#endif
