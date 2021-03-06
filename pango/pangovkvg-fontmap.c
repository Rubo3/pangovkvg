/* Pango
 * pangovkvg-fontmap.c: Vkvg font handling
 *
 * Copyright (C) 2000-2005 Red Hat Software
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

#include "pango/pango-impl-utils.h"
#include "pangovkvg.h"
#include "pangovkvg-private.h"

// HAVE_CAIRO_FREETYPE
#include "pangovkvg-fc.h"

typedef PangoVkvgFontMapIface PangoVkvgFontMapInterface;
G_DEFINE_INTERFACE (PangoVkvgFontMap, pango_vkvg_font_map, PANGO_TYPE_FONT_MAP)

static void
pango_vkvg_font_map_default_init (PangoVkvgFontMapIface *iface)
{
}

/**
 * pango_vkvg_font_map_new:
 *
 * Creates a new `PangoVkvgFontMap` object.
 *
 * A fontmap is used to cache information about available fonts,
 * and holds certain global parameters such as the resolution.
 * In most cases, you can use `func@PangoVkvg.font_map_get_default]
 * instead.
 *
 * Note that the type of the returned object will depend
 * on the particular font backend Vkvg was compiled to use;
 * You generally should only use the `PangoFontMap` and
 * `PangoVkvgFontMap` interfaces on the returned object.
 *
 * You can override the type of backend returned by using an
 * environment variable %PANGOVKVG_BACKEND. Supported types,
 * based on your build, are fc (fontconfig), win32, and coretext.
 * If requested type is not available, NULL is returned. Ie.
 * this is only useful for testing, when at least two backends
 * are compiled in.
 *
 * Return value: (transfer full): the newly allocated `PangoFontMap`,
 *   which should be freed with g_object_unref().
 *
 * Since: 1.10
 */
PangoFontMap *
pango_vkvg_font_map_new (void)
{
  const char *backend = getenv ("PANGOVKVG_BACKEND");
  if (backend && !*backend)
    backend = NULL;
#if defined(HAVE_CORE_TEXT) && defined (HAVE_CAIRO_QUARTZ)
  if (!backend || 0 == strcmp (backend, "coretext"))
    return g_object_new (PANGO_TYPE_CAIRO_CORE_TEXT_FONT_MAP, NULL);
#endif
#if defined(HAVE_CAIRO_WIN32)
  if (!backend || 0 == strcmp (backend, "win32"))
    return g_object_new (PANGO_TYPE_CAIRO_WIN32_FONT_MAP, NULL);
#endif
#if defined(HAVE_CAIRO_FREETYPE)
  if (!backend || 0 == strcmp (backend, "fc")
	       || 0 == strcmp (backend, "fontconfig"))
    return g_object_new (PANGO_TYPE_VKVG_FC_FONT_MAP, NULL);
#endif
  {
    const char backends[] = ""
#if defined(HAVE_CORE_TEXT) && defined (HAVE_CAIRO_QUARTZ)
      " coretext"
#endif
#if defined(HAVE_CAIRO_WIN32)
      " win32"
#endif
#if defined(HAVE_CAIRO_FREETYPE)
      " fontconfig"
#endif
      ;
    g_critical ("Unknown $PANGOVKVG_BACKEND value.\n  Available backends are:%s", backends);
  }
  return NULL;
}

/**
 * pango_vkvg_font_map_new_for_font_type:
 * @fonttype: desired #vkvg_font_type_t
 *
 * Creates a new `PangoVkvgFontMap` object of the type suitable
 * to be used with vkvg font backend of type @fonttype.
 *
 * In most cases one should simply use [func@PangoVkvg.FontMap.new], or
 * in fact in most of those cases, just use [func@PangoVkvg.FontMap.get_default].
 *
 * Return value: (transfer full) (nullable): the newly allocated
 *   `PangoFontMap` of suitable type which should be freed with
 *   g_object_unref(), or %NULL if the requested vkvg font backend
 *   is not supported / compiled in.
 *
 * Since: 1.18
 */
// PangoFontMap *
// pango_vkvg_font_map_new_for_font_type (vkvg_font_type_t fonttype)
// {
//   switch ((int) fonttype)
//   {
// #if defined(HAVE_CORE_TEXT) && defined (HAVE_CAIRO_QUARTZ)
//     case VKVG_FONT_TYPE_QUARTZ:
//       return g_object_new (PANGO_TYPE_CAIRO_CORE_TEXT_FONT_MAP, NULL);
// #endif
// #if defined(HAVE_CAIRO_WIN32)
//     case VKVG_FONT_TYPE_WIN32:
//       return g_object_new (PANGO_TYPE_CAIRO_WIN32_FONT_MAP, NULL);
// #endif
// #if defined(HAVE_CAIRO_FREETYPE)
//     case VKVG_FONT_TYPE_FT:
//       return g_object_new (PANGO_TYPE_VKVG_FC_FONT_MAP, NULL);
// #endif
//     default:
//       return NULL;
//   }
// }

static GPrivate default_font_map = G_PRIVATE_INIT (g_object_unref); /* MT-safe */

/**
 * pango_vkvg_font_map_get_default:
 *
 * Gets a default `PangoVkvgFontMap` to use with Vkvg.
 *
 * Note that the type of the returned object will depend on the
 * particular font backend Vkvg was compiled to use; you generally
 * should only use the `PangoFontMap` and `PangoVkvgFontMap`
 * interfaces on the returned object.
 *
 * The default Vkvg fontmap can be changed by using
 * [method@PangoVkvg.FontMap.set_default]. This can be used to
 * change the Vkvg font backend that the default fontmap uses
 * for example.
 *
 * Note that since Pango 1.32.6, the default fontmap is per-thread.
 * Each thread gets its own default fontmap. In this way, PangoVkvg
 * can be used safely from multiple threads.
 *
 * Return value: (transfer none): the default PangoVkvg fontmap
 *  for the current thread. This object is owned by Pango and must
 *  not be freed.
 *
 * Since: 1.10
 */
PangoFontMap *
pango_vkvg_font_map_get_default (void)
{
  PangoFontMap *fontmap = g_private_get (&default_font_map);

  if (G_UNLIKELY (!fontmap))
    {
      fontmap = pango_vkvg_font_map_new ();
      g_private_replace (&default_font_map, fontmap);
    }

  return fontmap;
}

/**
 * pango_vkvg_font_map_set_default:
 * @fontmap: (nullable): The new default font map
 *
 * Sets a default `PangoVkvgFontMap` to use with Vkvg.
 *
 * This can be used to change the Vkvg font backend that the
 * default fontmap uses for example. The old default font map
 * is unreffed and the new font map referenced.
 *
 * Note that since Pango 1.32.6, the default fontmap is per-thread.
 * This function only changes the default fontmap for
 * the current thread. Default fontmaps of existing threads
 * are not changed. Default fontmaps of any new threads will
 * still be created using [func@PangoVkvg.FontMap.new].
 *
 * A value of %NULL for @fontmap will cause the current default
 * font map to be released and a new default font map to be created
 * on demand, using [func@PangoVkvg.FontMap.new].
 *
 * Since: 1.22
 */
// void
// pango_vkvg_font_map_set_default (PangoVkvgFontMap *fontmap)
// {
//   g_return_if_fail (fontmap == NULL || PANGO_IS_VKVG_FONT_MAP (fontmap));

//   if (fontmap)
//     g_object_ref (fontmap);

//   g_private_replace (&default_font_map, fontmap);
// }

/**
 * pango_vkvg_font_map_set_resolution:
 * @fontmap: a `PangoVkvgFontMap`
 * @dpi: the resolution in "dots per inch". (Physical inches aren't actually
 *   involved; the terminology is conventional.)
 *
 * Sets the resolution for the fontmap.
 *
 * This is a scale factor between
 * points specified in a `PangoFontDescription` and Vkvg units. The
 * default value is 96, meaning that a 10 point font will be 13
 * units high. (10 * 96. / 72. = 13.3).
 *
 * Since: 1.10
 */
// void
// pango_vkvg_font_map_set_resolution (PangoVkvgFontMap *fontmap,
//                                      double             dpi)
// {
//   g_return_if_fail (PANGO_IS_VKVG_FONT_MAP (fontmap));

//   (* PANGO_VKVG_FONT_MAP_GET_IFACE (fontmap)->set_resolution) (fontmap, dpi);
// }

/**
 * pango_vkvg_font_map_get_resolution:
 * @fontmap: a `PangoVkvgFontMap`
 *
 * Gets the resolution for the fontmap.
 *
 * See [method@PangoVkvg.FontMap.set_resolution].
 *
 * Return value: the resolution in "dots per inch"
 *
 * Since: 1.10
 **/
// double
// pango_vkvg_font_map_get_resolution (PangoVkvgFontMap *fontmap)
// {
//   g_return_val_if_fail (PANGO_IS_VKVG_FONT_MAP (fontmap), 96.);

//   return (* PANGO_VKVG_FONT_MAP_GET_IFACE (fontmap)->get_resolution) (fontmap);
// }

/**
 * pango_vkvg_font_map_create_context: (skip)
 * @fontmap: a `PangoVkvgFontMap`
 *
 * Create a `PangoContext` for the given fontmap.
 *
 * Return value: the newly created context; free with g_object_unref().
 *
 * Since: 1.10
 *
 * Deprecated: 1.22: Use pango_font_map_create_context() instead.
 */
// PangoContext *
// pango_vkvg_font_map_create_context (PangoVkvgFontMap *fontmap)
// {
//   g_return_val_if_fail (PANGO_IS_VKVG_FONT_MAP (fontmap), NULL);

//   return pango_font_map_create_context (PANGO_FONT_MAP (fontmap));
// }

/**
 * pango_vkvg_font_map_get_font_type:
 * @fontmap: a `PangoVkvgFontMap`
 *
 * Gets the type of Vkvg font backend that @fontmap uses.
 *
 * Return value: the `vkvg_font_type_t` vkvg font backend type
 *
 * Since: 1.18
 */
// vkvg_font_type_t
// pango_vkvg_font_map_get_font_type (PangoVkvgFontMap *fontmap)
// {
//   g_return_val_if_fail (PANGO_IS_VKVG_FONT_MAP (fontmap), VKVG_FONT_TYPE_TOY);

//   return (* PANGO_VKVG_FONT_MAP_GET_IFACE (fontmap)->get_font_type) (fontmap);
// }
