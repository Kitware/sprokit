/*ckwg +5
 * Copyright 2011-2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_IMAGE_WARP_IMAGE_PROCESS_H
#define VISTK_PROCESSES_IMAGE_WARP_IMAGE_PROCESS_H

#include "image-config.h"

#include <vistk/pipeline/process.h>

#include <boost/scoped_ptr.hpp>

/**
 * \file warp_image_process.h
 *
 * \brief Declaration of the warp image process.
 */

namespace vistk
{

/**
 * \class warp_image_process
 *
 * \brief A process for warping images.
 *
 * \process Warp images.
 *
 * \iports
 *
 * \iport{transform} The transform to use to warp the image.
 * \iport{image} The image to warp.
 *
 * \oports
 *
 * \oport{warped_image} The warped image.
 *
 * \configs
 *
 * \config{pixtype} The pixel type of the input images.
 * \config{pixfmt} The pixel format of the input images.
 *
 * \reqs
 *
 * \req The \port{image}, \port{transform}, and \port{warped_image} ports must be connected.
 *
 * \ingroup process_image
 */
class VISTK_PROCESSES_IMAGE_NO_EXPORT warp_image_process
  : public process
{
  public:
    /**
     * \brief Constructor.
     *
     * \param config The configuration for the process.
     */
    warp_image_process(config_t const& config);
    /**
     * \brief Destructor.
     */
    ~warp_image_process();
  protected:
    /**
     * \brief Configure the process.
     */
    void _configure();

    /**
     * \brief Step the process.
     */
    void _step();
  private:
    class priv;
    boost::scoped_ptr<priv> d;
};

}

#endif // VISTK_PROCESSES_IMAGE_WARP_IMAGE_PROCESS_H