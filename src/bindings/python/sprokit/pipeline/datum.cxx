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

#include <sprokit/python/any_conversion/prototypes.h>
#include <sprokit/python/any_conversion/registration.h>
#include <sprokit/python/util/python_gil.h>

#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/enum.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/module.hpp>
#include <boost/python/object.hpp>
#include <boost/any.hpp>
#include <boost/cstdint.hpp>

#include <limits>
#include <string>

/**
 * \file datum.cxx
 *
 * \brief Python bindings for \link sprokit::datum\endlink.
 */

using namespace boost::python;

static sprokit::datum_t new_datum(object const& obj);
static sprokit::datum::type_t datum_type(sprokit::datum_t const& self);
static sprokit::datum::error_t datum_get_error(sprokit::datum_t const& self);
static object datum_get_datum(sprokit::datum_t const& self);
static bool datum_eq(sprokit::datum_t const& self, sprokit::datum_t const& other);

BOOST_PYTHON_MODULE(datum)
{
  enum_<sprokit::datum::type_t>("DatumType"
    , "A type for a datum packet.")
    .value("invalid", sprokit::datum::invalid)
    .value("data", sprokit::datum::data)
    .value("empty", sprokit::datum::empty)
    .value("flush", sprokit::datum::flush)
    .value("complete", sprokit::datum::complete)
    .value("error", sprokit::datum::error)
  ;

  class_<sprokit::datum::error_t>("DatumError"
    , "The type of an error message.");

  def("new", &new_datum
    , (arg("dat"))
    , "Creates a new datum packet.");
  def("empty", &sprokit::datum::empty_datum
    , "Creates an empty datum packet.");
  def("flush", &sprokit::datum::flush_datum
    , "Creates a flush marker datum packet.");
  def("complete", &sprokit::datum::complete_datum
    , "Creates a complete marker datum packet.");
  def("error", &sprokit::datum::error_datum
    , (arg("err"))
    , "Creates an error datum packet.");

  class_<sprokit::datum_t>("Datum"
    , "A packet of data within the pipeline."
    , no_init)
    .def("type", &datum_type
      , "The type of the datum packet.")
    .def("get_error", &datum_get_error
      , "The error contained within the datum packet.")
    .def("get_datum", &datum_get_datum
      , "Get the data contained within the packet.")
    .def("__eq__", &datum_eq)
  ;

  sprokit::python::register_type<std::string>(0);
  sprokit::python::register_type<int32_t>(1);
  sprokit::python::register_type<char>(2);
  sprokit::python::register_type<bool>(3);
  sprokit::python::register_type<double>(4);

  // At worst, pass the object itself through.
  sprokit::python::register_type<object>(std::numeric_limits<sprokit::python::priority_t>::max());

  implicitly_convertible<boost::any, object>();
  implicitly_convertible<object, boost::any>();
}

sprokit::datum_t
new_datum(object const& obj)
{
  sprokit::python::python_gil const gil;

  (void)gil;

  boost::any const any = extract<boost::any>(obj)();

  return sprokit::datum::new_datum(any);
}

sprokit::datum::type_t
datum_type(sprokit::datum_t const& self)
{
  return self->type();
}

sprokit::datum::error_t
datum_get_error(sprokit::datum_t const& self)
{
  return self->get_error();
}

object
datum_get_datum(sprokit::datum_t const& self)
{
  boost::any const any = self->get_datum<boost::any>();

  sprokit::python::python_gil const gil;

  (void)gil;

  return object(any);
}

bool
datum_eq(sprokit::datum_t const& self, sprokit::datum_t const& other)
{
  return (*self == *other);
}
