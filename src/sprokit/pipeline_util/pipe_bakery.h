/*ckwg +29
 * Copyright 2011-2013 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SPROKIT_PIPELINE_UTIL_PIPE_BAKERY_H
#define SPROKIT_PIPELINE_UTIL_PIPE_BAKERY_H

#include "pipeline_util-config.h"

#include "path.h"
#include "pipe_declaration_types.h"

#include <sprokit/pipeline/types.h>

#include <iosfwd>

/**
 * \file pipe_bakery.h
 *
 * \brief Functions to bake a pipeline.
 */

namespace sprokit
{

/**
 * \brief Convert a pipeline description file into a pipeline.
 *
 * \param fname The file to load the pipeline from.
 *
 * \returns A new pipeline baked from the given file.
 */
SPROKIT_PIPELINE_UTIL_EXPORT pipeline_t bake_pipe_from_file(path_t const& fname);

/**
 * \brief Bake a pipeline from a stream.
 *
 * \param istr The stream to load the pipeline from.
 * \param inc_root The root directory to search for includes from.
 *
 * \returns A pipeline baked from the given stream.
 */
SPROKIT_PIPELINE_UTIL_EXPORT pipeline_t bake_pipe(std::istream& istr, path_t const& inc_root = "");

/**
 * \brief Extract a configuration from a collection of blocks.
 *
 * \param blocks The blocks to use for baking the pipeline.
 *
 * \returns A pipeline baked from \p blocks.
 */
SPROKIT_PIPELINE_UTIL_EXPORT pipeline_t bake_pipe_blocks(pipe_blocks const& blocks);

/**
 * \class cluster_info pipe_bakery.h <sprokit/pipeline_util/pipe_bakery.h>
 *
 * \brief Information about a loaded cluster.
 */
class SPROKIT_PIPELINE_UTIL_EXPORT cluster_info
{
  public:
    /**
     * \brief Constructor.
     *
     * \param type_ The type of the cluster.
     * \param description_ A description of the cluster.
     * \param ctor_ A function to create an instance of the cluster.
     */
    cluster_info(process::type_t const& type_,
                 process_registry::description_t const& description_,
                 process_ctor_t const& ctor_);
    /**
     * \brief Destructor.
     */
    ~cluster_info();

    /// The type of the cluster.
    process::type_t const type;
    /// A description of the cluster.
    process_registry::description_t const description;
    /// A function to create an instance of the cluster.
    process_ctor_t const ctor;
};
/// A handle to information about a cluster.
typedef boost::shared_ptr<cluster_info> cluster_info_t;

/**
 * \brief Convert a cluster description file into a cluster.
 *
 * \param fname The file to load the cluster from.
 *
 * \returns Information about the cluster in the file.
 */
cluster_info_t SPROKIT_PIPELINE_UTIL_EXPORT bake_cluster_from_file(path_t const& fname);

/**
 * \brief Bake a cluster from a stream.
 *
 * \param istr The stream to load the cluster from.
 * \param inc_root The root directory to search for includes from.
 *
 * \returns Information about the cluster in the stream.
 */
cluster_info_t SPROKIT_PIPELINE_UTIL_EXPORT bake_cluster(std::istream& istr, path_t const& inc_root = "");

/**
 * \brief Extract a configuration from a collection of blocks.
 *
 * \param blocks The blocks to use for baking the cluster.
 *
 * \returns Information about the cluster based on \p blocks.
 */
cluster_info_t SPROKIT_PIPELINE_UTIL_EXPORT bake_cluster_blocks(cluster_blocks const& blocks);

/**
 * \brief Extract a configuration from a collection of blocks.
 *
 * \param blocks The blocks to use for baking the pipeline.
 *
 * \returns A configuration extracted from \p blocks.
 */
SPROKIT_PIPELINE_UTIL_EXPORT config_t extract_configuration(pipe_blocks const& blocks);

}

#endif // SPROKIT_PIPELINE_UTIL_PIPE_BAKERY_H
