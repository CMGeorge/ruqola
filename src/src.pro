TEMPLATE = subdirs

SUBDIRS += \
rocketchatrestapi-qt5 \
#widgets \
#plugins \
#icons \
core \
apps
#\
#data \
apps.depends = core
core.depends = rocketchatrestapi-qt5
