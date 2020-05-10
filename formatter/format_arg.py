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
# pylint: disable=R0903
# R0903 - too few public methods - ClassUtility
"""\
Python 3 module for format tool argument definition.
Module use in association with format_runner on files
in the \'DB Browser for SQLite\' (DB4S) project.
NOTE: See README file in this folder for more details.
"""
import operator
import re
from typing import List, Optional
# packaging library - should be installed by Python normally
# otherwise  -> pip install packaging
# NOTE: assuming version string conforms to Python PEP440
# unable to import packaging directly
from packaging.version import parse as pkgParse
from packaging.version import Version as pkgVersion
import format_logger as FrmtLog
# module format_logger
_arg_log = FrmtLog.get_logger(__name__)


####################
class ClassUtility:
    """\
    Utility Mixin for class - alias for common class methods.
    NOTE: Mixin as no __init__ method
    """
    @property
    def cls_name(self) -> str:
        """\
        Convenience property alias of current class name
        """
        return self.__class__.__name__

    @property
    def log_out(self) -> dict:
        """\
        Utility method to produce a dict of class name suitable to be
        added in log calls (i.e. logger.LEVEL(..., extra=obj.log_out)
        """
        return {'class_name': self.cls_name}

    @classmethod
    def cls_log_out(cls) -> dict:
        """\
        Class Method version of cls_name property
        """
        return {'class_name': cls.__name__}


####################
class FormatArgError(ValueError):
    """\
    Command Line Argument Exception - derived from ValueError
    """


####################
class FormatArg(ClassUtility):
    """\
    Logic for command line switch
    """
    _SW_SINGLE = re.compile(r'^[a-zA-z]{1}')
    _SW_DASH_SINGLE = re.compile(r'^[-]+[a-zA-z]{1}')
    _SW_MULTI = re.compile(r'^[a-zA-z]{2,}')
    _SW_DASH_MULTI = re.compile(r'^[-]{2}[a-zA-z]{2,}')

    def __init__(self,
                 arg_text: str,
                 arg_value: Optional[str] = None,
                 force_equal: Optional[bool] = None):
        _arg_log.debug('start init: %r', arg_text, extra=self.log_out)
        self._use_equal: bool = False
        self._force: Optional[bool] = force_equal if force_equal is not None else None
        self._value: str = str(arg_value) if arg_value is not None else str()
        self._text = arg_text
        self._process_dashes()
        _arg_log.debug('end init: %r', self, extra=self.log_out)

    def __str__(self):
        values = self.command_output()
        seperation = '=' if self._use_equal else ' '
        return seperation.join(str(x) for x in values)

    def __repr__(self):
        return f'{self._text} {self._value} eq: {self._use_equal} force: {self._force}'

    def command_output(self) -> List[str]:
        """\
        Process class returning key|value as a list of strings.
        """
        if not self._value:
            return [self._text]
        if (not self._force and not self._use_equal) or (self._force is False):
            return [self._text, self._value]
        # else self._force or self._use_equal
        return [self._text + '=' + self._value]

    def update_value(self, value):
        """\
        Update the value stored in class instance.
        NOTE: Argument value stored internally as a string value.
        """
        self._value = str(value)
        self._process_dashes()

    def _process_dashes(self):
        """\
        Internal method to process command argument.
        If argument is already in the form of '-[\\D]' or '--[\\D]{2,}' return.
        Else, attempt to determine if a single or multi dash argument and add to text.
        NOTE: exception raised if processing fails.
        """
        match_dash_single = self._SW_DASH_SINGLE.search(self._text)
        if match_dash_single and len(self._text) == 2:
            _arg_log.debug('%r - single dash', self._text, extra=self.log_out)
            return
        match_dash_multi = self._SW_DASH_MULTI.search(self._text)
        if match_dash_multi and len(self._text) > 3:
            _arg_log.debug('%r - multi dash', self._text, extra=self.log_out)
            self._use_equal = True
            return
        match_single = self._SW_SINGLE.search(self._text)
        if match_single and len(self._text) == 1:
            _arg_log.debug('%r - single no dash', self._text, extra=self.log_out)
            self._text = '-' + self._text
            return
        match_multi = self._SW_MULTI.search(self._text)
        if match_multi:
            _arg_log.debug('%r - multi no dash', self._text, extra=self.log_out)
            self._use_equal = True
            self._text = '--' + self._text
            return
        raise FormatArgError(f'Argument {self._text!r} may be malformed')


####################
class FormatArgVer(ClassUtility):
    """\
    Container for format tool version limit.
    """
    # regex pattern looking for version in form of X.Y.Z
    _VERSION_RE = re.compile(r'([\d]{1,}([.]([\d]{1,})){0,})')
    _VERSION_NULL = pkgVersion('0')     # empty version
    _OPER_STR = ('LT', 'LE', 'EQ', 'NE', 'GE', 'GT')

    def __init__(self,
                 value: Optional[str] = None,
                 operation: Optional[str] = None):
        self._limit: pkgVersion
        self._operation: Optional[str]
        self.set_version(value, operation)
        _arg_log.debug('end init', extra=self.log_out)

    def __repr__(self):
        return f'{self._operation} {self._limit!s}'

    @property
    def have_version(self):
        """\
        Return boolean True if current instance has a non-null version value.
        """
        return self._limit != self._VERSION_NULL

    def set_version(self,
                    value: Optional[str] = None,
                    operation: Optional[str] = None):
        """\
        Update instance with new values.
        """
        if value is None:
            self._limit = self._VERSION_NULL
            self._operation = None
        else:
            self._limit = self.parse_version(value)
            self._operation = self._parse_operation(operation)
        if self._limit == self._VERSION_NULL:
            _arg_log.debug('Version limit value not set.', extra=self.log_out)
        if self._operation is None:
            _arg_log.debug('Version compare operation not set.', extra=self.log_out)

    def compare(self, version_text: str) -> bool:
        """\
        Parse text retrieved from an executable called with version argument,
        attempt to extract a version string and perform a comparison to values
        stored in this instance.
        return True if...
            - instance version is NULL and text is not empty
            - instance version not null and extracted version meets criteria
              (i.e LT value or GE value).
        See README file for more details.
        """
        _arg_log.debug('compare: %s', str(version_text), extra=self.log_out)
        if not version_text:
            return False
        if not self.have_version:
            return True
        # NOTE: Win console operation needs to be verified
        # may need os.linesep to split
        # can text be split by CRs/LFs?
        lines = version_text.split('\n')
        for each_line in lines:
            match = self._VERSION_RE.search(each_line)
            if match:
                test_ver = self.parse_version(match.group(0))
                if test_ver == self._VERSION_NULL:
                    return False
                _arg_log.info('compare - found: %r op: %r want: %r',
                              str(test_ver), self._operation, str(self._limit),
                              extra=self.log_out)
                return self._get_logic_fn()(test_ver, self._limit)
        return False

    def _get_logic_fn(self):
        return {
            'LT': operator.lt,
            'LE': operator.le,
            'EQ': operator.eq,
            'NE': operator.ne,
            'GE': operator.ge,
            'GT': operator.gt
        }[self._operation]

    def _parse_operation(self, operation: Optional[str]) -> Optional[str]:
        """\
        Verify/Correct operator string.
        """
        if not operation:
            return None
        if len(operation) != 2 or operation.upper() not in self._OPER_STR:
            _arg_log.warning('Version operator %r not understood.', operation, extra=self.log_out)
            return None
        return operation.upper()

    @classmethod
    def parse_version(cls, version: str) -> pkgVersion:
        """\
        Regex search of a string to extract a version value.
        NOTE: assumption version is of the form X.Y.Z (major, minor, increment)
        NOTE: reliance on Python packaging Version for determination of version value.
        """
        match_ver = cls._VERSION_RE.search(version)
        if match_ver:
            parsed_ver = match_ver.group(0).strip()
            pkg_ver = pkgParse(parsed_ver)
            _arg_log.debug('version %r %s', str(pkg_ver), str(type(pkg_ver)), extra=cls.cls_log_out())
            return pkg_ver if isinstance(pkg_ver, pkgVersion) else cls._VERSION_NULL
        return cls._VERSION_NULL
