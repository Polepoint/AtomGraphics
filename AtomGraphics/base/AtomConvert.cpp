//
// Created by woo on 2018/4/27.
// Copyright (c) 2018 neo. All rights reserved.
//
#include "AtomConvert.h"
#include <ctype.h>
#include <map>
#include <cstdlib>
namespace AtomGraphics {
    Color4F StrValueToColor4F(const char *value) {
        static std::map<std::string, Color4F> colorMap;
        if (colorMap.empty()) {
            colorMap.insert(std::pair<std::string, Color4F>(
                    "black", Color4F(0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair<std::string, Color4F>(
                    "silver", Color4F(192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f)));
            colorMap.insert(std::pair<std::string, Color4F>(
                    "gray", Color4F(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "white", Color4F(255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "maroon", Color4F(128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "red", Color4F(255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "purple", Color4F(128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "fuchsia", Color4F(255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "green", Color4F(0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lime", Color4F(0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "olive", Color4F(128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "yellow", Color4F(255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "navy", Color4F(0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "blue", Color4F(0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "teal", Color4F(0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "aqua", Color4F(0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "aliceblue", Color4F(240.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "antiquewhite",
                    Color4F(250.0f / 255, 235.0f / 255, 215.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "aqua", Color4F(0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "aquamarine", Color4F(127.0f / 255, 255.0f / 255, 212.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "azure", Color4F(240.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "beige", Color4F(245.0f / 255, 245.0f / 255, 220.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "bisque", Color4F(255.0f / 255, 228.0f / 255, 196.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "black", Color4F(0.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "blanchedalmond",
                    Color4F(255.0f / 255, 235.0f / 255, 205.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "blue", Color4F(0.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "blueviolet", Color4F(138.0f / 255, 43.0f / 255, 226.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "brown", Color4F(165.0f / 255, 42.0f / 255, 42.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "burlywood", Color4F(222.0f / 255, 184.0f / 255, 135.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "cadetblue", Color4F(95.0f / 255, 158.0f / 255, 160.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "chartreuse", Color4F(127.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "chocolate", Color4F(210.0f / 255, 105.0f / 255, 30.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "coral", Color4F(255.0f / 255, 127.0f / 255, 80.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "cornflowerblue",
                    Color4F(100.0f / 255, 149.0f / 255, 237.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "cornsilk", Color4F(255.0f / 255, 248.0f / 255, 220.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "crimson", Color4F(220.0f / 255, 20.0f / 255, 60.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "cyan", Color4F(0.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkblue", Color4F(0.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkcyan", Color4F(0.0f / 255, 139.0f / 255, 139.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkgoldenrod",
                    Color4F(184.0f / 255, 134.0f / 255, 11.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkgray", Color4F(169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkgreen", Color4F(0.0f / 255, 100.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkgrey", Color4F(169.0f / 255, 169.0f / 255, 169.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkkhaki", Color4F(189.0f / 255, 183.0f / 255, 107.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkmagenta", Color4F(139.0f / 255, 0.0f / 255, 139.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkolivegreen",
                    Color4F(85.0f / 255, 107.0f / 255, 47.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkorange", Color4F(255.0f / 255, 140.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkorchid", Color4F(153.0f / 255, 50.0f / 255, 204.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkred", Color4F(139.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darksalmon", Color4F(233.0f / 255, 150.0f / 255, 122.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkseagreen",
                    Color4F(143.0f / 255, 188.0f / 255, 143.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkslateblue", Color4F(72.0f / 255, 61.0f / 255, 139.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkslategray", Color4F(47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkslategrey", Color4F(47.0f / 255, 79.0f / 255, 79.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkturquoise", Color4F(0.0f / 255, 206.0f / 255, 209.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "darkviolet", Color4F(148.0f / 255, 0.0f / 255, 211.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "deeppink", Color4F(255.0f / 255, 20.0f / 255, 147.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "deepskyblue", Color4F(0.0f / 255, 191.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "dimgray", Color4F(105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "dimgrey", Color4F(105.0f / 255, 105.0f / 255, 105.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "dodgerblue", Color4F(30.0f / 255, 144.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "firebrick", Color4F(178.0f / 255, 34.0f / 255, 34.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "floralwhite", Color4F(255.0f / 255, 250.0f / 255, 240.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "forestgreen", Color4F(34.0f / 255, 139.0f / 255, 34.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "fuchsia", Color4F(255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "gainsboro", Color4F(220.0f / 255, 220.0f / 255, 220.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "ghostwhite", Color4F(248.0f / 255, 248.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "gold", Color4F(255.0f / 255, 215.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "goldenrod", Color4F(218.0f / 255, 165.0f / 255, 32.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "gray", Color4F(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "green", Color4F(0.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "greenyellow", Color4F(173.0f / 255, 255.0f / 255, 47.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "grey", Color4F(128.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "honeydew", Color4F(240.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "hotpink", Color4F(255.0f / 255, 105.0f / 255, 180.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "indianred", Color4F(205.0f / 255, 92.0f / 255, 92.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "indigo", Color4F(75.0f / 255, 0.0f / 255, 130.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "ivory", Color4F(255.0f / 255, 255.0f / 255, 240.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "khaki", Color4F(240.0f / 255, 230.0f / 255, 140.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lavender", Color4F(230.0f / 255, 230.0f / 255, 250.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lavenderblush",
                    Color4F(255.0f / 255, 240.0f / 255, 245.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lawngreen", Color4F(124.0f / 255, 252.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lemonchiffon",
                    Color4F(255.0f / 255, 250.0f / 255, 205.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightblue", Color4F(173.0f / 255, 216.0f / 255, 230.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightcoral", Color4F(240.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightcyan", Color4F(224.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightgoldenrodyellow",
                    Color4F(250.0f / 255, 250.0f / 255, 210.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightgray", Color4F(211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightgreen", Color4F(144.0f / 255, 238.0f / 255, 144.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightgrey", Color4F(211.0f / 255, 211.0f / 255, 211.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightpink", Color4F(255.0f / 255, 182.0f / 255, 193.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightsalmon", Color4F(255.0f / 255, 160.0f / 255, 122.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightseagreen",
                    Color4F(32.0f / 255, 178.0f / 255, 170.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightskyblue",
                    Color4F(135.0f / 255, 206.0f / 255, 250.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightslategray",
                    Color4F(119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightslategrey",
                    Color4F(119.0f / 255, 136.0f / 255, 153.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightsteelblue",
                    Color4F(176.0f / 255, 196.0f / 255, 222.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lightyellow", Color4F(255.0f / 255, 255.0f / 255, 224.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "lime", Color4F(0.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "limegreen", Color4F(50.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "linen", Color4F(250.0f / 255, 240.0f / 255, 230.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "magenta", Color4F(255.0f / 255, 0.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "maroon", Color4F(128.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumaquamarine",
                    Color4F(102.0f / 255, 205.0f / 255, 170.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumblue", Color4F(0.0f / 255, 0.0f / 255, 205.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumorchid", Color4F(186.0f / 255, 85.0f / 255, 211.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumpurple",
                    Color4F(147.0f / 255, 112.0f / 255, 219.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumseagreen",
                    Color4F(60.0f / 255, 179.0f / 255, 113.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumslateblue",
                    Color4F(123.0f / 255, 104.0f / 255, 238.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumspringgreen",
                    Color4F(0.0f / 255, 250.0f / 255, 154.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumturquoise",
                    Color4F(72.0f / 255, 209.0f / 255, 204.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mediumvioletred",
                    Color4F(199.0f / 255, 21.0f / 255, 133.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "midnightblue", Color4F(25.0f / 255, 25.0f / 255, 112.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mintcream", Color4F(245.0f / 255, 255.0f / 255, 250.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "mistyrose", Color4F(255.0f / 255, 228.0f / 255, 225.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "moccasin", Color4F(255.0f / 255, 228.0f / 255, 181.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "navajowhite", Color4F(255.0f / 255, 222.0f / 255, 173.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "navy", Color4F(0.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "oldlace", Color4F(253.0f / 255, 245.0f / 255, 230.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "olive", Color4F(128.0f / 255, 128.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "olivedrab", Color4F(107.0f / 255, 142.0f / 255, 35.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "orange", Color4F(255.0f / 255, 165.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "orangered", Color4F(255.0f / 255, 69.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "orchid", Color4F(218.0f / 255, 112.0f / 255, 214.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "palegoldenrod",
                    Color4F(238.0f / 255, 232.0f / 255, 170.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "palegreen", Color4F(152.0f / 255, 251.0f / 255, 152.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "paleturquoise",
                    Color4F(175.0f / 255, 238.0f / 255, 238.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "palevioletred",
                    Color4F(219.0f / 255, 112.0f / 255, 147.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "papayawhip", Color4F(255.0f / 255, 239.0f / 255, 213.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "peachpuff", Color4F(255.0f / 255, 218.0f / 255, 185.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "peru", Color4F(205.0f / 255, 133.0f / 255, 63.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "pink", Color4F(255.0f / 255, 192.0f / 255, 203.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "plum", Color4F(221.0f / 255, 160.0f / 255, 221.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "powderblue", Color4F(176.0f / 255, 224.0f / 255, 230.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "purple", Color4F(128.0f / 255, 0.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "red", Color4F(255.0f / 255, 0.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "rosybrown", Color4F(188.0f / 255, 143.0f / 255, 143.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "royalblue", Color4F(65.0f / 255, 105.0f / 255, 225.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "saddlebrown", Color4F(139.0f / 255, 69.0f / 255, 19.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "salmon", Color4F(250.0f / 255, 128.0f / 255, 114.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "sandybrown", Color4F(244.0f / 255, 164.0f / 255, 96.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "seagreen", Color4F(46.0f / 255, 139.0f / 255, 87.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "seashell", Color4F(255.0f / 255, 245.0f / 255, 238.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "sienna", Color4F(160.0f / 255, 82.0f / 255, 45.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "silver", Color4F(192.0f / 255, 192.0f / 255, 192.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "skyblue", Color4F(135.0f / 255, 206.0f / 255, 235.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "slateblue", Color4F(106.0f / 255, 90.0f / 255, 205.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "slategray", Color4F(112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "slategrey", Color4F(112.0f / 255, 128.0f / 255, 144.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "snow", Color4F(255.0f / 255, 250.0f / 255, 250.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "springgreen", Color4F(0.0f / 255, 255.0f / 255, 127.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "steelblue", Color4F(70.0f / 255, 130.0f / 255, 180.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "tan", Color4F(210.0f / 255, 180.0f / 255, 140.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "teal", Color4F(0.0f / 255, 128.0f / 255, 128.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "thistle", Color4F(216.0f / 255, 191.0f / 255, 216.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "tomato", Color4F(255.0f / 255, 99.0f / 255, 71.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "turquoise", Color4F(64.0f / 255, 224.0f / 255, 208.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "violet", Color4F(238.0f / 255, 130.0f / 255, 238.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "wheat", Color4F(245.0f / 255, 222.0f / 255, 179.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "white", Color4F(255.0f / 255, 255.0f / 255, 255.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "whitesmoke", Color4F(245.0f / 255, 245.0f / 255, 245.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "yellow", Color4F(255.0f / 255, 255.0f / 255, 0.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "yellowgreen", Color4F(154.0f / 255, 205.0f / 255, 50.0f / 255, 1.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "transparent", Color4F(0, 0, 0, 0.0f)));
            colorMap.insert(std::pair< std::string, Color4F >(
                    "transparent_white", Color4F(1.f, 1.f, 1.f, 0.f)));
        }

        Color4F c = Color4F(0.0f, 0.0f, 0.0f, 1.0f);
        if (value == nullptr) {
            return c;
        }
        std::map<std::string, Color4F>::const_iterator iter =
                colorMap.find(value);
        if (iter != colorMap.end()) {
            return iter->second;
        }
        int length = (int) strlen(value);
        char str[] = "ffffff";
        if (length == 4) {// #f0f format
            str[0] = str[1] = value[3];
            str[2] = str[3] = value[2];
            str[4] = str[5] = value[1];
            unsigned int hex =
                    (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
            c = Color4F((hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                    (hex >> 16) / 255.0f, 1.0);
        }
        else if (length == 7) {// #ff00ff format
            str[0] = value[5];
            str[1] = value[6];
            str[2] = value[3];
            str[3] = value[4];
            str[4] = value[1];
            str[5] = value[2];
            unsigned int hex =
                    (unsigned int) (0x00000000 | strtol(str, nullptr, 16));
            c = Color4F((hex & 0xff) / 255.0f, ((hex & 0xffff) >> 8) / 255.0f,
                    (hex >> 16) / 255.0f, 1.0);
        }
        else {// assume rgb(255,0,255) or rgba(255,0,255,0.5) format
            int current = 0;
            float components[4] = {0, 0, 0, 1.f};
            for (int i = 4; i < length && current < 4; i++) {
                if (current == 3) {
                    // If we have an alpha component, copy the rest of the wide
                    // string into a char array and use atof() to parse it.
                    char alpha[8] = {0, 0, 0, 0, 0, 0, 0, 0};
                    for (int j = 0; i + j < length - 1 && j < 7; j++) {
                        alpha[j] = value[i + j];
                    }
                    components[current] = (float) atof(alpha);
                    current++;
                } else if (isdigit(value[i])) {
                    components[current] =
                            (components[current] * 10 + (value[i] - '0'));
                } else if (value[i] == ',' || value[i] == ')') {
                    components[current] /= 255.0f;
                    current++;
                }
            }
            c = Color4F(components[0], components[1], components[2], components[3]);
        }
        return c;
    }

    double floatByPrecision(double number , int precision){
        float step = static_cast<float>(pow(10, precision));
        //round四舍五入保留，会有分块填充显示多余边界线的问题，精度保留暂先选用进一的方式
        int tmpNum = (int)ceil(number * step);
        double result = (double)tmpNum/step;
        return result;
    }
}

