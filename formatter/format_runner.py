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
Python 3 module for automatic code formating of source code
for files in the \'DB Browser for SQLite\' (DB4S) project.
NOTE: See README file in this folder for more details.
"""
import sys
import argparse
from pathlib import Path as LibPath
import format_logger as FrmtLog
import format_fileio as FrmtFileIO
import format_tools as FrmtTools
# module format_logger
_main_log = FrmtLog.get_logger(__name__)
# argument values
_FORMAT_NAME = LibPath(__file__).stem
_FORMAT_VERSION = '0.1.0'


####################
class ExecutionError(ValueError):
    """\
    Generic Exception occured during execution.
    """


####################
def argument_processing():
    """\
    Process arguments passed module.
    """
    # lop off program argument name and use the rest for argparse
    sys_args = sys.argv[1:]

    parser = argparse.ArgumentParser(prog=_FORMAT_NAME, description="""\
        %(prog)s version {ver} - Python3 application for automated code formatting
        of files in the \'DB Browser for SQLite\' (DB4S) project. See README file
        in this folder for more details.""".format(ver=_FORMAT_VERSION))
    noise = parser.add_mutually_exclusive_group()
    noise.add_argument(
        '-q',
        '--quiet',
        action='store_true',
        default='False',
        help="""Silence all output including output from format tools."""
    )
    noise.add_argument(
        '-v',
        '--verbose',
        action='count',
        default=0,
        help="""\
        Enable verbose output during execution. Multiple switches increase output."""
    )
    arguments, unknown = parser.parse_known_args(sys_args)
    if unknown:
        message = 'Unknown argument supplied: {}'.format(unknown)
        parser.error(message)
        parser.print_help()
    quiet = getattr(arguments, 'quiet')
    count_verb = getattr(arguments, 'verbose')
    if quiet is True:
        count_verb = 0
    else:
        assert isinstance(count_verb, int), 'Problem in Argument Parsing - verbose count.'
        count_verb += 1
    FrmtLog.set_verbosity(count_verb)


####################
def main():
    """\
    Main function - execution entry point
    """
    extra_defn = {'class_name': 'function'}
    key_tools = 'tools'
    key_paths = 'paths'
    found_tools = list()
    argument_processing()
    config = FrmtFileIO.load_resource(_FORMAT_NAME)
    try:
        config_tools = config.get(key_tools, None)
        if not config_tools:
            raise ExecutionError(f'{key_tools!r} section missing in configuration')
        config_paths = config.get(key_paths, None)
        if not config_paths:
            raise ExecutionError(f'{key_paths!r} section missing in configuration')
        found_tools = FrmtTools.FormatTool.from_file(*config_tools)
        for each_tool in found_tools:
            each_tool.verify()
        for key, value in config_paths.items():
            cur_tool = next((tool for tool in found_tools if tool.name_text == key), None)
            if cur_tool:
                cur_tool.execute(*value)

    except ExecutionError as err:
        _main_log.exception('Error: %s', err, extra=extra_defn)
        sys.exit(1)
    except FrmtTools.FormatToolError as err:
        _main_log.exception('Error: %s', err, extra=extra_defn)
        sys.exit(1)


####################
if __name__ == '__main__':
    main()
