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

#include <string.h>                     // for strcmp
#include <upnp/upnp.h>                  // for UPNP_E_BAD_RESPONSE, etc
#include <ostream>                      // for endl
#include <string>                       // for string
#include <vector>                       // for vector

#include "libupnpp/control/ohproduct.hxx"
#include "libupnpp/expatmm.hxx"         // for inputRefXMLParser
#include "libupnpp/log.hxx"             // for LOGERR
#include "libupnpp/soaphelp.hxx"        // for SoapIncoming, etc
#include "libupnpp/upnpp_p.hxx"         // for stringToBool, trimstring

using namespace std;
using namespace UPnPP;

// SourceList XML format
// <SourceList>
//    <Source>
//        <Name>[user name for source]</Name>
//        <Type>[Type of the source from closed list of Supported types]</Type>
//        <Visible>[Boolean.]</Visible>
//    </sourcetag>
// </SourceList>
// - Playlist - the av.openhome.org:Playlist:1 service must be available
// - Radio - the av.openhome.org:Radio:1 service must be available
// - Receiver - the av.openhome.org:Receiver:1 service must be available
// - UpnpAv - the upnp.org:MediaRenderer:1 device must be available
// - NetAux - Specifies 3rd party, non OpenHome controllable, network
//   protocols such as AirPlay
// - Analog - Specifies an analog external input
// - Digital - Specifies a digital external input
// - Hdmi - Specifies a HDMI external input

namespace UPnPClient {
class OHSourceParser : public inputRefXMLParser {
public:
    OHSourceParser(const string& input, vector<OHProduct::Source>& sources)
        : inputRefXMLParser(input), m_sources(sources)
        {}

protected:
    virtual void StartElement(const XML_Char *name, const XML_Char **) {
        m_path.push_back(name);
    }
    virtual void EndElement(const XML_Char *name) {
        if (!strcmp(name, "Source")) {
            m_sources.push_back(m_tsrc);
            m_tsrc.clear();
        }
        m_path.pop_back();
    }
    virtual void CharacterData(const XML_Char *s, int len) {
        if (s == 0 || *s == 0)
            return;
        string str(s, len);
        trimstring(str);
        switch (m_path.back()[0]) {
        case 'N':
            if (!m_path.back().compare("Name"))
                m_tsrc.name = str;
            break;
        case 'T':
            if (!m_path.back().compare("Type"))
                m_tsrc.type = str;
            break;
        case 'V':
            if (!m_path.back().compare("Visible"))
                stringToBool(str, &m_tsrc.visible);
            break;
        }
    }

private:
    vector<OHProduct::Source>& m_sources;
    std::vector<std::string> m_path;
    OHProduct::Source m_tsrc;
};

const string OHProduct::SType("urn:av-openhome-org:service:Product:1");

// Check serviceType string (while walking the descriptions. We don't
// include a version in comparisons, as we are satisfied with version1
bool OHProduct::isOHPrService(const string& st)
{
    const string::size_type sz(SType.size()-2);
    return !SType.compare(0, sz, st, 0, sz);
}

int OHProduct::getSources(vector<Source>& sources)
{
    SoapOutgoing args(getServiceType(), "SourceXml");
    SoapIncoming data;
    int ret = runAction(args, data);
    if (ret != UPNP_E_SUCCESS) {
        return ret;
    }
    string sxml;
    if (!data.get("Value", &sxml)) {
        LOGERR("OHProduct:getSources: missing Value in response" << endl);
        return UPNP_E_BAD_RESPONSE;
    }
    OHSourceParser mparser(sxml, sources);
    if (!mparser.Parse())
        return UPNP_E_BAD_RESPONSE;

    return UPNP_E_SUCCESS;
}

int OHProduct::sourceIndex(int *index)
{
    string value;
    int ret;

    if ((ret = runSimpleGet("SourceIndex", "Value", &value)))
        return ret;

    *index = atoi(value.c_str());
    return 0;
}

int OHProduct::setSourceIndex(int index)
{
    return runSimpleAction("SetSourceIndex", "Value", index);
}


} // End namespace UPnPClient
