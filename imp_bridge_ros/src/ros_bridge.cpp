// Copyright (c) 2015-2016, ETH Zurich, Wyss Zurich, Zurich Eye
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the ETH Zurich, Wyss Zurich, Zurich Eye nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL ETH Zurich, Wyss Zurich, Zurich Eye BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <imp/bridge/ros/ros_bridge.hpp>

#include <sensor_msgs/image_encodings.h>

#include <imp/core/image_raw.hpp>
#include <ze/common/logging.hpp>
#include <ze/common/types.hpp>


namespace ze {

namespace imgenc = sensor_msgs::image_encodings;

//------------------------------------------------------------------------------
std::pair<PixelType, PixelOrder> getPixelTypeFromRosImageEncoding(
    const std::string& encoding)
{
  //! @todo (MWE) we do not support bayer or YUV images yet.
  if (encoding == imgenc::BGR8)
  {
    return std::make_pair(PixelType::i8uC3, PixelOrder::bgr);
  }
  else if (encoding == imgenc::MONO8)
  {
    return std::make_pair(PixelType::i8uC1, PixelOrder::gray);
  }
  else if (encoding == imgenc::RGB8)
  {
    return std::make_pair(PixelType::i8uC3, PixelOrder::rgb);
  }
  else if (encoding == imgenc::MONO16)
  {
    return std::make_pair(PixelType::i16uC1, PixelOrder::gray);
  }
  else if (encoding == imgenc::BGR16)
  {
    return std::make_pair(PixelType::i16uC3, PixelOrder::bgr);
  }
  else if (encoding == imgenc::RGB16)
  {
    return std::make_pair(PixelType::i16uC3, PixelOrder::rgb);
  }
  else if (encoding == imgenc::BGRA8)
  {
    return std::make_pair(PixelType::i8uC4, PixelOrder::bgra);
  }
  else if (encoding == imgenc::RGBA8)
  {
    return std::make_pair(PixelType::i8uC4, PixelOrder::rgba);
  }
  else if (encoding == imgenc::BGRA16)
  {
    return std::make_pair(PixelType::i16uC4, PixelOrder::bgra);
  }
  else if (encoding == imgenc::RGBA16)
  {
    return std::make_pair(PixelType::i16uC4, PixelOrder::rgba);
  }
  LOG(FATAL) << "Unsupported image encoding " + encoding + ".";
  return std::make_pair(PixelType::undefined, PixelOrder::undefined);
}

//------------------------------------------------------------------------------
ImageBase::Ptr toImageCpu(
    const sensor_msgs::Image& src, PixelOrder /*pixel_order*/)
{
  PixelType src_pixel_type;
  PixelOrder src_pixel_order;
  std::tie(src_pixel_type, src_pixel_order) =
      getPixelTypeFromRosImageEncoding(src.encoding);

  int bit_depth = imgenc::bitDepth(src.encoding);
  int num_channels = imgenc::numChannels(src.encoding);
  uint32_t width = src.width;
  uint32_t height = src.height;
  uint32_t pitch = src.step;

  // sanity check
  CHECK_GE(pitch, width * num_channels * bit_depth/8) << "Input image seem to wrongly formatted";

  switch (src_pixel_type)
  {
    case PixelType::i8uC1:
    {
      ImageRaw8uC1 src_wrapped(
          reinterpret_cast<Pixel8uC1*>(const_cast<uint8_t*>(&src.data[0])),
          width, height, pitch, true, PixelOrder::gray);
      ImageRaw8uC1::Ptr dst =
          std::make_shared<ImageRaw8uC1>(src_wrapped); // Deep copy of the image data.
      return dst;
    }
//  case imp::PixelType::i8uC2:
//  { } break;
//  case imp::PixelType::i8uC3:
//  { } break;
//  case imp::PixelType::i8uC4:
//  { } break;
//  case imp::PixelType::i16uC1:
//  { } break;
//  case imp::PixelType::i16uC2:
//  { } break;
//  case imp::PixelType::i16uC3:
//  { } break;
//  case imp::PixelType::i16uC4:
//  { } break;
//  case imp::PixelType::i32uC1:
//  { } break;
//  case imp::PixelType::i32uC2:
//  { } break;
//  case imp::PixelType::i32uC3:
//  { } break;
//  case imp::PixelType::i32uC4:
//  { } break;
//  case imp::PixelType::i32sC1:
//  { } break;
//  case imp::PixelType::i32sC2:
//  { } break;
//  case imp::PixelType::i32sC3:
//  { } break;
//  case imp::PixelType::i32sC4:
//  { } break;
//  case imp::PixelType::i32fC1:
//  { } break;
//  case imp::PixelType::i32fC2:
//  { } break;
//  case imp::PixelType::i32fC3:
//  { } break;
//  case imp::PixelType::i32fC4:
//  { } break;
    default:
    {
      LOG(FATAL) << "Unsupported pixel type" + src.encoding + ".";
      break;
    }
  }
  return nullptr;
}

} // namespace ze
