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

#include <sprokit/pipeline/datum.h>
#include <sprokit/pipeline/edge.h>
#include <sprokit/pipeline/stamp.h>

#include <sprokit/python/util/python_convert_optional.h>
#include <sprokit/python/util/python_gil.h>

#include <boost/chrono/duration.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/class.hpp>
#include <boost/python/module.hpp>

/**
 * \file edge.cxx
 *
 * \brief Python bindings for \link sprokit::edge\endlink.
 */

using namespace boost::python;

static bool edge_datum_eq(sprokit::edge_datum_t const& self, sprokit::edge_datum_t const& rhs);
static bool edge_try_push_datum(sprokit::edge_t const& self, sprokit::edge_datum_t const& datum, double duration);
static boost::optional<sprokit::edge_datum_t> edge_try_get_datum(sprokit::edge_t const& self, double duration);

BOOST_PYTHON_MODULE(edge)
{
  class_<sprokit::edge_datum_t>("EdgeDatum"
    , no_init)
    .def(init<>())
    .def(init<sprokit::datum_t, sprokit::stamp_t>())
    .def("__eq__", &edge_datum_eq)
    .def_readwrite("datum", &sprokit::edge_datum_t::datum)
    .def_readwrite("stamp", &sprokit::edge_datum_t::stamp)
  ;
  class_<sprokit::edge_data_t>("EdgeData"
    , "A collection of data packets that may be passed through an edge.")
    .def(vector_indexing_suite<sprokit::edge_data_t>())
  ;
  class_<sprokit::edges_t>("Edges"
    , "A collection of edges.")
    .def(vector_indexing_suite<sprokit::edges_t>())
  ;

  sprokit::python::register_optional_converter<sprokit::edge_datum_t>("EdgeDatumOpt", "An optional edge datum.");

  class_<sprokit::edge, sprokit::edge_t, boost::noncopyable>("Edge"
    , "A communication channel between processes."
    , no_init)
    .def(init<>())
    .def(init<sprokit::config_t>())
    .def("makes_dependency", &sprokit::edge::makes_dependency
      , "Returns True if the edge implies a dependency from downstream on upstream.")
    .def("has_data", &sprokit::edge::has_data
      , "Returns True if the edge contains data, False otherwise.")
    .def("full_of_data", &sprokit::edge::full_of_data
      , "Returns True if the edge cannot hold anymore data, False otherwise.")
    .def("datum_count", &sprokit::edge::datum_count
      , "Returns the number of data packets within the edge.")
    .def("push_datum", &sprokit::edge::push_datum
      , (arg("datum"))
      , "Pushes a datum packet into the edge.")
    .def("get_datum", &sprokit::edge::get_datum
      , "Returns the next datum packet from the edge, removing it in the process.")
    .def("peek_datum", &sprokit::edge::peek_datum
      , (arg("index") = 0)
      , "Returns the next datum packet from the edge.")
    .def("pop_datum", &sprokit::edge::pop_datum
      , "Remove the next datum packet from the edge.")
    .def("try_push_datum", &edge_try_push_datum
      , (arg("datum"), arg("duration"))
      , "Pushes a datum packet into the edge and returns True or returns False if unable to meet the duration.")
    .def("try_get_datum", &edge_try_get_datum
      , (arg("duration"))
      , "Returns the next datum packet from the edge, removing it in the process or None if unable to meet the duration.")
    .def("set_upstream_process", &sprokit::edge::set_upstream_process
      , (arg("process"))
      , "Set the process which is feeding data into the edge.")
    .def("set_downstream_process", &sprokit::edge::set_downstream_process
      , (arg("process"))
      , "Set the process which is reading data from the edge.")
    .def("mark_downstream_as_complete", &sprokit::edge::mark_downstream_as_complete
      , "Indicate that the downstream process is complete.")
    .def("is_downstream_complete", &sprokit::edge::is_downstream_complete
      , "Returns True if the downstream process is complete, False otherwise.")
    .def_readonly("config_dependency", &sprokit::edge::config_dependency)
    .def_readonly("config_capacity", &sprokit::edge::config_capacity)
  ;
}

bool
edge_datum_eq(sprokit::edge_datum_t const& self, sprokit::edge_datum_t const& rhs)
{
  return (self == rhs);
}

namespace
{

typedef boost::chrono::duration<double> py_duration_t;

}

bool
edge_try_push_datum(sprokit::edge_t const& self, sprokit::edge_datum_t const& datum, double duration)
{
  py_duration_t const duration_sec = py_duration_t(duration);
  sprokit::edge::duration_t const duration_edge = boost::chrono::duration_cast<sprokit::edge::duration_t>(duration_sec);

  return self->try_push_datum(datum, duration_edge);
}

boost::optional<sprokit::edge_datum_t>
edge_try_get_datum(sprokit::edge_t const& self, double duration)
{
  py_duration_t const duration_sec = py_duration_t(duration);
  sprokit::edge::duration_t const duration_edge = boost::chrono::duration_cast<sprokit::edge::duration_t>(duration_sec);

  return self->try_get_datum(duration_edge);
}
