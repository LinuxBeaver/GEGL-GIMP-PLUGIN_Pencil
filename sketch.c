/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright 2006 Øyvind Kolås <pippin@gimp.org>
 * GEGL Pencil, 2022 Beaver programmed filter) 2022 "cli345 Gimpchat.com' (Discoverer of the GEGL operations that made the filter)


GEGL Graph recreated by Beaver but likely discovered in Mid 2021 by Clli345. I'm impressed he knew about rgb-clip's use case two years before I did. I just recently learned what it does.

noise-reduction iterations=3
domain-transform n-iterations=1
gegl:difference-of-gaussians radius1=1 radius2=0.33
gray
levels in-low=0.004 in-high=0.009
invert-gamma
rgb-clip
noise-reduction
gaussian-blur std-dev-x=2 std-dev-y=2
*/

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

property_double (gaus, _("Blur"), 2.0)
  value_range (0.0, 2.5)
  ui_range (0.0, 2.5)
  ui_gamma (1.5)

property_int (dt, _("Smoothness"), 1)
  description(_("Number of filtering iterations. "
                "A value between 2 and 4 is usually enough."))
  value_range (1, 5)

property_double (dg1, _("Radius 1"), 1.0)
  value_range (0.4, 1.4)
  ui_range (0.4, 1.4)
  ui_gamma (1.5)

property_double (dg2, _("Radius 2"), 0.33)
  value_range (0.0, 1.4)
  ui_range (0.0, 1.4)
  ui_gamma (1.5)

property_double (low, _("Low Luminance"), 0.004)
    description ( _("Input luminance level to become lowest output"))
    ui_range    (0.0, 0.008)

property_double (high, _("High Luminance"), 0.009)
    description ( _("Input luminance levels"))
    ui_range    (0.004, 0.010)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     sketch
#define GEGL_OP_C_SOURCE sketch.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *nr, *dt, *dg, *gray, *levels, *ig, *rc, *nr2, *blur;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  nr = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction",
                                  NULL);

  dt = gegl_node_new_child (gegl,
                                  "operation", "gegl:domain-transform",
                                  NULL);

  dg = gegl_node_new_child (gegl,
                                  "operation", "gegl:difference-of-gaussians",
                                  NULL);



  gray = gegl_node_new_child (gegl,
                                  "operation", "gegl:gray",
                                  NULL);

  levels = gegl_node_new_child (gegl,
                                  "operation", "gegl:levels",
                                  NULL);


  ig = gegl_node_new_child (gegl,
                                  "operation", "gegl:invert-gamma",
                                  NULL);


  rc = gegl_node_new_child (gegl,
                                  "operation", "gegl:rgb-clip",
                                  NULL);

  nr2 = gegl_node_new_child (gegl,
                                  "operation", "gegl:noise-reduction",
                                  NULL);
  blur = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

 gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-x");
 gegl_operation_meta_redirect (operation, "gaus", blur, "std-dev-y");
 gegl_operation_meta_redirect (operation, "dt", dt, "n-iterations");
 gegl_operation_meta_redirect (operation, "dg1", dg, "radius1");
 gegl_operation_meta_redirect (operation, "dg2", dg, "radius2");
 gegl_operation_meta_redirect (operation, "low", levels, "in-low");
 gegl_operation_meta_redirect (operation, "high", levels, "in-high");


gegl_node_link_many (input, nr, dt, dg, gray, levels, ig, rc, nr2, blur, output, NULL);

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:pencil",
    "title",       _("Pencil drawing"),
    "reference-hash", "456j6bfghd50f435sf27ac",
    "description", _("Make your image have a drawing effect with GEGL.   "
                     ""),
    "gimp:menu-path", "<Image>/Filters/Artistic",
    "gimp:menu-label", _("Pencil..."),
    NULL);
}

#endif

