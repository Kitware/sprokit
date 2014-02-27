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

#include "thread_per_process_scheduler.h"

#include <sprokit/pipeline/config.h>
#include <sprokit/pipeline/datum.h>
#include <sprokit/pipeline/edge.h>
#include <sprokit/pipeline/pipeline.h>
#include <sprokit/pipeline/scheduler_exception.h>
#include <sprokit/pipeline/utils.h>

#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>

/**
 * \file thread_per_process_scheduler.cxx
 *
 * \brief Implementation of the thread-per-process scheduler.
 */

namespace sprokit
{

class thread_per_process_scheduler::priv
{
  public:
    priv();
    ~priv();

    void run_process(process_t const& process);

    boost::scoped_ptr<boost::thread_group> process_threads;

    typedef boost::shared_mutex mutex_t;
    typedef boost::shared_lock<mutex_t> shared_lock_t;

    mutable mutex_t mut;
};

thread_per_process_scheduler
::thread_per_process_scheduler(pipeline_t const& pipe, config_t const& config)
  : scheduler(pipe, config)
  , d(new priv)
{
  pipeline_t const p = pipeline();
  process::names_t const names = p->process_names();

  BOOST_FOREACH (process::name_t const& name, names)
  {
    process_t const proc = p->process_by_name(name);
    process::properties_t const consts = proc->properties();

    if (consts.count(process::property_no_threads))
    {
      std::string const reason = "The process \'" + name + "\' does "
                                 "not support being in its own thread";

      throw incompatible_pipeline_exception(reason);
    }
  }
}

thread_per_process_scheduler
::~thread_per_process_scheduler()
{
  shutdown();
}

void
thread_per_process_scheduler
::_start()
{
  pipeline_t const p = pipeline();
  process::names_t const names = p->process_names();

  d->process_threads.reset(new boost::thread_group);

  BOOST_FOREACH (process::name_t const& name, names)
  {
    process_t const process = pipeline()->process_by_name(name);

    d->process_threads->create_thread(boost::bind(&priv::run_process, d.get(), process));
  }
}

void
thread_per_process_scheduler
::_wait()
{
  d->process_threads->join_all();
}

void
thread_per_process_scheduler
::_pause()
{
  d->mut.lock();
}

void
thread_per_process_scheduler
::_resume()
{
  d->mut.unlock();
}

void
thread_per_process_scheduler
::_stop()
{
  d->process_threads->interrupt_all();
}

thread_per_process_scheduler::priv
::priv()
  : process_threads()
  , mut()
{
}

thread_per_process_scheduler::priv
::~priv()
{
}

static config_t monitor_edge_config();

void
thread_per_process_scheduler::priv
::run_process(process_t const& process)
{
  config_t const edge_conf = monitor_edge_config();

  name_thread(process->name());
  edge_t monitor_edge = boost::make_shared<edge>(edge_conf);

  process->connect_output_port(process::port_heartbeat, monitor_edge);

  bool complete = false;

  while (!complete)
  {
    shared_lock_t const lock(mut);

    (void)lock;

    boost::this_thread::interruption_point();

    process->step();

    while (monitor_edge->has_data())
    {
      edge_datum_t const edat = monitor_edge->get_datum();
      datum_t const dat = edat.datum;

      if (dat->type() == datum::complete)
      {
        complete = true;
      }
    }
  }
}

config_t
monitor_edge_config()
{
  config_t conf = config::empty_config();

  return conf;
}

}