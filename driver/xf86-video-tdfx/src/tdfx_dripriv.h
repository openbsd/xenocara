
#ifndef _TDFX_DRIPRIV_H_
#define _TDFX_DRIPRIV_H_

#define TDFX_MAX_DRAWABLES 256

extern void GlxSetVisualConfigs(
    int nconfigs,
    __GLXvisualConfig *configs,
    void **configprivs
);

typedef struct {
  /* Nothing here yet */
  int dummy;
} TDFXConfigPrivRec, *TDFXConfigPrivPtr;

typedef struct {
  /* Nothing here yet */
  int dummy;
} TDFXDRIContextRec, *TDFXDRIContextPtr;

#endif
