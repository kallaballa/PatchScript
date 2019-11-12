#include "rankfunc.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>

//TF-IDF based ranking algorithm
void rankfunc(sqlite3_context *pCtx, int nVal, sqlite3_value **apVal)
{
    int *aMatchinfo;                /* Return value of matchinfo() */
    int nCol;                       /* Number of columns in the table */
    int nPhrase;                    /* Number of phrases in the query */
    int iPhrase;                    /* Current phrase */
    double score = 0.0;             /* Value to return */

    assert( sizeof(int) == 4 );

    /* Check that the number of arguments passed to this function is correct.
     ** If not, jump to wrong_number_args. Set aMatchinfo to point to the array
     ** of unsigned integer values returned by FTS function matchinfo. Set
     ** nPhrase to contain the number of reportable phrases in the users full-text
     ** query, and nCol to the number of columns in the table.
     */
    if( nVal < 1 )
        sqlite3_result_error(pCtx, "error argument to function rank()", -1);
    aMatchinfo = (int *)sqlite3_value_blob(apVal[0]);
    nPhrase = aMatchinfo[0];
    nCol = aMatchinfo[1];
    if( nVal != (1 + nCol) )
        sqlite3_result_error(pCtx, "wrong number of arguments to function rank()", -1);

    int *aTokeninfo = &aMatchinfo[3 + nPhrase * nCol * 3];
    int nRows = aMatchinfo[2 + nPhrase * nCol * 3];
    /* Iterate through each phrase in the users query. */
    for(iPhrase = 0; iPhrase < nPhrase; iPhrase++){
        int iCol;                     /* Current column */
        int *aPhraseinfo = &aMatchinfo[2 + iPhrase * nCol * 3];

        for(iCol = 0; iCol < nCol; iCol++){
            int nHitCount = aPhraseinfo[3 * iCol];
            int nDocWithHitsCount = aPhraseinfo[3 * iCol + 2];
            int nTotalTokens = aTokeninfo[iCol];
            double weight = sqlite3_value_double(apVal[iCol + 1]);
            if( nHitCount > 0 ){
                double tf = (double)nHitCount / (double)nTotalTokens;
                double idf = log((double)nRows / ((double)nDocWithHitsCount + 1));
                score += tf * idf * weight;
            }
        }
    }

    sqlite3_result_double(pCtx, score);
    return;
}
