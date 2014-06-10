
#include <jim.h>

#include "lzf.h"
#include "lzfP.h"

static int LzfCompress_Cmd(Jim_Interp *interp, int argc, 
                            Jim_Obj *const argv[]) {
    if (argc != 2) {
        Jim_WrongNumArgs(interp,1,argv,"<string>");
        return JIM_ERR;
    }

    int l = Jim_Length(argv[1]);
    if (l == 0) {
        Jim_SetResultString(interp, "", 0);
        return JIM_OK;
    }
    int zl = l > 100 ? (l / 10) * 12 : 120;
    void *z = Jim_Alloc(zl);
    unsigned int n = lzf_compress(Jim_String(argv[1]),l,z,zl);
    if (n == 0) {
        Jim_SetResultString(interp, "Error compressing data", -1);
        Jim_Free(z);
        return JIM_ERR;
    }
	Jim_SetResult(interp,Jim_NewStringObjNoAlloc(interp,z,n));
    return JIM_OK;
}

static int LzfDecompress_Cmd(Jim_Interp *interp, int argc, 
                                Jim_Obj *const argv[]) {

    long len = 0;
    long max = 1 << 27;

    while (argc > 1 && Jim_String(argv[1])[0] == '-') {
        if (Jim_CompareStringImmediate(interp, argv[1], "-len") && 
                        argc > 2 ) {
            if (Jim_GetLong(interp, argv[2], &len) != JIM_OK) {
                Jim_SetResultString(interp,"Invalid decompressed length",-1);
                return JIM_ERR;
            }
            --argc;
            ++argv;
        } else if (Jim_CompareStringImmediate(interp, argv[1], "-max") &&
                       argc > 2) {
            if (Jim_GetLong(interp, argv[2], &max) != JIM_OK) {
                Jim_SetResultString(interp,"Invalid max memory length",-1);
                return JIM_ERR;
            }
            --argc;
            ++argv;
        } else {
            break;
        }
        --argc;
        ++argv;
    }

    if (argc != 2) {
        Jim_WrongNumArgs(interp,1,argv,
                "[-len <decompressed length>] [-max <max memory>] <string>");
        return JIM_ERR;
    }

    int l = Jim_Length(argv[1]);
    if (l == 0) {
        Jim_SetResultString(interp, "", 0);
        return JIM_OK;
    }

    unsigned int n = 0;
    unsigned int dl = l;
    void *d = NULL;

    if (len) {
        d = Jim_Alloc(len);
        n = lzf_decompress(Jim_String(argv[1]),l,d,len);
    } else {
        while (n == 0 && dl < max) {
            dl = dl * 2;
            if (dl > max) {
                dl = max;
            }
            if ((d = Jim_Realloc(d,dl)) == NULL) {
                break;
            }
            n = lzf_decompress(Jim_String(argv[1]),l,d,dl);
        }
    }

    if (n == 0) {
        Jim_Free(d);
        Jim_SetResultString(interp, "Error decompressing LZF data", -1);
        return JIM_ERR;
    }

	Jim_SetResult(interp,Jim_NewStringObjNoAlloc(interp,d,n));
    return JIM_OK;
}

Jim_lzfInit(Jim_Interp *interp)
{
    Jim_CreateCommand(interp, "lzf_compress", LzfCompress_Cmd, NULL, NULL);
    Jim_CreateCommand(interp, "lzf_decompress", LzfDecompress_Cmd, NULL, NULL);
    return JIM_OK;
}

