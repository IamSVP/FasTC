/* FasTC
 * Copyright (c) 2014 University of North Carolina at Chapel Hill.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for educational, research, and non-profit purposes, without
 * fee, and without a written agreement is hereby granted, provided that the
 * above copyright notice, this paragraph, and the following four paragraphs
 * appear in all copies.
 *
 * Permission to incorporate this software into commercial products may be
 * obtained by contacting the authors or the Office of Technology Development
 * at the University of North Carolina at Chapel Hill <otd@unc.edu>.
 *
 * This software program and documentation are copyrighted by the University of
 * North Carolina at Chapel Hill. The software program and documentation are
 * supplied "as is," without any accompanying services from the University of
 * North Carolina at Chapel Hill or the authors. The University of North
 * Carolina at Chapel Hill and the authors do not warrant that the operation of
 * the program will be uninterrupted or error-free. The end-user understands
 * that the program was developed for research purposes and is advised not to
 * rely exclusively on the program for any reason.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL OR THE
 * AUTHORS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE USE OF
 * THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF NORTH CAROLINA
 * AT CHAPEL HILL OR THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL AND THE AUTHORS SPECIFICALLY
 * DISCLAIM ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND ANY 
 * STATUTORY WARRANTY OF NON-INFRINGEMENT. THE SOFTWARE PROVIDED HEREUNDER IS ON
 * AN "AS IS" BASIS, AND THE UNIVERSITY  OF NORTH CAROLINA AT CHAPEL HILL AND
 * THE AUTHORS HAVE NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, 
 * ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Please send all BUG REPORTS to <pavel@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Pavel Krajcevski
 * Dept of Computer Science
 * 201 S Columbia St
 * Frederick P. Brooks, Jr. Computer Science Bldg
 * Chapel Hill, NC 27599-3175
 * USA
 * 
 * <http://gamma.cs.unc.edu/FasTC/>
 */

#ifndef ASTCENCODER_INCLUDE_ASTCCOMPRESSOR_H_
#define ASTCENCODER_INCLUDE_ASTCCOMPRESSOR_H_

#include "FasTC/CompressionJob.h"


namespace ASTCC {

  // Only LDR currently, specialized to match the numbers of the modes in spec 
  // Table C.2.10
  enum ColorEndPointMode {
    eColorMode_zero= 0, // LDR Luminance, direct
    eColorMode_one= 1, // LDR Luminance, base+offset
    eColorMode_four = 4, // LDR Luminance+Alpha, direct
    eColorMode_five = 5, // LDR Luminanc+Alpha, base+offset
    eColorMode_six = 6, // LDR RGB, base+scale
    eColorMode_eight = 8, // LDR RGB, direct
    eColorMode_nine = 9, // LDR RGB, base+offset
    eColorMode_ten = 10, // LDR RGB, base+scale plust two A
    eColorMode_twelve = 12, // LDR RGBA, direct
    eColorMode_thirteen = 13 // LDR RGBA, base+offset
 
  };
  // [10-0] bits of the 128 bit stream Table C.2.8
  typedef struct _IndexModeData{

    uint8 m_D; //[10] Dual Plane mode [10]th bit
    uint8 m_H; // [9] High bit to set the precision of the weight range of the weight data
    uint8 m_B; //[8-7] or [10-7] width and height specification
    uint8 m_A; // [6-5] with and height specification
    uint8 m_R; // [4-0] bits
    uint16 m_BMF; // Block mode feild 11 bits   
     
  } IndexModeData;
  
    // [11-28] bits of 128 bit stream
  typedef struct _ConfigurationData {

    uint8 m_PartitionCount; // [12-11] Partition Count
    uint16 m_PartitionIndex; // [13-22] Partition Index or 0 bits for one parition
    uint8 m_CEM; // [28-23] Color Encoded Modes or at [13-16] for one partition
    uint8 m_ExtraCEM; // Present right below the TexelData
    uint32 m_NumExtraCEMBits; // Extra CEM bits is variable so need to store
    ColorEndPointMode m_ColorModes[4];
 
  } ConfigurationData;


  // Structure of bits of a block;
  typedef struct _BlockData{

    IndexModeData m_Index; 
    ConfigurationData m_Configuration; // 
    FasTC::Pixel m_ColorEndPoints[4][2]; // Pixel EndPoints 
    uint8 *m_EncodedColor; // Pointer to ColorEndpoint Data, it's of variable size
    uint8 m_TexelWeights[12][12]; // Pointer to Texel Weight Dta, it's of variable size    
    uint32 m_NumColorBits; // Total number of bits in the Color End points
    uint32 m_NumWeightBits; // Total number of bits in the Texel Weight Data
    uint32 m_WeightWidht, m_WeightHeight; // Dimensions of the Weight array 
    uint32 m_BlockSz[2]; // Block size 

  } BlockData;


  // Takes a stream of compressed ASTC data and decompresses it into R8G8B8A8
  // format. The block size must be specified in order to properly
  // decompress the data, but it is included in the format descriptor passed
  // by FasTC::DecompressionJob
  void Decompress(const FasTC::DecompressionJob &);

  void Compress(const FasTC::CompressionJob &);

}  // namespace ASTCC

#endif  // ASTCENCODER_INCLUDE_ASTCCOMPRESSOR_H_
