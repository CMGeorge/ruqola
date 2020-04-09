#ifndef LIBRUQOLACORE_EXPORT_H
#define LIBRUQOLACORE_EXPORT_H


#ifndef LIBRUQOLACORE_EXPORT
#  ifndef QT_STATIC
#    if defined(IS_LIBRUQOLACORE_EXPORT)
#      define LIBRUQOLACORE_EXPORT Q_DECL_EXPORT
#    else
#      define LIBROCKETCHATRESTAPI_QT5_EXPORT Q_DECL_IMPORT
#    endif
#  else
#    define LIBRUQOLACORE_EXPORT
#  endif
#else
#define LIBRUQOLACORE_EXPORT
#endif


#endif // LIBRUQOLACORE_EXPORT_H
