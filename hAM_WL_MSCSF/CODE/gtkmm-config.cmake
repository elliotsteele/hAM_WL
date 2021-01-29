# Needed for windows
find_path(GTKMM_INCLUDE_DIR gtkmm.h)

find_library(ATK_LIBRARY atk-1.0)
find_library(ATKMM_LIBRARY atkmm)
find_library(BROTLICOMMON_LIBRARY brotlicommon)
find_library(BROTLIDEC_LIBRARY brotlidec)
find_library(BROTLIENC_LIBRARY brotlienc)
find_library(BZ2_LIBRARY bz2)
find_library(CAIRO_GOBJECT_LIBRARY cairo-gobject)
find_library(CAIRO_LIBRARY cairo)
find_library(CAIROMM_LIBRARY cairomm-1.0)
find_library(EPOXY_LIBRARY epoxy)
find_library(FONTCONFIG_LIBRARY fontconfig)
find_library(FREETYPE_LIBRARY freetype)
find_library(GAILUTIL_LIBRARY gailutil-3.0)
find_library(GDK_LIBRARY gdk-3.0)
find_library(GDKMM_LIBRARY gdkmm)
find_library(GDK_PIXBUF_LIBRARY gdk_pixbuf-2.0)
find_library(GIO_LIBRARY gio-2.0)
find_library(GIOMM_LIBRARY giomm)
find_library(GLIBMM_LIBRARY glibmm)
find_library(GMODULE_LIBRARY gmodule-2.0)
find_library(GOBJECT_LIBRARY gobject-2.0)
find_library(GTHREAD_LIBRARY gthread-2.0)
find_library(GTK_LIBRARY gtk-3.0)
find_library(GTKMM_LIBRARY gtkmm)
find_library(HARFBUZZ_LIBRARY harfbuzz)
find_library(LIBCHARSET_LIBRARY libcharset)
find_library(LIBEXPAT_LIBRARY libexpat)
find_library(LIBFFI_LIBRARY libffi)
find_library(LIBICONV_LIBRARY libiconv)
find_library(LIBINTL_LIBRARY libintl)
find_library(LIBPNG_LIBRARY libpng16)
find_library(PANGO_LIBRARY pango-1.0)
find_library(PANGOCAIRO_LIBRARY pangocairo-1.0)
find_library(PANGOFT_LIBRARY pangoft2-1.0)
find_library(PANGOMM_LIBRARY pangomm)
find_library(PANGOWIN32_LIBRARY pangowin32-1.0)
find_library(PCRE16_LIBRARY pcre16)
find_library(PCRE32_LIBRARY pcre32)
find_library(PCRECPP_LIBRARY pcrecpp)
find_library(PCRE_LIBRARY pcre)
find_library(PCREPOSIX_LIBRARY pcreposix)
find_library(PIXMAN_LIBRARY pixman-1)
find_library(SIGC_LIBRARY sigc-2.0)
find_library(ZLIBD_LIBRARY zlibd)

list(APPEND GTKMM_LIBRARIES
        "${ATK_LIBRARY}"
        "${ATKMM_LIBRARY}"
        "${BROTLICOMMON_LIBRARY}"
        "${BROTLIDEC_LIBRARY}"
        "${BROTLIENC_LIBRARY}"
        "${BZ2_LIBRARY}"
        "${CAIRO_GOBJECT_LIBRARY}"
        "${CAIRO_LIBRARY}"
        "${CAIROMM_LIBRARY}"
        "${EPOXY_LIBRARY}"
        "${FONTCONFIG_LIBRARY}"
        "${FREETYPE_LIBRARY}"
        "${GAILUTIL_LIBRARY}"
        "${GDK_LIBRARY}"
        "${GDKMM_LIBRARY}"
        "${GDK_PIXBUF_LIBRARY}"
        "${GIO_LIBRARY}"
        "${GIOMM_LIBRARY}"
        "${GLIBMM_LIBRARY}"
        "${GMODULE_LIBRARY}"
        "${GOBJECT_LIBRARY}"
        "${GTHREAD_LIBRARY}"
        "${GTK_LIBRARY}"
        "${GTKMM_LIBRARY}"
        "${HARFBUZZ_LIBRARY}"
        "${LIBCHARSET_LIBRARY}"
        "${LIBEXPAT_LIBRARY}"
        "${LIBFFI_LIBRARY}"
        "${LIBICONV_LIBRARY}"
        "${LIBINTL_LIBRARY}"
        "${LIBPNG_LIBRARY}"
        "${PANGO_LIBRARY}"
        "${PANGOCAIRO_LIBRARY}"
        "${PANGOFT_LIBRARY}"
        "${PANGOMM_LIBRARY}"
        "${PANGOWIN32_LIBRARY}"
        "${PCRE16_LIBRARY}"
        "${PCRE32_LIBRARY}"
        "${PCRECPP_LIBRARY}"
        "${PCRE_LIBRARY}"
        "${PCREPOSIX_LIBRARY}"
        "${PIXMAN_LIBRARY}"
        "${SIGC_LIBRARY}"
        "${ZLIBD_LIBRARY}"
        )