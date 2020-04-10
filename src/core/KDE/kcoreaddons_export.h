#ifndef KCOREADDONS_EXPORT
#  ifndef QT_STATIC
#    if defined(IS_LIBRUQOLACORE_EXPORT)
#      define KCOREADDONS_EXPORT Q_DECL_EXPORT
#    else
#      define KCOREADDONS_EXPORT Q_DECL_IMPORT
#    endif
#  else
#    define KCOREADDONS_EXPORT
#  endif
#else
#define KCOREADDONS_EXPORT
#endif
