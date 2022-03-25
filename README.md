# pangovkvg

Source code of `pangocairo` taken from the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and currently being modified to use [Vkvg](https://github.com/jpbruyere/vkvg) instead of [Cairo](https://gitlab.freedesktop.org/cairo/cairo).

## Notes

I am reimplementing few functions at a time, at the moment:

* `pango_vkvg_create_layout`
* `pango_vkvg_update_layout`
* `pango_vkvg_show_layout`

and all their dependencies.

Currently it doesn't compile, I get undefined references for `pango_fc_font_map_*` functions. If you want to try it out and figure why, clone the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and set up its build directory with Meson; then include `pangovkvg.h` in a source file and use the functions listed above. To compile it, add the Pango build directory to the include directories, as it contains the `config.h` file currently needed by the `pangovkvg` source files (as of now it's hardcoded); then link with Vkvg and Pango as usual (e.g. with the result of `pkgconf --cflags --libs pango`).

## License

Most of the code of Pango is licensed under the terms of the GNU Lesser Public License (LGPL) - see the file COPYING for details.
