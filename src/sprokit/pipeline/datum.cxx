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

#include "datum.h"

#include <sstream>

/**
 * \file datum.cxx
 *
 * \brief Implementation of a piece of \link sprokit::datum data\endlink in the pipeline.
 */

namespace sprokit
{

datum_t
datum::new_datum(boost::any const& dat)
{
  return datum_t(new datum(dat));
}

datum_t
datum
::empty_datum()
{
  return datum_t(new datum(empty));
}

datum_t
datum
::flush_datum()
{
  return datum_t(new datum(flush));
}

datum_t
datum
::complete_datum()
{
  return datum_t(new datum(complete));
}

datum_t
datum
::error_datum(error_t const& error)
{
  return datum_t(new datum(error));
}

datum::type_t
datum
::type() const
{
  return m_type;
}

datum::error_t
datum
::get_error() const
{
  return m_error;
}

datum
::datum(type_t ty)
  : m_type(ty)
  , m_error()
  , m_datum()
{
}

datum
::datum(error_t const& err)
  : m_type(error)
  , m_error(err)
  , m_datum()
{
}

datum
::datum(boost::any const& dat)
  : m_type(data)
  , m_error()
  , m_datum(dat)
{
}

datum_exception
::datum_exception() SPROKIT_NOTHROW
  : pipeline_exception()
{
}

datum_exception
::~datum_exception() SPROKIT_NOTHROW
{
}

static char const* string_for_type(datum::type_t type);

bad_datum_cast_exception
::bad_datum_cast_exception(std::string const& requested_typeid, std::string const& typeid_, datum::type_t const& type, datum::error_t const& error, char const* reason) SPROKIT_NOTHROW
  : datum_exception()
  , m_requested_typeid(requested_typeid)
  , m_typeid(typeid_)
  , m_type(type)
  , m_error(error)
  , m_reason(reason)
{
  std::ostringstream sstr;

  if (m_type == datum::error)
  {
    sstr << "Failed to cast datum of type "
            "\'" << string_for_type(m_type) << "\' (" << m_error << ") into "
         << m_typeid << ": "
         << m_reason << ".";
  }
  else if (m_type == datum::data)
  {
    sstr << "Failed to cast datum of type "
            "\'" << string_for_type(m_type) << "\' (" << m_typeid << ") into "
         << m_requested_typeid << ": "
         << m_reason << ".";
  }
  else
  {
    sstr << "Failed to cast datum of type "
            "\'" << string_for_type(m_type) << "\' into "
         << m_requested_typeid << ": "
         << m_reason << ".";
  }

  m_what = sstr.str();
}

bad_datum_cast_exception
::~bad_datum_cast_exception() SPROKIT_NOTHROW
{
}

char const*
string_for_type(datum::type_t type)
{
  switch (type)
  {
#define STRING_FOR_TYPE(type) \
  case datum::type:           \
    return #type

    STRING_FOR_TYPE(data);
    STRING_FOR_TYPE(empty);
    STRING_FOR_TYPE(error);
    STRING_FOR_TYPE(invalid);
    STRING_FOR_TYPE(flush);
    STRING_FOR_TYPE(complete);

#undef STRING_FOR_TYPE

    default:
      break;
  }

  return "unknown";
}

}
