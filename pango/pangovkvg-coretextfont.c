/* Pango
 * pangovkvg-coretextfont.c
 *
 * Copyright (C) 2000-2005 Red Hat Software
 * Copyright (C) 2005-2007 Imendio AB
 * Copyright (C) 2010  Kristian Rietveld  <kris@gtk.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <Carbon/Carbon.h>

#include "pango-impl-utils.h"
#include "pangocoretext-private.h"
#include "pangovkvg.h"
#include "pangovkvg-private.h"
#include "pangovkvg-coretext.h"
#include "pangovkvg-coretextfont.h"

struct _PangoVkvgCoreTextFont
{
  PangoCoreTextFont font;
  PangoVkvgFontPrivate cf_priv;
  int abs_size;
};

struct _PangoVkvgCoreTextFontClass
{
  PangoCoreTextFontClass parent_class;
};



static vkvg_font_face_t *pango_vkvg_core_text_font_create_font_face           (PangoVkvgFont *font);
static PangoFontMetrics  *pango_vkvg_core_text_font_create_base_metrics_for_context (PangoVkvgFont *font,
                                                                                      PangoContext    *context);

static void
vkvg_font_iface_init (PangoVkvgFontIface *iface)
{
  iface->create_font_face = pango_vkvg_core_text_font_create_font_face;
  iface->create_base_metrics_for_context = pango_vkvg_core_text_font_create_base_metrics_for_context;
  iface->cf_priv_offset = G_STRUCT_OFFSET (PangoVkvgCoreTextFont, cf_priv);
}

G_DEFINE_TYPE_WITH_CODE (PangoVkvgCoreTextFont, pango_vkvg_core_text_font, PANGO_TYPE_CORE_TEXT_FONT,
    { G_IMPLEMENT_INTERFACE (PANGO_TYPE_CAIRO_FONT, vkvg_font_iface_init) });

/* we want get_glyph_extents extremely fast, so we use a small wrapper here
 * to avoid having to lookup the interface data like we do for get_metrics
 * in _pango_vkvg_font_get_metrics(). */
static void
pango_vkvg_core_text_font_get_glyph_extents (PangoFont      *font,
                                              PangoGlyph      glyph,
                                              PangoRectangle *ink_rect,
                                              PangoRectangle *logical_rect)
{
  PangoVkvgCoreTextFont *cafont = (PangoVkvgCoreTextFont *) (font);

  _pango_vkvg_font_private_get_glyph_extents (&cafont->cf_priv,
					       glyph,
					       ink_rect,
					       logical_rect);
}

static vkvg_font_face_t *
pango_vkvg_core_text_font_create_font_face (PangoVkvgFont *font)
{
  PangoCoreTextFont *ctfont = (PangoCoreTextFont *) (font);
  CTFontRef font_id;
  CGFontRef cgfont;
  vkvg_font_face_t *vkvg_face;

  font_id = pango_core_text_font_get_ctfont (ctfont);
  cgfont = CTFontCopyGraphicsFont (font_id, NULL);

  vkvg_face = vkvg_quartz_font_face_create_for_cgfont (cgfont);

  CFRelease (cgfont);

  return vkvg_face;
}

static PangoFontMetrics *
pango_vkvg_core_text_font_create_base_metrics_for_context (PangoVkvgFont *font,
                                                            PangoContext   *context)
{
  PangoCoreTextFont *cfont = (PangoCoreTextFont *) font;
  PangoFontMetrics *metrics;
  CTFontRef ctfont;

  metrics = pango_font_metrics_new ();

  ctfont = pango_core_text_font_get_ctfont (cfont);

  metrics->ascent = CTFontGetAscent (ctfont) * PANGO_SCALE;
  metrics->descent = CTFontGetDescent (ctfont) * PANGO_SCALE;
  metrics->height = (CTFontGetAscent (ctfont) + CTFontGetDescent (ctfont) + CTFontGetLeading (ctfont)) * PANGO_SCALE;

  metrics->underline_position = CTFontGetUnderlinePosition (ctfont) * PANGO_SCALE;
  metrics->underline_thickness = CTFontGetUnderlineThickness (ctfont) * PANGO_SCALE;

  metrics->strikethrough_position = metrics->ascent / 3;
  metrics->strikethrough_thickness = CTFontGetUnderlineThickness (ctfont) * PANGO_SCALE;

  return metrics;
}

static PangoFontDescription *
pango_vkvg_core_text_font_describe_absolute (PangoFont *font)
{
  PangoVkvgCoreTextFont *cafont = (PangoVkvgCoreTextFont *)font;
  PangoFontDescription *desc = pango_font_describe (font);

  pango_font_description_set_absolute_size (desc, cafont->abs_size);

  return desc;
}

static void
pango_vkvg_core_text_font_finalize (GObject *object)
{
  PangoVkvgCoreTextFont *cafont = (PangoVkvgCoreTextFont *) object;

  _pango_vkvg_font_private_finalize (&cafont->cf_priv);

  G_OBJECT_CLASS (pango_vkvg_core_text_font_parent_class)->finalize (object);
}

static void
pango_vkvg_core_text_font_class_init (PangoVkvgCoreTextFontClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  PangoFontClass *font_class = PANGO_FONT_CLASS (class);

  object_class->finalize = pango_vkvg_core_text_font_finalize;
  /* font_class->describe defined by parent class PangoCoreTextFont. */
  font_class->get_glyph_extents = pango_vkvg_core_text_font_get_glyph_extents;
  font_class->get_metrics = _pango_vkvg_font_get_metrics;
  font_class->describe_absolute = pango_vkvg_core_text_font_describe_absolute;
}

static void
pango_vkvg_core_text_font_init (PangoVkvgCoreTextFont *cafont G_GNUC_UNUSED)
{
}

PangoCoreTextFont *
_pango_vkvg_core_text_font_new (PangoVkvgCoreTextFontMap  *cafontmap,
                                 PangoCoreTextFontKey       *key)
{
  gboolean synthesize_italic = FALSE;
  PangoVkvgCoreTextFont *cafont;
  PangoCoreTextFont *cfont;
  CTFontRef font_ref;
  CTFontDescriptorRef ctdescriptor;
  CGFontRef font_id;
  double size;
  vkvg_matrix_t font_matrix;

  size = pango_units_to_double (pango_core_text_font_key_get_size (key));

  size /= pango_matrix_get_font_scale_factor (pango_core_text_font_key_get_matrix (key));

  ctdescriptor = pango_core_text_font_key_get_ctfontdescriptor (key);
  font_ref = CTFontCreateWithFontDescriptor (ctdescriptor, size, NULL);

  if (pango_core_text_font_key_get_synthetic_italic (key))
    synthesize_italic = TRUE;

  font_id = CTFontCopyGraphicsFont (font_ref, NULL);
  if (!font_id)
    return NULL;

  cafont = g_object_new (PANGO_TYPE_CAIRO_CORE_TEXT_FONT, NULL);
  cfont = PANGO_CORE_TEXT_FONT (cafont);

  cafont->abs_size = pango_core_text_font_key_get_size (key);

  _pango_core_text_font_set_ctfont (cfont, font_ref);

  if (synthesize_italic)
    vkvg_matrix_init (&font_matrix,
                       1, 0,
                       -0.25, 1,
                       0, 0);
  else
    vkvg_matrix_init_identity (&font_matrix);

  vkvg_matrix_scale (&font_matrix, size, size);

  _pango_vkvg_font_private_initialize (&cafont->cf_priv,
					(PangoVkvgFont *) cafont,
                                        pango_core_text_font_key_get_gravity (key),
                                        pango_core_text_font_key_get_context_key (key),
                                        pango_core_text_font_key_get_matrix (key),
					&font_matrix);

  return cfont;
}
