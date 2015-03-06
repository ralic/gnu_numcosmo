/***************************************************************************
 *            ncm_model.c
 *
 *  Fri February 24 21:18:21 2012
 *  Copyright  2012  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) Sandro Dias Pinto Vitenti 2012 <sandro@isoftware.com.br>
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

/**
 * SECTION:ncm_model
 * @title: NcmModel
 * @short_description: Abstract class for implementing models.
 *
 * The #NcmModel abstract class represents a general model. This object serves
 * for two general objectives. First, all the numerical properties (doubles), i.e., 
 * parameters, are implemented by the class functions described below, this
 * allows the implementation of a general statistical analyses based on these
 * models. Second, each child of NcmModel can register itself as a model type.
 * This allows multiples models types to be used simultaneously. 
 * 
 * For example, in a problem where one must describe some physical model and 
 * some model for the measurement tool, lets say FooPhysical model and FooTool
 * both defined in the Foo namespace.
 * 
 * The FooPhysical and FooTools define virtual functions called respectively 
 * foo_physical_value and foo_tool_value which calculates some quantities needed
 * to compare the model with data. Note that both models do not implement 
 * anything thay just define which virtual functions must be implemented in 
 * order to define a model of each type.
 * 
 * Here is an example of the definition of the FooPhysical model type, note that
 * most of it is just the usual #GObject framework boilerplate code. 
 * <example>
 *  <title>Implementing a NcmModel (header: foo_physical.h).</title>
 *  <programlisting>
 * 
 *  #include <glib.h>
 *  #include <numcosmo/numcosmo.h> // Including NumCosmo headers
 * 
 *  #define FOO_TYPE_PHYSICAL             (foo_physical_get_type ())
 *  #define FOO_PHYSICAL(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), FOO_TYPE_PHYSICAL, FooPhysical))
 *  #define FOO_PHYSICAL_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), FOO_TYPE_PHYSICAL, FooPhysicalClass))
 *  #define FOO_IS_PHYSICAL(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), FOO_TYPE_PHYSICAL))
 *  #define FOO_IS_PHYSICAL_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), FOO_TYPE_PHYSICAL))
 *  #define FOO_PHYSICAL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NCM_TYPE_PHYSICAL, FooPhysicalClass))
 *
 *  // The lines above are just the basic GObject definition macros
 *  
 *  typedef struct _FooPhysicalClass FooPhysicalClass;
 *  typedef struct _FooPhysical FooPhysical;
 *  
 *  // To avoid a gtkdoc bug always declare the instance struct before the class
 *  // struct
 *  struct _FooPhysical 
 *  {
 *    NcmModel parent_instance;
 *    ...
 *  };
 * 
 *  struct _FooPhysicalClass
 *  {
 *    NcmModelClass parent_class;
 *    gdouble (*value) (NcmModel *model); // Virtual function.
 *    ...
 *  };
 *  
 *  ...
 *  
 *  GType foo_physical_get_type (void) G_GNUC_CONST;
 *  NCM_MSET_MODEL_DECLARE_ID (foo_physical); 
 *  // The last line above is part of the model type registry.
 * 
 *  gdouble foo_physical_value (FooPhysical *phys); // The virtual function caller.
 *  
 *  </programlisting>
 * </example>
 * 
 * <example>
 *  <title>Implementing a NcmModel (source: foo_physical.c).</title>
 *  <programlisting>
 *  
 *  #include "foo_physical.h"
 *  G_DEFINE_ABSTRACT_TYPE (FooPhysical, foo_physical, NCM_TYPE_MODEL);
 * 
 *  static void
 *  foo_physical_init (FooPhysical *object)
 *  {
 *    ... // Some basic initialization for an instance.
 *  }
 *  
 *  static void
 *  _foo_physical_finalize (GObject *object)
 *  {
 *  
 *    ... // Instance finalization, must deallocate all memory and release all references.
 * 
 *    // Chain up : end
 *    G_OBJECT_CLASS (foo_physical_parent_class)->finalize (object);
 *  }
 *  
 *  static gboolean _foo_physical_valid (NcmModel *model); // Define if the model is valid
 *  NCM_MSET_MODEL_REGISTER_ID (foo_physical, FOO_TYPE_PHYSICAL); // Second part of the model type registry.
 *  
 *  static void
 *  foo_physical_class_init (FooPhysicalClass *klass)
 *  {
 *    GObjectClass* object_class = G_OBJECT_CLASS (klass);
 *    NcmModelClass *model_class = NCM_MODEL_CLASS (klass);
 *    
 *    object_class->finalize = &_foo_physical_finalize;
 *  
 *    // It is a base model type definition so no parameters.
 *    ncm_model_class_add_params (model_class, 0, 0, 1);
 *    
 *    // Define its id short and long description.
 *    ncm_mset_model_register_id (model_class, 
 *                                "FooPhysical",
 *                                "Some brief description.",
 *                                "Some long description.");
 *  
 *    // Checks if everything is consistent.
 *    ncm_model_class_check_params_info (model_class);
 *    
 *    // Sets the "valid" function. 
 *    model_class->valid = &_foo_physical_valid;
 *  }
 *  
 *  static gboolean 
 *  _foo_physical_valid (NcmModel *model)
 *  {
 *    if (!NCM_MODEL_CLASS (foo_physical_parent_class)->valid (model))
 *      return FALSE;
 *    // Chain up : start
 *    
 *    ... // returns TRUE if value and FALSE if not.
 * 
 *  }
 *  
 *  ...
 *  
 *  // Virtual function accessor.
 *  G_INLINE_FUNC gdouble foo_physical_value (FooPhysical *phys) \
 *  {
 *    return FOO_PHYSICAL_GET_CLASS (m)->value (NCM_MODEL (phys)); \
 *  }
 * 
 *  ...
 * 
 *  </programlisting>
 * </example>
 * 
 * Now it is possible to give different implementations of each model, e.g.,
 * FooPhysicalSimple could be a simple implementation of the physical model
 * taking into account only the main effects and containing only a few parameters.
 * Then, FooPhysicalComplex a more complex implementation including several
 * effects and usually containing several parameters. Suppose that the same 
 * applies to FooToolSimple and FooToolComplex. 
 * 
 * Here an example of a FooPhysical implementation:
 * 
 * <example>
 *  <title>Implementing a FooPhysical (header: foo_physical_simple.h).</title>
 *  <programlisting>
 * 
 *  #include <glib.h>
 *  #include <numcosmo/numcosmo.h> // Including NumCosmo headers
 *  #include "foo_physical.h"
 * 
 *  #define NCM_TYPE_PHYSICAL_SIMPLE             (foo_physical_simple_get_type ())
 *  #define FOO_PHYSICAL_SIMPLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), NCM_TYPE_PHYSICAL_SIMPLE, FooPhysicalSimple))
 *  #define FOO_PHYSICAL_SIMPLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), NCM_TYPE_PHYSICAL_SIMPLE, FooPhysicalSimpleClass))
 *  #define FOO_IS_PHYSICAL_SIMPLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NCM_TYPE_PHYSICAL_SIMPLE))
 *  #define FOO_IS_PHYSICAL_SIMPLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), NCM_TYPE_PHYSICAL_SIMPLE))
 *  #define FOO_PHYSICAL_SIMPLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), NCM_TYPE_PHYSICAL_SIMPLE, FooPhysicalSimpleClass))
 *
 *  // The lines above are just the basic GObject definition macros
 *  
 *  typedef struct _FooPhysicalSimpleClass FooPhysicalSimpleClass;
 *  typedef struct _FooPhysicalSimple FooPhysicalSimple;
 *  
 *  // To avoid a gtkdoc bug always declare the instance struct before the class
 *  // struct
 *  struct _FooPhysicalSimple 
 *  {
 *    FooPhysical parent_instance; // Note that it is now a child of FooPhysical
 *    ...
 *  };
 * 
 *  struct _FooPhysicalSimpleClass
 *  {
 *    FooPhysicalClass parent_class; // Note that it is now a child of FooPhysical
 *    ...
 *  };
 *  
 *  ...
 *  
 *  GType foo_physical_simple_get_type (void) G_GNUC_CONST;
 *  // Note that we do not registry FooPhysicalSimple as it is of FooPhysical type.
 * 
 *  </programlisting>
 * </example>
 * 
 * <example>
 *  <title>Implementing a FooPhysical (source: foo_physical_simple.c).</title>
 *  <programlisting>
 *  
 *  #include "foo_physical_simple.h"
 *  G_DEFINE_ABSTRACT_TYPE (FooPhysicalSimple, foo_physical_simple, NCM_TYPE_PHYSICAL);
 * 
 *  static void
 *  foo_physical_simple_init (FooPhysicalSimple *object)
 *  {
 *    ... // Some basic initialization for an instance.
 *  }
 *  
 *  static void
 *  _foo_physical_simple_finalize (GObject *object)
 *  {
 *  
 *    ... // Instance finalization, must deallocate all memory and release all references.
 * 
 *    // Chain up : end
 *    G_OBJECT_CLASS (foo_physical_simple_parent_class)->finalize (object);
 *  }
 *  
 *  // Again, we do not need to register this model.
 *  
 *  // Here we need to define what FooPhysicalSimple actually calculates.
 *  gdouble _foo_physical_simple_value (NcmModel *model);
 * 
 *  static void
 *  foo_physical_simple_class_init (FooPhysicalSimpleClass *klass)
 *  {
 *    GObjectClass* object_class = G_OBJECT_CLASS (klass);
 *    NcmModelClass *model_class = NCM_MODEL_CLASS (klass);
 *    FooPhysicalClass *physical_class = FOO_PHYSICAL_CLASS (klass);
 *    
 *    object_class->finalize = &_foo_physical_finalize;
 *    
 *    physical_class->value = &_foo_physical_simple_value;
 *    
 *    // It is simple model so lets say is has one (01) scalar parameter and
 *    // zero vector parameters.
 *    ncm_model_class_add_params (model_class, 1, 0, 1);
 *    
 *    // Now we set the name and nick for our model. 
 *    ncm_model_class_set_name_nick (model_class, "Physical simple model", "PSimple");
 * 
 *    // Our parameter number 0 is p0 with its symbol being p_0, it can varies 
 *    // from -10.0 to 10.0 and its natural varying scale is 0.1
 *    // its absolute tolerance is 0 (no absolute tolerance) and its default
 *    // value is 2.0 and by default it should be set free in statistical
 *    // analysis.
 *    ncm_model_class_set_sparam (model_class, 0, "p_0", "p0",
 *                                -10.0, 10.0, 0.1,
 *                                0.0, 1.0,
 *                                NCM_PARAM_TYPE_FREE);
 * 
 * 
 *    // Checks if everything is consistent.
 *    ncm_model_class_check_params_info (model_class);
 *    
 *  }
 *  
 *  ...
 *  
 *  // The virtual function implementation.
 *  gdouble 
 *  _foo_physical_simple_value (NcmModel *model)
 *  {
 *    const gdouble p0 = ncm_vector_get (model->params, 0); // Get the value of the parameter p0 to perform calculations. 
 *    
 *    ...
 * 
 *    return ...; // Return the "value".
 *  }
 * 
 *  ...
 * 
 *  </programlisting>
 * </example>
 * 
 * Note that, we can now build a #NcmData to compare data with these models 
 * (FooPhysical and FooTool), in this way NcmData will only depend on these
 * two classes and not on their implementations, i.e., inside NcmData we only
 * call foo_physical_value and foo_tool_value which are independent of their
 * implementations. We can then build a #NcmMSet using any combination of
 * FooPhysicalSimple or FooPhysicalComplex and FooToolSimple or FooToolComplex,
 * to find their bestfit or make other statistical analysis (see #NcmFit and
 * related objects).
 * 
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */
#include "build_cfg.h"

#include "math/ncm_model.h"
#include "math/ncm_mset.h"
#include "math/ncm_serialize.h"
#include "math/ncm_cfg.h"

enum
{
  PROP_0,
  PROP_NAME,
  PROP_NICK,
  PROP_SPARAMS_LEN,
  PROP_VPARAMS_LEN,
  PROP_IMPLEMENTATION,
  PROP_PTYPES,
  PROP_REPARAM,
};

G_DEFINE_ABSTRACT_TYPE (NcmModel, ncm_model, G_TYPE_OBJECT);

/**
 * ncm_model_dup:
 * @model: a #NcmModel.
 * @ser: a #NcmSerialize.
 *
 * Duplicates @model by serializing and deserializing it.
 * 
 * Returns: (transfer full): a duplicate of @model.
 */
NcmModel *
ncm_model_dup (NcmModel *model, NcmSerialize *ser)
{
  return NCM_MODEL (ncm_serialize_dup_obj (ser, G_OBJECT (model)));
}

/**
 * ncm_model_free:
 * @model: a #NcmModel.
 *
 * Atomically decrements the reference count of @model by one. If the reference count drops to 0,
 * all memory allocated by @model is released.
 *
 */
void
ncm_model_free (NcmModel *model)
{
  g_object_unref (model);
}

/**
 * ncm_model_clear:
 * @model: a #NcmModel.
 *
 * Atomically decrements the reference count of @model by one. If the reference count drops to 0,
 * all memory allocated by @model is released. Set pointer to NULL.
 *
 */
void
ncm_model_clear (NcmModel **model)
{
  g_clear_object (model);
}

static void
ncm_model_init (NcmModel *model)
{
  NcmModelClass *model_class = NCM_MODEL_GET_CLASS (model);

  model->sparams         = g_ptr_array_new_with_free_func ((GDestroyNotify)&ncm_sparam_free);
  model->sparams_name_id = g_hash_table_new_full (&g_str_hash, &g_str_equal, &g_free, NULL);
  model->params          = NULL;
  model->p               = NULL;

  model->vparam_len = g_array_sized_new (TRUE, TRUE, sizeof (guint), model_class->vparam_len);
  g_array_set_size (model->vparam_len, model_class->vparam_len);
  model->vparam_pos = g_array_sized_new (TRUE, TRUE, sizeof (guint), model_class->vparam_len);
  g_array_set_size (model->vparam_pos, model_class->vparam_len);

  model->pkey    = 1;
  model->skey    = 0;
  model->reparam = NULL;
  model->ptypes  = g_array_new (FALSE, TRUE, sizeof (NcmParamType));
}

static void
_ncm_model_set_sparams (NcmModel *model)
{
  NcmModelClass *model_class = NCM_MODEL_GET_CLASS (model);
  guint i;

  g_hash_table_remove_all (model->sparams_name_id);
  g_ptr_array_set_size (model->sparams, 0);
  g_ptr_array_set_size (model->sparams, model->total_len);

  for (i = 0; i < model_class->sparam_len; i++)
  {
    NcmSParam *sp = g_ptr_array_index (model_class->sparam, i);
    g_array_index (model->ptypes, NcmParamType, i) = NCM_PARAM_TYPE_FIXED;
    g_ptr_array_index (model->sparams, i) = ncm_sparam_copy (sp);
    g_hash_table_insert (model->sparams_name_id, g_strdup (ncm_sparam_name (sp)), GUINT_TO_POINTER (i));
  }

  for (i = 0; i < model_class->vparam_len; i++)
  {
    const guint len = g_array_index (model->vparam_len, guint, i);
    const guint pos = g_array_index (model->vparam_pos, guint, i);
    NcmVParam *vp = ncm_vparam_copy (g_ptr_array_index (model_class->vparam, i));
    guint j;

    ncm_vparam_set_len (vp, len);

    for (j = 0; j < len; j++)
    {
      const guint n = pos + j;
      NcmSParam *sp = ncm_vparam_peek_sparam (vp, j);
      g_array_index (model->ptypes, NcmParamType, n) = NCM_PARAM_TYPE_FIXED;
      g_ptr_array_index (model->sparams, n) = ncm_sparam_ref (sp);
      g_hash_table_insert (model->sparams_name_id, g_strdup (ncm_sparam_name (sp)), GUINT_TO_POINTER (n));
    }

    ncm_vparam_free (vp);
  }
}

static void
_ncm_model_sparams_remove_reparam (NcmModel *model)
{
  if (model->reparam != NULL)
  {
    ncm_reparam_clear (&model->reparam);
    model->p = ncm_vector_ref (model->params);
  }
}

static void
_ncm_model_constructed (GObject *object)
{
  /* Chain up : start */
  G_OBJECT_CLASS (ncm_model_parent_class)->constructed (object);
  {
    NcmModel *model = NCM_MODEL (object);
    NcmModelClass *model_class = NCM_MODEL_GET_CLASS (model);
    guint i;

    model->total_len = model_class->sparam_len;
    for (i = 0; i < model_class->vparam_len; i++)
    {
      g_array_index (model->vparam_pos, guint, i) = model->total_len;
      model->total_len += g_array_index (model->vparam_len, guint, i);
    }
 
    model->params = ncm_vector_new (model->total_len);
    model->p      = ncm_vector_ref (model->params);
    g_array_set_size (model->ptypes, model->total_len);
    _ncm_model_set_sparams (model);
    ncm_model_params_set_default (model);
  }
}

static void
_ncm_model_dispose (GObject *object)
{
  NcmModel *model = NCM_MODEL (object);

  ncm_vector_clear (&model->params);
  ncm_vector_clear (&model->p);

  ncm_reparam_clear (&model->reparam);

  if (model->vparam_len != NULL)
  {
    g_array_unref (model->vparam_len);
    model->vparam_len = NULL;
  }

  if (model->vparam_pos != NULL)
  {
    g_array_unref (model->vparam_pos);
    model->vparam_pos = NULL;
  }

  if (model->ptypes != NULL)
  {
    g_array_unref (model->ptypes);
    model->ptypes = NULL;
  }

  if (model->sparams != NULL)
  {
    g_ptr_array_unref (model->sparams);
    model->sparams = NULL;
  }

  if (model->sparams_name_id != NULL)
  {
    g_hash_table_unref (model->sparams_name_id);
    model->sparams_name_id = NULL;
  }

  /* Chain up : end */
  G_OBJECT_CLASS (ncm_model_parent_class)->dispose (object);
}

static void
_ncm_model_finalize (GObject *object)
{
  /* Chain up : end */
  G_OBJECT_CLASS (ncm_model_parent_class)->finalize (object);
}

static void
_ncm_model_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  NcmModel *model = NCM_MODEL (object);
  g_return_if_fail (NCM_IS_MODEL (object));

  switch (prop_id)
  {
    case PROP_REPARAM:
      ncm_model_set_reparam (model, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
_ncm_model_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  NcmModel *model = NCM_MODEL (object);
  NcmModelClass *model_class = NCM_MODEL_GET_CLASS (object);

  g_return_if_fail (NCM_IS_MODEL (object));

  switch (prop_id)
  {
    case PROP_NAME:
      g_value_set_string (value, model_class->name);
      break;
    case PROP_NICK:
      g_value_set_string (value, model_class->nick);
      break;
    case PROP_SPARAMS_LEN:
      g_value_set_uint (value, model_class->sparam_len);
      break;
    case PROP_VPARAMS_LEN:
      g_value_set_uint (value, model_class->vparam_len);
      break;
    case PROP_IMPLEMENTATION:
      g_value_set_uint64 (value, model_class->impl);
      break;
    case PROP_REPARAM:
      g_value_set_object (value, model->reparam);
      break;
    case PROP_PTYPES:
      g_value_set_boxed (value, model->ptypes);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static gboolean
_ncm_model_valid (NcmModel *model)
{
  NCM_UNUSED (model);
  return TRUE;
}

static void
ncm_model_class_init (NcmModelClass *klass)
{
  GObjectClass* object_class = G_OBJECT_CLASS (klass);
  object_class->constructed  = &_ncm_model_constructed;
  object_class->set_property = &_ncm_model_set_property;
  object_class->get_property = &_ncm_model_get_property;
  object_class->dispose      = &_ncm_model_dispose;
  object_class->finalize     = &_ncm_model_finalize;

  klass->valid        = &_ncm_model_valid;
  klass->set_property = NULL;
  klass->get_property = NULL;

  klass->model_id          = -1;
  klass->name              = NULL;
  klass->nick              = NULL;
  klass->nonparam_prop_len = 0;
  klass->sparam_len        = 0;
  klass->vparam_len        = 0;
  klass->sparam            = NULL;
  klass->vparam            = NULL;

  g_object_class_install_property (object_class,
                                   PROP_NAME,
                                   g_param_spec_string ("name",
                                                        NULL,
                                                        "Model's name",
                                                        NULL,
                                                        G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
  g_object_class_install_property (object_class,
                                   PROP_NICK,
                                   g_param_spec_string ("nick",
                                                        NULL,
                                                        "Model's nick",
                                                        NULL,
                                                        G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

  g_object_class_install_property (object_class,
                                   PROP_SPARAMS_LEN,
                                   g_param_spec_uint ("scalar-params-len",
                                                      NULL,
                                                      "Number of scalar parameters",
                                                      0, G_MAXUINT, 0,
                                                      G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
  g_object_class_install_property (object_class,
                                   PROP_VPARAMS_LEN,
                                   g_param_spec_uint ("vector-params-len",
                                                      NULL,
                                                      "Number of vector parameters",
                                                      0, G_MAXUINT, 0,
                                                      G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

  g_object_class_install_property (object_class,
                                   PROP_IMPLEMENTATION,
                                   g_param_spec_uint64  ("implementation",
                                                         NULL,
                                                         "Bitwise specification of functions implementation",
                                                         0, G_MAXUINT64, 0,
                                                         G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
  
  g_object_class_install_property (object_class,
                                   PROP_PTYPES,
                                   g_param_spec_boxed  ("params-types",
                                                        NULL,
                                                        "Parameters' types",
                                                        G_TYPE_ARRAY,
                                                        G_PARAM_READABLE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));

  g_object_class_install_property (object_class,
                                   PROP_REPARAM,
                                   g_param_spec_object  ("reparam",
                                                         NULL,
                                                         "Model reparametrization",
                                                         NCM_TYPE_REPARAM,
                                                         G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
}

void
_ncm_model_class_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  NcmModel *model = NCM_MODEL (object);
  NcmModelClass *model_class = NCM_MODEL_CLASS (g_type_class_peek_static (pspec->owner_type));
  const guint sparam_id = prop_id - model_class->nonparam_prop_len + model_class->parent_sparam_len;
  const guint vparam_id = sparam_id	- model_class->sparam_len + model_class->parent_vparam_len;
  const guint vparam_len_id = vparam_id	- model_class->vparam_len + model_class->parent_vparam_len;
  const guint sparam_fit_id = vparam_len_id	- model_class->vparam_len + model_class->parent_sparam_len;
  const guint vparam_fit_id = sparam_fit_id	- model_class->sparam_len + model_class->parent_vparam_len;

  if (prop_id < model_class->nonparam_prop_len && model_class->get_property)
  {
    model_class->get_property (object, prop_id, value, pspec);
  }
  else if (sparam_id < model_class->sparam_len)
  {
    g_value_set_double (value, ncm_model_orig_param_get (model, sparam_id));
  }
  else if (vparam_id < model_class->vparam_len)
  {
    NcmVector *vp = ncm_model_orig_vparam_get_vector (model, vparam_id);
    GVariant *vp_var = ncm_vector_get_variant (vp);
    g_value_take_variant (value, vp_var);
    ncm_vector_free (vp);
  }
  else if (vparam_len_id < model_class->vparam_len)
  {
    g_value_set_uint (value, ncm_model_vparam_len (model, vparam_len_id));
  }
  else if (sparam_fit_id < model_class->sparam_len)
  {
    g_value_set_boolean (value, ncm_model_param_get_ftype (model, sparam_fit_id) == NCM_PARAM_TYPE_FREE ? TRUE: FALSE);
  }
  else if (vparam_fit_id < model_class->vparam_len)
  {
    gsize n = g_array_index (model->vparam_len, guint, vparam_fit_id);
    GVariantBuilder builder;
    GVariant *var;
    guint i;

    g_variant_builder_init (&builder, G_VARIANT_TYPE ("ab"));
    for (i = 0; i < n; i++)
    {
      guint pid = ncm_model_vparam_index (model, vparam_fit_id, i);
      gboolean tofit = ncm_model_param_get_ftype (model, pid) == NCM_PARAM_TYPE_FREE ? TRUE: FALSE;
      g_variant_builder_add (&builder, "b", tofit);
    }
    var = g_variant_builder_end (&builder);
    g_variant_ref_sink (var);
    
    g_value_take_variant (value, var);
  }
  else
    g_assert_not_reached ();
}

static void
_ncm_model_class_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  NcmModel *model = NCM_MODEL (object);
  NcmModelClass *model_class = NCM_MODEL_CLASS (g_type_class_peek_static (pspec->owner_type));
  const guint sparam_id = prop_id - model_class->nonparam_prop_len + model_class->parent_sparam_len;
  const guint vparam_id = sparam_id	- model_class->sparam_len + model_class->parent_vparam_len;
  const guint vparam_len_id = vparam_id	- model_class->vparam_len + model_class->parent_vparam_len;
  const guint sparam_fit_id = vparam_len_id	- model_class->vparam_len + model_class->parent_sparam_len;
  const guint vparam_fit_id = sparam_fit_id	- model_class->sparam_len + model_class->parent_vparam_len;

  //printf ("[%u %u] [%u %u] [%u %u] [%u %u] [%u %u] [%u %u]\n", prop_id, model_class->nonparam_prop_len, sparam_id, model_class->sparam_len, vparam_id, model_class->vparam_len, vparam_len_id, model_class->vparam_len, sparam_fit_id, model_class->sparam_len, vparam_fit_id, model_class->vparam_len);

  if (prop_id < model_class->nonparam_prop_len && model_class->set_property)
  {
    model_class->set_property (object, prop_id, value, pspec);
  }
  else if (sparam_id < model_class->sparam_len)
  {
    gdouble val = g_value_get_double (value);
    ncm_model_orig_param_set (model, sparam_id, val);
  }
  else if (vparam_id < model_class->vparam_len)
  {
    GVariant *var = g_value_get_variant (value);
    NcmVector *vals = ncm_vector_new_variant (var);
    gsize n = ncm_vector_len (vals);

    if (n != g_array_index (model->vparam_len, guint, vparam_id))
      g_error ("set_property: cannot set value of vector parameter, variant contains %zu childs but vector dimension is %u", 
                              n, ncm_model_vparam_len (model, vparam_id));

    ncm_model_orig_vparam_set_vector (model, vparam_id, vals);
    ncm_vector_free (vals);
  }
  else if (vparam_len_id < model_class->vparam_len)
  {
    guint psize = g_value_get_uint (value);
    g_array_index (model->vparam_len, guint, vparam_len_id) = psize;
  }
  else if (sparam_fit_id < model_class->sparam_len)
  {
    gboolean tofit = g_value_get_boolean (value);
    ncm_model_param_set_ftype (model, sparam_fit_id, tofit ? NCM_PARAM_TYPE_FREE : NCM_PARAM_TYPE_FIXED);
  }
  else if (vparam_fit_id < model_class->vparam_len)
  {
    GVariant *var = g_value_get_variant (value);
    gsize n = g_variant_n_children (var);
    gsize nv = g_array_index (model->vparam_len, guint, vparam_fit_id);
    guint i;

    if (n == 1)
    {
      gboolean tofit;
      GVariant *varc = g_variant_get_child_value (var, 0);
      
      if (g_variant_is_of_type (varc, G_VARIANT_TYPE ("b")))
        tofit = g_variant_get_boolean (varc);
      else if (g_variant_is_of_type (varc, G_VARIANT_TYPE ("i")))
        tofit = g_variant_get_int32 (varc) != 0;
      else
        g_error ("set_property: Cannot convert `%s' variant to an array of booleans", g_variant_get_type_string (varc));
      g_variant_unref (varc);
      
      for (i = 0; i < nv; i++)
      {
        guint pid = ncm_model_vparam_index (model, vparam_fit_id, i);
        ncm_model_param_set_ftype (model, pid, tofit ? NCM_PARAM_TYPE_FREE : NCM_PARAM_TYPE_FIXED);
      }
    }
    else if (n != nv)
      g_error ("set_property: cannot set fit type of vector parameter, variant contains %zu childs but vector dimension is %u", n, g_array_index (model->vparam_len, guint, vparam_fit_id));
    else
    {
      if (g_variant_is_of_type (var, G_VARIANT_TYPE ("ab")))
      {
        for (i = 0; i < n; i++)
        {
          guint pid = ncm_model_vparam_index (model, vparam_fit_id, i);
          gboolean tofit;
          g_variant_get_child (var, i, "b", &tofit);
          ncm_model_param_set_ftype (model, pid, tofit ? NCM_PARAM_TYPE_FREE : NCM_PARAM_TYPE_FIXED);
        }
      }
      else if (g_variant_is_of_type (var, G_VARIANT_TYPE ("ai")))
      {
        for (i = 0; i < n; i++)
        {
          guint pid = ncm_model_vparam_index (model, vparam_fit_id, i);
          gint tofit;
          g_variant_get_child (var, i, "i", &tofit);
          ncm_model_param_set_ftype (model, pid, tofit ? NCM_PARAM_TYPE_FREE : NCM_PARAM_TYPE_FIXED);
        }
      }
      else
        g_error ("set_property: Cannot convert `%s' variant to an array of booleans", g_variant_get_type_string (var));
    }
  }
  else
    g_assert_not_reached ();
}

/**
 * ncm_model_class_add_params:
 * @model_class: a #NcmModelClass.
 * @sparam_len: FIXME
 * @vparam_len: FIXME
 * @nonparam_prop_len: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_class_add_params (NcmModelClass *model_class, guint sparam_len, guint vparam_len, guint nonparam_prop_len)
{
  GObjectClass *object_class = G_OBJECT_CLASS (model_class);
  
  object_class->set_property = &_ncm_model_class_set_property;
  object_class->get_property = &_ncm_model_class_get_property;
  model_class->parent_sparam_len = model_class->sparam_len;
  model_class->parent_vparam_len = model_class->vparam_len;
  model_class->sparam_len += sparam_len;
  model_class->vparam_len += vparam_len;
  model_class->nonparam_prop_len = nonparam_prop_len;

  if (model_class->sparam_len > 0)
  {
    if (model_class->sparam == NULL)
    {
      model_class->sparam = g_ptr_array_new_with_free_func ((GDestroyNotify) &ncm_sparam_free);
      g_ptr_array_set_size (model_class->sparam, model_class->sparam_len);
    }
    else
    {
      GPtrArray *sparam = g_ptr_array_new_with_free_func ((GDestroyNotify) &ncm_sparam_free);
      guint i;
      g_ptr_array_set_size (sparam, model_class->sparam_len);
      /* Copy all parent params info */
      for (i = 0; i < model_class->parent_sparam_len; i++)
        g_ptr_array_index (sparam, i) = ncm_sparam_copy (g_ptr_array_index (model_class->sparam, i));
      model_class->sparam = sparam;
    }
  }

  if (model_class->vparam_len > 0)
  {
    if (model_class->vparam == NULL)
    {
      model_class->vparam = g_ptr_array_new_with_free_func ((GDestroyNotify) &ncm_vparam_free);
      g_ptr_array_set_size (model_class->vparam, model_class->vparam_len);
    }
    else
    {
      GPtrArray *vparam = g_ptr_array_new_with_free_func ((GDestroyNotify) &ncm_vparam_free);
      guint i;
      g_ptr_array_set_size (vparam, model_class->vparam_len);
      /* Copy all parent params info */
      for (i = 0; i < model_class->parent_vparam_len; i++)
        g_ptr_array_index (vparam, i) = ncm_vparam_copy (g_ptr_array_index (model_class->vparam, i));
      model_class->vparam = vparam;
    }
  }
}

/**
 * ncm_model_class_set_name_nick:
 * @model_class: a #NcmModelClass.
 * @name: FIXME
 * @nick: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_class_set_name_nick (NcmModelClass *model_class, const gchar *name, const gchar *nick)
{
/*  g_free (model_class->name); 
  g_free (model_class->nick); */

  model_class->name = g_strdup (name);
  model_class->nick = g_strdup (nick);
}

/**
 * ncm_model_class_set_sparam:
 * @model_class: a #NcmModelClass.
 * @sparam_id: FIXME
 * @symbol: FIXME
 * @name: FIXME
 * @lower_bound: FIXME
 * @upper_bound: FIXME
 * @scale: FIXME
 * @abstol: FIXME
 * @default_value: FIXME
 * @ppt: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_class_set_sparam (NcmModelClass *model_class, guint sparam_id, const gchar *symbol, const gchar *name, gdouble lower_bound, gdouble upper_bound, gdouble scale, gdouble abstol, gdouble default_value, NcmParamType ppt)
{
  GObjectClass* object_class = G_OBJECT_CLASS (model_class);
  const guint prop_id = sparam_id - model_class->parent_sparam_len + model_class->nonparam_prop_len;
  const guint prop_fit_id = prop_id + (model_class->sparam_len - model_class->parent_sparam_len) + 2 * (model_class->vparam_len - model_class->parent_vparam_len);

  NcmSParam *sparam = ncm_sparam_new (name, symbol, lower_bound, upper_bound, scale, abstol, default_value, ppt);

  g_assert (prop_id > 0);

  if (g_ptr_array_index (model_class->sparam, sparam_id) != NULL)
    g_error ("Scalar Parameter: %u is already set.", sparam_id);

  g_ptr_array_index (model_class->sparam, sparam_id) = sparam;

  g_object_class_install_property (object_class, prop_id,
                                   g_param_spec_double (name, NULL, symbol,
                                                        lower_bound, upper_bound, default_value,
                                                        G_PARAM_READWRITE));

  {
    gchar *param_fit_name = g_strdup_printf ("%s-fit", name);
    gchar *param_fit_symbol = g_strdup_printf ("%s:fit", symbol);
    g_object_class_install_property (object_class, prop_fit_id,
                                     g_param_spec_boolean (param_fit_name, NULL, param_fit_symbol,
                                                           ppt == NCM_PARAM_TYPE_FREE ? TRUE : FALSE,
                                                           G_PARAM_READWRITE));
    g_free (param_fit_name);
    g_free (param_fit_symbol);
  }
  
}

/**
 * ncm_model_class_set_vparam:
 * @model_class: a #NcmModelClass
 * @vparam_id: FIXME
 * @default_length: FIXME
 * @symbol: FIXME
 * @name: FIXME
 * @lower_bound: FIXME
 * @upper_bound: FIXME
 * @scale: FIXME
 * @abstol: FIXME
 * @default_value: FIXME
 * @ppt: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_class_set_vparam (NcmModelClass *model_class, guint vparam_id, guint default_length, const gchar *symbol, const gchar *name, gdouble lower_bound, gdouble upper_bound, gdouble scale, gdouble abstol, gdouble default_value, NcmParamType ppt)
{
  GObjectClass* object_class = G_OBJECT_CLASS (model_class);
  const guint prop_id = vparam_id + model_class->nonparam_prop_len - model_class->parent_vparam_len + (model_class->sparam_len - model_class->parent_sparam_len);
  const guint prop_len_id = prop_id + (model_class->vparam_len - model_class->parent_vparam_len);
  const guint prop_fit_id = prop_len_id + (model_class->vparam_len - model_class->parent_vparam_len) + (model_class->sparam_len - model_class->parent_sparam_len);

  NcmVParam *vparam = ncm_vparam_full_new (default_length, name, symbol, lower_bound, upper_bound, scale, abstol, default_value, ppt);

  g_assert (prop_id > 0);
  g_assert (prop_len_id > 0);

  if (g_ptr_array_index (model_class->vparam, vparam_id) != NULL)
    g_error ("Vector Parameter: %u is already set.", vparam_id);

  g_ptr_array_index (model_class->vparam, vparam_id) = vparam;
  g_object_class_install_property (object_class, prop_id,
                                   g_param_spec_variant (name, NULL, symbol, 
                                                         G_VARIANT_TYPE ("ad"), NULL, 
                                                         G_PARAM_READWRITE));
  {
    gchar *param_length_name = g_strdup_printf ("%s-length", name);
    gchar *param_length_symbol = g_strdup_printf ("%s:length", symbol);
    gchar *param_fit_name = g_strdup_printf ("%s-fit", name);
    gchar *param_fit_symbol = g_strdup_printf ("%s:fit", symbol);

    g_object_class_install_property (object_class, prop_len_id,
                                     g_param_spec_uint (param_length_name,
                                                        NULL,
                                                        param_length_symbol,
                                                        0, G_MAXUINT, default_length,
                                                        G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY));

    g_object_class_install_property (object_class, prop_fit_id,
                                     g_param_spec_variant (param_fit_name, NULL, param_fit_symbol,
                                                           G_VARIANT_TYPE_ARRAY, NULL,
                                                           G_PARAM_READWRITE));
    g_free (param_length_name);
    g_free (param_length_symbol);
    g_free (param_fit_name);
    g_free (param_fit_symbol);
  }
}

/**
 * ncm_model_class_check_params_info:
 * @model_class: a #NcmModelClass.
 *
 * FIXME
 *
 */
void
ncm_model_class_check_params_info (NcmModelClass *model_class)
{
  gulong i;
  guint total_params_len = model_class->sparam_len + model_class->vparam_len;
  if (total_params_len == 0 && model_class->nonparam_prop_len == 0)
  {
    g_error ("Class size or params not initialized, call ncm_model_class_add_params.");
  }

  for (i = 0; i < model_class->sparam_len; i++)
  {
    if (g_ptr_array_index (model_class->sparam, i) == NULL)
    {
      g_error ("Class (%s) didn't initialized scalar parameter %lu/%u", model_class->name ? model_class->name : "no-name", i + 1, model_class->sparam_len);
    }
    /* g_debug ("Model[%s][%s] id %lu\n", model_class->name, ((NcmSParam *)g_ptr_array_index (model_class->params_info, i))->name, i); */
  }

  for (i = 0; i < model_class->vparam_len; i++)
  {
    if (g_ptr_array_index (model_class->vparam, i) == NULL)
    {
      g_error ("Class (%s) didn't initialized vector parameter %lu/%u", model_class->name ? model_class->name : "no-name", i + 1, model_class->vparam_len);
    }
    /* g_debug ("Model[%s][%s] id %lu\n", model_class->name, ((NcmSParam *)g_ptr_array_index (model_class->params_info, i))->name, i); */
  }

  {
    GObjectClass *object_class = G_OBJECT_CLASS (model_class);
    if (object_class->set_property != &_ncm_model_class_set_property)
      g_error ("Class (%s) set object_class set_property, use model_class set_property instead.", model_class->name ? model_class->name : "no-name");
    if (object_class->get_property != &_ncm_model_class_get_property)
      g_error ("Class (%s) set object_class get_property, use model_class get_property instead.", model_class->name ? model_class->name : "no-name");
  }
}

/**
 * ncm_model_set_reparam:
 * @model: a #NcmModel.
 * @reparam: a #NcmReparam.
 *
 * FIXME
 *
 */
void
ncm_model_set_reparam (NcmModel *model, NcmReparam *reparam)
{
  if (reparam != NULL)
  {
    ncm_reparam_clear (&model->reparam);
    model->reparam = ncm_reparam_ref (reparam);

    ncm_vector_clear (&model->p);
    model->p = ncm_vector_ref (model->reparam->new_params);

    ncm_reparam_old2new (model->reparam, model, model->params, model->reparam->new_params);
  }
  else
    _ncm_model_sparams_remove_reparam (model);
}

/**
 * ncm_model_is_equal:
 * @model1: a #NcmModel.
 * @model2: a #NcmModel.
 *
 * Compares if model1 and model2 are the same,
 * with same dimension and reparametrization.
 *
 */
gboolean
ncm_model_is_equal (NcmModel *model1, NcmModel *model2)
{
  if (G_OBJECT_TYPE (model1) != G_OBJECT_TYPE (model2))
    return FALSE;
  if (ncm_vector_len (model1->params) != ncm_vector_len (model2->params))
    return FALSE;
  if (model1->reparam)
  {
    if (model2->reparam == NULL)
      return FALSE;
    if (G_OBJECT_TYPE (model1->reparam) != G_OBJECT_TYPE (model2->reparam))
      return FALSE;
  }
  return TRUE;
}

/**
 * ncm_model_get_reparam:
 * @model: a #NcmModel.
 *
 * FIXME
 *
 * Returns: FIXME
 */
NcmReparam *
ncm_model_get_reparam (NcmModel *model)
{
  NcmReparam *reparam;
  g_object_get (model, "reparam", &reparam, NULL);
  g_assert (NCM_IS_REPARAM (reparam));
  return reparam;
}

/**
 * ncm_model_reparam_df:
 * @model: a #NcmModel.
 * @fv: a #NcmVector.
 * @v: a #NcmVector.
 *
 * FIXME
 *
 */
void
ncm_model_reparam_df (NcmModel *model, NcmVector *fv, NcmVector *v)
{
  g_assert (model->reparam);
  g_assert_not_reached ();
  ncm_reparam_grad_old2new (model->reparam, model, NULL, fv, v);
}

/**
 * ncm_model_reparam_J:
 * @model: a #NcmModel.
 * @fJ: a #NcmMatrix.
 * @J: a #NcmMatrix.
 *
 * FIXME
 *
 */
void
ncm_model_reparam_J (NcmModel *model, NcmMatrix *fJ, NcmMatrix *J)
{
  g_assert (model->reparam);
  g_assert_not_reached ();
  ncm_reparam_M_old2new (model->reparam, model, NULL, fJ, J);
}

/**
 * ncm_model_params_set_default:
 * @model: a #NcmModel.
 *
 * FIXME
 *
 */
void
ncm_model_params_set_default (NcmModel *model)
{
  guint i;

  for (i = 0; i < model->total_len; i++)
  {
    const NcmSParam *p = ncm_model_param_peek_desc (model, i);
    ncm_vector_set (model->p, i, ncm_sparam_get_default_value (p));
  }
  ncm_model_params_update (model);
}

/**
 * ncm_model_params_save_as_default:
 * @model: a #NcmModel.
 *
 * FIXME
 *
 */
void
ncm_model_params_save_as_default (NcmModel *model)
{
  guint i;
  for (i = 0; i < model->total_len; i++)
  {
    NcmSParam *p = ncm_model_param_peek_desc (model, i);
    ncm_sparam_set_default_value (p, ncm_vector_get (model->p, i));
  }
}

/**
 * ncm_model_params_copyto:
 * @model: a #NcmModel.
 * @model_dest: a #NcmModel.
 *
 * FIXME
 *
 */
void
ncm_model_params_copyto (NcmModel *model, NcmModel *model_dest)
{
  g_assert (ncm_model_is_equal (model, model_dest));
  ncm_model_params_set_vector (model_dest, model->p);
}

/**
 * ncm_model_params_set_all:
 * @model: a #NcmModel.
 * @...: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_params_set_all (NcmModel *model, ...)
{
  guint i;
  va_list ap;
  va_start(ap, model);

  for (i = 0; i < ncm_vector_len (model->p); i++)
    ncm_vector_set (model->p, i, va_arg (ap, gdouble));

  va_end(ap);

  ncm_model_params_update (model);

  return;
}

/**
 * ncm_model_params_set_all_data:
 * @model: a #NcmModel.
 * @data: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_params_set_all_data (NcmModel *model, gdouble *data)
{
  guint i;

  for (i = 0; i < ncm_vector_len (model->p); i++)
    ncm_vector_set (model->p, i, data[i]);

  ncm_model_params_update (model);
  return;
}

/**
 * ncm_model_params_set_vector:
 * @model: a #NcmModel.
 * @v: a #NcmVector.
 *
 * FIXME
 *
 */
void
ncm_model_params_set_vector (NcmModel *model, NcmVector *v)
{
  ncm_vector_memcpy (model->p, v);
  ncm_model_params_update (model);
}

/**
 * ncm_model_params_set_model:
 * @model: a #NcmModel.
 * @model_src: a #NcmModel.
 *
 * FIXME
 *
 */
void
ncm_model_params_set_model (NcmModel *model, NcmModel *model_src)
{
  g_assert (ncm_model_is_equal (model, model_src));
  ncm_model_params_set_vector (model, model_src->p);
}

/**
 * ncm_model_params_print_all: (skip)
 * @model: a #NcmModel
 * @out: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_params_print_all (NcmModel *model, FILE *out)
{
  guint i;
  for (i = 0; i < ncm_vector_len (model->p); i++)
    fprintf (out, "  % 20.16g", ncm_vector_get (model->p, i));
  fprintf (out, "\n");
  fflush (out);
  return;
}

/**
 * ncm_model_params_log_all:
 * @model: a #NcmModel
 *
 * FIXME
 *
 */
void
ncm_model_params_log_all (NcmModel *model)
{
  guint i;
  for (i = 0; i < ncm_vector_len (model->p); i++)
    g_message ("  % 20.16g", ncm_vector_get (model->p, i));
  g_message ("\n");
  return;
}

/**
 * ncm_model_params_get_all:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: (transfer full): FIXME
 */
NcmVector *
ncm_model_params_get_all (NcmModel *model)
{
  return ncm_vector_dup (model->p);
}

/**
 * ncm_model_params_valid:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
gboolean 
ncm_model_params_valid (NcmModel *model)
{
  NcmModelClass *model_class = NCM_MODEL_GET_CLASS (model);
  return model_class->valid (model);
}

/**
 * ncm_model_params_valid_bounds:
 * @model: a #NcmModel
 *
 * Check whenever the paremeters respect the bounds.
 *
 * Returns: if the parameter respect the bounds.
 */
gboolean 
ncm_model_params_valid_bounds (NcmModel *model)
{
  guint i;
  for (i = 0; i < model->total_len; i++)
  {
    const gdouble lb  = ncm_model_param_get_lower_bound (model, i);
    const gdouble ub  = ncm_model_param_get_lower_bound (model, i);
    const gdouble val = ncm_model_param_get (model, i);
    if (val < lb || val > ub)
      return FALSE;
  }
  return TRUE;
}

/**
 * ncm_model_ref:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: (transfer full): FIXME
 */
/**
 * ncm_model_id:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_impl:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_len:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_state_is_update:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_state_set_update:
 * @model: a #NcmModel
 * 
 * FIXME
 * 
 */
/**
 * ncm_model_sparam_len:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_vparam_array_len:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_name:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
   */
/**
 * ncm_model_nick:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
 */
/**
 * ncm_model_peek_reparam:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
   */
/**
 * ncm_model_param_finite:
 * @model: a #NcmModel
 * @i: FIXME
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_params_finite:
 * @model: a #NcmModel
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_params_update:
 * @model: a #NcmModel
 *
 * Force the parameters to the update its internal flags and
 * update the original parameters if necessary.
 *
 */
/**
 * ncm_model_orig_params_update:
 * @model: a #NcmModel
 *
 * Update the new parameters. It causes an error to call this
 * function with a model without reparametrization.
 *
 */
/**
 * ncm_model_vparam_index:
 * @model: a #NcmModel
 * @n: vector index
 * @i: vector component index
 *
 * FIXME
 *
 * Returns: index of the i-th component of the n-th vector
 */
/**
 * ncm_model_vparam_len:
 * @model: a #NcmModel
 * @n: vector index
 *
 * FIXME
 *
 * Returns: length of the n-th vector
 */
/**
 * ncm_model_param_set:
 * @model: a #NcmModel
 * @n: FIXME
 * @val: FIXME
 *
 * FIXME
 *
 */
/**
 * ncm_model_param_set_default:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 */
/**
 * ncm_model_orig_param_peek_desc:
 * @model: a #NcmModel.
 * @n: parameter index.
 *
 * Peeks the @n-th original parameter description.
 *
 * Returns: (transfer none): The @n-th #NcmSParam of the original parametrization.
 */
/**
 * ncm_model_param_peek_desc:
 * @model: a #NcmModel.
 * @n: parameter index.
 *
 * Peeks the @n-th parameter description.
 *
 * Returns: (transfer none): The @n-th #NcmSParam.
 */
/**
 * ncm_model_param_get:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_orig_param_set:
 * @model: a #NcmModel
 * @n: FIXME
 * @val: FIXME
 *
 * FIXME
 *
 */
/**
 * ncm_model_orig_param_get:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_vparam_set:
 * @model: a #NcmModel
 * @n: FIXME
 * @i: FIXME
 * @val: FIXME
 *
 * FIXME
 *
 */
/**
 * ncm_model_vparam_get:
 * @model: a #NcmModel
 * @n: FIXME
 * @i: FIXME
 *
 * FIXME
 *
 * Returns: FIXME
 */
/**
 * ncm_model_orig_vparam_set_vector:
 * @model: a #NcmModel
 * @n: FIXME
 * @val: FIXME
 *
 * FIXME
 *
 */
/**
 * ncm_model_orig_vparam_get_vector:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: (transfer full): FIXME
 */

/**
 * ncm_model_orig_param_get_scale:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the scale of the original @n-th parameter.
 *
 * Returns: the scale of the original @n-th parameter.
 */
gdouble
ncm_model_orig_param_get_scale (NcmModel *model, guint n)
{
  return ncm_sparam_get_scale (ncm_model_orig_param_peek_desc (model, n));
}

/**
 * ncm_model_orig_param_get_lower_bound:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the lower bound of the original @n-th parameter.
 *
 * Returns: the lower bound of the original @n-th parameter.
 */
gdouble
ncm_model_orig_param_get_lower_bound (NcmModel *model, guint n)
{
  return ncm_sparam_get_lower_bound (ncm_model_orig_param_peek_desc (model, n));
}

/**
 * ncm_model_orig_param_get_upper_bound:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the upper bound of the original @n-th parameter.
 *
 * Returns: the upper bound of the original @n-th parameter.
 */
gdouble
ncm_model_orig_param_get_upper_bound (NcmModel *model, guint n)
{
  return ncm_sparam_get_upper_bound (ncm_model_orig_param_peek_desc (model, n));
}

/**
 * ncm_model_orig_param_get_abstol:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the absolute tolerance of the original @n-th parameter.
 *
 * Returns: the absolute tolerance of the original @n-th parameter.
 */
gdouble
ncm_model_orig_param_get_abstol (NcmModel *model, guint n)
{
  return ncm_sparam_get_absolute_tolerance (ncm_model_orig_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_scale:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the scale of the @n-th parameter.
 *
 * Returns: the scale of the @n-th parameter.
 */
gdouble
ncm_model_param_get_scale (NcmModel *model, guint n)
{
  return ncm_sparam_get_scale (ncm_model_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_lower_bound:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the lower bound of the @n-th parameter.
 *
 * Returns: the lower bound of the @n-th parameter.
 */
gdouble
ncm_model_param_get_lower_bound (NcmModel *model, guint n)
{
  return ncm_sparam_get_lower_bound (ncm_model_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_upper_bound:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the upper bound of the @n-th parameter.
 *
 * Returns: the upper bound of the @n-th parameter.
 */
gdouble
ncm_model_param_get_upper_bound (NcmModel *model, guint n)
{
  return ncm_sparam_get_upper_bound (ncm_model_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_abstol:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the absolute tolerance of the @n-th parameter.
 *
 * Returns: the absolute tolerance of the @n-th parameter.
 */
gdouble
ncm_model_param_get_abstol (NcmModel *model, guint n)
{
  return ncm_sparam_get_absolute_tolerance (ncm_model_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_ftype:
 * @model: a #NcmModel
 * @n: parameter index.
 *
 * Gets the fitting type of the @n-th parameter.
 *
 * Returns: the fitting type of the @n-th parameter.
 */
NcmParamType
ncm_model_param_get_ftype (NcmModel *model, guint n)
{
  return g_array_index (model->ptypes, NcmParamType, n);
}

/**
 * ncm_model_param_set_scale:
 * @model: a #NcmModel
 * @n: FIXME
 * @scale: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_param_set_scale (NcmModel *model, guint n, const gdouble scale)
{
  ncm_sparam_set_scale (ncm_model_param_peek_desc (model, n), scale);
}

/**
 * ncm_model_param_set_lower_bound:
 * @model: a #NcmModel
 * @n: FIXME
 * @lb: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_param_set_lower_bound (NcmModel *model, guint n, const gdouble lb)
{
  ncm_sparam_set_lower_bound (ncm_model_param_peek_desc (model, n), lb);
}

/**
 * ncm_model_param_set_upper_bound:
 * @model: a #NcmModel
 * @n: FIXME
 * @ub: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_param_set_upper_bound (NcmModel *model, guint n, const gdouble ub)
{
  ncm_sparam_set_upper_bound (ncm_model_param_peek_desc (model, n), ub);
}

/**
 * ncm_model_param_set_abstol:
 * @model: a #NcmModel
 * @n: FIXME
 * @abstol: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_param_set_abstol (NcmModel *model, guint n, const gdouble abstol)
{
  ncm_sparam_set_absolute_tolerance (ncm_model_param_peek_desc (model, n), abstol);
}

/**
 * ncm_model_param_set_ftype:
 * @model: a #NcmModel
 * @n: FIXME
 * @ptype: FIXME
 *
 * FIXME
 *
 */
void
ncm_model_param_set_ftype (NcmModel *model, guint n, const NcmParamType ptype)
{
  g_array_index (model->ptypes, NcmParamType, n) = ptype;
}

/**
 * ncm_model_orig_param_get_name:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
 */
const gchar *
ncm_model_orig_param_name (NcmModel *model, guint n)
{
  return ncm_sparam_name (ncm_model_orig_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_name:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
 */
const gchar *
ncm_model_param_name (NcmModel *model, guint n)
{
  return ncm_sparam_name (ncm_model_param_peek_desc (model, n));
}

/**
 * ncm_model_orig_param_get_symbol:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
 */
const gchar *
ncm_model_orig_param_symbol (NcmModel *model, guint n)
{
  return ncm_sparam_symbol (ncm_model_orig_param_peek_desc (model, n));
}

/**
 * ncm_model_param_get_symbol:
 * @model: a #NcmModel
 * @n: FIXME
 *
 * FIXME
 *
 * Returns: (transfer none): FIXME
 */
const gchar *
ncm_model_param_symbol (NcmModel *model, guint n)
{
  g_assert (n < model->total_len);
  return ncm_sparam_symbol (ncm_model_param_peek_desc (model, n));
}

/**
 * ncm_model_orig_param_index_from_name:
 * @model: a #NcmModel.
 * @param_name: parameter name.
 * @i: (out): parameter index.
 *
 * Looks for parameter named @param_name in the original parameters of @model 
 * and puts its index in @i and returns TRUE if found.
 * 
 * Returns: whether the parameter @param_name is found in the @model.
 */
gboolean
ncm_model_orig_param_index_from_name (NcmModel *model, const gchar *param_name, guint *i)
{
  gpointer param_id;
  gboolean found = g_hash_table_lookup_extended (model->sparams_name_id, param_name, NULL, &param_id);
  if (found)
    *i = GPOINTER_TO_UINT (param_id);
  else
    *i = -1; /* Yup, I know. */
  return found;
}

/**
 * ncm_model_param_index_from_name:
 * @model: a #NcmModel.
 * @param_name: parameter name.
 * @i: (out): parameter index.
 *
 * Looks for parameter named @param_name in @model and puts its index in @i
 * and returns TRUE if found.
 * 
 * Returns: whether the parameter @param_name is found in the @model.
 */
gboolean
ncm_model_param_index_from_name (NcmModel *model, const gchar *param_name, guint *i)
{
  NcmReparam *reparam = ncm_model_peek_reparam (model); 
  if (reparam != NULL)
  {
    if (ncm_reparam_index_from_name (reparam, param_name, i))
      return TRUE;
    else if (ncm_model_orig_param_index_from_name (model, param_name, i))
    {
      if (ncm_reparam_peek_param_desc (reparam, *i) != NULL)
      {
        g_error ("ncm_model_param_index_from_name: parameter (%s) was changed by a NcmReparam, it is now named (%s).", 
                 param_name, ncm_sparam_name (ncm_reparam_peek_param_desc (reparam, *i)));
        return FALSE;
      }
      else
        return TRUE;
    }
    else
      return FALSE;
  }
  else
    return ncm_model_orig_param_index_from_name (model, param_name, i);
}

/**
 * ncm_model_param_set_by_name:
 * @model: a #NcmModel.
 * @param_name: parameter name.
 * @val: parameter value.
 *
 * Sets the parameter value @val by @param_name.
 * 
 */
void 
ncm_model_param_set_by_name (NcmModel *model, const gchar *param_name, gdouble val)
{
  guint i;
  gboolean has_param = ncm_model_param_index_from_name (model, param_name, &i);
  g_assert (has_param);
  ncm_model_param_set (model, i, val);
}

/**
 * ncm_model_orig_param_set_by_name:
 * @model: a #NcmModel.
 * @param_name: parameter name.
 * @val: parameter value.
 *
 * Sets the parameter value @val by @param_name.
 * 
 */
void 
ncm_model_orig_param_set_by_name (NcmModel *model, const gchar *param_name, gdouble val)
{
  guint i;
  gboolean has_param = ncm_model_orig_param_index_from_name (model, param_name, &i);
  g_assert (has_param);
  ncm_model_orig_param_set (model, i, val);
}

/**
 * ncm_model_param_get_by_name:
 * @model: a #NcmModel.
 * @param_name: parameter name.
 *
 * Gets the parameter value by @param_name.
 * 
 * Returns: parameter value.
 */
gdouble 
ncm_model_param_get_by_name (NcmModel *model, const gchar *param_name)
{
  guint i;
  gboolean has_param = ncm_model_param_index_from_name (model, param_name, &i);
  g_assert (has_param);
  return ncm_model_param_get (model, i);
}

/**
 * ncm_model_orig_param_get_by_name:
 * @model: a #NcmModel.
 * @param_name: parameter name.
 *
 * Gets the original parameter value by @param_name.
 * 
 * Returns: parameter value.
 */
gdouble
ncm_model_orig_param_get_by_name (NcmModel *model, const gchar *param_name)
{
  guint i;
  gboolean has_param = ncm_model_orig_param_index_from_name (model, param_name, &i);
  g_assert (has_param);
  return ncm_model_orig_param_get (model, i);
}
