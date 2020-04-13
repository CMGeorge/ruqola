TEMPLATE = subdirs

SUBDIRS  = rocketchatrestapi-qt5
SUBDIRS += corelib
SUBDIRS += apps
SUBDIRS += plugins
#rocketchatrestapi-qt5 \
#widgets \
#plugins \
#icons \
#core
#apps
#\
#data \
#apps.depends = core
#core.depends = plugins
plugins.depends = rocketchatrestapi-qt5
#rocketchatrestapi-qt5
