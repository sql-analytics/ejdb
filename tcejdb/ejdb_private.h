/*
 * File:   ejdb_private.h
 * Author: adam
 *
 * Created on September 28, 2012, 11:43 AM
 */

#ifndef EJDB_PRIVATE_H
#define        EJDB_PRIVATE_H

#include "ejdb.h"

#include "myconf.h"
#include "tcutil.h"
#include "tctdb.h"
#include "tchdb.h"

#include <assert.h>
#include <pthread.h>

EJDB_EXTERN_C_START

#define BSON_IS_IDXSUPPORTED_TYPE(atype) (atype == BSON_STRING || \
                                          atype == BSON_INT || atype == BSON_LONG || atype == BSON_DOUBLE || \
                                          atype == BSON_SYMBOL || \
                                          atype == BSON_ARRAY)

struct EJDB {
    EJCOLL *cdbs; /*> Collection DBs for JSON collections. */
    int cdbsnum; /*> Count of collection DB. */
    TCTDB *metadb; /*> Metadata DB. */
    void *mmtx; /*> Mutex for method */
};

struct EJCOLL { /**> EJDB Collection. */
    char *cname; /**> Collection name. */
    int cnamesz; /**> Collection name length. */
    TCTDB *tdb; /**> Collection TCTDB. */
    EJDB *jb; /**> Database handle. */
    void *mmtx; /*> Mutex for method */
};

enum { //Query field flags
    // Comparison flags
    EJCOMPGT = 1, //Comparison GT
    EJCOMPGTE = 1 << 1, //Comparison GTE
    EJCOMPLT = 1 << 2, //Comparison LT
    EJCOMPLTE = 1 << 3, //Comparison LTE
    EJCONDSTARTWITH = 1 << 4, //Starts with

    EJFEXCLUDED = 1 << 5, //If query field excluded from matching
    EJFNOINDEX = 1 << 6, //Do not use index for field
    EJFORDERUSED = 1 << 7, //This ordering field was used
    EJFPKMATCHING = 1 << 8, //_id PK field matching

    EJCONDICASE = 1 << 9, //Ignore case iin matching
};


enum { //Query flags
    EJQINTERNAL = 1 //Internal query object used in _ejdbqrysearch
};

struct EJQF { /**> Matching field and status */
    char *fpath; /**>JSON field path */
    int fpathsz; /**>JSON field path size */
    char *expr; /**> Query operand expression, string or TCLIST data */
    int exprsz; /**> Size of query operand expression */
    int64_t exprlongval; /** Integer value represeintation */
    double exprdblval; /** Double value representation */
    TCLIST *exprlist; /** List representation of expression */
    regex_t *regex; /**> Regular expression object */
    int tcop; /**> Matching operation eg. TDBQCSTREQ */
    bool negate; /**> Negate expression */
    uint32_t flags; /**> Various field matching|status flags */
    int order; /**> 0 no order, 1 ASC, -1 DESC */
    int orderseq; /**> Seq number for order fields */
    bson_type ftype; /**> BSON field type */
    const TDBIDX *idx; /**> Column index for this field if exists */
    bson *idxmeta; /**> Index metainfo */
    EJDB *jb; /**> Reference to the EJDB during query processing */
};
typedef struct EJQF EJQF;

struct EJQ { /**> Query object. */
    TCMAP *qobjmap; /**> Mapping of field path (char*) -> *EJQF */
    EJQ *orqobjs; /** OR Query objects */
    int orqobjsnum; /** Number of OR query objects */
    bson *hints; /**> Hints bson object */
    uint32_t skip; /**> Number of records to skip. */
    uint32_t max; /**> Max number of results */
    uint32_t flags; /**> Control flags */
    const EJQF *lastmatchedorqf; /**> Reference to the last matched or query field */
};


#define JDBIDKEYNAME "_id"  /**> Name of PK _id field in BSONs */
#define JDBIDKEYNAMEL 3

#define JDBCOLBSON "$"  /**> TCDB colname with BSON byte data */
#define JDBCOLBSONL 1  /**> TCDB colname with BSON byte data columen len */



EJDB_EXTERN_C_END

#endif        /* EJDB_PRIVATE_H */
