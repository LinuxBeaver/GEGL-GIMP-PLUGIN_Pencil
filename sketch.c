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
 * 2022 Beaver (GEGL Sketch)
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES


property_double (brightness, _("White Overlay"), 0.2)
   description  (_("Amount to increase the white overlay"))
   value_range  (0, 0.3)
   ui_range     (0, 0.3)


property_double (edgeamount, _("Pen Intensity"), 0.30)
   description (_("Strength of Effect"))
   value_range (0.0, 0.5)
   ui_range    (0.0, 0.5)


property_double (threshold, _("Threshold"), 0.50)
    value_range (-0.25, 0.8)
    ui_range    (-0.25, 0.8)
    description(_("Scalar threshold level (overridden if an auxiliary input buffer is provided.)."))

property_double (gaus, _("Blur"), 1.5)
   description (_("Standard deviation (spatial scale factor)"))
   value_range (1.5, 2.5)
   ui_range    (1.5, 2.5)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "y")



#else

#define GEGL_OP_META
#define GEGL_OP_NAME     sketch
#define GEGL_OP_C_SOURCE sketch.c

#include "gegl-op.h"

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglNode *input, *output, *gray, *edge, *threshold, *invert, *gaus, *med, *vp, *brightness;

  input    = gegl_node_get_input_proxy (gegl, "input");
  output   = gegl_node_get_output_proxy (gegl, "output");


  gray = gegl_node_new_child (gegl,
                                  "operation", "gegl:gray",
                                  NULL);


  edge = gegl_node_new_child (gegl,
                                  "operation", "gegl:edge-neon",
                                  NULL);


  threshold = gegl_node_new_child (gegl,
                                  "operation", "gegl:threshold",
                                  NULL);


   invert= gegl_node_new_child (gegl,
                                  "operation", "gegl:invert",
                                  NULL);



   gaus = gegl_node_new_child (gegl,
                                  "operation", "gegl:gaussian-blur",
                                  NULL);

   med = gegl_node_new_child (gegl,
                                  "operation", "gegl:median-blur",
                                  NULL);

   vp = gegl_node_new_child (gegl,
                                  "operation", "gegl:value-propagate",
                                  NULL);

   brightness = gegl_node_new_child (gegl,
                                  "operation", "gegl:brightness-contrast",
                                  NULL);




  gegl_operation_meta_redirect (operation, "gray", gray, "gray");

  gegl_operation_meta_redirect (operation, "edgeradius", edge, "radius");

  gegl_operation_meta_redirect (operation, "edgeamount", edge, "amount");

  gegl_operation_meta_redirect (operation, "threshold", threshold, "value");

  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-x");

  gegl_operation_meta_redirect (operation, "gaus", gaus, "std-dev-y");

  gegl_operation_meta_redirect (operation, "medianshape", med, "neighborhood");


  gegl_operation_meta_redirect (operation, "medianradius", med, "radius");

  gegl_operation_meta_redirect (operation, "medianpercentile", med, "percentile");

  gegl_operation_meta_redirect (operation, "brightness", brightness, "brightness");














  gegl_node_link_many (input, gray, edge, threshold, gaus, invert, med, vp, vp, brightness, output, NULL);




}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;

  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;

  gegl_operation_class_set_keys (operation_class,
    "name",        "gegl:pencil",
    "title",       _("Pencil drawing"),
    "categories",  "Aristic",
    "reference-hash", "456j6bfghd50f4f25sb27ac",
    "description", _("Make your image have a drawing effect with GEGL. Works best on images with solid color backgrounds.   "
                     ""),
    NULL);
}

#endif
