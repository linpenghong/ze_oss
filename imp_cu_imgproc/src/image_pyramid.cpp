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
#include <imp/cu_imgproc/image_pyramid.hpp>

#include <glog/logging.h>
#include <imp/core/pixel_enums.hpp>

namespace ze {

//------------------------------------------------------------------------------
template<typename Pixel>
ImagePyramid<Pixel>::ImagePyramid(
    Size2u size, float scale_factor, uint32_t size_bound, uint32_t max_num_levels)
  : scale_factor_(scale_factor)
  , size_bound_(size_bound)
  , max_num_levels_(max_num_levels)
{
  this->init(size);
}

//------------------------------------------------------------------------------
template<typename Pixel>
void ImagePyramid<Pixel>::clear() noexcept
{
  levels_.clear();
  scale_factors_.clear();
}

//------------------------------------------------------------------------------
template<typename Pixel>
void ImagePyramid<Pixel>::init(const ze::Size2u& size)
{
  CHECK_GT(scale_factor_, 0.0f);
  CHECK_LT(scale_factor_, 1.0f);

  if (!levels_.empty() || scale_factors_.empty())
  {
    this->clear();
  }

  uint32_t shorter_side = std::min(size.width(), size.height());

  // calculate the maximum number of levels
  float ratio = static_cast<float>(shorter_side)/static_cast<float>(size_bound_);
  // +1 because the original size is level 0
  size_t possible_num_levels =
      static_cast<int>(-std::log(ratio)/std::log(scale_factor_)) + 1;
  num_levels_ = std::min(max_num_levels_, possible_num_levels);

  // init rate for each level
  for (size_t i = 0; i<num_levels_; ++i)
  {
    scale_factors_.push_back(std::pow(scale_factor_, static_cast<float>(i)));
  }
}

//=============================================================================
// Explicitely instantiate the desired classes
// (sync with typedefs at the end of the hpp file)
template class ImagePyramid<ze::Pixel8uC1>;
template class ImagePyramid<ze::Pixel8uC2>;
//template class ImagePyramid<imp::Pixel8uC3>;
template class ImagePyramid<ze::Pixel8uC4>;

template class ImagePyramid<ze::Pixel16uC1>;
template class ImagePyramid<ze::Pixel16uC2>;
//template class ImagePyramid<imp::Pixel16uC3>;
template class ImagePyramid<ze::Pixel16uC4>;

template class ImagePyramid<ze::Pixel32sC1>;
template class ImagePyramid<ze::Pixel32sC2>;
//template class ImagePyramid<imp::Pixel32sC3>;
template class ImagePyramid<ze::Pixel32sC4>;

template class ImagePyramid<ze::Pixel32fC1>;
template class ImagePyramid<ze::Pixel32fC2>;
//template class ImagePyramid<imp::Pixel32fC3>;
template class ImagePyramid<ze::Pixel32fC4>;

} // namespace ze

