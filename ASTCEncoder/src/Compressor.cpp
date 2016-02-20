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


// Only LDR modes
// First 4x4 blocks, one partition, color mode-8 class-2, Textel array size 4x4 no scaling donw

#include "FasTC\ASTCCompressor.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <vector>

#include "Utils.h"
#include "IntegerEncoding.h"

#include "FasTC/TexCompTypes.h"
#include "FasTC/CompressionFormat.h"

#include "FasTC/Bits.h."
#include "FasTC/BitStream.h"
using FasTC::BitStream;
using FasTC::BitStreamReadOnly;

#include "FasTC/Pixel.h"

namespace ASTCC {
  
  uint32 vBlockSz; 
  uint32 vBlock[2];
  void GetBlock(uint32 Bx, uint32 By, uint32 PixelWidth, const uint32 *inPixels, uint32 *block){
    
    for(int i = 0; i < vBlock[1]; i++)
      memcpy(&block[vBlock[0] * i], &inPixels[Bx + (By + i) * PixelWidth], vBlock[0]); 
     
  } 
  void Compress(FasTC::CompressionJob &cj){
    
      const uint32 *inPixels = reinterpret_cast<const uint32 *>(cj.InBuf);
      // have to fix this tc.cpp to TexComp.cpp to get the correct format 
      // currently assuming 4x4 block
       FasTC::GetBlockDimensions(FasTC::eCompressionFormat_ASTC4x4,vBlock);
       vBlockSz = vBlock[0] * vBlock[1];
       uint8 *outBuf = cj.OutBuf() + cj.CoordsToBlockIdx(cj.XStart(), cj.YStart()) * (vBlockSz[0] * vBlockSz[1]);
       
       uint32 startX = cj.XStart();
       for(uint32 h = cj.YStart(); h<= cj.YEnd(); h += vBlock[1]){
         const uint32 endX = h == cj.YEnd() ? cj.XEnd() : cj.Width();
         for(uint32 w = startX; w < endX; w += vBlock[0] ){
           uint32 block[144];
           GetBlock(w, h, cj.Width(), inPixels, block);  
          }
       }
  }
 


}
