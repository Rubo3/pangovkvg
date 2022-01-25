# pangovkvg

Source code of `pangocairo` taken from the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and currently being modified to use [`vkvg`](https://github.com/jpbruyere/vkvg) instead of [`cairo`](https://github.com/freedesktop/cairo).

## Notes

Missing replacements for:

* `cairo_font_face_t`
* `cairo_font_options_t`
* `cairo_font_type_t`
* `cairo_glyph_t`
* `cairo_hint_metrics_t`
* `cairo_scaled_font_t`
* `cairo_text_cluster_t`
* `cairo_text_cluster_flags_t`
* `"cairo-ft.h"`

## License

Most of the code of Pango is licensed under the terms of the GNU Lesser Public License (LGPL) - see the file COPYING for details.
