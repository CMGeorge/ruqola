#ifndef LIBROCKETCHATRESTAPIQT5_EXPORT_H
#define LIBROCKETCHATRESTAPIQT5_EXPORT_H

#ifndef LIBROCKETCHATRESTAPI_QT5_EXPORT
#  ifndef QT_STATIC
#    if defined(IS_LIBROCKETCHATRESTAPI_QT5_EXPORT)
#      define LIBROCKETCHATRESTAPI_QT5_EXPORT Q_DECL_EXPORT
#    else
#      define LIBROCKETCHATRESTAPI_QT5_EXPORT Q_DECL_IMPORT
#    endif
#  else
#    define LIBROCKETCHATRESTAPI_QT5_EXPORT
#  endif
#else
#define LIBROCKETCHATRESTAPI_QT5_EXPORT
#endif


#endif // LIBROCKETCHATRESTAPIQT5_EXPORT_H