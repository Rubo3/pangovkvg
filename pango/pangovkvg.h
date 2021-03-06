#ifndef __PANGOVKVG_H__
#define __PANGOVKVG_H__

#include <pango/pango.h>
#include <vkvg.h>

G_BEGIN_DECLS

/* Temporary typedefs. */

typedef void vkvg_font_options_t;
typedef void vkvg_font_face_t;
typedef void vkvg_scaled_font_t;

typedef enum {
    VKVG_FONT_TYPE_FT,
} vkvg_font_type_t;

typedef struct {
    int num_bytes;
    int num_glyphs;
} vkvg_text_cluster_t;

typedef enum _vkvg_text_cluster_flags {
    VKVG_TEXT_CLUSTER_FLAG_BACKWARD = 0x00000001
} vkvg_text_cluster_flags_t;

/**
 * PangoVkvgFont:
 *
 * `PangoVkvgFont` is an interface exported by fonts for
 * use with Vkvg.
 *
 * The actual type of the font will depend on the particular
 * font technology Vkvg was compiled to use.
 *
 * Since: 1.18
 **/
typedef struct _PangoVkvgFont      PangoVkvgFont;

/* This is a hack because PangoVkvg is hijacking the Pango namespace, but
 * consumers of the PangoVkvg API expect these symbols to live under the
 * PangoVkvg namespace.
 */
#ifdef __GI_SCANNER__
#define PANGO_VKVG_TYPE_FONT           (pango_vkvg_font_get_type())
#define PANGO_VKVG_FONT(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), PANGO_VKVG_TYPE_FONT, PangoVkvgFont))
#define PANGO_VKVG_IS_FONT(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PANGO_VKVG_TYPE_FONT))
#else
#define PANGO_TYPE_VKVG_FONT           (pango_vkvg_font_get_type ())
#define PANGO_VKVG_FONT(object)        (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_VKVG_FONT, PangoVkvgFont))
#define PANGO_IS_VKVG_FONT(object)     (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_VKVG_FONT))
#endif

/**
 * PangoVkvgFontMap:
 *
 * `PangoVkvgFontMap` is an interface exported by font maps for
 * use with Vkvg.
 *
 * The actual type of the font map will depend on the particular
 * font technology Vkvg was compiled to use.
 *
 * Since: 1.10
 **/
typedef struct _PangoVkvgFontMap        PangoVkvgFontMap;

#ifdef __GI_SCANNER__
#define PANGO_VKVG_TYPE_FONT_MAP       (pango_vkvg_font_map_get_type())
#define PANGO_VKVG_FONT_MAP(obj)       (G_TYPE_CHECK_INSTANCE_CAST ((obj), PANGO_VKVG_TYPE_FONT_MAP, PangoVkvgFontMap))
#define PANGO_VKVG_IS_FONT_MAP(obj)    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PANGO_VKVG_TYPE_FONT_MAP))
#else
#define PANGO_TYPE_VKVG_FONT_MAP       (pango_vkvg_font_map_get_type ())
#define PANGO_VKVG_FONT_MAP(object)    (G_TYPE_CHECK_INSTANCE_CAST ((object), PANGO_TYPE_VKVG_FONT_MAP, PangoVkvgFontMap))
#define PANGO_IS_VKVG_FONT_MAP(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), PANGO_TYPE_VKVG_FONT_MAP))
#endif

/**
 * PangoVkvgShapeRendererFunc:
 * @ctx: a Vkvg context with current point set to where the shape should
 * be rendered
 * @attr: the %PANGO_ATTR_SHAPE to render
 * @do_path: whether only the shape path should be appended to current
 * path of @cr and no filling/stroking done.  This will be set
 * to %TRUE when called from pango_vkvg_layout_path() and
 * pango_vkvg_layout_line_path() rendering functions.
 * @data: (closure): user data passed to pango_vkvg_context_set_shape_renderer()
 *
 * Function type for rendering attributes of type %PANGO_ATTR_SHAPE
 * with Pango's Vkvg renderer.
 */
typedef void (*PangoVkvgShapeRendererFunc)(VkvgContext     cr,
										   PangoAttrShape *attr,
										   gboolean        do_path,
										   gpointer        data);

/*
 * PangoVkvgFontMap
 */
PANGO_AVAILABLE_IN_1_10
GType pango_vkvg_font_map_get_type(void) G_GNUC_CONST;

PANGO_AVAILABLE_IN_1_10
PangoFontMap * pango_vkvg_font_map_new(void);
PANGO_AVAILABLE_IN_1_18
PangoFontMap * pango_vkvg_font_map_new_for_font_type (vkvg_font_type_t fonttype);
PANGO_AVAILABLE_IN_1_10
PangoFontMap * pango_vkvg_font_map_get_default(void);
PANGO_AVAILABLE_IN_1_22
void pango_vkvg_font_map_set_default(PangoVkvgFontMap *fontmap);
PANGO_AVAILABLE_IN_1_18
vkvg_font_type_t pango_vkvg_font_map_get_font_type(PangoVkvgFontMap *fontmap);

PANGO_AVAILABLE_IN_1_10
void pango_vkvg_font_map_set_resolution(PangoVkvgFontMap *fontmap, double pi);
PANGO_AVAILABLE_IN_1_10
double pango_vkvg_font_map_get_resolution(PangoVkvgFontMap *fontmap);
#ifndef PANGO_DISABLE_DEPRECATED
PANGO_DEPRECATED_IN_1_22_FOR(pango_font_map_create_context)
PangoContext * pango_vkvg_font_map_create_context(PangoVkvgFontMap *fontmap);
#endif

/*
 * PangoVkvgFont
 */
PANGO_AVAILABLE_IN_1_18
GType pango_vkvg_font_get_type(void) G_GNUC_CONST;

PANGO_AVAILABLE_IN_1_18
vkvg_scaled_font_t *pango_vkvg_font_get_scaled_font(PangoVkvgFont *font);

/* Update a Pango context for the current state of a vkvg context
 */
PANGO_AVAILABLE_IN_1_10
void pango_vkvg_update_context(VkvgContext cr, PangoContext *context);

// PANGO_AVAILABLE_IN_1_10
// void pango_vkvg_context_set_font_options(PangoContext *context, const vkvg_font_options_t *options);
// PANGO_AVAILABLE_IN_1_10
// const vkvg_font_options_t *pango_vkvg_context_get_font_options(PangoContext *context);

// PANGO_AVAILABLE_IN_1_10
void pango_vkvg_context_set_resolution(PangoContext *context, double dpi);
// PANGO_AVAILABLE_IN_1_10
double pango_vkvg_context_get_resolution(PangoContext *context);

// PANGO_AVAILABLE_IN_1_18
// void pango_vkvg_context_set_shape_renderer (PangoContext              *context,
// 								               PangoVkvgShapeRendererFunc func,
// 								               gpointer                   data,
// 								               GDestroyNotify             dnotify);
PANGO_AVAILABLE_IN_1_18
PangoVkvgShapeRendererFunc pango_vkvg_context_get_shape_renderer(PangoContext *context, gpointer *data);

/* Convenience
 */
PANGO_AVAILABLE_IN_1_22
PangoContext *pango_vkvg_create_context(VkvgContext cr);
PANGO_AVAILABLE_IN_ALL
PangoLayout  *pango_vkvg_create_layout(VkvgContext cr);
PANGO_AVAILABLE_IN_1_10
void pango_vkvg_update_layout(VkvgContext cr, PangoLayout *layout);

/*
 * Rendering
 */
// PANGO_AVAILABLE_IN_1_10
// void pango_vkvg_show_glyph_string (VkvgContext cr,
// 				    PangoFont        *font,
// 				    PangoGlyphString *glyphs);
// PANGO_AVAILABLE_IN_1_22
// void pango_vkvg_show_glyph_item   (VkvgContext cr,
// 				    const char       *text,
// 				    PangoGlyphItem   *glyph_item);
// PANGO_AVAILABLE_IN_1_10
// void pango_vkvg_show_layout_line  (VkvgContext cr,
// 				    PangoLayoutLine  *line);
PANGO_AVAILABLE_IN_1_10
void pango_vkvg_show_layout(VkvgContext cr, PangoLayout *layout);

// PANGO_AVAILABLE_IN_1_14
// void pango_vkvg_show_error_underline (VkvgContext cr,
// 				       double         x,
// 				       double         y,
// 				       double         width,
// 				       double         height);

/*
 * Rendering to a path
 */
// PANGO_AVAILABLE_IN_1_10
// void pango_vkvg_glyph_string_path (VkvgContext cr,
// 				    PangoFont        *font,
// 				    PangoGlyphString *glyphs);
// PANGO_AVAILABLE_IN_1_10
// void pango_vkvg_layout_line_path  (VkvgContext cr,
// 				    PangoLayoutLine  *line);
// PANGO_AVAILABLE_IN_1_10
// void pango_vkvg_layout_path       (VkvgContext cr,
// 				    PangoLayout      *layout);

// PANGO_AVAILABLE_IN_1_14
// void pango_vkvg_error_underline_path (VkvgContext cr,
// 				       double         x,
// 				       double         y,
// 				       double         width,
// 				       double         height);

G_END_DECLS

#endif /* __PANGOVKVG_H__ */
