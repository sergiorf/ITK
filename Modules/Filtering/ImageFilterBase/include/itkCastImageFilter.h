/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkCastImageFilter_h
#define itkCastImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkProgressReporter.h"

#include <type_traits>

namespace itk
{
#if !defined( ITK_LEGACY_REMOVE )
namespace Functor
{
/** \class Cast
 *
 *  \deprecated This functor is no longer used by the CastImageFilter.
 * \ingroup ITKImageFilterBase
 */

template< typename TInput, typename TOutput >
class ITK_TEMPLATE_EXPORT Cast
{
public:
  Cast() = default;
  virtual ~Cast() = default;
  bool operator!=(const Cast &) const
  {
    return false;
  }

  bool operator==(const Cast & other) const
  {
    return !( *this != other );
  }

  inline TOutput operator()(const TInput & A) const
  {
    return static_cast< TOutput >( A );
  }
};
}
#endif


/** \class CastImageFilter
 *
 * \brief Casts input pixels to output pixel type.
 *
 * This filter is templated over the input image type
 * and the output image type.
 *
 * A typical use is to cast a
   \code
   itk::Image<type1, dim>
   \endcode
 * to a
   \code
   itk::Image<type2, dim>
   \endcode
 *
 * This filter can also be used to cast a
   \code
   itk::VectorImage<type1, dim>
   \endcode
 * to a
   \code
   itk::VectorImage<type2, dim>
   \endcode
 *
 * If you need to perform a dimensionaly reduction, you may want
 * to use the ExtractImageFilter instead of the CastImageFilter.
 *
 * \ingroup IntensityImageFilters  MultiThreaded
 * \sa UnaryFunctorImageFilter
 * \sa ExtractImageFilter
 * \ingroup ITKImageFilterBase
 *
 * \wiki
 * \wikiexample{ImageProcessing/CastImageFilter,Cast an image from one type to another}
 * \endwiki
 */
template< typename TInputImage, typename TOutputImage >
class ITK_TEMPLATE_EXPORT CastImageFilter:
    public InPlaceImageFilter< TInputImage, TOutputImage >
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(CastImageFilter);

  /** Standard class type aliases. */
  using Self = CastImageFilter;

  using Superclass = InPlaceImageFilter< TInputImage, TOutputImage >;

  using Pointer = SmartPointer< Self >;
  using ConstPointer = SmartPointer< const Self >;


  using OutputImageRegionType = typename Superclass::OutputImageRegionType;

  using InputPixelType = typename TInputImage::PixelType;
  using OutputPixelType = typename TOutputImage::PixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CastImageFilter, InPlaceImageFilter);

protected:
  CastImageFilter();
  virtual ~CastImageFilter() = default;

  void GenerateOutputInformation() override;

  void GenerateData() override;

  void DynamicThreadedGenerateData(const OutputImageRegionType & outputRegionForThread) override;

  template<typename TInputPixelType>
    void DynamicThreadedGenerateDataDispatched(const OutputImageRegionType & outputRegionForThread, std::true_type isConvertible);

  template<typename TInputPixelType>
    void DynamicThreadedGenerateDataDispatched(const OutputImageRegionType & outputRegionForThread, std::false_type isConvertible);

private:

  template <typename TFromType, typename TToType, typename TNoDeclType = TToType>
    struct is_static_castable
    : std::false_type
  {};

  template <typename TFromType, typename TToType>
    struct is_static_castable<TFromType,
                              TToType,
                              decltype(static_cast<TToType>(std::declval<TFromType>()))>
    : std::true_type
  {};

};
} // end namespace itk

#endif


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCastImageFilter.hxx"
#endif
