# pangovkvg

Source code of `pangocairo` taken from the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and currently being modified to use [`vkvg`](https://github.com/jpbruyere/vkvg) instead of [`cairo`](https://github.com/freedesktop/cairo).

## Notes

Missing replacements for:

* Headers:
  * [`"cairo-ft.h"`](https://github.com/freedesktop/cairo/blob/master/src/cairo-ft.h)

* Structures and enumerations:
  * [`cairo_font_face_t`](https://github.com/freedesktop/cairo/blob/994e33215e35e49981dea5b459a9b5b85d4cd1b1/src/cairoint.h#L443)
  * [`cairo_font_options_t`](https://github.com/freedesktop/cairo/blob/9f44d4c7c509e7aa348f627f6b21dad62762810c/src/cairo-types-private.h#L190)
  * [`cairo_font_type_t`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo.h#L1579)
  * [`cairo_glyph_t`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo.h#L1115)
  * [`cairo_hint_metrics_t`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo.h#L1364)
  * [`cairo_scaled_font_t`](https://github.com/freedesktop/cairo/blob/d87fe096b90005ced23c76f3b44c1a3ad03d9b55/src/cairo-scaled-font-private.h#L52)
  * [`cairo_text_cluster_t`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo.h#L1147)
  * [`cairo_text_cluster_flags_t`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo.h#L1167)

* Functions:
  * [`cairo_font_options_copy`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo-font-options.c#L139)
  * [`cairo_font_options_create`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo-font-options.c#L108)
  * [`cairo_font_options_destroy`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo-font-options.c#L173)
  * [`cairo_font_options_equal`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo-font-options.c#L274)
  * [`cairo_font_options_get_hint_metrics`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo-font-options.c#L543)
  * [`cairo_font_options_merge`](https://github.com/freedesktop/cairo/blob/a04786b9330109ce54bf7f65c7068281419cec6a/src/cairo-font-options.c#L218)

## License

Most of the code of Pango is licensed under the terms of the GNU Lesser Public License (LGPL) - see the file COPYING for details.
