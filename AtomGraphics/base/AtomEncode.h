//
// Created by woo on 2018/4/25.
// Copyright (c) 2018 neo. All rights reserved.
//


namespace AtomGraphics
{
////////////////////////////base64 encoding////////////////////////////
    int Base64EncodeLen(int inputLen);
    int Base64EncodeBuf(char *szDest, const char *szSrc, int &iBufSize);
    int Base64DecodeBuf(char *szDest, const char *szSrc, int &iBufSize);
}