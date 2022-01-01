#!/usr/bin/env python
#
# Python-bindings file type test script
#
# Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pyevtx


class FileTypeTests(unittest.TestCase):
  """Tests the file type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    evtx_file = pyevtx.file()

    evtx_file.signal_abort()

  def test_open(self):
    """Tests the open function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    evtx_file = pyevtx.file()

    evtx_file.open(test_source)

    with self.assertRaises(IOError):
      evtx_file.open(test_source)

    evtx_file.close()

    with self.assertRaises(TypeError):
      evtx_file.open(None)

    with self.assertRaises(ValueError):
      evtx_file.open(test_source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    if not os.path.isfile(test_source):
      raise unittest.SkipTest("source not a regular file")

    evtx_file = pyevtx.file()

    with open(test_source, "rb") as file_object:

      evtx_file.open_file_object(file_object)

      with self.assertRaises(IOError):
        evtx_file.open_file_object(file_object)

      evtx_file.close()

      with self.assertRaises(TypeError):
        evtx_file.open_file_object(None)

      with self.assertRaises(ValueError):
        evtx_file.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    evtx_file = pyevtx.file()

    with self.assertRaises(IOError):
      evtx_file.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    test_source = unittest.source
    if not test_source:
      return

    evtx_file = pyevtx.file()

    # Test open and close.
    evtx_file.open(test_source)
    evtx_file.close()

    # Test open and close a second time to validate clean up on close.
    evtx_file.open(test_source)
    evtx_file.close()

    if os.path.isfile(test_source):
      with open(test_source, "rb") as file_object:

        # Test open_file_object and close.
        evtx_file.open_file_object(file_object)
        evtx_file.close()

        # Test open_file_object and close a second time to validate clean up on close.
        evtx_file.open_file_object(file_object)
        evtx_file.close()

        # Test open_file_object and close and dereferencing file_object.
        evtx_file.open_file_object(file_object)
        del file_object
        evtx_file.close()

  def test_set_ascii_codepage(self):
    """Tests the set_ascii_codepage function."""
    supported_codepages = (
        "ascii", "cp874", "cp932", "cp936", "cp949", "cp950", "cp1250",
        "cp1251", "cp1252", "cp1253", "cp1254", "cp1255", "cp1256", "cp1257",
        "cp1258")

    evtx_file = pyevtx.file()

    for codepage in supported_codepages:
      evtx_file.set_ascii_codepage(codepage)

    unsupported_codepages = (
        "iso-8859-1", "iso-8859-2", "iso-8859-3", "iso-8859-4", "iso-8859-5",
        "iso-8859-6", "iso-8859-7", "iso-8859-8", "iso-8859-9", "iso-8859-10",
        "iso-8859-11", "iso-8859-13", "iso-8859-14", "iso-8859-15",
        "iso-8859-16", "koi8_r", "koi8_u")

    for codepage in unsupported_codepages:
      with self.assertRaises(RuntimeError):
        evtx_file.set_ascii_codepage(codepage)

  def test_get_ascii_codepage(self):
    """Tests the get_ascii_codepage function and ascii_codepage property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    evtx_file = pyevtx.file()

    evtx_file.open(test_source)

    ascii_codepage = evtx_file.get_ascii_codepage()
    self.assertIsNotNone(ascii_codepage)

    self.assertIsNotNone(evtx_file.ascii_codepage)

    evtx_file.close()

  def test_get_number_of_records(self):
    """Tests the get_number_of_records function and number_of_records property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    evtx_file = pyevtx.file()

    evtx_file.open(test_source)

    number_of_records = evtx_file.get_number_of_records()
    self.assertIsNotNone(number_of_records)

    self.assertIsNotNone(evtx_file.number_of_records)

    evtx_file.close()

  def test_get_number_of_recovered_records(self):
    """Tests the get_number_of_recovered_records function and number_of_recovered_records property."""
    test_source = unittest.source
    if not test_source:
      raise unittest.SkipTest("missing source")

    evtx_file = pyevtx.file()

    evtx_file.open(test_source)

    number_of_recovered_records = evtx_file.get_number_of_recovered_records()
    self.assertIsNotNone(number_of_recovered_records)

    self.assertIsNotNone(evtx_file.number_of_recovered_records)

    evtx_file.close()


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
