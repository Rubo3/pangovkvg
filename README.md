# pangovkvg

Source code of `pangocairo` taken from the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and currently being modified to use [`vkvg`](https://github.com/jpbruyere/vkvg) instead of [`cairo`](https://gitlab.freedesktop.org/cairo/cairo).

## Notes

I am reimplementing few functions at a time, at the moment:

* `pango_vkvg_create_layout`
* `pango_vkvg_update_layout`
* `pango_vkvg_show_layout`

and all their dependencies.

Currently it doesn't compile.

## License

Most of the code of Pango is licensed under the terms of the GNU Lesser Public License (LGPL) - see the file COPYING for details.
