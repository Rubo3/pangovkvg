# pangovkvg

Source code of `pangocairo` taken from the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and currently being modified to use [Vkvg](https://github.com/jpbruyere/vkvg) instead of [Cairo](https://gitlab.freedesktop.org/cairo/cairo).

## Notes

I am reimplementing few functions at a time, at the moment:

* `pango_vkvg_create_layout`
* `pango_vkvg_update_layout`
* `pango_vkvg_show_layout`

and all their dependencies.

If you want to help, I have provided an example Makefile, put it at the root directory of your project. It expects two directories to be present: a copy of the [Pango repository](https://gitlab.gnome.org/GNOME/pango) and a copy of this repository. You also need [Meson](https://mesonbuild.com) as it creates a `config.h` file, needed by `pangovkvg` sources. Compiler flags and libraries are determined with [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config), and the libraries required by `pangocairo` are parsed with `awk`.

## License

Most of the code of Pango is licensed under the terms of the GNU Lesser Public License (LGPL) - see the file COPYING for details.
