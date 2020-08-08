//
// Created by neo on 2020/4/26.
//

#ifndef ATOMGRAPHICS_URL_H
#define ATOMGRAPHICS_URL_H

#include <string>

namespace AtomGraphics {

class URL {

public:

    URL(const std::string &string) : m_string(string) {}

    const std::string &string() const {
        return m_string;
    }

    bool protocolIsData() const;

private:

    std::string m_string;

};


}


#endif //ATOMGRAPHICS_URL_H
