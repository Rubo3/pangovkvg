/* Pango
 * pangovkvg-private.h: private symbols for the Vkvg backend
 *
 * Copyright (C) 2000,2004 Red Hat, Inc.
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

#ifndef __PANGOVKVG_PRIVATE_H__
#define __PANGOVKVG_PRIVATE_H__

#include <pango/pangovkvg.h>
#include <pango/pango-renderer.h>

G_BEGIN_DECLS


#define PANGO_VKVG_FONT_MAP_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), PANGO_TYPE_VKVG_FONT_MAP, PangoVkvgFontMapIface))

typedef struct _PangoVkvgFontMapIface PangoVkvgFontMapIface;

struct _PangoVkvgFontMapIface
{
  GTypeInterface g_iface;

  void           (*set_resolution) (PangoVkvgFontMap *fontmap,
				    double             dpi);
  double         (*get_resolution) (PangoVkvgFontMap *fontmap);
};


#define PANGO_VKVG_FONT_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), PANGO_TYPE_VKVG_FONT, PangoVkvgFontIface))

typedef struct _PangoVkvgFontIface                  PangoVkvgFontIface;
typedef struct _PangoVkvgFontPrivate                PangoVkvgFontPrivate;
typedef struct _PangoVkvgFontHexBoxInfo             PangoVkvgFontHexBoxInfo;
typedef struct _PangoVkvgFontPrivateScaledFontData  PangoVkvgFontPrivateScaledFontData;
typedef struct _PangoVkvgFontGlyphExtentsCacheEntry PangoVkvgFontGlyphExtentsCacheEntry;

struct _PangoVkvgFontHexBoxInfo
{
  PangoVkvgFont *font;
  int rows;
  double digit_width;
  double digit_height;
  double pad_x;
  double pad_y;
  double line_width;
  double box_descent;
  double box_height;
};

struct _PangoVkvgFontPrivateScaledFontData
{
  vkvg_matrix_t font_matrix;
  vkvg_matrix_t ctm;
  // vkvg_font_options_t *options;
};

struct _PangoVkvgFontPrivate
{
  PangoVkvgFont *cfont;

  PangoVkvgFontPrivateScaledFontData *data;

  // vkvg_scaled_font_t *scaled_font;
  PangoVkvgFontHexBoxInfo *hbi;

  gboolean is_hinted;
  PangoGravity gravity;

  PangoRectangle font_extents;
  PangoVkvgFontGlyphExtentsCacheEntry *glyph_extents_cache;

  GSList *metrics_by_lang;
};

struct _PangoVkvgFontIface
{
  GTypeInterface g_iface;

  // vkvg_font_face_t *(*create_font_face) (PangoVkvgFont *cfont);
  PangoFontMetrics *(*create_base_metrics_for_context) (PangoVkvgFont *cfont,
							PangoContext   *context);

  gssize cf_priv_offset;
};

gboolean _pango_vkvg_font_install (PangoFont *font,
				    VkvgContext cr);
PangoFontMetrics * _pango_vkvg_font_get_metrics (PangoFont     *font,
						  PangoLanguage *language);
PangoVkvgFontHexBoxInfo *_pango_vkvg_font_get_hex_box_info (PangoVkvgFont *cfont);

void _pango_vkvg_font_private_initialize (PangoVkvgFontPrivate      *cf_priv,
					   PangoVkvgFont             *font,
					   PangoGravity                gravity,
					   // const vkvg_font_options_t *font_options,
					   const PangoMatrix          *pango_ctm,
					   const vkvg_matrix_t       *font_matrix);
void _pango_vkvg_font_private_finalize (PangoVkvgFontPrivate *cf_priv);
// vkvg_scaled_font_t *_pango_vkvg_font_private_get_scaled_font (PangoVkvgFontPrivate *cf_priv);
gboolean _pango_vkvg_font_private_is_metrics_hinted (PangoVkvgFontPrivate *cf_priv);
void _pango_vkvg_font_private_get_glyph_extents (PangoVkvgFontPrivate *cf_priv,
						  PangoGlyph             glyph,
						  PangoRectangle        *ink_rect,
						  PangoRectangle        *logical_rect);

#define PANGO_TYPE_VKVG_RENDERER            (pango_vkvg_renderer_get_type())
#define PANGO_VKVG_RENDERER(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_VKVG_RENDERER, PangoVkvgRenderer))
#define PANGO_IS_VKVG_RENDERER(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_VKVG_RENDERER))

typedef struct _PangoVkvgRenderer PangoVkvgRenderer;

_PANGO_EXTERN
GType pango_vkvg_renderer_get_type    (void) G_GNUC_CONST;


// const vkvg_font_options_t *_pango_vkvg_context_get_merged_font_options (PangoContext *context);


G_END_DECLS

#endif /* __PANGOVKVG_PRIVATE_H__ */
