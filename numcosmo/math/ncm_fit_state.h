/***************************************************************************
 *            ncm_fit_state.h
 *
 *  Thu November 29 15:27:17 2012
 *  Copyright  2012  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) 2012 Sandro Dias Pinto Vitenti <sandro@isoftware.com.br>
 * 
 * numcosmo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * numcosmo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NCM_FIT_STATE_H_
#define _NCM_FIT_STATE_H_

#include <glib-object.h>
#include <numcosmo/build_cfg.h>
#include <gsl/gsl_blas.h>

G_BEGIN_DECLS

#define NCM_TYPE_FIT_STATE             (ncm_fit_state_get_type ())
#define NCM_FIT_STATE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NCM_TYPE_FIT_STATE, NcmFitState))
#define NCM_FIT_STATE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NCM_TYPE_FIT_STATE, NcmFitStateClass))
#define NCM_IS_FIT_STATE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NCM_TYPE_FIT_STATE))
#define NCM_IS_FIT_STATE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NCM_TYPE_FIT_STATE))
#define NCM_FIT_STATE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NCM_TYPE_FIT_STATE, NcmFitStateClass))

typedef struct _NcmFitStateClass NcmFitStateClass;
typedef struct _NcmFitState NcmFitState;

struct _NcmFitState
{
  /*< private >*/
  GObject parent_instance;
  guint data_len;
  guint fparam_len;
  guint alloc_data_len;
  guint alloc_fparam_len;
  gint dof;
  guint niter;
  guint func_eval;
  guint grad_eval;
  gdouble m2lnL_prec;
  gdouble params_prec;
  gdouble elapsed_time;
  gdouble m2lnL_curval;
  NcmVector *dm2lnL;
  NcmVector *fparams;
  NcmVector *ls_f;
  NcmMatrix *ls_J;
  NcmMatrix *covar;
  NcmMatrix *hessian;
  gboolean is_best_fit;
  gboolean is_least_squares;
  gboolean has_covar;
};

struct _NcmFitStateClass
{
  /*< private >*/
  GObjectClass parent_class;
};

GType ncm_fit_state_get_type (void) G_GNUC_CONST;

NcmFitState *ncm_fit_state_new (guint data_len, guint fparam_len, gint dof, gboolean is_least_squares);
NcmFitState *ncm_fit_state_ref (NcmFitState *fstate);
void ncm_fit_state_free (NcmFitState *fstate);
void ncm_fit_state_clear (NcmFitState **fstate);

void ncm_fit_state_set_all (NcmFitState *fstate, guint data_len, guint fparam_len, gint dof, gboolean is_least_squares);
void ncm_fit_state_reset (NcmFitState *fstate);
void ncm_fit_state_realloc (NcmFitState *fstate);

G_INLINE_FUNC void ncm_fit_state_set_ls (NcmFitState *fstate, NcmVector *f, NcmMatrix *J);
G_INLINE_FUNC void ncm_fit_state_set_niter (NcmFitState *fstate, guint niter);
G_INLINE_FUNC guint ncm_fit_state_get_niter (NcmFitState *fstate);
G_INLINE_FUNC void ncm_fit_state_set_m2lnL_prec (NcmFitState *fstate, gdouble prec);
G_INLINE_FUNC gdouble ncm_fit_state_get_m2lnL_prec (NcmFitState *fstate);
G_INLINE_FUNC void ncm_fit_state_set_params_prec (NcmFitState *fstate, gdouble prec);
G_INLINE_FUNC gdouble ncm_fit_state_get_params_prec (NcmFitState *fstate);

G_END_DECLS

#endif /* _NCM_FIT_STATE_H_ */

#ifndef _NCM_FIT_STATE_INLINE_H_
#define _NCM_FIT_STATE_INLINE_H_
#ifdef NUMCOSMO_HAVE_INLINE

G_BEGIN_DECLS

G_INLINE_FUNC void 
ncm_fit_state_set_ls (NcmFitState *fstate, NcmVector *f, NcmMatrix *J)
{
  g_assert (fstate->is_least_squares);

  fstate->m2lnL_curval = gsl_blas_dnrm2 (ncm_vector_gsl (f));

  ncm_vector_memcpy (fstate->ls_f, f);
  
  gsl_blas_dgemv (CblasTrans, 2.0, ncm_matrix_gsl (fstate->ls_J), 
                  ncm_vector_gsl (fstate->ls_f), 0.0, 
                  ncm_vector_gsl (fstate->dm2lnL));

  ncm_matrix_memcpy (fstate->ls_J, J);
}

G_INLINE_FUNC void 
ncm_fit_state_set_niter (NcmFitState *fstate, guint niter)
{
  fstate->niter = niter;
}

G_INLINE_FUNC guint 
ncm_fit_state_get_niter (NcmFitState *fstate)
{
  return fstate->niter;
}

G_INLINE_FUNC void 
ncm_fit_state_set_m2lnL_prec (NcmFitState *fstate, gdouble prec)
{
  fstate->m2lnL_prec = prec;
}

G_INLINE_FUNC gdouble 
ncm_fit_state_get_m2lnL_prec (NcmFitState *fstate)
{
  return fstate->m2lnL_prec;
}

G_INLINE_FUNC void 
ncm_fit_state_set_m2lnL_curval (NcmFitState *fstate, gdouble m2lnL)
{
  fstate->m2lnL_curval = m2lnL;
}

G_INLINE_FUNC gdouble 
ncm_fit_state_get_m2lnL_curval (NcmFitState *fstate)
{
  return fstate->m2lnL_curval;
}

G_INLINE_FUNC void 
ncm_fit_state_set_params_prec (NcmFitState *fstate, gdouble prec)
{
  fstate->params_prec = prec;
}

G_INLINE_FUNC gdouble 
ncm_fit_state_get_params_prec (NcmFitState *fstate)
{
  return fstate->params_prec;
}

G_END_DECLS

#endif /* NUMCOSMO_HAVE_INLINE */
#endif /* _NCM_FIT_STATE_INLINE_H_ */
