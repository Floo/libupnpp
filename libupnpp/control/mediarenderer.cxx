/* Copyright (C) 2014 J.F.Dockes
 *       This program is free software; you can redistribute it and/or modify
 *       it under the terms of the GNU General Public License as published by
 *       the Free Software Foundation; either version 2 of the License, or
 *       (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public License
 *       along with this program; if not, write to the
 *       Free Software Foundation, Inc.,
 *       59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "libupnpp/config.h"

#include "libupnpp/control/mediarenderer.hxx"

#include <ostream>                      // for endl
#include <string>                       // for string
#include <utility>                      // for pair
#include <vector>                       // for vector

#include "libupnpp/control/description.hxx"  // for UPnPDeviceDesc, etc
#include "libupnpp/control/discovery.hxx"  // for UPnPDeviceDirectory, etc
#include "libupnpp/control/renderingcontrol.hxx"  // for RenderingControl, etc
#include "libupnpp/log.hxx"             // for LOGERR, LOGINF

using namespace std;
using namespace STD_PLACEHOLDERS;
using namespace UPnPP;

namespace UPnPClient {

const string 
MediaRenderer::DType("urn:schemas-upnp-org:device:MediaRenderer:1");

class MediaRenderer::Internal {
public:
    STD_WEAK_PTR<RenderingControl> rdc;
    STD_WEAK_PTR<AVTransport> avt;
    STD_WEAK_PTR<OHProduct> ohpr;
    STD_WEAK_PTR<OHPlaylist> ohpl;
    STD_WEAK_PTR<OHTime> ohtm;
    STD_WEAK_PTR<OHVolume> ohvl;
    STD_WEAK_PTR<OHReceiver> ohrc;
    STD_WEAK_PTR<OHRadio> ohrd;
    STD_WEAK_PTR<OHInfo> ohif;
    STD_WEAK_PTR<OHSender> ohsn;
};

// We don't include a version in comparisons, as we are satisfied with
// version 1
bool MediaRenderer::isMRDevice(const string& st)
{
    const string::size_type sz(DType.size()-2);
    return !DType.compare(0, sz, st, 0, sz);
}

// Look at all service descriptions and store parent devices for
// either UPnP RenderingControl or OpenHome Product. Some entries will
// be set multiple times, which does not matter
static bool MDAccum(STD_UNORDERED_MAP<string, UPnPDeviceDesc>* out,
                    const string& friendlyName,
                    const UPnPDeviceDesc& device, 
                    const UPnPServiceDesc& service)
{
    //LOGDEB("MDAccum: friendlyname: " << friendlyName << 
    //    " dev friendlyName " << device.friendlyName << endl);
    if (
        (RenderingControl::isRDCService(service.serviceType) ||
         OHProduct::isOHPrService(service.serviceType))
        &&
        (friendlyName.empty() || !friendlyName.compare(device.friendlyName))) {
        //LOGDEB("MDAccum setting " << device.UDN << endl);
        (*out)[device.UDN] = device;
    }
    return true;
}

bool MediaRenderer::getDeviceDescs(vector<UPnPDeviceDesc>& devices, 
                                   const string& friendlyName)
{
    STD_UNORDERED_MAP<string, UPnPDeviceDesc> mydevs;

    UPnPDeviceDirectory::Visitor visitor = bind(MDAccum, &mydevs, friendlyName,
                                                _1, _2);
    UPnPDeviceDirectory::getTheDir()->traverse(visitor);
    for (STD_UNORDERED_MAP<string, UPnPDeviceDesc>::iterator it = 
             mydevs.begin(); it != mydevs.end(); it++)
        devices.push_back(it->second);
    return !devices.empty();
}

MediaRenderer::MediaRenderer(const UPnPDeviceDesc& desc)
    : Device(desc)
{
    if ((m = new Internal()) == 0) {
        LOGERR("MediaRenderer::MediaRenderer: out of memory" << endl);
        return;
    }
}

MediaRenderer::~MediaRenderer()
{
    delete m;
}

bool MediaRenderer::hasOpenHome()
{
    return ohpr() ? true : false;
}

RDCH MediaRenderer::rdc() 
{
    if (desc() == 0)
        return RDCH();

    RDCH rdcl = m->rdc.lock();
    if (rdcl)
        return rdcl;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (RenderingControl::isRDCService(it->serviceType)) {
            rdcl = RDCH(new RenderingControl(*desc(), *it));
            break;
        }
    }
    if (!rdcl)
        LOGDEB("MediaRenderer: RenderingControl service not found" << endl);
    m->rdc = rdcl;
    return rdcl;
}

AVTH MediaRenderer::avt() 
{
    AVTH avtl = m->avt.lock();
    if (avtl)
        return avtl;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (AVTransport::isAVTService(it->serviceType)) {
            avtl = AVTH(new AVTransport(*desc(), *it));
            break;
        }
    }
    if (!avtl)
        LOGDEB("MediaRenderer: AVTransport service not found" << endl);
    m->avt = avtl;
    return avtl;
}

OHPRH MediaRenderer::ohpr() 
{
    OHPRH ohprl = m->ohpr.lock();
    if (ohprl)
        return ohprl;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHProduct::isOHPrService(it->serviceType)) {
            ohprl = OHPRH(new OHProduct(*desc(), *it));
            break;
        }
    }
    if (!ohprl)
        LOGDEB("MediaRenderer: OHProduct service not found" << endl);
    m->ohpr = ohprl;
    return ohprl;
}

OHPLH MediaRenderer::ohpl() 
{
    OHPLH ohpll = m->ohpl.lock();
    if (ohpll)
        return ohpll;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHPlaylist::isOHPlService(it->serviceType)) {
            ohpll = OHPLH(new OHPlaylist(*desc(), *it));
            break;
        }
    }
    if (!ohpll)
        LOGDEB("MediaRenderer: OHPlaylist service not found" << endl);
    m->ohpl = ohpll;
    return ohpll;
}

OHRCH MediaRenderer::ohrc() 
{
    OHRCH ohrcl = m->ohrc.lock();
    if (ohrcl)
        return ohrcl;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHReceiver::isOHRcService(it->serviceType)) {
            ohrcl = OHRCH(new OHReceiver(*desc(), *it));
            break;
        }
    }
    if (!ohrcl)
        LOGDEB("MediaRenderer: OHReceiver service not found" << endl);
    m->ohrc = ohrcl;
    return ohrcl;
}

OHRDH MediaRenderer::ohrd() 
{
    OHRDH handle = m->ohrd.lock();
    if (handle)
        return handle;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHRadio::isOHRdService(it->serviceType)) {
            handle = OHRDH(new OHRadio(*desc(), *it));
            break;
        }
    }
    if (!handle)
        LOGDEB("MediaRenderer: OHRadio service not found" << endl);
    m->ohrd = handle;
    return handle;
}

OHIFH MediaRenderer::ohif() 
{
    OHIFH handle = m->ohif.lock();
    if (handle)
        return handle;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHInfo::isOHInfoService(it->serviceType)) {
            handle = OHIFH(new OHInfo(*desc(), *it));
            break;
        }
    }
    if (!handle)
        LOGDEB("MediaRenderer: OHInfo service not found" << endl);
    m->ohif = handle;
    return handle;
}

OHSNH MediaRenderer::ohsn() 
{
    OHSNH handle = m->ohsn.lock();
    if (handle)
        return handle;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHSender::isOHSenderService(it->serviceType)) {
            handle = OHSNH(new OHSender(*desc(), *it));
            break;
        }
    }
    if (!handle)
        LOGDEB("MediaRenderer: OHSender service not found" << endl);
    m->ohsn = handle;
    return handle;
}

OHTMH MediaRenderer::ohtm() 
{
    OHTMH ohtml = m->ohtm.lock();
    if (ohtml)
        return ohtml;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHTime::isOHTMService(it->serviceType)) {
            ohtml = OHTMH(new OHTime(*desc(), *it));
            break;
        }
    }
    if (!ohtml)
        LOGDEB("MediaRenderer: OHTime service not found" << endl);
    m->ohtm = ohtml;
    return ohtml;
}

OHVLH MediaRenderer::ohvl() 
{
    OHVLH ohvll = m->ohvl.lock();
    if (ohvll)
        return ohvll;
    for (vector<UPnPServiceDesc>::const_iterator it = desc()->services.begin();
         it != desc()->services.end();it++) {
        if (OHVolume::isOHVLService(it->serviceType)) {
            ohvll = OHVLH(new OHVolume(*desc(), *it));
            break;
        }
    }
    if (!ohvll)
        LOGDEB("MediaRenderer: OHVolume service not found" << endl);
    m->ohvl = ohvll;
    return ohvll;
}

}
