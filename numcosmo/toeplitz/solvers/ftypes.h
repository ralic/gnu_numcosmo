/*                    Header file ftypes.h                   */
/* generated by ftypes V 1.0 (c) Mathias Froehlich 1996      */
/* This types are needed for calling Fortran BLAS and LAPACK */
/* routines.                                                 */

#ifndef FTYPES_H_
#define FTYPES_H_

#include <glib.h>

/* Integration with NumCosmo bulding by SDPV */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */
#include "build_cfg.h"

#define FTRUE (1)
#define FFALSE (0)

#define F77CALL(name) F77_FUNC (name, name)

typedef  gint  finteger;
typedef  gboolean  flogical;
typedef  gfloat  fsingle ;
typedef  gdouble  fdouble ;

typedef struct {  fsingle  r, i; }  fsinglecomplex ;
typedef struct {  fdouble  r, i; }  fdoublecomplex ;

#endif /* FTYPES_H_ */
