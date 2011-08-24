/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PROCESSES_FLOW_COLLATE_PROCESS_H
#define VISTK_PROCESSES_FLOW_COLLATE_PROCESS_H

#include "flow-config.h"

#include <vistk/pipeline/process.h>

namespace vistk
{

/**
 * \class collate_process
 *
 * \brief A process which collating input data from multiple input edges.
 *
 * \note Edges for a \portvar{res} may \em only be connected after the
 * \port{color_\portvar{res}} is connected to. Before this connection happens,
 * the other ports to not exist and will cause errors. In short: The first
 * connection for any \portvar{res} must be \port{color_\portvar{res}}.
 *
 * \process A process for collating data from multiple sources.
 *
 * \iports
 *
 * \iport{color_\portvar{res}} The color of the result \portvar{res}.
 * \iport{coll_\portvar{res}_\portvar{any}} A port to collate data for
 *                                          \portvar{res} from. Data is collated
 *                                          from ports in ASCII-betical order.
 *
 * \oports
 *
 * \oport{out_\portvar{res}} The collated result \portvar{res}.
 *
 * \reqs
 *
 * \req Each input port \port{color_\portvar{res}} must be connected.
 * \req Each \portvar{res} must have at least two inputs to collate.
 * \req Each output port \port{out_\portvar{res}} must be connected.
 *
 * \todo Add configuration to allow forcing a number of inputs for a result.
 * \todo Add configuration to allow same number of sources for all results.
 */
class VISTK_PROCESSES_FLOW_NO_EXPORT collate_process
  : public process
{
  public:
    /**
     * \brief Constructor.
     *
     * \param config Contains config for the process.
     */
    collate_process(config_t const& config);
    /**
     * \brief Destructor.
     */
    ~collate_process();
  protected:
    /**
     * \brief Checks the output port connections and the configuration.
     */
    void _init();

    /**
     * \brief Collate data from the input edges.
     */
    void _step();

    /**
     * \brief Subclass input connection method.
     *
     * \param port The port to connect to.
     * \param edge The edge to connect to the port.
     */
    void _connect_input_port(port_t const& port, edge_ref_t edge);
  private:
    class priv;
    boost::shared_ptr<priv> d;
};

}

#endif // VISTK_PROCESSES_FLOW_COLLATE_PROCESS_H