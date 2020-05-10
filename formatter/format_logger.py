#!/bin/env python3
####################
# Database Browser for SQLite project - format_runner
# Copyright (C) 2020 Scott Furry<scott.wl.furry@gmail.com>
#                    for DB Browser for SQLite Development team.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
####################
# pylint: disable=
# no pylint tests disabled
"""\
Python logging module for \'DB Browser for SQLite\' (DB4S) project.
NOTE: See README file in this folder for more details.
"""
import sys
import logging
# module logging level - modified with set function
# see https://pymotw.com/3/logging/
LOG_QUIET = 'error'
LOG_WARNING = 'warning'
LOG_INFO = 'info'
LOG_DEBUG = 'debug'

_LOG_LEVELS = {
    LOG_QUIET: logging.ERROR,       # log only errors
    LOG_WARNING: logging.WARNING,   # default level for execution
    LOG_INFO: logging.INFO,         # a little verbosity
    LOG_DEBUG: logging.DEBUG,       # uber verbosity
    'notset': logging.NOTSET        # not set
}
_EXTRA_DEFN = {'class_name': 'logger'}
_ROOT_LOGGER = logging.getLogger('ROOT')


####################
def test_enable_output() -> bool:
    """\
    Test current effective log level and return TRUE if log level is less than logging.ERROR.
    Need functionality to control subprocess output to console (i.e. 'quiet' or 'silent' operation).
    """
    return _ROOT_LOGGER.getEffectiveLevel() < logging.ERROR


####################
def set_verbosity(level_value: int):
    """\
    Value is a count of \'v\' switches from ArgumentParser(command line).
    Calculate log level from that count and set accordingly.
    """
    # 50 = logging.ERROR -> quiet
    # 10 = logging.DEBUG -> max debug info
    calc_verbose: int = logging.ERROR - (level_value * 10)
    if calc_verbose < logging.DEBUG:
        calc_verbose = logging.DEBUG
    _ROOT_LOGGER.setLevel(calc_verbose)
    for hdlr in _ROOT_LOGGER.handlers:
        hdlr.setFormatter(_choose_formatter())
    _ROOT_LOGGER.info('Log level %d', _ROOT_LOGGER.getEffectiveLevel(), extra=_EXTRA_DEFN)


####################
def set_log_env(level_str: str):
    """\
    Set log level based on string argument.
    NOTE: should be same as str(logging.LEVEL).lower()
    """
    if level_str in _LOG_LEVELS.keys():
        level_value = _LOG_LEVELS.get(level_str, logging.NOTSET)
        _ROOT_LOGGER.setLevel(level_value)
        for hdlr in _ROOT_LOGGER.handlers:
            hdlr.setFormatter(_choose_formatter())


####################
def get_logger(logger_name):
    """\
    Create a root logger with appropriate console log handler
    """
    logger = _ROOT_LOGGER.getChild(logger_name)
    # disable logging level (NOTSET) to allow propagation to root logger.
    # "noise" output level of root logger is modified.
    logger.setLevel(logging.NOTSET)
    return logger


####################
def _choose_formatter():
    """\
    Select a formatter based on log level.
    DEBUG and INFO uses a different format than WARNING or above.
    """
    # see https://docs.Python.org/3.8/library/logging.html#logrecord-attributes
    # for all available LogRecord attributes - i.e. levelname, funcName, et al.
    # log formatter for logging level DEBUG
    log_frmt_debug = logging.Formatter('%(module)-14s:: %(class_name)-14s:%(funcName)-16s::%(message)-s')
    # log formatter for logging level info
    log_frmt_info = logging.Formatter('%(class_name)-14s:: %(message)-s')
    # log formatter for logging level WARNING and above
    log_frmt_warn = logging.Formatter('%(message)-s')
    level_value = _ROOT_LOGGER.getEffectiveLevel()
    if level_value in (logging.DEBUG, logging.NOTSET):
        return log_frmt_debug
    if level_value == logging.INFO:
        return log_frmt_info
    return log_frmt_warn


####################
def _create_console_logger():
    """\
    Create a default console logging handler
    """
    con_hdlr = logging.StreamHandler(sys.stdout)
    con_hdlr.setFormatter(_choose_formatter())
    return con_hdlr


####################
# add a console handler to the root logger
_ROOT_LOGGER.addHandler(_create_console_logger())
####################
