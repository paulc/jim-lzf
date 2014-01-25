
#include <jim.h>

#include "lzf.h"
#include "lzfP.h"

static int LzfCompress_Cmd(Jim_Interp *interp, int argc, Jim_Obj *const argv[]) {
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
        return JIM_ERR;
    }
    Jim_SetResultString(interp,z,n);
    Jim_Free(z);
    return JIM_OK;
}

static int LzfDecompress_Cmd(Jim_Interp *interp, int argc, Jim_Obj *const argv[]) {
    if (argc != 2) {
        Jim_WrongNumArgs(interp,1,argv,"<string>");
        return JIM_ERR;
    }

    int l = Jim_Length(argv[1]);
    if (l == 0) {
        Jim_SetResultString(interp, "", 0);
        return JIM_OK;
    }
    void *d = Jim_Alloc(l*5);
    unsigned int n = lzf_decompress(Jim_String(argv[1]),l,d,l*10);
    if (n == 0) {
        Jim_SetResultString(interp, "Error decompressing data", -1);
        return JIM_ERR;
    }
    Jim_SetResultString(interp,d,n);
    Jim_Free(d);
    return JIM_OK;
}

Jim_lzfInit(Jim_Interp *interp)
{
    Jim_CreateCommand(interp, "lzf_compress", LzfCompress_Cmd, NULL, NULL);
    Jim_CreateCommand(interp, "lzf_decompress", LzfDecompress_Cmd, NULL, NULL);
    return JIM_OK;
}

