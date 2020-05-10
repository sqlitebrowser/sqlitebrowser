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
# pylint: disable=R0912
# R0912 - too-many-branches - FormatTools.execute
# W0212 - check protected-access (local only).
# See method FormatTools.copy() for rationale
"""\
Python 3 module for format tools definitions used on
files in the \'DB Browser for SQLite\' (DB4S) project.
NOTE: See README file in this folder for more details.
"""
import os
import sys
from copy import deepcopy
import filecmp
import shutil
import subprocess
from pathlib import Path as LibPath
from typing import List, Dict, Optional
import format_logger as FrmtLog
import format_fileio as FrmtFileIO
import format_arg as FrmtArg

# module format_logger
_mod_log = FrmtLog.get_logger(__name__)
# flag to control print statement console outputs
# circumvent "noise" for global variable usage
_OUTPUT_STMTS = {'enable': FrmtLog.test_log_level_warning()}


####################
class FormatToolError(ValueError):
    """\
    Format Tool Exception - derived from ValueError
    """


####################
class FormatTool(FrmtArg.ClassUtility):
    """\
    Class defining a format tool and its behavior/execution.
    """
    _LINE_OUT = '-' * 60    # line of 60 '-' as used in astyle
    # KEY DEFINTIONS (used in settings file)
    KEY_NAME = 'name'               # required (dictionary)
    KEY_DISPLAY = 'name_text'       # displayed name of instance
    KEY_EXEC = 'name_exec'          # exec name
    KEY_VALID = 'valid'             # internal
    KEY_COPY = 'copy'               # str name of instance to copy
    KEY_VERS = 'arg_version'        # required (dictionary)
    KEY_CONF = 'arg_conf'           # optional (dictionary)
    KEY_FILE = 'arg_file'           # optional (dictionary)
    KEY_FILE_INPUT = 'arg_input'    # internal
    KEY_FILE_OUTPUT = 'arg_output'  # internal
    KEY_EXTRA = 'arg_extra'         # optional (dictionary)
    KEY_ENV = 'env'                 # optional (dictionary)
    # order of keys is important - do not change
    ARG_ORDER = [KEY_NAME, KEY_VERS, KEY_CONF, KEY_FILE, KEY_ENV, KEY_EXTRA]
    ARG_REQUIRED = [KEY_NAME, KEY_VERS]
    ARG_OPTIONAL = [KEY_CONF, KEY_FILE, KEY_ENV, KEY_EXTRA]

    def __init__(self,
                 name_text: str,
                 name_exec: str,
                 force_equal: Optional[bool] = None):
        _mod_log.debug('start init %r', name_text, extra=self.log_out)
        self._defn = {
            self.KEY_DISPLAY: name_text,
            self.KEY_EXEC: name_exec,
            self.KEY_VALID: False
        }
        self._exec_path: Optional[LibPath] = None
        self._args: Dict[str, FrmtArg.FormatArg] = dict()
        self._environ_flags: Optional[Dict[str, str]] = None
        self._version: FrmtArg.FormatArgVer = FrmtArg.FormatArgVer()
        self._valid_paths: list = list()
        self._cmd_force = force_equal
        _mod_log.debug('end init %r', self._defn[self.KEY_DISPLAY], extra=self.log_out)
        _mod_log.info('%r initialized', self._defn[self.KEY_DISPLAY], extra=self.log_out)

    @property
    def name_text(self) -> str:
        """\
        Return Format Tool display name.
        """
        return str(self._defn[self.KEY_DISPLAY])

    @property
    def name_exec(self) -> str:
        """\
        Return Format Tool executable name.
        """
        return str(self._defn[self.KEY_EXEC])

    @property
    def is_valid(self) -> bool:
        """\
        Return if format tool is valid (exec operation confirmed).
        """
        return bool(self._defn[self.KEY_VALID])

    @property
    def header(self) -> str:
        """\
        Return a formatted header string.
        """
        return '\n'.join(['', self._LINE_OUT, self.name_text, self._LINE_OUT])

    def set_arg_version(self,
                        arg: str,
                        value: Optional[str] = None,
                        oper: Optional[str] = None):
        """\
        (Required) Parameter \'arg\' is required to identify command argument to format tool
        to retrieve its \'version\' string. Format tool version string is used to verify
        format tool execution.

        Other parameters can be supplied to limit allowed version of format tool.

        params:
            arg     format tool command line switch to trigger version string
            value   (optional) string form of wanted version in X.Y.Z. form.
            oper    (optional) string value one of LT, LE, EQ, NE, GE, GT
                    corresponds to logical operators: <, <=, ==, !=, >=, >
        NOTE: assignment clears previous arguments
        NOTE: see README for more details.
        """
        self._args[self.KEY_VERS] = FrmtArg.FormatArg(arg)
        self._version.set_version(value, oper)
        _mod_log.info('%r version: %r %r',
                      self.name_text,
                      str(self._args[self.KEY_VERS]),
                      str(self._version), extra=self.log_out)

    def set_arg_conf(self, str_arg: str, filename: str):
        """\
        Set configuration file that Format Tool instance is to use during execution.
        NOTE: path assumed to be in same folder as this module.
        """
        conf_path = FrmtFileIO.FORMAT_FLDR.joinpath(filename)
        if not conf_path.exists():
            raise FormatToolError(f'{self.name_text} configuration file {filename!r} not found')
        try:
            arg = FrmtArg.FormatArg(str_arg, str(conf_path), self._cmd_force)
            self._args[self.KEY_CONF] = arg
        except FrmtArg.FormatArgError as err:
            _mod_log.exception('Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)
        _mod_log.info('%r conf file:\n\t%r', self.name_text, self._args[self.KEY_CONF], extra=self.log_out)

    def set_arg_file(self,
                     arg_input: Optional[str] = None,
                     arg_output: Optional[str] = None):
        """\
        (Optional) define arguments if format tool is to redirect file output(not in-place).
        NOTE: not verified for correctness WRT format tool.
        NOTE: assignment clears previous arguments
        """
        _mod_log.debug('%r args: %s %s', self.name_text, str(arg_input), str(arg_output), extra=self.log_out)
        try:
            if not arg_input and not arg_output:
                _mod_log.warning('%r empty file arguments provided', self.name_text, extra=self.log_out)
                return
            if arg_input:
                arg = FrmtArg.FormatArg(arg_input, force_equal=self._cmd_force)
                self._args[self.KEY_FILE_INPUT] = arg
                _mod_log.info('%r input arg: %r',
                              self.name_text, self._args[self.KEY_FILE_INPUT], extra=self.log_out)
            if arg_output:
                arg = FrmtArg.FormatArg(arg_output, force_equal=self._cmd_force)
                self._args[self.KEY_FILE_OUTPUT] = arg
                _mod_log.info('%r output arg: %r',
                              self.name_text, self._args[self.KEY_FILE_OUTPUT], extra=self.log_out)
        except FrmtArg.FormatArgError as err:
            _mod_log.exception('Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)

    def set_env_flags(self, **kwargs):
        """\
        (Optional) Set environment key/value pairs.
        NOTE: not verified for correctness WRT format tool.
        NOTE: assignment clears previous flag usage
        """
        self._environ_flags = dict()
        # for key, value in flags.items():
        for key, value in kwargs.items():
            self._environ_flags[key] = value
        _mod_log.info(
            '%r env flags:\n\t%s', self.name_text,
            '\n\t'.join('{}: {!r}'.format(k, v) for (k, v) in self._environ_flags.items()),
            extra=self.log_out)

    def set_args_extra(self, **kwargs):
        """\
        (Optional) Set extra arguments needed for format tool execution.
        NOTE: not verified for correctness WRT format tool.
        NOTE: assignment clears previous arguments
        """
        self._args[self.KEY_EXTRA] = list()
        for key, value in kwargs.items():
            try:
                set_value = value if value else None
                arg = FrmtArg.FormatArg(key, set_value, self._cmd_force)
                self._args[self.KEY_EXTRA].append(arg)
            except FrmtArg.FormatArgError as err:
                _mod_log.exception('Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
                sys.exit(1)
        _mod_log.info(
            '%r extra args:\n\t%s', self.name_text,
            '\n\t'.join(str(x) for x in self._args[self.KEY_EXTRA]), extra=self.log_out)

    def set_paths(self, *args):
        """\
        (Required) Set file paths to use with this format tool.
        NOTE: arguments assumed to be a list of strings.
        NOTE: each path is verified (or file glob will produce results) WRT project root.
        NOTE: exception raised if path verification fails.
        NOTE: assignment clears previous arguments
        """
        valid_paths = list()
        if not args:
            raise FormatToolError(f'{self.name_text} empty file list encountered')
        for each_str in args:
            if each_str.find('*') >= 0:
                # wildcard character found in string path
                # glob check in project root produce results?
                glob_paths = list(each_glob for each_glob in FrmtFileIO.PROJECT_FLDR.glob(each_str))
                if not glob_paths:
                    raise FormatToolError(f'{self.name_text} empty file glob result {each_str!r}')
                each_path = FrmtFileIO.PROJECT_FLDR.joinpath(each_str)
                valid_paths.append(each_path)
            else:
                # str to file in project. File exists? add path to valid
                each_path = FrmtFileIO.PROJECT_FLDR.joinpath(each_str)
                if not each_path.exists():
                    raise FormatToolError(f'{self.name_text} non existant file in tool path\n{each_str!r}')
                valid_paths.append(each_path)
        _mod_log.info('%r set paths - number of patterns: %d', self.name_text, len(valid_paths), extra=self.log_out)
        _mod_log.debug('%r paths:\n\t%s', self.name_text, '\n\t'.join(str(x) for x in valid_paths), extra=self.log_out)
        if valid_paths:
            self._valid_paths = valid_paths[:]

    def execute(self, *args):
        """\
        Execute format tool with current settings.
        (Required) Set file paths to use with this format tool.
        NOTE: arguments assumed to be a list of strings.
        NOTE: each path is verified (or file glob will produce results) WRT project root.
        NOTE: will raise FormatToolError exception if executable fails validation.
        """
        try:
            # display header as early as possible
            if _OUTPUT_STMTS['enable']:
                print(self.header)
            if args:
                self.set_paths(*args)
            if not self._valid_paths:
                raise FormatToolError(f'{self.name_text} does not have any valid paths set')
            if self._environ_flags:
                # establish any environment values for format tool
                for key, value in self._environ_flags.items():
                    os.environ[key] = value
            if not self.is_valid:
                raise FormatToolError(f'{self.name_text!r} not verified')
            command = [str(self._exec_path)]
            # TODO: Python 3.8 - > can change assignment/if to PEP572
            conf = self._args.get(self.KEY_CONF, None)
            if conf:
                command += conf.command_output()
            # TODO: Python 3.8 - > can change assignment/if to PEP572
            args_extra = self._args.get(self.KEY_EXTRA, None)
            if args_extra:
                for each in args_extra:
                    command += each.command_output()
            for each_path in self._valid_paths:
                # format tool work on a file path done here
                self._process_path(each_path, command)
            # revert environment settings
            self._unset_environ_values()
            # print empty line at end of processing
            if _OUTPUT_STMTS['enable']:
                print()
        except FormatToolError as err:
            self._unset_environ_values()
            _mod_log.exception('Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)
        finally:
            _mod_log.info('%r execute end', self.name_text, extra=self.log_out)

    def verify(self):
        """\
        Find executable and verify its operation by retrieving version string.
        NOTE: Exception will be raised and execution terminated validation fails at any point.
        """
        try:
            _mod_log.info('%r verify start', self.name_text, extra=self.log_out)
            path_str = shutil.which(self.name_exec)
            if not path_str:
                raise FormatToolError(f'{self.name_text} executable not found in PATH')
            path = LibPath(path_str)
            if not path.exists():
                raise FormatToolError(f'{self.name_text} executable not found in System')
            ver_cmd = [str(path_str)]
            ver_cmd += self._args[self.KEY_VERS].command_output()
            result = subprocess.run(ver_cmd, check=True, shell=False, capture_output=True)
            if result.returncode != 0:
                raise FormatToolError(result.stderr)
            self._exec_path = path
            # NOTE: some format tools (i.e. xmllint) return version info on stderr not stdout
            # guard against this and not add any extra definitions to instance.
            result_text = '\n'.join([result.stdout.decode('utf-8'), result.stderr.decode('utf-8')])
            _mod_log.info('%r returncode: %d', self.name_text, result.returncode, extra=self.log_out)
            _mod_log.debug('%r\nstdout: %s\nstderr: %s',
                           self.name_text, str(result.stdout), str(result.stderr), extra=self.log_out)
            self._defn[self.KEY_VALID] = self._version.compare(result_text)
        except FrmtArg.FormatArgError as err:
            _mod_log.exception('Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)
        except FormatToolError as err:
            _mod_log.exception('Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)
        except subprocess.CalledProcessError as err:
            _mod_log.exception('Process Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)
        except OSError as err:
            _mod_log.exception('OS Error(%s) - %r:\n%s', self.name_text, self.name_exec, err, extra=self.log_out)
            sys.exit(1)
        finally:
            _mod_log.info('%r verify end', self.name_text, extra=self.log_out)

    def _unset_environ_values(self):
        """\
        Internal method to unset any environment flags
        established before executing format code tool.
        """
        if self._environ_flags:
            for key in self._environ_flags:
                os.environ.pop(key)

    def _process_path(self, src_path: LibPath, args: list):
        """\
        Execute defined format code tool(class instance) on the path value.
        Parameter \'args\' is a list of commands established prior to need
        for path values (see Execute function for setup).
        """
        temp_path = LibPath()
        cur_args = args[:]
        rel_path = src_path.relative_to(FrmtFileIO.PROJECT_FLDR)
        # NOTE: Python 3.8 - > can change assignment/if to PEP572
        arg_out = self._args.get(self.KEY_FILE_OUTPUT, None)
        if arg_out and src_path.exists():
            # setup temporary file - append underscore to name
            temp_path = LibPath(str(src_path) + "_")
            arg_out.update_value(temp_path)
            cur_args += arg_out.command_output()
        # NOTE: Python 3.8 - > can change assignment/if to PEP572
        arg_in = self._args.get(self.KEY_FILE_INPUT, None)
        if arg_in:
            arg_in.update_value(src_path)
            cur_args += arg_in.command_output()
        else:
            cur_args.append(str(src_path))
        try:
            args_str = '\n\t'.join(x for x in cur_args)
            _mod_log.debug('%r execute args:\n\t%s',
                           self.name_text, args_str,
                           extra=self.log_out)
            result = subprocess.run(cur_args,
                                    capture_output=(not _OUTPUT_STMTS['enable']),
                                    check=True)
            if result.returncode != 0:
                raise FormatToolError(result.stderr)
        except subprocess.CalledProcessError as err:
            self._unset_environ_values()
            _mod_log.exception('%s process %r file: %s\n%s',
                               self.name_text, self.name_exec, rel_path, err,
                               extra=self.log_out)
            sys.exit(1)
        if self._args.get(self.KEY_FILE_OUTPUT, None) is not None and src_path.exists():
            self._process_temp_file(src_path, temp_path)

    def _process_temp_file(self, src_path: LibPath, temp_path: LibPath):
        """\
        Temporary File Processing. If temp_path exists, determine if it differs
        from its source. If different, replace source file with temp file. Else,
        remove temp file.
        """
        try:
            rel_src = src_path.relative_to(FrmtFileIO.PROJECT_FLDR)
            rel_temp = temp_path.relative_to(FrmtFileIO.PROJECT_FLDR)
            if not temp_path.exists():
                raise FormatToolError(
                    f'Missing temporary file {rel_temp!r}. Check Format Tool arguments.')
            # clear comparison cache
            filecmp.clear_cache()
            # comparison of format output
            if filecmp.cmp(src_path, temp_path, shallow=False):
                # remove temp file
                temp_path.unlink()
                if _OUTPUT_STMTS['enable']:
                    print(f"Unchanged: {rel_src!s}")
            else:
                # remove original and move temp to replace
                new_path = str(src_path)
                src_path.unlink()
                temp_path.rename(new_path)
                if _OUTPUT_STMTS['enable']:
                    print(f"Formatted: {rel_src!s}")
        except ValueError as err:
            # encountered if path.relative_to(project root) fails
            # this **should not** happen if paths have been verified
            self._unset_environ_values()
            _mod_log.exception(
                'Processing(%s) - %r - file: %s\n%s',
                self.name_text, self.name_exec, rel_src, err, extra=self.log_out)
            sys.exit(1)

    @classmethod
    def copy(cls, self, new_name: str):
        """\
        Duplicate a FormatTool instance.
        param:
            self        instance to copy
            new_name    text name for new instance
        return:
            deep copy of instance.
        """
        ret_cls = deepcopy(self)
        # disable pylint check protected-access (W0212)
        # this should be the only occasion allowed of changing displayed name.
        # displayed name is NOT (nor will ever be) a public setter method.
        ret_cls._defn[self.KEY_DISPLAY] = new_name    # pylint: disable=W0212
        _mod_log.info('copy - from %r to %r', self.name_text, ret_cls.name_text, extra=self.log_out)
        return ret_cls

    @classmethod
    def instance_args(cls, self, tool_key: str, **config):
        """\
        With a new instance of Format Tool, set arguments from provided dictionary.
        Return the modified instance.
        """
        _mod_log.debug('%r %r %r', self.name_text, tool_key, config, extra=self.log_out)
        if not self:
            raise FormatToolError('Settings error - Format Tool not created')
        if config is None and tool_key in cls.ARG_OPTIONAL:
            _mod_log.debug('%r %r not in data',
                           self.name_text, tool_key,
                           extra=self.log_out)
            return self
        if tool_key == cls.KEY_VERS:
            self.set_arg_version(**config)
        elif tool_key == cls.KEY_CONF:
            self.set_arg_conf(config.get('arg', None), config.get('value', None))
        elif tool_key == cls.KEY_FILE:
            self.set_arg_file(**config)
        elif tool_key == cls.KEY_EXTRA:
            self.set_args_extra(**config)
        elif tool_key == cls.KEY_ENV:
            self.set_env_flags(**config)
        else:
            _mod_log.warning('%r unknown key encountered: %r',
                             self.name_text, tool_key,
                             extra=self.log_out)
        return self

    @classmethod
    def from_file(cls, *args) -> List['FormatTool']:
        """\
        Create instance of Format Tool from provided dictionary.
        Assumption is kwargs is a list of dictionaries where each entry
        defines a format tool. Method returns a list of format tools.
        """
        # update logging output
        _OUTPUT_STMTS['enable'] = FrmtLog.test_log_level_warning()
        # process list of tools
        tool_list: List[FormatTool] = list()
        for tool in args:
            tool_instance: Optional[FormatTool] = None
            is_copy = False
            for tool_key in cls.ARG_ORDER:
                elem_data = tool.get(tool_key, None)
                if elem_data is None and tool_key in cls.ARG_REQUIRED and not is_copy:
                    raise FormatToolError(f'Settings error - missing {tool_key!r} entry.')
                if elem_data is None:
                    continue
                if tool_key == cls.KEY_NAME:
                    # create tool instance
                    copy_name = elem_data.get(cls.KEY_COPY, None)
                    new_name = elem_data.get(cls.KEY_DISPLAY, None)
                    if not new_name:
                        raise FormatToolError('Settings error - missing display name')
                    if copy_name is not None:
                        # copy tool
                        tool_src = next((tool for tool in tool_list if tool.name_text == copy_name), None)
                        if not tool_src:
                            raise FormatToolError(f'Settings error - copy failed. Tool {copy_name!r} not found')
                        tool_instance = FormatTool.copy(tool_src, new_name)
                        is_copy = True
                    else:
                        # create tool instance
                        tool_instance = FormatTool(**elem_data)
                    continue
                tool_instance = cls.instance_args(tool_instance, tool_key, **elem_data)
            if tool_instance:
                tool_list.append(tool_instance)
        return tool_list
