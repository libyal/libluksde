#!/usr/bin/env python
#
# Python-bindings volume type test script
#
# Copyright (C) 2013-2019, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.

import argparse
import os
import sys
import unittest

import pyluksde


class VolumeTypeTests(unittest.TestCase):
  """Tests the volume type."""

  def test_signal_abort(self):
    """Tests the signal_abort function."""
    luksde_volume = pyluksde.volume()

    luksde_volume.signal_abort()

  def test_open(self):
    """Tests the open function."""
    if not unittest.source:
      return

    luksde_volume = pyluksde.volume()

    luksde_volume.open(unittest.source)

    with self.assertRaises(IOError):
      luksde_volume.open(unittest.source)

    luksde_volume.close()

    with self.assertRaises(TypeError):
      luksde_volume.open(None)

    with self.assertRaises(ValueError):
      luksde_volume.open(unittest.source, mode="w")

  def test_open_file_object(self):
    """Tests the open_file_object function."""
    if not unittest.source:
      return

    file_object = open(unittest.source, "rb")

    luksde_volume = pyluksde.volume()

    luksde_volume.open_file_object(file_object)

    with self.assertRaises(IOError):
      luksde_volume.open_file_object(file_object)

    luksde_volume.close()

    # TODO: change IOError into TypeError
    with self.assertRaises(IOError):
      luksde_volume.open_file_object(None)

    with self.assertRaises(ValueError):
      luksde_volume.open_file_object(file_object, mode="w")

  def test_close(self):
    """Tests the close function."""
    if not unittest.source:
      return

    luksde_volume = pyluksde.volume()

    with self.assertRaises(IOError):
      luksde_volume.close()

  def test_open_close(self):
    """Tests the open and close functions."""
    if not unittest.source:
      return

    luksde_volume = pyluksde.volume()

    # Test open and close.
    luksde_volume.open(unittest.source)
    luksde_volume.close()

    # Test open and close a second time to validate clean up on close.
    luksde_volume.open(unittest.source)
    luksde_volume.close()

    file_object = open(unittest.source, "rb")

    # Test open_file_object and close.
    luksde_volume.open_file_object(file_object)
    luksde_volume.close()

    # Test open_file_object and close a second time to validate clean up on close.
    luksde_volume.open_file_object(file_object)
    luksde_volume.close()

    # Test open_file_object and close and dereferencing file_object.
    luksde_volume.open_file_object(file_object)
    del file_object
    luksde_volume.close()

  def test_read_buffer(self):
    """Tests the read_buffer function."""
    if not unittest.source:
      return

    luksde_volume = pyluksde.volume()

    luksde_volume.open(unittest.source)

    file_size = luksde_volume.get_size()

    # Test normal read.
    data = luksde_volume.read_buffer(size=4096)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(file_size, 4096))

    if file_size < 4096:
      data = luksde_volume.read_buffer()

      self.assertIsNotNone(data)
      self.assertEqual(len(data), file_size)

    # Test read beyond file size.
    if file_size > 16:
      luksde_volume.seek_offset(-16, os.SEEK_END)

      data = luksde_volume.read_buffer(size=4096)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 16)

    with self.assertRaises(ValueError):
      luksde_volume.read_buffer(size=-1)

    luksde_volume.close()

    # Test the read without open.
    with self.assertRaises(IOError):
      luksde_volume.read_buffer(size=4096)

  def test_read_buffer_file_object(self):
    """Tests the read_buffer function on a file-like object."""
    if not unittest.source:
      return

    file_object = open(unittest.source, "rb")

    luksde_volume = pyluksde.volume()

    luksde_volume.open_file_object(file_object)

    file_size = luksde_volume.get_size()

    # Test normal read.
    data = luksde_volume.read_buffer(size=4096)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(file_size, 4096))

    luksde_volume.close()

  def test_read_buffer_at_offset(self):
    """Tests the read_buffer_at_offset function."""
    if not unittest.source:
      return

    luksde_volume = pyluksde.volume()

    luksde_volume.open(unittest.source)

    file_size = luksde_volume.get_size()

    # Test normal read.
    data = luksde_volume.read_buffer_at_offset(4096, 0)

    self.assertIsNotNone(data)
    self.assertEqual(len(data), min(file_size, 4096))

    # Test read beyond file size.
    if file_size > 16:
      data = luksde_volume.read_buffer_at_offset(4096, file_size - 16)

      self.assertIsNotNone(data)
      self.assertEqual(len(data), 16)

    with self.assertRaises(ValueError):
      luksde_volume.read_buffer_at_offset(-1, 0)

    with self.assertRaises(ValueError):
      luksde_volume.read_buffer_at_offset(4096, -1)

    luksde_volume.close()

    # Test the read without open.
    with self.assertRaises(IOError):
      luksde_volume.read_buffer_at_offset(4096, 0)

  def test_seek_offset(self):
    """Tests the seek_offset function."""
    if not unittest.source:
      return

    luksde_volume = pyluksde.volume()

    luksde_volume.open(unittest.source)

    file_size = luksde_volume.get_size()

    luksde_volume.seek_offset(16, os.SEEK_SET)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, 16)

    luksde_volume.seek_offset(16, os.SEEK_CUR)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, 32)

    luksde_volume.seek_offset(-16, os.SEEK_CUR)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, 16)

    luksde_volume.seek_offset(-16, os.SEEK_END)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, file_size - 16)

    luksde_volume.seek_offset(16, os.SEEK_END)

    offset = luksde_volume.get_offset()
    self.assertEqual(offset, file_size + 16)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(-1, os.SEEK_SET)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(-32 - file_size, os.SEEK_CUR)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(-32 - file_size, os.SEEK_END)

    # TODO: change IOError into ValueError
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(0, -1)

    luksde_volume.close()

    # Test the seek without open.
    with self.assertRaises(IOError):
      luksde_volume.seek_offset(16, os.SEEK_SET)


if __name__ == "__main__":
  argument_parser = argparse.ArgumentParser()

  argument_parser.add_argument(
      "-p", "--password", dest="password", action="store", default=None,
      type=str, help="password to unlock the source file.")

  argument_parser.add_argument(
      "source", nargs="?", action="store", metavar="PATH",
      default=None, help="path of the source file.")

  options, unknown_options = argument_parser.parse_known_args()
  unknown_options.insert(0, sys.argv[0])

  setattr(unittest, "password", options.password)
  setattr(unittest, "source", options.source)

  unittest.main(argv=unknown_options, verbosity=2)
