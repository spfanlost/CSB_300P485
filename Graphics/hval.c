/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.28.                           *
*        Compiled Jan 30 2015, 16:40:04                              *
*                                                                    *
*        (c) 1998 - 2015 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: hval                                                  *
* Dimensions:  32 * 32                                               *
* NumColors:   65536 colors + 8 bit alpha channel                    *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmhval;

static GUI_CONST_STORAGE unsigned char _achval[] = {
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFE,0xEF,0x7D, 0xFE,0xDE,0xFB, 0xFA,0xDF,0x1C, 0xF5,0xDE,0xFB, 0xF3,0xDE,0xDB, 0xEB,0xDE,0xFB, 0xE4,0xDE,0xFB, 0xE1,0xD6,0xDB, 0xD2,0x84,0x52, 0xBE,0x73,0xD0, 0xAD,0x6B,0x6E, 0xA5,0x6B,0x6E, 
        0xE5,0x94,0xD3, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFE,0xE7,0x3D, 0xFA,0x9D,0x15, 0xF5,0x9D,0x15, 0xE5,0xA5,0x35, 
        0xDA,0x94,0xB3, 0xCD,0x9C,0xF4, 0xB2,0x9C,0xF4, 0x96,0x6B,0x8F, 0x8A,0x4A,0x6A, 0x75,0x52,0xCC, 0x5B,0x4A,0x8B, 0x53,0x42,0x4A, 0x3E,0x4A,0x6B, 0x2E,0x42,0x4A, 0x29,0x42,0x4A, 0x19,0x42,0x2A, 0x0C,0x42,0x09, 0x05,0x3A,0x09, 0x0E,0x3A,0x09, 
        0xC2,0x73,0xAF, 0xFF,0xFF,0xFF,
  0xFE,0xE7,0x3C, 0xEE,0xBD,0xF8, 0xDD,0xBD,0xD7, 0xD6,0xB5,0xB7, 0xC9,0xB5,0x96, 0xB7,0x6B,0x4F, 0xAE,0x4A,0x6B, 0x98,0x5A,0xED, 0x80,0x52,0x8B, 0x75,0x4A,0x6B, 0x5C,0x52,0xAC, 0x3F,0x4A,0x8B, 0x2C,0x4A,0x4A, 0x23,0x4A,0x6A, 0x11,0x4A,0x6A, 
        0x0A,0x4A,0x4A, 0x06,0x4A,0x4A, 0x03,0x4A,0x4A, 0x02,0x4A,0x4A, 0x02,0x42,0x4A, 0x01,0x42,0x4A, 0x01,0x42,0x2A, 0x01,0x42,0x2A, 0x00,0x42,0x2A, 0x00,0x42,0x29, 0x00,0x42,0x4A, 0x00,0x42,0x09, 0x00,0x42,0x09, 0x00,0x3A,0x09, 0x0D,0x3A,0x09, 
        0xC3,0x73,0xAF, 0xFF,0xFF,0xFF,
  0xEF,0xBD,0xF8, 0x5D,0x4A,0x4A, 0x27,0x42,0x09, 0x22,0x42,0x09, 0x1A,0x42,0x29, 0x11,0x42,0x09, 0x0E,0x42,0x2A, 0x08,0x42,0x4A, 0x03,0x42,0x29, 0x02,0x42,0x4A, 0x01,0x4A,0x4A, 0x01,0x42,0x4A, 0x00,0x42,0x4A, 0x01,0x4A,0x4A, 0x01,0x4A,0x4A, 
        0x01,0x4A,0x4A, 0x00,0x4A,0x4A, 0x01,0x4A,0x6A, 0x01,0x4A,0x6A, 0x01,0x4A,0x4A, 0x00,0x4A,0x4A, 0x01,0x4A,0x6A, 0x01,0x4A,0x6A, 0x01,0x4A,0x6A, 0x00,0x42,0x2A, 0x01,0x4A,0x6A, 0x01,0x4A,0x6A, 0x01,0x42,0x4A, 0x00,0x3A,0x09, 0x0D,0x42,0x29, 
        0xC3,0x73,0xD0, 0xFF,0xFF,0xFF,
  0xEA,0xBD,0xD7, 0x33,0x42,0x29, 0x01,0x39,0xE8, 0x01,0x42,0x09, 0x00,0x42,0x29, 0x01,0x42,0x29, 0x01,0x4A,0x4A, 0x01,0x42,0x4A, 0x00,0x4A,0x6A, 0x01,0x4A,0x8B, 0x01,0x52,0xAB, 0x01,0x4A,0x6A, 0x00,0x4A,0x4A, 0x01,0x52,0x8B, 0x01,0x52,0xAB, 
        0x01,0x4A,0x6B, 0x00,0x4A,0x6A, 0x01,0x52,0xAB, 0x01,0x4A,0x8B, 0x01,0x4A,0x6A, 0x00,0x4A,0x6A, 0x01,0x4A,0x6B, 0x01,0x4A,0x6B, 0x01,0x4A,0x4A, 0x00,0x42,0x2A, 0x01,0x42,0x4A, 0x01,0x42,0x29, 0x01,0x3A,0x09, 0x00,0x3A,0x09, 0x0D,0x3A,0xEB, 
        0xC3,0x74,0x51, 0xFF,0xFF,0xFF,
  0xEB,0xBD,0xF8, 0x35,0x4A,0x6A, 0x01,0x42,0x29, 0x01,0x42,0x29, 0x00,0x4A,0x4A, 0x01,0x4A,0x4A, 0x01,0x4A,0x4A, 0x01,0x42,0x4A, 0x00,0x4A,0x4A, 0x01,0x4A,0x4A, 0x01,0x4A,0x6A, 0x01,0x4A,0x4A, 0x00,0x4A,0x4A, 0x01,0x4A,0x4A, 0x01,0x4A,0x4A, 
        0x01,0x4A,0x4A, 0x00,0x4A,0x4A, 0x01,0x4A,0x4A, 0x01,0x4A,0x4A, 0x01,0x42,0x4A, 0x00,0x42,0x4A, 0x01,0x42,0x4A, 0x01,0x42,0x4A, 0x01,0x42,0x29, 0x00,0x42,0x29, 0x01,0x42,0x4A, 0x01,0x3A,0x09, 0x01,0x32,0xAA, 0x00,0x2C,0xEF, 0x0E,0x2D,0x91, 
        0xC4,0x74,0xF2, 0xFF,0xFF,0xFF,
  0xEC,0xBD,0xD7, 0x37,0x42,0x29, 0x00,0x39,0xE8, 0x00,0x42,0x09, 0x00,0x42,0x29, 0x00,0x42,0x29, 0x00,0x4A,0x4A, 0x00,0x4A,0x4A, 0x00,0x42,0x2A, 0x00,0x42,0x4A, 0x00,0x4A,0x6A, 0x00,0x4A,0x4A, 0x00,0x4A,0x6A, 0x00,0x4A,0x6A, 0x00,0x4A,0x6A, 
        0x00,0x4A,0x6A, 0x00,0x42,0x4A, 0x00,0x4A,0x4A, 0x00,0x4A,0x6B, 0x00,0x4A,0x8B, 0x00,0x4A,0x8B, 0x00,0x4A,0x6A, 0x00,0x42,0x2A, 0x00,0x42,0x4A, 0x00,0x42,0x4A, 0x00,0x4A,0xAB, 0x00,0x3B,0xCD, 0x00,0x2D,0x71, 0x00,0x2C,0x6E, 0x0F,0x3A,0x8A, 
        0xC6,0x7C,0x11, 0xFF,0xFF,0xFF,
  0xED,0xBD,0xF8, 0x38,0x4A,0x4A, 0x01,0x42,0x09, 0x01,0x42,0x09, 0x00,0x42,0x29, 0x01,0x4A,0x4A, 0x01,0x52,0x8B, 0x01,0x4A,0x8B, 0x00,0x4A,0x4A, 0x01,0x42,0x4A, 0x01,0x4A,0x6B, 0x01,0x4A,0x8B, 0x00,0x4A,0x6B, 0x01,0x4A,0x6A, 0x01,0x4A,0x6A, 
        0x01,0x42,0x4A, 0x00,0x42,0x4A, 0x01,0x42,0x4A, 0x01,0x42,0x4A, 0x01,0x4A,0x6A, 0x00,0x4A,0x6A, 0x01,0x42,0x4A, 0x01,0x42,0x29, 0x01,0x42,0x29, 0x00,0x3A,0x8A, 0x01,0x34,0xF0, 0x01,0x25,0x91, 0x01,0x33,0x2B, 0x00,0x31,0xC8, 0x11,0x31,0xA7, 
        0xC8,0x73,0xB0, 0xFF,0xFF,0xFF,
  0xEE,0xB5,0xD7, 0x39,0x42,0x29, 0x01,0x39,0xE8, 0x01,0x42,0x09, 0x00,0x42,0x09, 0x01,0x42,0x09, 0x01,0x42,0x4A, 0x01,0x42,0x4A, 0x00,0x42,0x29, 0x01,0x42,0x4A, 0x01,0x42,0x4A, 0x01,0x42,0x29, 0x00,0x3A,0x8A, 0x01,0x34,0xD0, 0x01,0x34,0x6F, 
        0x01,0x3B,0x0C, 0x00,0x3A,0x6A, 0x01,0x3A,0x09, 0x01,0x42,0x29, 0x01,0x42,0x29, 0x00,0x42,0x29, 0x01,0x42,0x29, 0x01,0x3A,0x29, 0x01,0x33,0xAD, 0x00,0x2D,0xB2, 0x01,0x34,0x8F, 0x01,0x32,0x49, 0x01,0x31,0xA7, 0x00,0x29,0x66, 0x12,0x31,0xA7, 
        0xC9,0x7B,0xF0, 0xFF,0xFF,0xFF,
  0xEF,0xB5,0xD7, 0x3B,0x42,0x29, 0x01,0x39,0xE9, 0x01,0x42,0x09, 0x00,0x42,0x09, 0x01,0x42,0x29, 0x01,0x4A,0x6A, 0x01,0x4A,0x6A, 0x00,0x4A,0x6A, 0x01,0x4A,0x8B, 0x01,0x4A,0x8B, 0x01,0x4A,0x6A, 0x00,0x34,0x8F, 0x01,0x2C,0xEF, 0x01,0x34,0x6F, 
        0x01,0x2D,0x72, 0x00,0x2D,0x72, 0x01,0x34,0x2F, 0x01,0x3B,0x0C, 0x01,0x42,0x8A, 0x00,0x42,0x6A, 0x01,0x42,0xEC, 0x01,0x34,0xD0, 0x01,0x2D,0x71, 0x00,0x3B,0x8C, 0x01,0x42,0x6A, 0x01,0x31,0xA7, 0x01,0x31,0xA7, 0x00,0x39,0xE8, 0x13,0x42,0x09, 
        0xCB,0x7C,0x11, 0xFF,0xFF,0xFF,
  0xF0,0xB5,0xB7, 0x3C,0x42,0x09, 0x00,0x39,0xE9, 0x00,0x42,0x09, 0x00,0x42,0x09, 0x00,0x42,0x09, 0x00,0x4A,0x4A, 0x00,0x4A,0x4A, 0x00,0x42,0x4A, 0x00,0x42,0x4A, 0x00,0x42,0x4A, 0x00,0x3B,0x4C, 0x00,0x2D,0x30, 0x00,0x32,0x28, 0x00,0x31,0xC8, 
        0x00,0x3A,0x29, 0x00,0x33,0x0B, 0x00,0x2C,0x8F, 0x00,0x2D,0xB2, 0x00,0x2D,0xB2, 0x00,0x2C,0xB0, 0x00,0x2D,0x92, 0x00,0x2C,0x8E, 0x00,0x32,0x49, 0x00,0x31,0xA7, 0x00,0x31,0xA7, 0x00,0x31,0xA7, 0x00,0x39,0xE8, 0x00,0x3A,0x09, 0x14,0x42,0x09, 
        0xCD,0x7B,0xF1, 0xFF,0xFF,0xFF,
  0xF1,0xB5,0xB7, 0x3E,0x39,0xE9, 0x01,0x39,0xE8, 0x01,0x3A,0x09, 0x00,0x3A,0x09, 0x01,0x42,0x09, 0x01,0x42,0x4A, 0x01,0x42,0x2A, 0x00,0x42,0x29, 0x01,0x42,0x29, 0x01,0x3A,0xAA, 0x01,0x2D,0x50, 0x00,0x2A,0xA9, 0x01,0x29,0x86, 0x01,0x31,0x87, 
        0x01,0x31,0xA7, 0x00,0x31,0xC7, 0x01,0x39,0xE8, 0x01,0x3A,0x49, 0x01,0x32,0xEB, 0x00,0x34,0x2E, 0x01,0x33,0x0B, 0x01,0x31,0xE8, 0x01,0x31,0x87, 0x00,0x31,0xA7, 0x01,0x42,0x29, 0x01,0x42,0x09, 0x01,0x42,0x09, 0x00,0x42,0x09, 0x15,0x42,0x09, 
        0xCF,0x7B,0xF0, 0xFF,0xFF,0xFF,
  0xF2,0xB5,0x96, 0x3E,0x39,0xE8, 0x01,0x39,0xE8, 0x01,0x3A,0x09, 0x00,0x3A,0x09, 0x01,0x3A,0x09, 0x01,0x42,0x29, 0x01,0x42,0x29, 0x00,0x42,0x09, 0x01,0x3A,0x29, 0x01,0x2C,0x6E, 0x01,0x2B,0xCC, 0x00,0x29,0xA7, 0x01,0x39,0xE8, 0x01,0x39,0xE8, 
        0x01,0x31,0xC8, 0x00,0x31,0xA7, 0x01,0x31,0xA7, 0x01,0x31,0xA7, 0x01,0x31,0xC8, 0x00,0x39,0xE8, 0x01,0x31,0xC8, 0x01,0x31,0x87, 0x01,0x31,0xC8, 0x00,0x3A,0x09, 0x01,0x4A,0x6A, 0x01,0x42,0x29, 0x01,0x42,0x29, 0x00,0x42,0x29, 0x17,0x42,0x29, 
        0xD0,0x73,0xD0, 0xFF,0xFF,0xFF,
  0xF3,0xB5,0x96, 0x40,0x39,0xE8, 0x01,0x39,0xE8, 0x01,0x39,0xE8, 0x00,0x39,0xE8, 0x01,0x39,0xE8, 0x01,0x42,0x29, 0x01,0x42,0x09, 0x00,0x39,0xE8, 0x01,0x33,0x6C, 0x01,0x24,0x8E, 0x01,0x29,0xA7, 0x00,0x31,0xC8, 0x01,0x42,0x09, 0x01,0x42,0x09, 
        0x01,0x42,0x09, 0x00,0x3A,0x09, 0x01,0x39,0xE8, 0x01,0x31,0xC7, 0x01,0x31,0xA7, 0x00,0x31,0x86, 0x01,0x31,0xA7, 0x01,0x39,0xE8, 0x01,0x42,0x09, 0x00,0x42,0x09, 0x01,0x4A,0x6A, 0x01,0x42,0x09, 0x01,0x3A,0x09, 0x00,0x3A,0x09, 0x18,0x3A,0x09, 
        0xD2,0x73,0xAF, 0xFF,0xFF,0xFF,
  0xF3,0xAD,0x96, 0x41,0x39,0xC8, 0x00,0x39,0xC8, 0x00,0x39,0xE8, 0x00,0x39,0xE8, 0x00,0x3A,0x09, 0x00,0x3A,0xCB, 0x00,0x3A,0x4A, 0x00,0x3A,0xAA, 0x00,0x24,0xCE, 0x00,0x2A,0x69, 0x00,0x31,0xA7, 0x00,0x3A,0x09, 0x00,0x42,0x09, 0x00,0x42,0x29, 
        0x00,0x42,0x2A, 0x00,0x42,0x4A, 0x00,0x4A,0x4A, 0x00,0x42,0x4A, 0x00,0x42,0x29, 0x00,0x52,0xAB, 0x00,0x8C,0x72, 0x00,0xBD,0xD7, 0x00,0xCE,0x79, 0x00,0xCE,0x79, 0x00,0xC6,0x18, 0x00,0x94,0xB3, 0x00,0x5A,0xEC, 0x00,0x42,0x09, 0x19,0x3A,0x09, 
        0xD3,0x6B,0x8F, 0xFF,0xFF,0xFF,
  0xF3,0xAD,0x76, 0x41,0x31,0xC7, 0x01,0x39,0xC8, 0x01,0x39,0xC8, 0x00,0x31,0xE8, 0x01,0x2B,0x4B, 0x01,0x25,0x50, 0x01,0x2C,0x0D, 0x00,0x24,0x2D, 0x01,0x22,0xC9, 0x01,0x29,0x86, 0x01,0x39,0xC8, 0x00,0x3A,0x09, 0x01,0x3A,0x09, 0x01,0x42,0x09, 
        0x01,0x42,0x09, 0x00,0x42,0x29, 0x01,0x42,0x2A, 0x01,0x42,0x4A, 0x01,0x73,0x8F, 0x00,0xC6,0x38, 0x01,0xB5,0xF6, 0x01,0x85,0x32, 0x01,0x65,0x51, 0x00,0x65,0xB2, 0x01,0x7D,0xD3, 0x01,0xAE,0x37, 0x01,0xC6,0x38, 0x00,0x84,0x31, 0x1A,0x42,0x29, 
        0xD5,0x6B,0x4E, 0xFF,0xFF,0xFF,
  0xF4,0xAD,0x76, 0x41,0x39,0xE8, 0x01,0x39,0xC8, 0x01,0x31,0xE8, 0x00,0x2B,0xAC, 0x01,0x23,0xEC, 0x01,0x32,0x69, 0x01,0x24,0xAE, 0x00,0x23,0xCB, 0x01,0x29,0x66, 0x01,0x31,0xC7, 0x01,0x3A,0x09, 0x00,0x3A,0x09, 0x01,0x42,0x09, 0x01,0x3A,0x09, 
        0x01,0x3A,0x09, 0x00,0x3A,0x09, 0x01,0x42,0x09, 0x01,0x73,0xCF, 0x01,0xCE,0x79, 0x00,0x95,0x12, 0x01,0x95,0xD4, 0x01,0x96,0xB6, 0x01,0x8E,0xF7, 0x00,0x8E,0xF7, 0x01,0x8E,0xF7, 0x01,0x9E,0xB7, 0x01,0x9E,0x56, 0x00,0xCE,0xB9, 0x15,0x84,0x51, 
        0xD3,0x63,0x2D, 0xFF,0xFF,0xFF,
  0xF4,0xAD,0x55, 0x41,0x31,0xA7, 0x01,0x2A,0x28, 0x01,0x23,0xAB, 0x00,0x22,0xE9, 0x01,0x29,0x86, 0x01,0x29,0x66, 0x01,0x29,0xA7, 0x00,0x29,0x86, 0x01,0x29,0x86, 0x01,0x39,0xE8, 0x01,0x3A,0x09, 0x00,0x42,0x09, 0x01,0x42,0x29, 0x01,0x42,0x09, 
        0x01,0x39,0xE9, 0x00,0x39,0xE9, 0x01,0x5A,0xEC, 0x01,0xC6,0x38, 0x01,0x7C,0xAF, 0x00,0x7D,0xD2, 0x01,0xF7,0xDE, 0x01,0xFF,0xFF, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x01,0xFF,0xFF, 0x01,0xFF,0xFF, 0x01,0xB7,0x19, 0x00,0x65,0xD1, 0x04,0xCE,0x99, 
        0x9C,0xAD,0x75, 0xFF,0xEF,0x9E,
  0xDD,0x4D,0x71, 0x39,0x2A,0xA9, 0x00,0x1B,0x8B, 0x00,0x22,0x47, 0x00,0x21,0x45, 0x00,0x21,0x25, 0x00,0x29,0x66, 0x00,0x29,0x45, 0x00,0x29,0x45, 0x00,0x31,0xA7, 0x00,0x31,0xC7, 0x00,0x31,0xC8, 0x00,0x39,0xC8, 0x00,0x39,0xC8, 0x00,0x39,0xC8, 
        0x00,0x39,0xC8, 0x00,0x39,0xE8, 0x00,0x94,0x92, 0x00,0xAD,0x94, 0x00,0x3C,0x6A, 0x00,0x3D,0xCF, 0x00,0x6E,0x73, 0x00,0x6E,0x74, 0x00,0x95,0xD3, 0x00,0xAE,0xB7, 0x00,0x76,0x74, 0x00,0x6E,0x54, 0x00,0x4D,0xF1, 0x00,0x2D,0x8E, 0x00,0x9E,0x56, 
        0x41,0xC6,0x18, 0xEA,0xD6,0xBA,
  0x8F,0x1C,0xEE, 0x1D,0x1B,0x4A, 0x01,0x21,0xA6, 0x01,0x21,0x05, 0x00,0x21,0x25, 0x01,0x29,0x86, 0x01,0x31,0xA7, 0x01,0x29,0x66, 0x00,0x31,0xA7, 0x01,0x39,0xC8, 0x01,0x39,0xC8, 0x01,0x31,0xA7, 0x00,0x31,0xA7, 0x01,0x39,0xE8, 0x01,0x39,0xC8, 
        0x01,0x31,0xA8, 0x00,0x42,0x09, 0x01,0xC6,0x39, 0x01,0x6C,0x6D, 0x01,0x2D,0x4D, 0x00,0x2D,0xCF, 0x01,0x2D,0x8E, 0x01,0x4C,0xED, 0x01,0xDF,0x1A, 0x00,0xEF,0xBD, 0x01,0x86,0x54, 0x01,0x2D,0xAF, 0x01,0x2D,0xAF, 0x00,0x2D,0xAF, 0x01,0x5D,0xB0, 
        0x11,0xCE,0x99, 0xBA,0xDE,0xFB,
  0xE3,0x7C,0x10, 0x3A,0x21,0x45, 0x01,0x21,0x04, 0x01,0x21,0x05, 0x00,0x29,0x45, 0x01,0x29,0x46, 0x01,0x29,0x66, 0x01,0x29,0x66, 0x00,0x29,0x66, 0x01,0x29,0x66, 0x01,0x29,0x86, 0x01,0x29,0x86, 0x00,0x31,0x87, 0x01,0x31,0x87, 0x01,0x31,0x87, 
        0x01,0x31,0x87, 0x00,0x42,0x09, 0x01,0xD6,0x9A, 0x01,0x4C,0x0A, 0x01,0x2D,0x4D, 0x00,0x2D,0x6D, 0x01,0x5D,0x0D, 0x01,0xD6,0xF9, 0x01,0xFF,0xFF, 0x00,0xFF,0xFF, 0x01,0xEF,0xBD, 0x01,0x7E,0x33, 0x01,0x35,0x8E, 0x00,0x2D,0x8E, 0x01,0x45,0x4E, 
        0x04,0xC6,0xB9, 0xA0,0xE7,0x1C,
  0x51,0x31,0xA7, 0x10,0x29,0x66, 0x01,0x29,0x66, 0x01,0x29,0x66, 0x00,0x29,0x86, 0x01,0x31,0x87, 0x01,0x29,0x86, 0x01,0x31,0xA7, 0x00,0x31,0x87, 0x01,0x29,0x66, 0x01,0x29,0x66, 0x01,0x29,0x86, 0x00,0x29,0x66, 0x01,0x29,0x66, 0x01,0x29,0x66, 
        0x01,0x29,0x66, 0x00,0x3A,0x08, 0x01,0xD6,0x9A, 0x01,0x4C,0x0A, 0x01,0x2D,0x2C, 0x00,0x54,0xCC, 0x01,0xCE,0xD9, 0x01,0xFF,0xDF, 0x01,0xFF,0xDF, 0x00,0xFF,0xDF, 0x01,0xF7,0xDE, 0x01,0xDF,0x5C, 0x01,0x75,0xF2, 0x00,0x2D,0x4D, 0x01,0x3D,0x2D, 
        0x01,0xC6,0xB9, 0x9C,0xE7,0x3C,
  0x0A,0x31,0xA7, 0x04,0x31,0xA7, 0x01,0x31,0x87, 0x00,0x31,0x87, 0x00,0x31,0x87, 0x00,0x31,0x87, 0x00,0x31,0xA7, 0x00,0x31,0xA7, 0x00,0x31,0xA7, 0x00,0x31,0xA7, 0x00,0x31,0xA7, 0x00,0x39,0xC8, 0x00,0x31,0xA7, 0x00,0x31,0xC7, 0x00,0x31,0xC8, 
        0x00,0x31,0x87, 0x00,0x42,0x29, 0x00,0xCE,0x79, 0x00,0x5C,0x2B, 0x00,0x34,0xAA, 0x00,0xB6,0x76, 0x00,0xF7,0xDE, 0x00,0xBE,0xB8, 0x00,0xDF,0x1A, 0x00,0xEF,0x9D, 0x00,0xAE,0x76, 0x00,0xE7,0x3C, 0x00,0xCE,0xB9, 0x00,0x4D,0x4E, 0x00,0x4D,0x0D, 
        0x07,0xCE,0xB9, 0xA6,0xDE,0xFB,
  0xC0,0x6B,0x6E, 0xB2,0x63,0x4D, 0x98,0x6B,0x4E, 0x73,0x5A,0xCC, 0x60,0x31,0xC8, 0x47,0x39,0xE8, 0x35,0x31,0xC8, 0x28,0x31,0xC8, 0x19,0x39,0xC8, 0x10,0x31,0xC8, 0x09,0x31,0xC8, 0x03,0x31,0xC8, 0x01,0x31,0xC8, 0x01,0x31,0xC8, 0x01,0x31,0xC8, 
        0x01,0x31,0xC8, 0x00,0x39,0xE8, 0x01,0xAD,0x76, 0x01,0x8C,0xF0, 0x01,0x34,0x8A, 0x00,0x55,0x6F, 0x01,0x96,0x35, 0x01,0x4D,0x2D, 0x01,0xC6,0x98, 0x00,0xE7,0x5C, 0x01,0x5D,0xB0, 0x01,0x7D,0xD2, 0x01,0x6D,0x91, 0x00,0x34,0xEC, 0x01,0x75,0xB1, 
        0x1D,0xC6,0x58, 0xCB,0xD6,0x9A,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFE,0xE7,0x3C, 0xFB,0xB5,0xD7, 0xF2,0xBD,0xD7, 0xE9,0xB5,0xD7, 0xDC,0xB5,0xD7, 0xC6,0x8C,0x52, 0xB4,0x4A,0x6A, 0x9F,0x4A,0x6A, 0x84,0x42,0x4A, 0x6F,0x42,0x29, 0x51,0x42,0x29, 0x41,0x3A,0x09, 
        0x24,0x3A,0x09, 0x10,0x39,0xE8, 0x09,0x73,0xAF, 0x02,0xBE,0x17, 0x01,0x4C,0xAC, 0x00,0x2C,0xEB, 0x01,0x2D,0x0C, 0x01,0x34,0xCB, 0x01,0xBE,0x57, 0x00,0xDF,0x1B, 0x01,0x55,0x6F, 0x01,0x2C,0xEC, 0x01,0x2C,0xCB, 0x00,0x34,0xCB, 0x01,0xC6,0xB8, 
        0x5C,0xA5,0x35, 0xF5,0xC6,0x59,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFE,0xCE,0x9A, 
        0xF2,0x9C,0xF4, 0xE0,0x94,0xD3, 0xCB,0x9C,0xF4, 0x48,0xC6,0x39, 0x02,0xB5,0xF6, 0x00,0x43,0xE9, 0x01,0x2C,0x28, 0x01,0x2C,0x49, 0x01,0x75,0x0F, 0x00,0x7D,0x50, 0x01,0x44,0x8B, 0x01,0x2C,0x49, 0x01,0x3C,0x8A, 0x00,0xA6,0x35, 0x24,0xC6,0x18, 
        0xCE,0xBD,0xF7, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xD7,0xC6,0x38, 0x4C,0xC6,0x38, 0x04,0xBE,0x37, 0x00,0x74,0xCE, 0x00,0x44,0x4A, 0x00,0x3C,0x49, 0x00,0x3C,0x49, 0x00,0x44,0x6A, 0x00,0x6D,0x2E, 0x02,0xB6,0x77, 0x1D,0xC6,0x38, 0xB0,0x8C,0x71, 
        0xFC,0xE7,0x5D, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xF7,0xBE, 0xE2,0xDE,0xDB, 0x6E,0xB5,0xD7, 0x16,0xCE,0x79, 0x02,0xC6,0x78, 0x01,0xB6,0x36, 0x01,0xB6,0x57, 0x01,0xC6,0xB8, 0x0A,0xCE,0x9A, 0x4B,0xAD,0x75, 0xBE,0xB5,0xB6, 0xFF,0xDE,0xFB, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xDE,0xDB, 0xD9,0xBE,0x18, 0x9A,0xCE,0x59, 0x75,0xD6,0xBA, 0x72,0xD6,0x9A, 0x87,0xC6,0x59, 0xC5,0xB5,0xB6, 0xF8,0xB5,0xD7, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,
  0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF, 
        0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF
};

GUI_CONST_STORAGE GUI_BITMAP bmhval = {
  32, // xSize
  32, // ySize
  96, // BytesPerLine
  24, // BitsPerPixel
  (unsigned char *)_achval,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPA565
};

/*************************** End of file ****************************/