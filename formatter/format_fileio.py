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
Python 3 module for file handling used as part of code
formatting for \'DB Browser for SQLite\' (DB4S) project.
NOTE: See README file in this folder for more details.
"""
import sys
from pathlib import Path as LibPath
import toml
# setup logging in module
import format_logger
_rsrc_log = format_logger.get_logger(__name__)

# directory containing this script - assume one folder up
FORMAT_FLDR = LibPath(__file__).parents[0].resolve()
# project directory assumed to be one folder up from format folder
PROJECT_FLDR = LibPath(FORMAT_FLDR.parents[0])
_EXT_TOML = '.toml'


####################
class ResourceError(ValueError):
    """\
    Generic Exception occured during execution.
    """


####################
def pretty_dictionary(dict_obj: dict,
                      indt_init: int = 0,
                      indt_size: int = 4,
                      width: int = 80) -> list:
    """\
    Display string type presentation of selected dictionary.
    """
    if not dict_obj or not isinstance(dict_obj, dict):
        raise ResourceError('Invalid Dictionary Object given to method.')
    if not hasattr(dict_obj, 'items'):
        raise ResourceError('Empty Dictionary Object given to method.')
    top_indt = str(' ' * indt_init)
    lines = ['{indt}{{'.format(indt=top_indt)]
    for key, value in dict_obj.items():
        indt_cur = indt_init + indt_size
        indt_sp = str(' ' * indt_cur)
        if isinstance(value, dict):
            lines.append('{indt}{key:s}:'.format(indt=indt_sp, key=key))
            lines += pretty_dictionary(value, indt_init=indt_cur, indt_size=indt_size, width=width)
        elif isinstance(value, list):
            lines.append('{indt}{key:s}:'.format(indt=indt_sp, key=key))
            lines.append('{indt}['.format(indt=indt_sp))
            spacing = indt_cur + indt_size
            sp_str = ' ' * spacing
            for each in value:
                lines.append('{indt}{v!r}'.format(indt=sp_str, v=str(each)))
            lines.append('{indt}]'.format(indt=indt_sp))
        else:
            lines.append('{indt}{key:s}: {val!r}'.format(indt=indt_sp, key=key, val=str(value)))
    lines.append('{indt}}}'.format(indt=top_indt))
    return lines


####################
def load_resource(filename: str) -> dict:
    """\
    Load the resource file that defines what to do for each type of project file.
    return: populated dictionary of values
    NOTE: will throw exception if:
        - dictionary cannot be found
        - resource file cannot be parsed as a TOML file
        - dictionary is empty
    NOTE: see README file for details of TOML file format/contents
    """
    extra_defn = {'class_name': 'function'}
    try:
        rsrc_name = filename
        if not rsrc_name.endswith(_EXT_TOML):
            rsrc_name += _EXT_TOML
        rsrc_path = FORMAT_FLDR.joinpath(rsrc_name)
        config = dict(toml.load(rsrc_path))
        _rsrc_log.info('%s loaded.', rsrc_path.name, extra=extra_defn)
        if not config:
            # flag error if returned dictionary emtpy
            raise ResourceError('No Data Found')
        _rsrc_log.debug(
            'configuration:\n%s',
            '\n'.join(line for line in pretty_dictionary(config)), extra=extra_defn)
        return config
    except ResourceError as err:
        _rsrc_log.exception('Resource Error - File %s\n%s', rsrc_path.name, err)
        sys.exit(1)
    except toml.TomlDecodeError as err:
        _rsrc_log.exception('Decode Error - File %s\n%s', rsrc_path.name, err)
        sys.exit(1)
    except OSError as err:
        _rsrc_log.exception('OS Error - File %s\n%s', str(rsrc_path), err)
        sys.exit(1)
