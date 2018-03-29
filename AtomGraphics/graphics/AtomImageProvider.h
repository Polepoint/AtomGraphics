//
// Created by neo on 2018/3/28.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMIMAGEPROVIDER_H
#define ATOMIMAGEPROVIDER_H


class ImageProvider {

public:
    void *getData();

private:
    // bitmap?
    // file?
    // dynamic renderer?
    void *_data;

};


#endif //ATOMIMAGEPROVIDER_H
