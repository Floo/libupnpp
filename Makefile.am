
AM_CPPFLAGS = -DDEBUG -g -Wall \
            -DDATADIR=\"${pkgdatadir}\" -DCONFIGDIR=\"${sysconfdir}\" \
            -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\"

AM_CXXFLAGS = -std=c++0x 

lib_LTLIBRARIES = libupnpp.la

libupnpp_la_SOURCES = \
    libupnpp/control/avlastchg.cxx \
    libupnpp/control/avlastchg.hxx \
    libupnpp/control/avtransport.cxx \
    libupnpp/control/avtransport.hxx \
    libupnpp/control/cdircontent.cxx \
    libupnpp/control/cdircontent.hxx \
    libupnpp/control/cdirectory.cxx \
    libupnpp/control/cdirectory.hxx \
    libupnpp/control/description.cxx \
    libupnpp/control/description.hxx \
    libupnpp/control/device.hxx \
    libupnpp/control/device.cxx \
    libupnpp/control/discovery.cxx \
    libupnpp/control/discovery.hxx \
    libupnpp/control/httpdownload.cxx \
    libupnpp/control/httpdownload.hxx \
    libupnpp/control/linnsongcast.cxx \
    libupnpp/control/linnsongcast.hxx \
    libupnpp/control/mediarenderer.cxx \
    libupnpp/control/mediarenderer.hxx \
    libupnpp/control/mediaserver.cxx \
    libupnpp/control/mediaserver.hxx \
    libupnpp/control/ohplaylist.cxx \
    libupnpp/control/ohplaylist.hxx \
    libupnpp/control/ohproduct.cxx \
    libupnpp/control/ohproduct.hxx \
    libupnpp/control/ohradio.cxx \
    libupnpp/control/ohradio.hxx \
    libupnpp/control/ohinfo.cxx \
    libupnpp/control/ohinfo.hxx \
    libupnpp/control/ohreceiver.cxx \
    libupnpp/control/ohreceiver.hxx \
    libupnpp/control/ohsender.cxx \
    libupnpp/control/ohsender.hxx \
    libupnpp/control/ohtime.cxx \
    libupnpp/control/ohtime.hxx \
    libupnpp/control/ohvolume.cxx \
    libupnpp/control/ohvolume.hxx \
    libupnpp/control/renderingcontrol.cxx \
    libupnpp/control/renderingcontrol.hxx \
    libupnpp/control/service.cxx \
    libupnpp/control/service.hxx \
    libupnpp/device/device.cxx \
    libupnpp/device/device.hxx \
    libupnpp/device/vdir.cxx \
    libupnpp/device/vdir.hxx \
    libupnpp/base64.hxx \
    libupnpp/base64.cxx \
    libupnpp/expatmm.hxx \
    libupnpp/getsyshwaddr.c \
    libupnpp/getsyshwaddr.h \
    libupnpp/ixmlwrap.cxx \
    libupnpp/ixmlwrap.hxx \
    libupnpp/log.cxx \
    libupnpp/log.hxx \
    libupnpp/md5.cxx \
    libupnpp/md5.hxx \
    libupnpp/ptmutex.hxx \
    libupnpp/workqueue.hxx \
    libupnpp/soaphelp.cxx \
    libupnpp/soaphelp.hxx \
    libupnpp/upnpavutils.cxx \
    libupnpp/upnpavutils.hxx \
    libupnpp/upnpp_p.hxx \
    libupnpp/upnpplib.cxx \
    libupnpp/upnpplib.hxx \
    libupnpp/workqueue.hxx

nobase_include_HEADERS = \
    libupnpp/base64.hxx \
    libupnpp/conf_post.h \
    libupnpp/config.h \
    libupnpp/control/avtransport.hxx \
    libupnpp/control/cdircontent.hxx \
    libupnpp/control/cdirectory.hxx \
    libupnpp/control/description.hxx \
    libupnpp/control/device.hxx \
    libupnpp/control/discovery.hxx \
    libupnpp/control/linnsongcast.hxx \
    libupnpp/control/mediarenderer.hxx \
    libupnpp/control/mediaserver.hxx \
    libupnpp/control/ohinfo.hxx \
    libupnpp/control/ohplaylist.hxx \
    libupnpp/control/ohproduct.hxx \
    libupnpp/control/ohradio.hxx \
    libupnpp/control/ohreceiver.hxx \
    libupnpp/control/ohsender.hxx \
    libupnpp/control/ohtime.hxx \
    libupnpp/control/ohvolume.hxx \
    libupnpp/control/renderingcontrol.hxx \
    libupnpp/control/service.hxx \
    libupnpp/device/device.hxx \
    libupnpp/log.hxx \
    libupnpp/ptmutex.hxx \
    libupnpp/soaphelp.hxx \
    libupnpp/upnpavutils.hxx \
    libupnpp/upnpplib.hxx \
    libupnpp/upnpputils.hxx \
    libupnpp/workqueue.hxx

libupnpp_la_LDFLAGS = -version-info $(VERSION_INFO)

libupnpp_la_LIBADD = $(LIBUPNPP_LIBS)

