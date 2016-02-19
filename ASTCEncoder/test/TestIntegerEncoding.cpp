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

#include "gtest/gtest.h"
#include "IntegerEncoding.h"
#include "FasTC\BitStream.h"

using ASTCC::IntegerEncodedValue;
using ASTCC::EncodeIntegerSeq;


TEST(IntegerDecoding, GetEncoding) {
  // According to table C.2.7
  IntegerEncodedValue val = IntegerEncodedValue::CreateEncoding(1);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_JustBits);
  EXPECT_EQ(val.BaseBitLength(), 1U);

  val = IntegerEncodedValue::CreateEncoding(2);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Trit);
  EXPECT_EQ(val.BaseBitLength(), 0U);

  val = IntegerEncodedValue::CreateEncoding(3);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_JustBits);
  EXPECT_EQ(val.BaseBitLength(), 2U);

  val = IntegerEncodedValue::CreateEncoding(4);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Quint);
  EXPECT_EQ(val.BaseBitLength(), 0U);

  val = IntegerEncodedValue::CreateEncoding(5);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Trit);
  EXPECT_EQ(val.BaseBitLength(), 1U);

  val = IntegerEncodedValue::CreateEncoding(7);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_JustBits);
  EXPECT_EQ(val.BaseBitLength(), 3U);

  val = IntegerEncodedValue::CreateEncoding(9);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Quint);
  EXPECT_EQ(val.BaseBitLength(), 1U);

  val = IntegerEncodedValue::CreateEncoding(11);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Trit);
  EXPECT_EQ(val.BaseBitLength(), 2U);

  val = IntegerEncodedValue::CreateEncoding(15);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_JustBits);
  EXPECT_EQ(val.BaseBitLength(), 4U);

  val = IntegerEncodedValue::CreateEncoding(19);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Quint);
  EXPECT_EQ(val.BaseBitLength(), 2U);

  val = IntegerEncodedValue::CreateEncoding(23);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_Trit);
  EXPECT_EQ(val.BaseBitLength(), 3U);

  val = IntegerEncodedValue::CreateEncoding(31);
  EXPECT_EQ(val.GetEncoding(), ASTCC::eIntegerEncoding_JustBits);
  EXPECT_EQ(val.BaseBitLength(), 5U);
}

TEST(IntegerEncoding, Encoding){

  uint8 values[] = { 3, 3, 3, 3, 3 };
  uint16 exp = 0x03FF;

  EncodeIntegerSeq<uint8> encoder(values, 5);
  uint8 output[10] = {0};
  FasTC::BitStream stream(output, 10*8, 0);
  encoder.EncodeIntegers(stream);
  
  FasTC::BitStreamReadOnly ReadStream(output);

  uint16 result = ReadStream.ReadBits(16);

  EXPECT_EQ(result, exp);


  // Test trits basic
  values[0] = 3;
  values[1] = 3;
  values[2] = 3;
  values[3] = 3;
  values[4] = 5;
  for(int i = 0; i < 10; i++)
	  output[i] = 0;
  exp = 0x1D6D; 
  FasTC:: BitStream stream1(output, 10*8, 0);
  EncodeIntegerSeq<uint8> encoder1(values, 5);
  encoder1.EncodeIntegers(stream1);

  FasTC::BitStreamReadOnly ReadStream1(output);
  uint16 result1 = ReadStream1.ReadBits(16);
  EXPECT_EQ(result1, exp);
  
  uint8 values10[] = {3, 3, 3, 3, 5, 3, 3, 3, 3, 5};
  uint8 output10[10] = {0};
  uint32 exp10 = 0x3ADBD6D;
  FasTC::BitStream stream10(output10, 10*8, 0);
  EncodeIntegerSeq<uint8> encoder10(values10, 10);
  encoder10.EncodeIntegers(stream10);

  FasTC::BitStreamReadOnly ReadStream10(output10);
  uint32 result10 = ReadStream10.ReadBits(32);
  EXPECT_EQ(result10, exp10);

  // Test trits larger numbers 

  uint32 valuesLarge1[] = {10, 21, 40, 95, 80};
  uint8 outputLarge1[10] = {0};
  uint64 expLarge1 = 0x187F20AAA;
  uint32 expLarge[] = {0x187F2, 0xAAA };
  FasTC::BitStream streamLarge1(outputLarge1, 10 * 8, 0);
  EncodeIntegerSeq<uint32> encoderLarge1(valuesLarge1, 5);
  encoderLarge1.EncodeIntegers(streamLarge1);

  FasTC::BitStreamReadOnly ReadStreamLarge1(outputLarge1);

  uint32 resultLarge1 = ReadStreamLarge1.ReadBits(16);
  EXPECT_EQ(resultLarge1, expLarge[1]);
  resultLarge1 = ReadStreamLarge1.ReadBits(32);
  EXPECT_EQ(resultLarge1, expLarge[0]);


  // Test Quits 

  uint8 values3[] = {3, 3, 9};
  uint8 output3[10] = {0};
  uint16 exp3 = 0x03B5;
  FasTC::BitStream stream3(output3, 10*8, 0);
  EncodeIntegerSeq<uint8> encoder3(values3, 3);
  encoder3.EncodeIntegers(stream3);

  FasTC::BitStreamReadOnly ReadStream3(output3);
  uint16 result3 = ReadStream3.ReadBits(16);
  EXPECT_EQ(result3, exp3);

  // Test quits longer stream
  uint32 values4[] = {3, 3, 9, 3, 3, 9};
  uint8 output4[10] = {0};
  uint32 exp4 = 0xED7B5;
  FasTC::BitStream stream4(output4, 10*8, 0);
  EncodeIntegerSeq<uint32> encoder4(values4, 6);
  encoder4.EncodeIntegers(stream4);

  FasTC::BitStreamReadOnly ReadStream4(output4);
  uint32 result4 = ReadStream4.ReadBits(32);
  EXPECT_EQ(result4, exp4);

  // Test quits large values

  uint32 valuesLarge2[] = {13,60,79};
  uint8 outputLarge2[10] = {0};
  uint32 expLarge2 = 0x7F63D;
  FasTC::BitStream streamLarge2(outputLarge2, 10*8, 0);

  EncodeIntegerSeq<uint32> encoderLarge2(valuesLarge2,3);
  encoderLarge2.EncodeIntegers(streamLarge2);

  FasTC::BitStreamReadOnly ReadStreamLarge2(outputLarge2);
  uint32 resultLarge2 = ReadStreamLarge2.ReadBits(32);
  EXPECT_EQ(expLarge2, resultLarge2);

  // Test 1. Quantization
  //      2. Edgecase when things nor multiple of things
 // It's not crashing so good news but need the hex values of the quantized bits to get correct test
  uint8 valuesEdge1[] = {10, 21, 40, 91, 80};
  uint8 outputEdge1[10] = {0};
  uint64 expEdge1 = 0x187F20AAA;
  uint32 expEdge[] = {0x1A8F, 0x25178 };
  FasTC::BitStream streamEdge1(outputEdge1, 10 * 8, 0);
  EncodeIntegerSeq<uint8> encoderEdge1(valuesEdge1, 5);
  encoderEdge1.EncodeIntegers(streamEdge1);

  FasTC::BitStreamReadOnly ReadStreamEdge1(outputEdge1);

  uint32 resultEdge1 = ReadStreamEdge1.ReadBits(19);
  EXPECT_EQ(resultEdge1, expEdge[1]);
  resultEdge1 = ReadStreamEdge1.ReadBits(13);
  EXPECT_EQ(resultEdge1, expEdge[0]);


}
