//
// Created by neo on 2020/4/22.
//

#ifndef ATOMGRAPHICS_RESOURCEREQUEST_H
#define ATOMGRAPHICS_RESOURCEREQUEST_H

#include <string>

namespace AtomGraphics {

class ResourceRequest {

public:

    ResourceRequest(const std::string &url) : m_url(url) {}

    ResourceRequest(const ResourceRequest &other) {
        m_url = other.m_url;
    }

    const std::string &url() const {
        return m_url;
    }

private:
    std::string m_url;

};

}

#endif //ATOMGRAPHICS_RESOURCEREQUEST_H
